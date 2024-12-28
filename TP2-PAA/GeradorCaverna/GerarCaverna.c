/*
    AFAZERES
    - makefile (ok)
    - interface (extra) (ok)
    - gerador da caverna (ok)
    - pesquisar heuristicas (ok)
    - explicacao da movimentacao para os 4 lados
*/

#include "GerarCaverna.h"

// Gerando o numero aleatorio
int gerarNumeroAleatorio(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

int geraCavernaTeste(Parametros* parametros){
    
    int caverna[parametros->linhas][parametros->colunas]; // Cria a caverna
    
    FILE* arquivo = fopen(parametros->nomeArquivo, "w"); // Cria o arquivo para salvar a caverna
    if (!arquivo) {
        printf("Erro ao criar o arquivo!\n");
        return 1; // Retorna 1 se houver erro
    }

    fprintf(arquivo, "%d %d %d\n", parametros->linhas, parametros->colunas, parametros-> vidaInicial); // coloca no arquivo as informacoes

    // Inicializa a caverna com celulas (0)
    for (int i = 0; i < parametros->linhas; i++) {
        for (int j = 0; j < parametros->colunas; j++) {
            caverna[i][j] = 0;  
        }
    }
   //Por causa da heuristica que considera que o maior valor eh 40, delimitamos vida máxima para ela funcionar corretamente
    if(parametros->vidaMaxima > 50){
        parametros->vidaMaxima = parametros->vidaMinima + rand() % (51 - parametros->vidaMaxima);
       
    }

    // Gerando os valores de danos e as pocoes de acordo com a dificuldade da caverna
    for (int i = 0; i < parametros->linhas; i++) {
        for (int j = 0; j < parametros->colunas; j++) {
            if(parametros->dificuldade == 1){ // DIFICULDADE FACIL
                if (gerarNumeroAleatorio(0, 4) == 0) {  // A cada 5 posicoes, uma tem chance ser dano
                    caverna[i][j] = -gerarNumeroAleatorio(parametros->danoMinimo, parametros->danoMaximo); 
                }

                if (gerarNumeroAleatorio(0, 2) == 0) {  // A cada 3 posicoes, uma tem chance ser pocao
                    caverna[i][j] = +gerarNumeroAleatorio(parametros->vidaMinima, parametros->vidaMaxima); 
                }
            }
            else if(parametros->dificuldade == 2){ // DIFICULDADE MEDIA
                if (gerarNumeroAleatorio(0, 3) == 0 ) {  // A cada 4 posicoes, uma tem chance ser dano 
                    caverna[i][j] = -gerarNumeroAleatorio(parametros->danoMinimo, parametros->danoMaximo);
                }
                if (gerarNumeroAleatorio(0, 3) == 0 ) {  // A cada 4 posicoes, uma tem chance ser pocao
                    caverna[i][j] = +gerarNumeroAleatorio(parametros->vidaMinima, parametros->vidaMaxima);
                }
            }
            else if(parametros->dificuldade>= 3){ // SE FOR UM NUMERO MAIOR OU IGUAL A 3
                if (gerarNumeroAleatorio(0, 2) == 0){  // A cada 3 posicoes, uma tem chance de ser dano 
                    caverna[i][j] = -gerarNumeroAleatorio(parametros->danoMinimo, parametros->danoMaximo);
                }

                if (gerarNumeroAleatorio(0, 4) == 0) { // A cada 5 posicoes, uma tem chance ser pocao
                    caverna[i][j] = +gerarNumeroAleatorio(parametros->vidaMinima, parametros->vidaMaxima); 
                }
            }
            else{
                printf("Dificuldade Invalida");
            }
        }
    }

    // Calculando a posicao que o 'I' sera gerado
    int borda = gerarNumeroAleatorio(0, 3); // Escolhe entre 4 bordas (0: topo, 1: base, 2: esquerda, 3: direita)
    int inicioX, inicioY;

    // Detalhe: A equipe optou por nao fazer com que o estudante inicie o jogo pela borda de cima e dar mais chances dele iniciar a direita
    if (borda == 1) {
        // Ultima linha
        inicioX = parametros->linhas - 1;
        inicioY = gerarNumeroAleatorio(0, parametros->colunas - 1);
    } else if (borda == 2) {
        // Primeira coluna
        inicioX = gerarNumeroAleatorio(0, parametros->linhas - 1);
        inicioY = 0;
    } else {
        // Ultima coluna
        inicioX = gerarNumeroAleatorio(0, parametros->linhas - 1);
        inicioY = parametros->colunas - 1;
    }

    // Marca a posição final com 'I'
    caverna[inicioX][inicioY] = 'I';
    
    // Calculando a posicao que o 'F' sera gerado
    int fimX = gerarNumeroAleatorio(0, parametros->linhas/2);  // Metade superior
    int fimY = gerarNumeroAleatorio(0, parametros->colunas-1);
    caverna[fimX][fimY] = 'F';

      // Salva o caverna no arquivo
    for (int i = 0; i < parametros->linhas; i++) {
        for (int j = 0; j < parametros->colunas; j++) {
            if (caverna[i][j] == 'I' || caverna[i][j] == 'F') {
                fprintf(arquivo, "%c ", caverna[i][j]); // Escreve os caracteres diretamente
            } else if (caverna[i][j] > 0){
                fprintf(arquivo, "+%d ", caverna[i][j]);
            }else {
                fprintf(arquivo, "%d ", caverna[i][j]); // Escreve os valores numéricos
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Caso de teste gerado com sucesso em %s!\n", parametros->nomeArquivo);
    printf("Dimensões: %dx%d\n", parametros->linhas, parametros->colunas);
    printf("Vida inicial: %d\n", parametros->vidaInicial);
    printf("Posição inicial (I): (%d,%d)\n", inicioX, inicioY);
    printf("Posição final (F): (%d,%d)\n", fimX, fimY);
    return 0;
}

