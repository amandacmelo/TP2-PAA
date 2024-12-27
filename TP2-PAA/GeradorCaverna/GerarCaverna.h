#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    int linhas;
    int colunas;
    int vidaInicial;
    int danoMinimo;     // Valor mínimo de dano
    int danoMaximo;     // Valor máximo de dano
    int vidaMinima;     // Valor mínimo de vida (poções)
    int vidaMaxima;     // Valor máximo de vida (poções)
    float probPotion;   // Probabilidade de gerar uma poção (0 a 1)
    char* nomeArquivo;  // Nome do arquivo de saída
} Parametros;

int gerarNumeroAleatorio(int min, int max);
