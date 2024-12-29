#include "GerarCaverna.h"

int main(int argc, char* argv[]) {
    // Inicializa gerador de numeros aleatorios
    srand(time(NULL));

    // Parametros padrao (Caso o usuario nao digite os argumentos)
    Parametros parametros = {
        .linhas = 4,
        .colunas = 5,
        .vidaInicial = 40,
        .danoMinimo = 10,
        .danoMaximo = 20,
        .pocaoMinima = 10,
        .pocaoMaxima = 20,
        .dificuldade = 2,
        .nomeArquivo = "caverna.txt"
    };

    // Verifica se os argumentos foram passados
    if (argc < 10 && argc > 1) {
      printf("Argumentos insuficientes, utilizando parametros padrao para os valores faltantes\n");
      return 0; 
    }
    if (argc > 1) {
        parametros.linhas = argc > 1 ? atoi(argv[1]) : parametros.linhas;
        parametros.colunas = argc > 2 ? atoi(argv[2]) : parametros.colunas;
        parametros.vidaInicial = argc > 3 ? atoi(argv[3]) : parametros.vidaInicial;
        parametros.danoMinimo = argc > 4 ? atoi(argv[4]) : parametros.danoMinimo;
        parametros.danoMaximo = argc > 5 ? atoi(argv[5]) : parametros.danoMaximo;
        parametros.pocaoMinima = argc > 6 ? atoi(argv[6]) : parametros.pocaoMinima;
        parametros.pocaoMaxima = argc > 7 ? atoi(argv[7]) : parametros.pocaoMaxima;
        parametros.dificuldade = argc > 8 ? atoi(argv[8]) : parametros.dificuldade;
        parametros.nomeArquivo = argc > 9 ? argv[9] : parametros.nomeArquivo;
    }


    // Validacoes basicas
    if (parametros.linhas <= 0 || parametros.colunas <= 0) {
        printf("Erro: Dimensões devem ser positivas\n");
        return 0;
    }
    if (parametros.danoMaximo < parametros.danoMinimo) {
        printf("Erro: Dano máximo deve ser maior que dano mínimo\n");
        return 0;
    }
    if (parametros.pocaoMaxima < parametros.pocaoMinima) {
        printf("Erro: Vida máxima deve ser maior que vida mínima\n");
        return 0;
    }
    

    geraCavernaTeste(&parametros);
    return 0;
}