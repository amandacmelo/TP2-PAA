#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>


#define cor_azul "\x1b[44m"
#define resetar_cor "\x1b[0m"


// Estrutura para armazenar as coordenadas
typedef struct {
    int x, y;
} Ponto;

// Estrutura para armazenar o estado da celula
typedef struct {
    int vidaMaxima;    // Maxima vida possível ao chegar neste ponto
    Ponto anterior;    // Coordenada anterior no caminho ótimo
    int visitado;      // Flag para indicar se já foi visitado
} Celula;


typedef struct {
    int** matriz;
    Celula** dp;
    int linhas;
    int colunas;
    int vidaInicial;
    Ponto inicio;
    Ponto fim;
} Caverna;


// Funcoes
int** alocaMatriz(int linhas, int colunas);
Celula** alocaMatrizDP(int linhas, int colunas);
void liberaCaverna(Caverna* caverna);
int posicaoValida(Caverna* caverna, int x, int y);
int heuristica(Caverna* caverna, int x, int y);
int encontraMelhorCaminho(Caverna* caverna, int x, int y);
void imprimeCaminho(Caverna* caverna);

