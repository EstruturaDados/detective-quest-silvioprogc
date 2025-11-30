#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Definição da estrutura para representar uma Sala (nó da árvore binária).
 * Cada sala tem um nome e ponteiros para as salas à esquerda e à direita.
 */
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/**
 * @brief Cria e aloca dinamicamente uma nova sala com o nome especificado.
 *
 * @param nome O nome da nova sala.
 * @return Um ponteiro para a sala recém-criada.
 */
Sala* criarSala(const char *nome) {
    // 1. Aloca memória para a nova sala usando malloc.
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));

    // Verifica se a alocação foi bem-sucedida.
    if (novaSala == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(EXIT_FAILURE);
    }

    // 2. Copia o nome para o campo 'nome' da estrutura.
    // Usamos strncpy para evitar overflow.
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0'; // Garante o terminador nulo

    // 3. Inicializa os ponteiros dos filhos como NULL (sem caminhos inicialmente).
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * @brief Permite ao jogador navegar interativamente pela mansão (árvore binária).
 * A exploração continua até que o jogador chegue a um cômodo sem saída (nó-folha).
 *
 * @param atual O ponteiro para a sala (nó) atual onde o jogador está.
 */
void explorarSalas(Sala *atual) {
    char escolha;
    
    // O loop continua enquanto o jogador não chegar a uma sala sem camidas (nó folha)
    // ou não escolher 's' (sair)
    while (atual != NULL) {
        printf("\n Você está na sala: **%s**\n", atual->nome);

        // Verifica se é um cômodo sem saídas (nó-folha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho!  Este cômodo não tem mais saídas.\n");
            break; // Sai do loop, encerrando a exploração
        }

        printf("Opções de navegação:\n");
        
        // Exibe as opções de caminhos disponíveis
        if (atual->esquerda != NULL) {
            printf("  [e] Esquerda (para %s)\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf("  [d] Direita (para %s)\n", atual->direita->nome);
        }
        printf("  [s] Sair da exploração\n");
        
        printf("Sua escolha (e/d/s): ");
        // Limpa o buffer de entrada e lê a escolha do usuário
        scanf(" %c", &escolha);

        // Converte a escolha para minúsculo para facilitar a comparação
        if (escolha >= 'A' && escolha <= 'Z') {
            escolha = escolha + ('a' - 'A');
        }

        // Processa a escolha do jogador
        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda; // Move para a esquerda
            } else {
                printf(" Não há caminho para a esquerda a partir desta sala.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita; // Move para a direita
            } else {
                printf(" Não há caminho para a direita a partir desta sala.\n");
            }
        } else if (escolha == 's') {
            printf("Exploração encerrada pelo jogador. 👋\n");
            break; // Sai do loop
        } else {
            printf(" Opção inválida. Tente novamente.\n");
        }
    }
}

/**
 * @brief Libera a memória alocada dinamicamente para a árvore.
 * (Importante para evitar vazamento de memória).
 *
 * @param sala A sala (nó) a ser liberada.
 */
void liberarSalas(Sala *sala) {
    if (sala == NULL) {
        return;
    }
    // Percorre a árvore em pós-ordem para liberar os nós filhos primeiro
    liberarSalas(sala->esquerda);
    liberarSalas(sala->direita);
    
    // Libera o nó atual
    free(sala);
}

/**
 * @brief Função principal. Monta o mapa inicial da mansão (árvore) e inicia a exploração.
 */
int main() {
    printf("==========================================\n");
    printf("   DETETIVE QUEST - MAPA DA MANSÃO (C)    \n");
    printf("==========================================\n");
    printf("Bem-vindo(a) ao Hall de Entrada! Sua missão é explorar a mansão.\n");

    // 1. Montagem do mapa da mansão (árvore binária manual)
    // A complexidade do mapa pode ser alterada aqui, seguindo a estrutura de uma árvore.
    
    Sala *hallEntrada = criarSala("Hall de Entrada");

    // Nível 1
    hallEntrada->esquerda = criarSala("Sala de Estar");
    hallEntrada->direita  = criarSala("Cozinha");

    // Nível 2 (Filhos da Sala de Estar)
    hallEntrada->esquerda->esquerda = criarSala("Biblioteca"); // Nó folha
    hallEntrada->esquerda->direita  = criarSala("Escritório"); // Não tem caminho a esquerda

    // Nível 3 (Filhos do Escritório)
    // Sala sem caminho à esquerda (NULL)
    hallEntrada->esquerda->direita->direita = criarSala("Quarto Principal"); // Nó folha

    // Nível 2 (Filhos da Cozinha)
    hallEntrada->direita->esquerda = criarSala("Dispensa"); // Nó folha
    hallEntrada->direita->direita  = criarSala("Jardim"); // Nó folha
    
    // 
    // 2. Início da exploração
    explorarSalas(hallEntrada);

    // 3. Liberação de memória
    liberarSalas(hallEntrada);
    
    printf("\nFim do programa. Memória liberada com sucesso.\n");
    
    return 0;
}