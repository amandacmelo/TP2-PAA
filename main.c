#include "Caverna/Caverna.h"

Caverna* lerArquivo(const char* nomeArquivo) {

    // Abre o arquivo para leitura
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro: Não foi possível abrir o arquivo %s\n", nomeArquivo);
        return NULL;
    }

    // Aloca a estrutura da caverna
    Caverna* caverna = (Caverna*)malloc(sizeof(Caverna));
    if (!caverna) {
        fclose(arquivo);
        return NULL;
    }
    // Le as dimensoes da caverna e a vida inicial
    fscanf(arquivo, "%d %d %d", &caverna->linhas, &caverna->colunas, &caverna->vidaInicial);

    caverna->matriz = alocaMatriz(caverna->linhas, caverna->colunas);
    caverna->dp = alocaMatrizDP(caverna->linhas, caverna->colunas);
    
    if (!caverna->matriz || !caverna->dp) {
        liberaCaverna(caverna);
        free(caverna);
        fclose(arquivo);
        return NULL;
    }

    // Inicializa a matriz dp com valores negativos para indicar que ainda não foram calculados (visitados)
    for (int i = 0; i < caverna->linhas; i++) {
        for (int j = 0; j < caverna->colunas; j++) {
            caverna->dp[i][j].vidaMaxima = -1;
        }
    }

    char aux[10]; // armazena temporariamente os valores que estao sendo lidos como string
    for (int i = 0; i < caverna->linhas; i++) {
        for (int j = 0; j < caverna->colunas; j++) {
            
            fscanf(arquivo, "%s", aux); // Le o valor da celula

            // Verifica se a celula eh o inicio ou o fim (eles serao representados pelo valor 0 e suas coordenadas serao armazenadas)
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

void escreveResultado(Caverna* caverna) {
    // Abre o arquivo para escrita
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

    clock_t tempo_inicial = clock();  // Inicia a contagem do tempo

    int vidaFinal = encontraMelhorCaminho(caverna, caverna->inicio.x, caverna->inicio.y); // Encontra o melhor caminho

    clock_t tempo_final = clock();  // Finaliza a contagem do tempo
    double tempo_total = ((double)(tempo_final - tempo_inicial)) / CLOCKS_PER_SEC; // Calcula o tempo total

    if(caverna->linhas == caverna->colunas){
        fprintf(file_grafico, "%d %f\n", caverna->colunas, tempo_total); // O grafico so eh gerado para labirintos quadrados
    }

    // Se não houver caminho valido (vidas maior que 0), imprime impossível
    if (vidaFinal <= 0) {
        fprintf(arquivo, "impossível\n");
        fclose(arquivo);
        return;
    }

    Ponto* caminho = (Ponto*)malloc(caverna->linhas * caverna->colunas * sizeof(Ponto)); // Vetor que armazena o caminho feito pelo estudante
    if (!caminho) {
        fclose(arquivo);
        printf("Erro ao alocar memória para caminho\n");
        return;
    }

    int tamCaminho = 0; 
    Ponto atual = caverna->inicio;

    // Constroi o caminho do inicio ate o fim
    while (!(atual.x == caverna->fim.x && atual.y == caverna->fim.y)) {
        caminho[tamCaminho++] = atual;
        atual = caverna->dp[atual.x][atual.y].anterior;
    }
    caminho[tamCaminho++] = caverna->fim;

    // Imprime o caminho na ordem correta
    for (int i = 0; i < tamCaminho; i++) {
        fprintf(arquivo, "%d %d\n", caminho[i].x + 1, caminho[i].y);
    }

    free(caminho);
    fclose(arquivo);
}

void exibeCaminhoTerminal(Caverna* caverna) {
    FILE* arquivo = fopen("resultado.txt", "w"); // Abre o arquivo para escrita
    if (!arquivo) {
        printf("Erro ao abrir arquivo de saida\n");
        return;
    }

    FILE *file_grafico = fopen("Grafico/dados_grafico.txt", "a"); // Abre e cria, se nao existir, o arquivo para escrever os dados que serao plotados no grafico
    if (file_grafico == NULL) {
        printf("Erro: arquivo para plotar o gráfico não foi aberto corretamente.\n"); 
        return; 
    }

    clock_t tempo_inicial = clock(); // Inicia a contagem do tempo

    int vidaFinal = encontraMelhorCaminho(caverna, caverna->inicio.x, caverna->inicio.y);

    clock_t tempo_final = clock();  // Finaliza a contagem do tempo
    double tempo_total = ((double)(tempo_final - tempo_inicial)) / CLOCKS_PER_SEC; // Calcula o tempo total

    if(caverna->linhas == caverna->colunas){
        fprintf(file_grafico, "%d %f\n", caverna->colunas, tempo_total); // O grafico so eh gerado para labirintos quadrados
    }

    // Se nao houver caminho valido (vidas maior que 0), imprime impossível
    if (vidaFinal <= 0) {
        fprintf(arquivo, "impossível\n");
        printf("Impossível encontrar caminho válido\n");
        fclose(arquivo);
        return;
    }

    Ponto* caminho = (Ponto*)malloc(caverna->linhas * caverna->colunas * sizeof(Ponto)); // Vetor que armazena o caminho feito pelo estudante
    if (!caminho) {
        fclose(arquivo);
        return;
    }

    int tamCaminho = 0;
    Ponto atual = caverna->inicio;

    // Constroi o caminho do inicio até o fim
    while (!(atual.x == caverna->fim.x && atual.y == caverna->fim.y)) {
        caminho[tamCaminho++] = atual;
        atual = caverna->dp[atual.x][atual.y].anterior;
    }
    caminho[tamCaminho++] = caverna->fim;

    // Imprime o caminho na ordem correta
    for (int i = 0; i < tamCaminho; i++) {
        fprintf(arquivo, "%d %d\n", caminho[i].x + 1, caminho[i].y);
        
        // Usado para visualizacao do caminho no terminal

        // Marca o caminho feito pelo estudante com INT_MIN pois eh um valor muito baixo, logo nao sera estara como um valor valido (peso da matriz)
        caverna->matriz[caminho[i].x][caminho[i].y] = INT_MIN; 
        system("clear");
        printf("\nPercorrendo célula (%d, %d):\n\n", caminho[i].x+1, caminho[i].y);
        imprimeCaminho(caverna);
        sleep(1); // Pausa para visualização
    }
    
    printf("\nCaminho encontrado com %d passos\n", tamCaminho);
    printf("Vida final: %d\n", vidaFinal);
    free(caminho);
    fclose(arquivo);
}

int main(int argc, char* argv[]) {
    char *nomearquivo = "teste.txt"; // Deixa determinado arquivo teste.txt como padrao
    int exibir = 0;  // Deixa determinado que nao sera exibido o caminho no terminal, caso o usuario deseje, ele pode colocar o segundo argumento como 1

    if (argc > 1) { // Verifica se o usuario passou o nome do arquivo como argumento ou sera usado o padrao
        nomearquivo = argv[1]; 
        // Se o usuario passar o segundo argumento, ele sera usado para determinar se o caminho sera exibido no terminal
        if (argc > 2) {
            exibir = atoi(argv[2]);
        }
    }

    Caverna* caverna = lerArquivo(nomearquivo); 
    if (!caverna) {
        return 1;
    }

    // Se usuario colocar o segundo argumento como 1, o caminho eh imprimido no terminal
    if(exibir == 1) {
        exibeCaminhoTerminal(caverna); // Funcao que imprime o caminho no terminal e escreve no arquivo resultado.txt
    } else {
        escreveResultado(caverna); // Funcao que escreve o caminho no arquivo resultado.txt
    }

    liberaCaverna(caverna);
    free(caverna);
    return 0;
}