#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para armazenar as coordenadas
typedef struct {
    int x, y;
} Ponto;

// Estrutura para armazenar o estado da celula
typedef struct {
    int vidaMaxima;    // Máxima vida possível ao chegar neste ponto
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
int posicaoValida(const Caverna* caverna, int x, int y);
int encontrarMelhorCaminho(Caverna* caverna, int x, int y);
