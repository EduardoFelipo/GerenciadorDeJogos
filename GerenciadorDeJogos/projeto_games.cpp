#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "jogos.txt"  // Nome do arquivo para armazenar os dados

// Estrutura de dados para o n� da fila
typedef struct Nodo {
    char nome[100];
    char progresso[20];
    struct Nodo* proximo;
} Nodo;

// Fun��o para enfileirar (adicionar um novo jogo � fila)
void enfileirar(Nodo** frente, Nodo** tras, const char* nome, const char* progresso) {
    Nodo* novoJogo = (Nodo*) malloc(sizeof(Nodo));
    if (!novoJogo) {
        printf("Erro ao alocar mem�ria.\n");
        return;
    }
    strcpy(novoJogo->nome, nome);
    strcpy(novoJogo->progresso, progresso);
    novoJogo->proximo = NULL;

    if (*tras) {
        (*tras)->proximo = novoJogo;
    } else {
        *frente = novoJogo;  // Se a fila estiver vazia, o novo jogo � o primeiro
    }
    *tras = novoJogo;  // O novo jogo se torna o final da fila
}

// Fun��o para salvar os jogos no arquivo
void salvarJogos(Nodo* frente) {
    FILE* arquivo = fopen(ARQUIVO, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os jogos.\n");
        return;
    }

    Nodo* temp = frente;
    while (temp != NULL) {
        fprintf(arquivo, "%s;%s\n", temp->nome, temp->progresso);
        temp = temp->proximo;
    }
    fclose(arquivo);
}

// Fun��o para carregar os jogos do arquivo
int carregarJogos(Nodo** frente, Nodo** tras) {
    FILE* arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        return 0;
    }

    int numJogos = 0;
    char nome[100], progresso[20];
    while (fscanf(arquivo, "%99[^;];%19[^\n]\n", nome, progresso) == 2) {
        enfileirar(frente, tras, nome, progresso);
        numJogos++;
    }

    fclose(arquivo);
    return numJogos;
}

// Fun��o para cadastrar um novo jogo
void cadastrarJogo(Nodo** frente, Nodo** tras) {
    char nome[100], progresso[20];
    printf("Digite o nome do jogo: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';  // Remover o '\n' no final do nome

    printf("Qual o seu progresso no jogo '%s'? (1 para 'terminado' / 2 para 'jogando'): ", nome);
    int escolha;
    while (1) {
        if (scanf("%d", &escolha) != 1 || (escolha != 1 && escolha != 2)) {
            printf("Entrada inv�lida! Tente novamente. Digite 1 para 'terminado' ou 2 para 'jogando': ");
            while (getchar() != '\n');  // Limpa o buffer
            continue;
        }

        if (escolha == 1) {
            strcpy(progresso, "terminado");
        } else {
            strcpy(progresso, "jogando");
        }
        break;
    }

    enfileirar(frente, tras, nome, progresso);
    salvarJogos(*frente);
}

// Fun��o para editar o progresso de um jogo
void editarProgresso(Nodo* frente) {
    if (frente == NULL) {
        printf("Nenhum jogo para editar.\n");
        return;
    }

    printf("\nJogos cadastrados:\n");
    Nodo* temp = frente;
    int indice = 1;
    while (temp != NULL) {
        printf("%d. Jogo: %s | Progresso: %s\n", indice++, temp->nome, temp->progresso);
        temp = temp->proximo;
    }

    int escolha;
    printf("Escolha o n�mero do jogo para editar o progresso: ");
    if (scanf("%d", &escolha) != 1 || escolha < 1) {
        printf("Entrada inv�lida!\n");
        return;
    }

    temp = frente;
    for (int i = 1; i < escolha && temp != NULL; i++) {
        temp = temp->proximo;
    }

    if (temp == NULL) {
        printf("Jogo n�o encontrado!\n");
        return;
    }

    printf("Jogo selecionado: %s\n", temp->nome);
    int novoProgresso;
    printf("Digite 1 para 'terminado' ou 2 para 'jogando': ");
    while (1) {
        if (scanf("%d", &novoProgresso) != 1 || (novoProgresso != 1 && novoProgresso != 2)) {
            printf("Entrada inv�lida! Tente novamente. Digite 1 para 'terminado' ou 2 para 'jogando': ");
            while (getchar() != '\n');  // Limpa o buffer
            continue;
        }

        if (novoProgresso == 1) {
            strcpy(temp->progresso, "terminado");
        } else {
            strcpy(temp->progresso, "jogando");
        }

        break;
    }

    printf("Progresso do jogo '%s' atualizado para: %s\n", temp->nome, temp->progresso);
    salvarJogos(frente);
}

// Fun��o para exibir todos os jogos cadastrados
void exibirJogos(Nodo* frente) {
    if (frente == NULL) {
        printf("Nenhum jogo cadastrado ainda.\n");
        return;
    }

    printf("\nJogos cadastrados:\n");
    Nodo* temp = frente;
    while (temp != NULL) {
        printf("Jogo: %s | Progresso: %s\n", temp->nome, temp->progresso);
        temp = temp->proximo;
    }
}

// Fun��o para mostrar o menu e capturar a op��o escolhida
void menu() {
    printf("\nMenu de Op��es:\n");
    printf("1. Cadastrar um jogo\n");
    printf("2. Exibir jogos cadastrados\n");
    printf("3. Editar o progresso de um jogo\n");
    printf("4. Sair\n");
    printf("Escolha uma op��o: ");
}

int main() {
    Nodo* frente = NULL;  // Ponteiro para o primeiro jogo na fila
    Nodo* tras = NULL;    // Ponteiro para o �ltimo jogo na fila
    int opcao;            // Vari�vel para armazenar a op��o escolhida

    int numJogos = carregarJogos(&frente, &tras);  // Carrega os jogos j� cadastrados do arquivo

    while (1) {
        menu();  // Exibe o menu

        // Ler a op��o do menu
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inv�lida! Tente novamente.\n");
            while (getchar() != '\n');  // Limpa o buffer do teclado
            continue;
        }

        while (getchar() != '\n');  // Limpa o buffer de entrada

        switch (opcao) {
            case 1:
                cadastrarJogo(&frente, &tras);
                break;
            case 2:
                exibirJogos(frente);
                break;
            case 3:
                editarProgresso(frente);  // Edita o progresso do jogo
                break;
            case 4:
                printf("Saindo do programa...\n");
                free(frente);  // Libera a mem�ria alocada
                return 0;
            default:
                printf("Op��o inv�lida. Tente novamente.\n");
        }
    }

    return 0;
}

