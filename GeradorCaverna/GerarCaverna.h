#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    int linhas;
    int colunas;
    int vidaInicial;
    int danoMinimo;     // Valor minimo de dano
    int danoMaximo;     // Valor maximo de dano
    int pocaoMinima;     // Valor minimo de vida (pocoes)
    int pocaoMaxima;     // Valor maximo de vida (pocoes)
    int dificuldade; // Valor que define a dificuldade da caverna a ser gerada
    char* nomeArquivo;  // Nome do arquivo de saida
} Parametros;

int geraNumeroAleatorio(int min, int max);
int geraCavernaTeste(Parametros* parametros);
