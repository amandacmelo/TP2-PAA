#include "Caverna.h"

// Funcoes de alocacao de memoria
int** alocaMatriz(int linhas, int colunas) {
    int** matriz = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int*)malloc(colunas * sizeof(int));
    }
    return matriz;
}

Celula** alocaMatrizDP(int linhas, int colunas) {
    Celula** matriz = (Celula**)malloc(linhas * sizeof(Celula*));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (Celula*)calloc(colunas, sizeof(Celula));
    }
    return matriz;
}

void liberaCaverna(Caverna* caverna) {
    // Antes de liberar memoria, verifica se a caverna, matriz e dp foram alocados, para evitar erros de memoria 
    if (!caverna) return;
    
    if(caverna->matriz){
        for (int i = 0; i < caverna->linhas; i++){
            free(caverna->matriz[i]);
        }
        free(caverna->matriz);
    }
    
    if(caverna->dp) {
        for (int i = 0; i < caverna->linhas; i++){
            free(caverna->dp[i]);
        }
        free(caverna->dp);
    }
}


int posicaoValida(Caverna* caverna, int x, int y) {
    return x >= 0 && x < caverna->linhas && y >= 0 && y < caverna->colunas; // Verifica se esta dentro dos limites da matriz
}

// Heuristica para estimar o valor maximo possivel a partir de uma posicao
int heuristica(Caverna* caverna, int x, int y) {
    // Considera o melhor caso: todos os valores positivos no caminho ate o fim
    int distancia = abs(caverna->fim.x - x) + abs(caverna->fim.y - y); // Distancia de manhattan (heuristica que foi adaptada)
    
    // Levando em conta que o valor maximo de uma celula eh 50 (definido no GerarCaverna) eh utilizado 30% desse valor como referencia para nao superestimar os caminhos
    return caverna->vidaInicial + (distancia * 30); 
}

int encontraMelhorCaminho(Caverna* caverna, int x, int y) {
    // Verifica se a posicao eh valida
    if (!posicaoValida(caverna, x, y)) { 
        return INT_MIN;
    }

    // Se ja calculamos este estado, retorna o valor memorizado
    Celula* estado = &caverna->dp[x][y];
    if (estado->visitado) {
        return estado->vidaMaxima;
    }

    // Poda com heuristica
    int estimativa = heuristica(caverna, x, y);
    if (estimativa < 0) {
        return INT_MIN;
    }

    // Marca o estado como visitado
    estado->visitado = 1;

    // Chegamos ao fim
    if (x == caverna->fim.x && y == caverna->fim.y) {
        // Estamos usando programacao dinamica de baixo para cima, por isso passamos a vidaInicial (eh o caso base), 
        // que sera somado com as vidas de cada cada chamada recursiva, dentro do for abaixo, para encontrar o melhor caminho,
        // ou seja, a melhor vida que eh o valor que sera retornad
        estado->vidaMaxima = caverna->vidaInicial;
        return estado->vidaMaxima;
    }

    // Vai armazenar o maior valor possivel de vida
    int melhorVida = INT_MIN;
    const Ponto movimentos[] = {{-1, 0}, {0, -1}}; // cima, esquerda

    // Verifica os movimentos possiveis
    for (int i = 0; i < 2; i++) {
        // Calcula a nova posicao
        int novoX = x + movimentos[i].x;
        int novoY = y + movimentos[i].y;
        
        // Encontra a melhor vida possivel a partir da nova posicao

        int vidaProximaCelula = encontraMelhorCaminho(caverna, novoX, novoY);  
        // Se a vidaProximaCelula for maior que INT_MIN, significa que eh possivel passar por essa posicao (eh uma posicao valida)
        if (vidaProximaCelula > INT_MIN) {
            int novaVida = vidaProximaCelula + caverna->matriz[x][y];
            if (novaVida > melhorVida) { // Utiliza o valor que maximiza a vida
                melhorVida = novaVida;
                estado->anterior = (Ponto){novoX, novoY};
            }
        }
    }

    estado->vidaMaxima = melhorVida; // Armazena o valor maximo de vida possivel
    return melhorVida;
}

void imprimeCaminho(Caverna* caverna) {
    for (int i = 0; i < caverna->linhas; i++) {
        for (int j = 0; j < caverna->colunas; j++) {
            if (caverna->matriz[i][j] == INT_MIN){ // Se for INT_MIN, eh o caminho feito pelo estudante
                printf(cor_azul " ⍩⃝ " resetar_cor); // Destaca a celula atual
            } 
            else{
                printf(" * "); // Celula ja visitada 
            }
        }
        printf("\n");
    } 
}



