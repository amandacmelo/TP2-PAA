#include "Caverna.h"

// Funcoes de alocacao de memoria
int** alocarMatriz(int linhas, int colunas) {
    int** matriz = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int*)malloc(colunas * sizeof(int));
    }
    return matriz;
}

Celula** alocarMatrizDP(int linhas, int colunas) {
    Celula** matriz = (Celula**)malloc(linhas * sizeof(Celula*));
    
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (Celula*)calloc(colunas, sizeof(Celula));
    }
    return matriz;
}

void liberarCaverna(Caverna* caverna) {
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

int posicaoValida(const Caverna* caverna, int x, int y){
    if(x >= 0 && x < caverna->linhas && y >= 0 && y < caverna->colunas){ // Verifica se a posicao esta dentro dos limites da caverna
        return 1; 
    }
    return 0; 
}

int encontrarMelhorCaminho(Caverna* caverna, int x, int y){
    // Se posicao invalida, retorna -1
    if (!posicaoValida(caverna, x, y)){
        return -1;
    }

    // Se ja calculamos este estado, retorna o valor memorizado
    if (caverna->dp[x][y].visitado){
        return caverna->dp[x][y].vidaMaxima;
    }

    // Marca como visitado
    caverna->dp[x][y].visitado = 1;

    // Se eh a posicao final
    if (x == caverna->fim.x && y == caverna->fim.y){
        // Estamos usando programacao dinamica de baixo para cima, por isso passamos a vidaInicial (eh o caso base), 
        // que sera somado com as vidas de cada cada chamada recursiva, dentro do for abaixo, para encontrar o melhor caminho,
        // ou seja, a melhor vida que eh o valor que sera retornado  
        caverna->dp[x][y].vidaMaxima = caverna->vidaInicial; 
        return caverna->dp[x][y].vidaMaxima;
    }

    int melhorVida = -1; // Vai armazenar o maior valor possivel de vida
    const Ponto movimentos[] = {{-1, 0}, {0, -1}}; // cima, esquerda

    // Tenta cada movimento possivel
    for (int i = 0; i < 2; i++){
        // Calcula a nova posicao
        int novoX = x + movimentos[i].x; 
        int novoY = y + movimentos[i].y;
        
        int vidaProximaCelula = encontrarMelhorCaminho(caverna, novoX, novoY);
        if (vidaProximaCelula > 0){ // Se a vidaProximaCelula for maior que 0, significa que eh possivel passar por essa posicao (eh uma posicao valida)
            int novaVida = vidaProximaCelula + caverna->matriz[x][y]; 
            if (novaVida > melhorVida) { // Pega a maior vida possivel (melhor caminho)
                melhorVida = novaVida;
                caverna->dp[x][y].anterior = (Ponto){novoX, novoY}; // Guarda a posicao anterior no melhor caminho 
            }
        }
    }

    caverna->dp[x][y].vidaMaxima = melhorVida; // Guarda a melhor vida possivel para a posicao x, y
    return melhorVida;
}

