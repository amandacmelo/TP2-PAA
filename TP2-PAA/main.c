#include "Caverna/Caverna.h"

Caverna* lerArquivo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro: Não foi possível abrir o arquivo %s\n", nomeArquivo);
        return NULL;
    }

    Caverna* caverna = (Caverna*)malloc(sizeof(Caverna));
    if (!caverna) {
        fclose(arquivo);
        return NULL;
    }

    fscanf(arquivo, "%d %d %d", &caverna->linhas, &caverna->colunas, &caverna->vidaInicial);

    caverna->matriz = alocarMatriz(caverna->linhas, caverna->colunas);
    caverna->dp = alocarMatrizDP(caverna->linhas, caverna->colunas);
    
    if (!caverna->matriz || !caverna->dp) {
        liberarCaverna(caverna);
        free(caverna);
        fclose(arquivo);
        return NULL;
    }

    // Inicializa a matriz dp com valores negativos
    for (int i = 0; i < caverna->linhas; i++) {
        for (int j = 0; j < caverna->colunas; j++) {
            caverna->dp[i][j].vidaMaxima = -1;
        }
    }

    char aux[10]; // armazena temporariamente os valores que estao sendo lidos como string
    for (int i = 0; i < caverna->linhas; i++) {
        for (int j = 0; j < caverna->colunas; j++) {
            
            fscanf(arquivo, "%s", aux);

            if (aux[0] == 'F') {
                caverna->fim = (Ponto){i, j};
                caverna->matriz[i][j] = 0;
            }

            else if (aux[0] == 'I') {
                caverna->inicio = (Ponto){i, j};
                caverna->matriz[i][j] = 0;
            }

            else {
                caverna->matriz[i][j] = atoi(aux); // Converte da tabela ASCII para inteiro
            }
        }
    }

    fclose(arquivo);
    return caverna;
}

void escreverResultado(Caverna* caverna) {
    FILE* arquivo = fopen("resultado.txt", "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo de saida\n");
        return;
    }

    FILE *file_grafico = fopen("Grafico/dados_grafico.txt", "a"); // Abre e cria, se nao existir, o arquivo para escrever os dados que serao plotados no grafico
    if (file_grafico == NULL) {
        printf("Erro: arquivo para plotar o gráfico não foi aberto corretamente.\n"); 
        return; 
    }

    clock_t tempo_inicial = clock();
    int vidaFinal = encontrarMelhorCaminho(caverna, caverna->inicio.x, caverna->inicio.y);
    clock_t tempo_final = clock();
    double tempo_total = ((double)(tempo_final - tempo_inicial)) / CLOCKS_PER_SEC;
    if(caverna->linhas == caverna->colunas){
        fprintf(file_grafico, "%d %f\n", caverna->colunas, tempo_total); // O grafico so eh gerado para labirintos quadrados
    }
    if (vidaFinal <= 0) {
        fprintf(arquivo, "impossivel\n");
        //printf("Impossível encontrar caminho válido\n");
        //printf("Tempo de execução: %.3f segundos\n", tempo);
        fclose(arquivo);
        return;
    }

    Ponto* caminho = (Ponto*)malloc(caverna->linhas * caverna->colunas * sizeof(Ponto));
    if (!caminho) {
        fclose(arquivo);
        return;
    }

    int tamCaminho = 0;
    Ponto atual = caverna->inicio;

    // Constrói o caminho do início até o fim
    while (!(atual.x == caverna->fim.x && atual.y == caverna->fim.y)) {
        caminho[tamCaminho++] = atual;
        atual = caverna->dp[atual.x][atual.y].anterior;
    }
    caminho[tamCaminho++] = caverna->fim;

    // Imprime o caminho na ordem correta
    for (int i = 0; i < tamCaminho; i++) {
        fprintf(arquivo, "%d %d\n", caminho[i].x + 1, caminho[i].y);
    }

    /*
    PRINTS
    printf("Caminho encontrado com %d passos\n", tamCaminho);
    printf("Vida final: %d\n", vidaFinal);
    printf("Tempo de execução: %.3f segundos\n", tempo);
*/
    free(caminho);
    fclose(arquivo);
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    Caverna* caverna = lerArquivo(argv[1]);
    if (!caverna) {
        return 1;
    }

    escreverResultado(caverna);
    
    liberarCaverna(caverna);
    free(caverna);
    return 0;
}