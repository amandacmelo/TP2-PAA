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
    return x >= 0 && x < caverna->linhas && y >= 0 && y < caverna->colunas;
}

// Heurística para estimar o valor máximo possível a partir de uma posição
int heuristica(Caverna* caverna, int x, int y) {
    // Considera o melhor caso: todos os valores positivos no caminho até o fim
    int distancia = abs(caverna->fim.x - x) + abs(caverna->fim.y - y); // Distancia de manhattan (heuristica que foi adaptada)
    return caverna->vidaInicial + (distancia * 40); // 40 é o valor máximo positivo estimado
}

int encontraMelhorCaminho(Caverna* caverna, int x, int y) {
    if (!posicaoValida(caverna, x, y)) {
        return INT_MIN;
    }

    Celula* estado = &caverna->dp[x][y];
    if (estado->visitado) {
        return estado->vidaMaxima;
    }

    // Poda com heurística
    int estimativa = heuristica(caverna, x, y);
    if (estimativa < 0) {
        return INT_MIN;
    }

    estado->visitado = 1;

    if (x == caverna->fim.x && y == caverna->fim.y) {
        estado->vidaMaxima = caverna->vidaInicial;
        return estado->vidaMaxima;
    }

    int melhorVida = INT_MIN;
    const Ponto movimentos[] = {{-1, 0}, {0, -1}}; // cima, esquerda

    // Otimização: tenta primeiro o movimento que parece mais promissor
    for (int i = 0; i < 2; i++) {
        int novoX = x + movimentos[i].x;
        int novoY = y + movimentos[i].y;
        
        int vidaProximaCelula = encontraMelhorCaminho(caverna, novoX, novoY);
        if (vidaProximaCelula > INT_MIN) {
            int novaVida = vidaProximaCelula + caverna->matriz[x][y];
            if (novaVida > melhorVida) {
                melhorVida = novaVida;
                estado->anterior = (Ponto){novoX, novoY};
            }
        }
    }

    estado->vidaMaxima = melhorVida;
    return melhorVida;
}

void imprimeCaminho(Caverna* caverna) {
    for (int i = 0; i < caverna->linhas; i++) {
        for (int j = 0; j < caverna->colunas; j++) {
            if (caverna->matriz[i][j] == INT_MIN){
                printf(cor_azul " ⍩⃝ " resetar_cor); // Destaca a célula atual
            } 
            else{
                printf(" * "); // Célula já visitada 
            }
        }
        printf("\n");
    } 
}



