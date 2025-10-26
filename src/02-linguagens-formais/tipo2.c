#include <stdio.h>
#include <string.h>

#define MAX 100  // Tamanho máximo da pilha

/**
 * TIPO 2 - LINGUAGENS LIVRES DE CONTEXTO (Hierarquia de Chomsky)
 * 
 * Este programa implementa um reconhecedor para a linguagem a^n b^n
 * usando um Autômato de Pilha (Pushdown Automaton - PDA), demonstrando
 * as características de uma linguagem livre de contexto (Tipo 2).
 * 
 * LINGUAGEM: L = { a^n b^n | n ≥ 1 }
 * - Aceita strings com número igual de 'a's seguidos de 'b's
 * - Exemplos aceitos: "ab", "aabb", "aaabbb"
 * - Exemplos rejeitados: "ba", "abb", "aab", ""
 * 
 * GRAMÁTICA: S → aSb | ab
 * Esta é uma gramática livre de contexto que gera a linguagem.
 * 
 * MÉTODO: Autômato de Pilha
 * - Empilha cada 'a' encontrado
 * - Desempilha para cada 'b' encontrado
 * - Aceita se a pilha estiver vazia ao final
 * 
 * DIFERENÇA DO TIPO 3: Tipo 2 pode usar uma pilha para "lembrar"
 * informações, permitindo contagem não limitada, enquanto Tipo 3
 * está restrito a autômatos finitos (sem memória auxiliar).
 * 
 * Referências:
 * - Sipser, M. (2013). Introduction to the Theory of Computation.
 * - Hopcroft, J. E., et al. (2001). Introduction to Automata Theory.
 * - Aho, A. V., et al. (2006). Compilers: Principles, Techniques, and Tools.
 */

/**
 * Estrutura: Pilha
 * 
 * Implementa uma pilha (stack) usando um array estático.
 * A pilha é fundamental para reconhecer linguagens livres de contexto.
 * 
 * Campos:
 * - dados[MAX]: Array que armazena os elementos da pilha
 * - topo: Índice do elemento no topo da pilha (-1 indica pilha vazia)
 */
typedef struct {
    char dados[MAX];  // Array para armazenar elementos
    int topo;         // Índice do topo (-1 = vazia)
} Pilha;

/**
 * Função: inicializar_pilha
 * 
 * Inicializa uma pilha vazia
 * 
 * @param p - Ponteiro para a estrutura Pilha
 * 
 * Complexidade: O(1)
 */
void inicializar_pilha(Pilha *p) {
    p->topo = -1;  // -1 indica que a pilha está vazia
}

/**
 * Função: pilha_vazia
 * 
 * Verifica se a pilha está vazia
 * 
 * @param p - Ponteiro para a estrutura Pilha
 * @return 1 se vazia, 0 caso contrário
 * 
 * Complexidade: O(1)
 */
int pilha_vazia(Pilha *p) {
    return (p->topo == -1);
}

/**
 * Função: empilhar
 * 
 * Adiciona um elemento no topo da pilha (operação PUSH)
 * 
 * @param p - Ponteiro para a estrutura Pilha
 * @param c - Caractere a ser empilhado
 * 
 * Verifica se há espaço antes de inserir para evitar overflow.
 * 
 * Complexidade: O(1)
 */
void empilhar(Pilha *p, char c) {
    if (p->topo < MAX - 1) {  // Verifica se não ultrapassou o limite
        p->dados[++p->topo] = c;  // Incrementa topo e insere elemento
    }
    // Nota: Em produção, deveria tratar erro de overflow
}

/**
 * Função: desempilhar
 * 
 * Remove e retorna o elemento do topo da pilha (operação POP)
 * 
 * @param p - Ponteiro para a estrutura Pilha
 * @return Caractere removido, ou '\0' se a pilha estiver vazia
 * 
 * Complexidade: O(1)
 */
char desempilhar(Pilha *p) {
    if (!pilha_vazia(p)) {  // Verifica se há elementos
        return p->dados[p->topo--];  // Retorna elemento e decrementa topo
    }
    return '\0';  // Retorna null character se pilha vazia
}

/**
 * Função: reconhece_anbn
 * 
 * Reconhece strings da linguagem a^n b^n usando um Autômato de Pilha
 * 
 * @param str - String a ser verificada (terminada em null)
 * @return 1 se a string pertence à linguagem, 0 caso contrário
 * 
 * Algoritmo (Autômato de Pilha):
 * 1. Para cada 'a' na entrada: EMPILHA 'a' na pilha
 * 2. Para cada 'b' na entrada: 
 *    - Se pilha vazia, REJEITA (mais 'b's que 'a's)
 *    - Senão, DESEMPILHA um 'a'
 * 3. Ao final:
 *    - Se pilha vazia: ACEITA (mesmo número de 'a's e 'b's)
 *    - Se pilha não vazia: REJEITA (mais 'a's que 'b's)
 * 
 * INVARIANTE: A qualquer momento, o número de elementos na pilha
 * é igual ao número de 'a's não casados com 'b's
 * 
 * Complexidade: O(n), onde n é o comprimento da string
 */
int reconhece_anbn(const char *str) {
    Pilha pilha;
    inicializar_pilha(&pilha);  // Inicializa pilha vazia
    int i, len = strlen(str);
    
    // Rejeita string vazia (a linguagem requer n ≥ 1)
    if (len == 0) {
        return 0;  // REJEITA
    }

    // Percorre toda a string caractere por caractere
    for (i = 0; i < len; i++) {
        if (str[i] == 'a') {
            // REGRA 1: Para cada 'a', empilha na pilha
            empilhar(&pilha, 'a');
        } else if (str[i] == 'b') {
            // REGRA 2: Para cada 'b', deve desempilhar um 'a'
            if (pilha_vazia(&pilha)) {
                // Se não há 'a' para desempilhar, temos mais 'b's que 'a's
                return 0;  // REJEITA
            }
            desempilhar(&pilha);  // Remove um 'a' correspondente
        } else {
            // Caractere inválido (não é 'a' nem 'b')
            return 0;  // REJEITA
        }
    }

    // CONDIÇÃO FINAL: Pilha deve estar vazia
    // Se vazia: todos os 'a's foram casados com 'b's → ACEITA
    // Se não vazia: sobraram 'a's não casados → REJEITA
    return pilha_vazia(&pilha);
}

/**
 * Função: main
 * 
 * Testa o reconhecedor de Autômato de Pilha com várias strings
 * 
 * Demonstra a capacidade do PDA de reconhecer a^n b^n através
 * do uso de uma pilha para "contar" os símbolos.
 * 
 * CASOS DE TESTE:
 * ✓ Válidos: "ab" (n=1), "aabb" (n=2), "aaabbb" (n=3), "aaaabbbb" (n=4)
 * ✗ Inválidos: 
 *   - "abb": falta 1 'a' (mais 'b's que 'a's)
 *   - "ba": ordem errada ('b' antes de 'a')
 *   - "aab": falta 1 'b' (mais 'a's que 'b's)
 *   - "aaaaabb": 5 'a's mas apenas 2 'b's
 *   - "": string vazia (n=0, mas requeremos n≥1)
 */
int main() {
    // Array de strings de teste
    // Combina casos aceitos e rejeitados para validação completa
    char *testes[] = { 
        "ab",        // ✓ n=1: 1 'a' + 1 'b'
        "aabb",      // ✓ n=2: 2 'a's + 2 'b's
        "aaabbb",    // ✓ n=3: 3 'a's + 3 'b's
        "aaaabbbb",  // ✓ n=4: 4 'a's + 4 'b's
        "abb",       // ✗ 1 'a' + 2 'b's (desbalanceado)
        "ba",        // ✗ ordem invertida
        "aab",       // ✗ 2 'a's + 1 'b' (desbalanceado)
        "aaaaabb",   // ✗ 5 'a's + 2 'b's
        ""           // ✗ string vazia
    };
    
    int num_testes = sizeof(testes) / sizeof(testes[0]);
    
    printf("=== Teste do Reconhecedor PDA para L = {a^n b^n | n≥1} ===\n\n");
    
    // Testa cada string e exibe o resultado
    for (int i = 0; i < num_testes; i++) {
        if (reconhece_anbn(testes[i]))
            printf("✓ A string \"%s\" pertence à linguagem a^n b^n\n", testes[i]);
        else
            printf("✗ A string \"%s\" NÃO pertence à linguagem a^n b^n\n", testes[i]);
    }
    
    printf("\n=== Fim dos testes ===\n");
    
    return 0;
}
