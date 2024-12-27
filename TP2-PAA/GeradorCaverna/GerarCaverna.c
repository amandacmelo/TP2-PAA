/*
    AFAZERES
    - makefile (ok)
    - interface (extra)
    - gerador da caverna
    - pesquisar heuristicas
    - explicação da movimentação para os 4 lados
*/

#include "GerarCaverna.h"

// Função para gerar um número aleatório entre min e max
int gerarNumeroAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Função para gerar um valor aleatório que pode ser dano ou vida
int gerarValor(Parametros* params) {
    if ((float)rand() / RAND_MAX < params->probPotion) {
        // Gera um valor positivo (poção)
        return gerarNumeroAleatorio(params->vidaMinima, params->vidaMaxima);
    } else {
        // Gera um valor negativo (dano)
        return -gerarNumeroAleatorio(params->danoMinimo, params->danoMaximo);
    }
}

// Função para gerar o arquivo de entrada
void gerarCasoTeste(Parametros* params) {
    FILE* arquivo = fopen(params->nomeArquivo, "w");
    if (!arquivo) {
        printf("Erro ao criar arquivo %s\n", params->nomeArquivo);
        return;
    }

    // Primeira linha: dimensões e vida inicial
    fprintf(arquivo, "%d %d %d\n", params->linhas, params->colunas, params->vidaInicial);

    // Escolhe posições aleatórias para I e F
    int inicioX = params->linhas - 1;  // Última linha
    int inicioY = params->colunas - 1;  // Última coluna
    int fimX = gerarNumeroAleatorio(0, params->linhas/2);  // Metade superior
    int fimY = gerarNumeroAleatorio(0, params->colunas-1);

    // Gera a matriz
    for (int i = 0; i < params->linhas; i++) {
        for (int j = 0; j < params->colunas; j++) {
            if (i == inicioX && j == inicioY) {
                fprintf(arquivo, "I");
            }
            else if (i == fimX && j == fimY) {
                fprintf(arquivo, "F");
            }
            else {
                fprintf(arquivo, "%d", gerarValor(params));
            }
            
            // Adiciona espaço entre valores, exceto no último da linha
            if (j < params->colunas - 1) {
                fprintf(arquivo, " ");
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Caso de teste gerado com sucesso em %s!\n", params->nomeArquivo);
    printf("Dimensões: %dx%d\n", params->linhas, params->colunas);
    printf("Vida inicial: %d\n", params->vidaInicial);
    printf("Posição inicial (I): (%d,%d)\n", inicioX, inicioY);
    printf("Posição final (F): (%d,%d)\n", fimX, fimY);
}

// Função para imprimir ajuda
void imprimirAjuda() {
    printf("Uso: gerador [opções]\n");
    printf("Opções:\n");
    printf("  -l N        Número de linhas (padrão: 4)\n");
    printf("  -c N        Número de colunas (padrão: 5)\n");
    printf("  -v N        Vida inicial (padrão: 40)\n");
    printf("  -dmin N     Dano mínimo (padrão: 10)\n");
    printf("  -dmax N     Dano máximo (padrão: 20)\n");
    printf("  -vmin N     Vida mínima das poções (padrão: 10)\n");
    printf("  -vmax N     Vida máxima das poções (padrão: 20)\n");
    printf("  -p N        Probabilidade de poções (0-100) (padrão: 20)\n");
    printf("  -o arquivo  Nome do arquivo de saída (padrão: caverna.txt)\n");
    printf("  -h          Mostra esta ajuda\n");
}

int main(int argc, char* argv[]) {
    // Inicializa gerador de números aleatórios
    srand(time(NULL));

    // Parâmetros padrão
    Parametros params = {
        .linhas = 4,
        .colunas = 5,
        .vidaInicial = 40,
        .danoMinimo = 10,
        .danoMaximo = 20,
        .vidaMinima = 10,
        .vidaMaxima = 20,
        .probPotion = 0.2,
        .nomeArquivo = "caverna.txt"
    };

    // Processa argumentos da linha de comando
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            imprimirAjuda();
            return 0;
        }
        else if (i + 1 < argc) {
            if (strcmp(argv[i], "-l") == 0)
                params.linhas = atoi(argv[i+1]);
            else if (strcmp(argv[i], "-c") == 0)
                params.colunas = atoi(argv[i+1]);
            else if (strcmp(argv[i], "-v") == 0)
                params.vidaInicial = atoi(argv[i+1]);
            else if (strcmp(argv[i], "-dmin") == 0)
                params.danoMinimo = atoi(argv[i+1]);
            else if (strcmp(argv[i], "-dmax") == 0)
                params.danoMaximo = atoi(argv[i+1]);
            else if (strcmp(argv[i], "-vmin") == 0)
                params.vidaMinima = atoi(argv[i+1]);
            else if (strcmp(argv[i], "-vmax") == 0)
                params.vidaMaxima = atoi(argv[i+1]);
            else if (strcmp(argv[i], "-p") == 0)
                params.probPotion = atof(argv[i+1]) / 100.0;
            else if (strcmp(argv[i], "-o") == 0)
                params.nomeArquivo = argv[i+1];
            i++;  // Pula o próximo argumento
        }
    }

    // Validações básicas
    if (params.linhas <= 0 || params.colunas <= 0) {
        printf("Erro: Dimensões devem ser positivas\n");
        return 1;
    }
    if (params.danoMaximo < params.danoMinimo) {
        printf("Erro: Dano máximo deve ser maior que dano mínimo\n");
        return 1;
    }
    if (params.vidaMaxima < params.vidaMinima) {
        printf("Erro: Vida máxima deve ser maior que vida mínima\n");
        return 1;
    }
    if (params.probPotion < 0 || params.probPotion > 1) {
        printf("Erro: Probabilidade deve estar entre 0 e 1\n");
        return 1;
    }

    gerarCasoTeste(&params);
    return 0;
}