#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

#define cor_verde "\x1b[42m" 
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
int** alocarMatriz(int linhas, int colunas);
Celula** alocarMatrizDP(int linhas, int colunas);
void liberarCaverna(Caverna* caverna);
int posicaoValida(Caverna* caverna, int x, int y);
int calcularHeuristica(int x1, int y1, int x2, int y2);
int encontrarMelhorCaminho(Caverna* caverna, int x, int y);
//int ExibirMelhorCaminho(Caverna* caverna, int x, int y);
void imprimeCaminho(Caverna* caverna);

