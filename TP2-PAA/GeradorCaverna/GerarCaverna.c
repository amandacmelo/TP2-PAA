#include "GerarCaverna.h"

// Gerando o numero aleatorio
int geraNumeroAleatorio(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

int geraCavernaTeste(Parametros* parametros){
    
    int caverna[parametros->linhas][parametros->colunas]; // Cria a caverna
    
    FILE* arquivo = fopen(parametros->nomeArquivo, "w"); // Cria o arquivo para salvar a caverna
    if (!arquivo) {
        printf("Erro ao criar o arquivo!\n");
        return 1; // Retorna 1 se houver erro
    }

    
    if(parametros->danoMinimo <= 0 || parametros->danoMaximo <= 0 || parametros->vidaMinima <= 0 || parametros->vidaMaxima <= 0 || parametros->vidaInicial <= 0){
        printf("Valores nulos e negativos nao sao permitidos");
        return 1;
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
        parametros->vidaMaxima = geraNumeroAleatorio(parametros->vidaMinima, 50);
    }

    // Gerando os valores de danos e as pocoes de acordo com a dificuldade da caverna
    for (int i = 0; i < parametros->linhas; i++) {
        for (int j = 0; j < parametros->colunas; j++) {
            if(parametros->dificuldade == 1){ // DIFICULDADE FACIL
                if (geraNumeroAleatorio(0, 4) == 0) {  // A cada 5 posicoes, uma tem chance ser dano
                    caverna[i][j] = -geraNumeroAleatorio(parametros->danoMinimo, parametros->danoMaximo); 
                }

                if (geraNumeroAleatorio(0, 2) == 0) {  // A cada 3 posicoes, uma tem chance ser pocao
                    caverna[i][j] = +geraNumeroAleatorio(parametros->vidaMinima, parametros->vidaMaxima); 
                }
            }
            else if(parametros->dificuldade == 2){ // DIFICULDADE MEDIA
                if (geraNumeroAleatorio(0, 3) == 0 ) {  // A cada 4 posicoes, uma tem chance ser dano 
                    caverna[i][j] = -geraNumeroAleatorio(parametros->danoMinimo, parametros->danoMaximo);
                }
                if (geraNumeroAleatorio(0, 3) == 0 ) {  // A cada 4 posicoes, uma tem chance ser pocao
                    caverna[i][j] = +geraNumeroAleatorio(parametros->vidaMinima, parametros->vidaMaxima);
                }
            }
            else if(parametros->dificuldade>= 3){ // SE FOR UM NUMERO MAIOR OU IGUAL A 3
                if (geraNumeroAleatorio(0, 2) == 0){  // A cada 3 posicoes, uma tem chance de ser dano 
                    caverna[i][j] = -geraNumeroAleatorio(parametros->danoMinimo, parametros->danoMaximo);
                }

                if (geraNumeroAleatorio(0, 4) == 0) { // A cada 5 posicoes, uma tem chance ser pocao
                    caverna[i][j] = +geraNumeroAleatorio(parametros->vidaMinima, parametros->vidaMaxima); 
                }
            }
            else{
                printf("Dificuldade Invalida");
            }
        }
    }

    // Calculando a posicao que o 'I' sera gerado
    int borda = geraNumeroAleatorio(0, 3); // Escolhe entre 4 bordas (0: topo, 1: base, 2: esquerda, 3: direita)
    int inicioX, inicioY;

    // Detalhe: A equipe optou por nao fazer com que o estudante inicie o jogo pela borda de cima e dar mais chances dele iniciar a direita
    if (borda == 1) {
        // Ultima linha
        inicioX = parametros->linhas - 1;
        inicioY = geraNumeroAleatorio(0, parametros->colunas - 1);
    } else if (borda == 2) {
        // Primeira coluna
        inicioX = geraNumeroAleatorio(0, parametros->linhas - 1);
        inicioY = 0;
    } else {
        // Ultima coluna
        inicioX = geraNumeroAleatorio(0, parametros->linhas - 1);
        inicioY = parametros->colunas - 1;
    }

    // Marca a posição final com 'I'
    caverna[inicioX][inicioY] = 'I';
    
    // Calculando a posicao que o 'F' sera gerado
    int fimX = geraNumeroAleatorio(0, parametros->linhas/2);  // Metade superior
    int fimY = geraNumeroAleatorio(0, parametros->colunas/2); // Metade esquerda
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

