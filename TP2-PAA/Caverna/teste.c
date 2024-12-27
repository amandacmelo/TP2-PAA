#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
    int x, y;
} Ponto;

typedef struct {
    short g;           // Custo real até aqui
    short h;           // Valor heurístico estimado
    short f;           // f = g + h
    char visitado;
    Ponto anterior;
} Estado;

typedef struct {
    signed char** matriz;
    Estado** dp;
    short linhas;
    short colunas;
    short vidaInicial;
    Ponto inicio;
    Ponto fim;
} Caverna;

// Função para inicializar a caverna
Caverna* inicializarCaverna(int linhas, int colunas, int vidaInicial) {
  Caverna* caverna = (Caverna*)malloc(sizeof(Caverna));
  caverna->linhas = linhas;
  caverna->colunas = colunas;
  caverna->vidaInicial = vidaInicial;
  
  // Aloca matriz de valores
  caverna->matriz = (signed char**)malloc(linhas * sizeof(signed char*));
  for (int i = 0; i < linhas; i++) {
    caverna->matriz[i] = (signed char*)malloc(colunas * sizeof(signed char));
  }
  
  // Aloca matriz de estados
  caverna->dp = (Estado**)malloc(linhas * sizeof(Estado*));
  for (int i = 0; i < linhas; i++) {
    caverna->dp[i] = (Estado*)malloc(colunas * sizeof(Estado));
    for (int j = 0; j < colunas; j++) {
      caverna->dp[i][j].visitado = 0;
    }
  }
  
  return caverna;
}

// Função para liberar a memória da caverna
void liberarCaverna(Caverna* caverna) {
  for (int i = 0; i < caverna->linhas; i++) {
    free(caverna->matriz[i]);
    free(caverna->dp[i]);
  }
  free(caverna->matriz);
  free(caverna->dp);
  free(caverna);
}
int posicaoValida(Caverna* caverna, int x, int y) {
    return x >= 0 && x < caverna->linhas && y >= 0 && y < caverna->colunas;
}
// Heurística do A*: estima o melhor ganho possível até o destino
static inline int calcularH(const Caverna* caverna, int x, int y) {
    int dx = caverna->fim.x - x;
    int dy = caverna->fim.y - y;
    
    // Se não podemos chegar ao destino, retorna impossível
    if (dx < 0 || dy < 0) return INT_MIN;
    
    // Distância Manhattan até o destino
    int dist = dx + dy;
    
    // Conta células com peso positivo no caminho
    int celulasPositivas = 0;
    for(int i = x; i <= caverna->fim.x; i++) {
        for(int j = y; j <= caverna->fim.y; j++) {
            if(caverna->matriz[i][j] == 20) {
                celulasPositivas++;
            }
        }
    }
    
    // Heurística admissível: assume melhor caso possível
    return celulasPositivas * 20;
}

int encontrarMelhorCaminho(Caverna* caverna, int x, int y, int gAtual) {
    // Validação rápida
    if (!posicaoValida(caverna, x, y)) {
        return INT_MIN;
    }
    
    // Se já visitamos e temos um resultado melhor ou igual, usa ele
    if (caverna->dp[x][y].visitado && caverna->dp[x][y].g >= gAtual) {
        return caverna->dp[x][y].g;
    }
    
    // Atualiza valores do A*
    caverna->dp[x][y].g = gAtual;
    caverna->dp[x][y].h = calcularH(caverna, x, y);
    caverna->dp[x][y].f = gAtual + caverna->dp[x][y].h;
    caverna->dp[x][y].visitado = 1;
    
    // Chegou ao destino
    if (x == caverna->fim.x && y == caverna->fim.y) {
        return caverna->vidaInicial;
    }
    
    // Movimentos possíveis com seus valores f
    const Ponto dirs[] = {{-1,0}, {0,-1}};
    int valores[2];
    int proxG[2];
    
    // Calcula valores f para cada direção
    for (int i = 0; i < 2; i++) {
        int nx = x + dirs[i].x;
        int ny = y + dirs[i].y;
        
        if (posicaoValida(caverna, nx, ny)) {
            proxG[i] = gAtual + caverna->matriz[x][y];
            int h = calcularH(caverna, nx, ny);
            valores[i] = proxG[i] + h;
        } else {
            valores[i] = INT_MIN;
        }
    }
    
    int melhorVida = INT_MIN;
    
    // Tenta movimentos em ordem de valor f
    for (int i = 0; i < 2; i++) {
        // Escolhe movimento com maior valor f
        int melhorDir = (valores[0] > valores[1]) ? 0 : 1;
        
        if (valores[melhorDir] != INT_MIN) {
            int nx = x + dirs[melhorDir].x;
            int ny = y + dirs[melhorDir].y;
            
            int vidaProx = encontrarMelhorCaminho(caverna, nx, ny, proxG[melhorDir]);
            
            if (vidaProx > INT_MIN) {
                int novaVida = vidaProx + caverna->matriz[x][y];
                if (novaVida > melhorVida) {
                    melhorVida = novaVida;
                    caverna->dp[x][y].anterior = (Ponto){nx, ny};
                }
            }
        }
        
        valores[melhorDir] = INT_MIN;
    }
    
    return melhorVida;
}



// Função para imprimir o caminho encontrado
void imprimirCaminho(Caverna* caverna, int vidaFinal) {
    if (vidaFinal == INT_MIN) {
        printf("Caminho impossível!\n");
        return;
    }

    printf("Vida Final: %d\n", vidaFinal);
    
    // Array para armazenar o caminho
    int tamMax = caverna->linhas * caverna->colunas;
    Ponto* caminho = (Ponto*)malloc(tamMax * sizeof(Ponto));
    int tamCaminho = 0;
    
    // Reconstrói o caminho do fim para o início
    Ponto atual = caverna->fim;
    while (atual.x != caverna->inicio.x || atual.y != caverna->inicio.y) {
        caminho[tamCaminho++] = atual;
        atual = caverna->dp[atual.x][atual.y].anterior;
    }
    caminho[tamCaminho++] = caverna->inicio;
    
    // Imprime o caminho na ordem correta
    printf("Caminho encontrado:\n");
    for (int i = tamCaminho - 1; i >= 0; i--) {
        printf("(%d,%d)", caminho[i].x, caminho[i].y);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
    
    free(caminho);
}

int main() {
    int L, C, V;
    
    // Lê dimensões da caverna e vida inicial
    scanf("%d %d %d", &L, &C, &V);
    
    // Inicializa a caverna
    Caverna* caverna = inicializarCaverna(L, C, V);
    
    // Lê a matriz da caverna
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < C; j++) {
            int valor;
            scanf("%d", &valor);
            caverna->matriz[i][j] = (signed char)valor;
        }
    }
    
    // Lê posição inicial e final
    scanf("%d %d", &caverna->inicio.x, &caverna->inicio.y);
    scanf("%d %d", &caverna->fim.x, &caverna->fim.y);
    
    // Encontra o melhor caminho
    int vidaFinal = encontrarMelhorCaminho(caverna, caverna->inicio.x, caverna->inicio.y, 0);
    
    // Imprime resultado
    imprimirCaminho(caverna, vidaFinal);
    
    // Libera memória
    liberarCaverna(caverna);
    
    return 0;
}
