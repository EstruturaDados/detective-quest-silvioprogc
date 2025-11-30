#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da constante para o tamanho máximo das strings (nomes e pistas)
#define MAX_STR 50

/* -------------------------------------------------------------------
 * 1. ESTRUTURAS DE DADOS
 * -------------------------------------------------------------------
 */

/**
 * @brief Estrutura para representar um Nó da Árvore de Pistas (BST).
 * Armazena a pista coletada e ponteiros para os filhos esquerdo e direito.
 */
typedef struct PistaNode {
    char pista[MAX_STR];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/**
 * @brief Estrutura para representar uma Sala da Mansão (Árvore Binária).
 * Armazena o nome da sala, a pista associada (opcional) e ponteiros para os caminhos.
 */
typedef struct Sala {
    char nome[MAX_STR];
    char pista[MAX_STR]; // A pista associada à sala (pode ser vazia "")
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* -------------------------------------------------------------------
 * 2. FUNÇÕES DO MAPA (SALA)
 * -------------------------------------------------------------------
 */

/**
 * @brief Cria e aloca dinamicamente uma nova sala com nome e pista opcional.
 *
 * @param nome O nome da sala.
 * @param pista O conteúdo da pista. Se não houver pista, passe uma string vazia ("").
 * @return Um ponteiro para a sala recém-criada.
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));

    if (novaSala == NULL) {
        printf("Erro de alocação de memória para Sala!\n");
        exit(EXIT_FAILURE);
    }

    // Copia nome e pista, garantindo o terminador nulo
    strncpy(novaSala->nome, nome, MAX_STR - 1);
    novaSala->nome[MAX_STR - 1] = '\0';

    strncpy(novaSala->pista, pista, MAX_STR - 1);
    novaSala->pista[MAX_STR - 1] = '\0';

    // Inicializa os caminhos como NULL
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/* -------------------------------------------------------------------
 * 3. FUNÇÕES DA BST DE PISTAS
 * -------------------------------------------------------------------
 */

/**
 * @brief Função auxiliar que cria um novo nó para a árvore de pistas.
 *
 * @param pista O conteúdo da pista.
 * @return Um ponteiro para o novo nó PistaNode.
 */
PistaNode* criarPistaNode(const char *pista) {
    PistaNode *novoNo = (PistaNode*)malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        printf("Erro de alocação de memória para PistaNode!\n");
        exit(EXIT_FAILURE);
    }
    strncpy(novoNo->pista, pista, MAX_STR - 1);
    novoNo->pista[MAX_STR - 1] = '\0';
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

/**
 * @brief Insere uma nova pista na Árvore Binária de Busca (BST) de forma recursiva.
 * Garante que as pistas fiquem ordenadas alfabeticamente.
 *
 * @param raiz O nó raiz atual da sub-árvore.
 * @param novaPista O conteúdo da pista a ser inserida.
 * @return O ponteiro para a raiz da sub-árvore atualizada.
 */
PistaNode* inserirPista(PistaNode *raiz, const char *novaPista) {
    // 1. Caso base: se a raiz é NULL, cria e retorna o novo nó.
    if (raiz == NULL) {
        return criarPistaNode(novaPista);
    }

    // 2. Compara a nova pista com a pista na raiz.
    int comparacao = strcmp(novaPista, raiz->pista);

    if (comparacao < 0) {
        // Se a nova pista é alfabeticamente menor, insere à esquerda.
        raiz->esquerda = inserirPista(raiz->esquerda, novaPista);
    } else if (comparacao > 0) {
        // Se a nova pista é alfabeticamente maior, insere à direita.
        raiz->direita = inserirPista(raiz->direita, novaPista);
    } 
    // Se comparacao == 0, a pista já existe; ignoramos a inserção (BST não permite duplicatas).

    return raiz;
}

/**
 * @brief Exibe todas as pistas coletadas em ordem alfabética.
 * Usa o percurso In-order (esquerda -> raiz -> direita) da BST.
 *
 * @param raiz O nó raiz da BST de pistas.
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        // 1. Percorre a sub-árvore esquerda (menores)
        exibirPistas(raiz->esquerda);

        // 2. Imprime a pista atual
        printf("  * %s\n", raiz->pista);

        // 3. Percorre a sub-árvore direita (maiores)
        exibirPistas(raiz->direita);
    }
}

/**
 * @brief Libera a memória alocada dinamicamente para a BST de pistas.
 *
 * @param raiz O nó raiz da BST.
 */
void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

/* -------------------------------------------------------------------
 * 4. FUNÇÃO DE EXPLORAÇÃO
 * -------------------------------------------------------------------
 */

/**
 * @brief Permite ao jogador navegar pela mansão, coletando pistas e inserindo-as na BST.
 *
 * @param hallDeEntrada O ponteiro para o nó inicial da árvore da mansão.
 * @param pistasColetadas O ponteiro para a raiz da BST de pistas. Passado por referência.
 */
void explorarSalasComPistas(Sala *hallDeEntrada, PistaNode **pistasColetadas) {
    Sala *atual = hallDeEntrada;
    char escolha;

    printf("\n--- Início da Exploração ---\n");
    
    // O loop de exploração continua até que o jogador digite 's'
    while (atual != NULL) {
        printf("\n Você está na sala: **%s**\n", atual->nome);

        // Verifica e coleta a pista, se houver
        if (strlen(atual->pista) > 0) {
            printf("✨ Pista encontrada! **\"%s\"**\n", atual->pista);
            // Insere a pista na BST (função recursiva)
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
            // Limpa a pista na sala para que não seja coletada novamente
            atual->pista[0] = '\0';
        } else {
            printf("A sala está limpa. Nenhuma pista nova por aqui.\n");
        }

        // Exibe as opções de caminhos disponíveis
        printf("\nOpções de navegação:\n");
        if (atual->esquerda != NULL) {
            printf("  [e] Esquerda\n");
        }
        if (atual->direita != NULL) {
            printf("  [d] Direita\n");
        }
        printf("  [s] Sair e conferir as pistas\n");
        
        printf("Sua escolha (e/d/s): ");
        scanf(" %c", &escolha);

        // Converte para minúsculo
        if (escolha >= 'A' && escolha <= 'Z') {
            escolha = escolha + ('a' - 'A');
        }

        // Processa a escolha
        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda; // Move para a esquerda
            } else {
                printf(" Não há caminho para a esquerda.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita; // Move para a direita
            } else {
                printf(" Não há caminho para a direita.\n");
            }
        } else if (escolha == 's') {
            printf("\n--- Exploração encerrada. Preparando o relatório de pistas. ---\n");
            break; // Sai do loop
        } else {
            printf(" Opção inválida. Tente novamente.\n");
        }
    }
}

/**
 * @brief Libera a memória alocada dinamicamente para o mapa da mansão.
 *
 * @param sala A sala (nó) a ser liberada.
 */
void liberarMapa(Sala *sala) {
    if (sala == NULL) {
        return;
    }
    liberarMapa(sala->esquerda);
    liberarMapa(sala->direita);
    free(sala);
}

/* -------------------------------------------------------------------
 * 5. FUNÇÃO PRINCIPAL (MAIN)
 * -------------------------------------------------------------------
 */

int main() {
    // Ponteiro para a raiz da BST de pistas, inicializado como NULL
    PistaNode *pistasColetadas = NULL;
    
    printf("==========================================\n");
    printf("  DETETIVE QUEST - COLETA DE PISTAS (C)   \n");
    printf("==========================================\n");

    // 1. Montagem do mapa da mansão (Árvore Binária)
    // Sala: (Nome da Sala, Pista Associada)
    Sala *hallEntrada = criarSala("Hall de Entrada", "Impressao Digital");

    // Nível 1
    Sala *salaEstar = criarSala("Sala de Estar", "Copo Quebrado");
    Sala *cozinha = criarSala("Cozinha", ""); // Sem pista
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita  = cozinha;

    // Nível 2
    salaEstar->esquerda = criarSala("Biblioteca", "Relogio Parado");
    salaEstar->direita  = criarSala("Escritório", "Bilhete Rasgado"); 

    cozinha->esquerda = criarSala("Despensa", "Faca Sumida");
    cozinha->direita  = criarSala("Jardim", "Pecas de Xadrez"); // Nó folha

    // Nível 3 (Filho do Escritório)
    salaEstar->direita->direita = criarSala("Quarto Principal", "Sapato Sujo"); // Nó folha
    
    // 
    // 2. Início da exploração
    // Passamos o ponteiro da BST por endereço (&pistasColetadas) para que a função possa modificar a raiz.
    explorarSalasComPistas(hallEntrada, &pistasColetadas);

    // 3. Exibição do relatório final
    printf("\n\n=============== RELATÓRIO FINAL DE PISTAS ==============\n");
    if (pistasColetadas != NULL) {
        printf("As seguintes pistas foram coletadas, em ordem alfabética:\n");
        exibirPistas(pistasColetadas);
    } else {
        printf("Nenhuma pista foi coletada durante a exploração.\n");
    }
    printf("========================================================\n");

    // 4. Liberação de memória
    liberarMapa(hallEntrada);
    liberarPistas(pistasColetadas);
    
    printf("\nFim do programa. Memória liberada.\n");
    
    return 0;
}