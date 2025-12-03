#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da constante para o tamanho máximo das strings (nomes e pistas)
#define MAX_STR 50
// Tamanho da Tabela Hash (deve ser um número primo para melhor distribuição, mas para simplificação usamos 7)
#define HASH_SIZE 7

/* -------------------------------------------------------------------
 * 1. ESTRUTURAS DE DADOS
 * -------------------------------------------------------------------
 */

// --- 1.1 BST de Pistas Coletadas ---
typedef struct PistaNode {
    char pista[MAX_STR];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// --- 1.2 Mapa da Mansão (Árvore Binária) ---
typedef struct Sala {
    char nome[MAX_STR];
    char pista[MAX_STR]; // A pista associada à sala (se houver)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// --- 1.3 Tabela Hash (Lista Encadeada) ---
// Nó da lista encadeada para lidar com colisões na Tabela Hash
typedef struct HashEntry {
    char pista[MAX_STR];    // Chave (Pista)
    char suspeito[MAX_STR]; // Valor (Suspeito)
    struct HashEntry *next;
} HashEntry;

// A tabela hash é um array de ponteiros para o início das listas encadeadas
HashEntry *hashTable[HASH_SIZE];

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

    strncpy(novaSala->nome, nome, MAX_STR - 1);
    novaSala->nome[MAX_STR - 1] = '\0';

    strncpy(novaSala->pista, pista, MAX_STR - 1);
    novaSala->pista[MAX_STR - 1] = '\0';

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/* -------------------------------------------------------------------
 * 3. FUNÇÕES DA BST DE PISTAS (COLETADAS)
 * -------------------------------------------------------------------
 */

// Cria um nó para a BST de pistas
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
 * @brief Insere a pista coletada na Árvore Binária de Busca (BST).
 *
 * @param raiz O nó raiz atual da sub-árvore.
 * @param novaPista O conteúdo da pista a ser inserida.
 * @return O ponteiro para a raiz da sub-árvore atualizada.
 */
PistaNode* inserirPista(PistaNode *raiz, const char *novaPista) {
    if (raiz == NULL) {
        return criarPistaNode(novaPista);
    }

    int comparacao = strcmp(novaPista, raiz->pista);

    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, novaPista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, novaPista);
    } 
    // Ignora se a pista já existir (comparacao == 0)

    return raiz;
}

/**
 * @brief Exibe todas as pistas coletadas em ordem alfabética (In-order).
 *
 * @param raiz O nó raiz da BST de pistas.
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("  - %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/* -------------------------------------------------------------------
 * 4. FUNÇÕES DA TABELA HASH
 * -------------------------------------------------------------------
 */

// Função hash simples (soma dos valores ASCII modulo HASH_SIZE)
unsigned int hash(const char *key) {
    unsigned int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum += key[i];
    }
    return sum % HASH_SIZE;
}

/**
 * @brief Inicializa a tabela hash, definindo todos os ponteiros como NULL.
 */
void inicializarHash() {
    for (int i = 0; i < HASH_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

/**
 * @brief Insere a associação Pista/Suspeito na Tabela Hash.
 * Usa encadeamento para resolver colisões.
 *
 * @param pista A chave (Pista).
 * @param suspeito O valor (Suspeito).
 */
void inserirNaHash(const char *pista, const char *suspeito) {
    unsigned int index = hash(pista);

    // Cria um novo nó de entrada
    HashEntry *novaEntrada = (HashEntry*)malloc(sizeof(HashEntry));
    if (novaEntrada == NULL) {
        printf("Erro de alocação de memória para HashEntry!\n");
        return;
    }
    strncpy(novaEntrada->pista, pista, MAX_STR - 1);
    novaEntrada->pista[MAX_STR - 1] = '\0';
    strncpy(novaEntrada->suspeito, suspeito, MAX_STR - 1);
    novaEntrada->suspeito[MAX_STR - 1] = '\0';
    novaEntrada->next = NULL;

    // Insere no início da lista encadeada no índice hash
    novaEntrada->next = hashTable[index];
    hashTable[index] = novaEntrada;
}

/**
 * @brief Consulta o suspeito correspondente a uma pista na Tabela Hash.
 *
 * @param pista A chave (Pista) a ser procurada.
 * @return O nome do suspeito, ou NULL se a pista não for encontrada.
 */
const char* encontrarSuspeito(const char *pista) {
    unsigned int index = hash(pista);
    HashEntry *current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->pista, pista) == 0) {
            return current->suspeito; // Encontrado
        }
        current = current->next;
    }
    return NULL; // Não encontrado
}

/* -------------------------------------------------------------------
 * 5. FUNÇÃO DE JULGAMENTO FINAL
 * -------------------------------------------------------------------
 */

// Variável global para armazenar as pistas coletadas na BST
PistaNode *g_pistasColetadas = NULL; 

/**
 * @brief Função recursiva para contar quantas pistas coletadas (na BST) apontam para o suspeito acusado.
 *
 * @param raiz O nó raiz da BST de pistas coletadas.
 * @param suspeitoAcusado O nome do suspeito sendo verificado.
 * @return A contagem de pistas que apontam para o suspeito.
 */
int contarPistasParaSuspeito(PistaNode *raiz, const char *suspeitoAcusado) {
    if (raiz == NULL) {
        return 0;
    }

    int count = 0;

    // 1. Visita o nó atual (Raiz)
    const char *suspeitoDaPista = encontrarSuspeito(raiz->pista);
    if (suspeitoDaPista != NULL && strcmp(suspeitoDaPista, suspeitoAcusado) == 0) {
        count = 1; // 1 pista encontrada
    }

    // 2. Chama recursivamente para a esquerda e direita
    count += contarPistasParaSuspeito(raiz->esquerda, suspeitoAcusado);
    count += contarPistasParaSuspeito(raiz->direita, suspeitoAcusado);

    return count;
}


/**
 * @brief Conduz a fase de julgamento final.
 */
void verificarSuspeitoFinal() {
    char acusacao[MAX_STR];
    
    printf("\n\n=============== FASE DE ACUSAÇÃO ==============\n");
    
    // Lista as pistas coletadas
    if (g_pistasColetadas == NULL) {
        printf("Você não coletou nenhuma pista. Acusação impossível!\n");
        return;
    }

    printf("Pistas Coletadas (em ordem alfabética):\n");
    exibirPistas(g_pistasColetadas);
    
    // Solicita a acusação
    printf("\nCom base nas evidências, quem você acusa (ex: 'Mordomo', 'Jardineiro')? ");
    scanf(" %49[^\n]", acusacao); // Lê a linha inteira, limitando o tamanho

    // Inicia a contagem
    int count = contarPistasParaSuspeito(g_pistasColetadas, acusacao);

    printf("\n--- VEREDICTO ---\n");
    printf("Você acusou: **%s**\n", acusacao);
    printf("Número de pistas que apontam para %s: **%d**\n", acusacao, count);

    // Requisito: Pelo menos duas pistas
    if (count >= 2) {
        printf("\n **SUCESSO!** A evidência é robusta. Você tem **%d pistas** que sustentam a acusação contra %s.\n", count, acusacao);
        printf("Caso resolvido! Parabéns, Detetive!\n");
    } else {
        printf("\n **FALHA!** Sua acusação contra %s é frágil, pois você só possui %d pistas que o incriminam.\n", acusacao, count);
        printf("Volte e encontre mais evidências!\n");
    }
}

/* -------------------------------------------------------------------
 * 6. EXPLORAÇÃO INTERATIVA
 * -------------------------------------------------------------------
 */

/**
 * @brief Controla a navegação pela mansão e a coleta de pistas.
 *
 * @param hallDeEntrada O ponteiro para o nó inicial da árvore da mansão.
 */
void explorarSalas(Sala *hallDeEntrada) {
    Sala *atual = hallDeEntrada;
    char escolha;

    printf("\n--- Início da Exploração ---\n");
    
    while (atual != NULL) {
        printf("\n📍 Você está na sala: **%s**\n", atual->nome);

        // Verifica e coleta a pista, se houver
        if (strlen(atual->pista) > 0) {
            printf("✨ Pista encontrada! **\"%s\"**\n", atual->pista);
            
            // Verifica o suspeito associado à pista (usando a Tabela Hash)
            const char *suspeito = encontrarSuspeito(atual->pista);
            if (suspeito != NULL) {
                printf("  [Relacionado a: %s]\n", suspeito);
            }

            // Insere a pista na BST de pistas coletadas
            g_pistasColetadas = inserirPista(g_pistasColetadas, atual->pista);
            
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
        printf("  [s] Sair para fazer a acusação\n");
        
        printf("Sua escolha (e/d/s): ");
        scanf(" %c", &escolha);

        // Converte para minúsculo
        if (escolha >= 'A' && escolha <= 'Z') {
            escolha = escolha + ('a' - 'A');
        }

        // Processa a escolha
        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf(" Não há caminho para a esquerda.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf(" Não há caminho para a direita.\n");
            }
        } else if (escolha == 's') {
            printf("\n--- Exploração encerrada. Início da fase de julgamento. ---\n");
            break; 
        } else {
            printf(" Opção inválida. Tente novamente.\n");
        }
    }
}

/* -------------------------------------------------------------------
 * 7. FUNÇÕES DE LIMPEZA
 * -------------------------------------------------------------------
 */

// Libera a memória alocada para o mapa da mansão
void liberarMapa(Sala *sala) {
    if (sala != NULL) {
        liberarMapa(sala->esquerda);
        liberarMapa(sala->direita);
        free(sala);
    }
}

// Libera a memória alocada para a BST de pistas
void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// Libera a memória alocada para a Tabela Hash
void liberarHash() {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *current = hashTable[i];
        while (current != NULL) {
            HashEntry *temp = current;
            current = current->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }
}


/* -------------------------------------------------------------------
 * 8. FUNÇÃO PRINCIPAL (MAIN)
 * -------------------------------------------------------------------
 */

int main() {
    printf("==========================================\n");
    printf("  DETETIVE QUEST - NÍVEL MESTRE (C)       \n");
    printf("==========================================\n");

    // 1. Inicialização da Tabela Hash
    inicializarHash();
    
    // 2. Montagem da Tabela Hash (Associações Pista -> Suspeito)
    // Pistas que incriminam o Mordomo:
    inserirNaHash("Carta Rasgada", "Mordomo"); 
    inserirNaHash("Impressao Digital", "Mordomo");
    inserirNaHash("Chave Perdida", "Mordomo"); 
    
    // Pistas que incriminam o Jardineiro:
    inserirNaHash("Tesoura Enferrujada", "Jardineiro");
    inserirNaHash("Sapato Sujo", "Jardineiro");

    // Pistas que incriminam a Governanta:
    inserirNaHash("Copo Quebrado", "Governanta");
    inserirNaHash("Bilhete Rasgado", "Governanta");


    // 3. Montagem do Mapa da Mansão (Árvore Binária)
    // Sala: (Nome da Sala, Pista Associada)
    Sala *hallEntrada = criarSala("Hall de Entrada", "Impressao Digital");

    Sala *salaEstar = criarSala("Sala de Estar", "Copo Quebrado");
    Sala *cozinha = criarSala("Cozinha", "");
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita  = cozinha;

    Sala *biblioteca = criarSala("Biblioteca", "Carta Rasgada");
    Sala *escritorio = criarSala("Escritório", "Bilhete Rasgado");
    salaEstar->esquerda = biblioteca;
    salaEstar->direita  = escritorio;

    Sala *despensa = criarSala("Despensa", "Tesoura Enferrujada");
    Sala *jardim = criarSala("Jardim", "Chave Perdida");
    cozinha->esquerda = despensa;
    cozinha->direita  = jardim;

    Sala *quarto = criarSala("Quarto Principal", "Sapato Sujo");
    escritorio->direita = quarto;
    
    // 4. Início da exploração
    explorarSalas(hallEntrada);

    // 5. Fase de Julgamento
    verificarSuspeitoFinal();

    // 6. Liberação de memória
    liberarMapa(hallEntrada);
    liberarPistas(g_pistasColetadas);
    liberarHash();
    
    printf("\nFim do programa. Memória liberada.\n");
    
    return 0;
}