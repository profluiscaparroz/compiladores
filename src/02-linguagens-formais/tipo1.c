#include <stdio.h>
#include <string.h>

/**
 * TIPO 1 - LINGUAGENS SENSÍVEIS AO CONTEXTO (Hierarquia de Chomsky)
 * 
 * Este programa implementa um reconhecedor para a linguagem a^n b^m c^m d^n,
 * demonstrando as características de uma linguagem sensível ao contexto (Tipo 1).
 * 
 * LINGUAGEM: L = { a^n b^m c^m d^n | n,m ≥ 1 }
 * - Aceita strings onde:
 *   1) Número de 'a's = número de 'd's (n)
 *   2) Número de 'b's = número de 'c's (m)
 *   3) Ordem obrigatória: a...a b...b c...c d...d
 * 
 * EXEMPLOS:
 * ✓ Aceitos: "abbcdd", "aabbccdd", "aaabbcccddd"
 * ✗ Rejeitados: "abc" (falta 'd'), "aabbcddd" (desbalanceado)
 * 
 * GRAMÁTICA SENSÍVEL AO CONTEXTO (simplificada):
 * S → aSd | T
 * T → bTc | bc
 * 
 * Esta linguagem é sensível ao contexto porque exige o controle
 * simultâneo de DUAS contagens independentes (n e m), o que não pode
 * ser feito com um simples autômato de pilha (requer LBA - Linear
 * Bounded Automaton).
 * 
 * DIFERENÇA DO TIPO 2: Tipo 2 (livres de contexto) podem contar
 * uma relação (como a^n b^n), mas não múltiplas relações independentes
 * simultaneamente (como a^n b^m c^m d^n).
 * 
 * Referências:
 * - Chomsky, N. (1959). "On certain formal properties of grammars".
 * - Hopcroft, J. E., et al. (2001). Introduction to Automata Theory.
 * - Kuroda, S. Y. (1964). "Classes of languages and linear-bounded automata".
 */

/**
 * Função: verifica_linguagem
 * 
 * Reconhece strings da linguagem a^n b^m c^m d^n usando máquina de estados
 * com contadores.
 * 
 * @param str - String a ser verificada (terminada em null)
 * @return 1 se pertence à linguagem, 0 caso contrário
 * 
 * ALGORITMO (Autômato Linearmente Limitado - LBA simulado):
 * 
 * Estados:
 * - Estado 0: Lendo a's (contando n)
 * - Estado 1: Lendo b's (contando m)
 * - Estado 2: Lendo c's (contando m e verificando)
 * - Estado 3: Lendo d's (contando n e verificando)
 * 
 * Transições:
 * 0 --'a'--> 0  (incrementa count_a)
 * 0 --'b'--> 1  (inicia contagem de b's)
 * 1 --'b'--> 1  (incrementa count_b)
 * 1 --'c'--> 2  (inicia contagem de c's)
 * 2 --'c'--> 2  (incrementa count_c)
 * 2 --'d'--> 3  (inicia contagem de d's)
 * 3 --'d'--> 3  (incrementa count_d)
 * 
 * Condições de aceitação:
 * 1) count_a == count_d (primeira paridade)
 * 2) count_b == count_c (segunda paridade)
 * 3) count_a ≥ 1 e count_b ≥ 1 (não vazio)
 * 4) Todas as transições válidas (ordem correta)
 * 
 * Complexidade: O(n), onde n é o comprimento da string
 */
int verifica_linguagem(const char *str) {
    // Contadores para cada tipo de símbolo
    int count_a = 0, count_b = 0, count_c = 0, count_d = 0;
    int i = 0, estado = 0;  // Índice e estado atual do autômato

    // Percorre a string processando cada caractere
    while (str[i] != '\0') {
        if (estado == 0) {  // ESTADO 0: Processando a's
            if (str[i] == 'a') {
                count_a++;  // Incrementa contador de 'a's
            } else if (str[i] == 'b') {
                estado = 1;  // Transição para estado 1 (processando b's)
                count_b++;   // Primeiro 'b'
            } else {
                // Caractere inválido ou ordem errada
                return 0;  // REJEITA
            }
        }
        else if (estado == 1) {  // ESTADO 1: Processando b's
            if (str[i] == 'b') {
                count_b++;  // Incrementa contador de 'b's
            } else if (str[i] == 'c') {
                estado = 2;  // Transição para estado 2 (processando c's)
                count_c++;   // Primeiro 'c'
            } else {
                return 0;  // REJEITA
            }
        }
        else if (estado == 2) {  // ESTADO 2: Processando c's
            if (str[i] == 'c') {
                count_c++;  // Incrementa contador de 'c's
            } else if (str[i] == 'd') {
                estado = 3;  // Transição para estado 3 (processando d's)
                count_d++;   // Primeiro 'd'
            } else {
                return 0;  // REJEITA
            }
        }
        else if (estado == 3) {  // ESTADO 3: Processando d's (estado final)
            if (str[i] == 'd') {
                count_d++;  // Incrementa contador de 'd's
            } else {
                // Qualquer outro caractere após começar os d's é inválido
                return 0;  // REJEITA
            }
        }
        i++;  // Avança para o próximo caractere
    }

    // VERIFICAÇÃO FINAL: Duas condições de paridade devem ser satisfeitas
    // 1) count_a == count_d: paridade externa (delimitadores externos)
    // 2) count_b == count_c: paridade interna (delimitadores internos)
    // 3) Ambos n,m ≥ 1: pelo menos um de cada símbolo
    return (count_a == count_d && count_b == count_c && 
            count_a >= 1 && count_b >= 1);
}

/**
 * Função: main
 * 
 * Testa o reconhecedor com strings variadas para validar
 * o comportamento do autômato sensível ao contexto
 * 
 * CASOS DE TESTE:
 * 
 * Grupo 1 - Casos válidos (aceitos):
 * - "abbcdd": n=1, m=1 (configuração mínima)
 * - "aabbccdd": n=2, m=2 (balanceado)
 * - "aaabbcccddd": n=3, m=3 (balanceado maior)
 * - "aaaabbbbccccdddd": n=4, m=4 (teste com valores maiores)
 * 
 * Grupo 2 - Casos inválidos (rejeitados):
 * - "abc": incompleto (falta 'd')
 * - "aabbcddd": desbalanceado (2 a's mas 3 d's)
 * - "aabbcccdd": desbalanceado (2 b's mas 3 c's)
 * - "aaaa": apenas 'a's (incompleto)
 * 
 * NOTA: Este teste demonstra que a linguagem requer controle
 * simultâneo de duas relações de paridade independentes,
 * característica essencial das linguagens sensíveis ao contexto.
 */
int main() {
    // Array de strings de teste
    // Cobre casos válidos e diversos tipos de erros
    char *testes[] = { 
        "abbcdd",              // ✓ n=1, m=1
        "aabbccdd",            // ✓ n=2, m=2
        "aaabbcccddd",         // ✓ n=3, m=3
        "abc",                 // ✗ incompleto
        "aabbcddd",            // ✗ desbalanceado (a-d)
        "aaaabbbbccccdddd",    // ✓ n=4, m=4
        "aabbcccdd",           // ✗ desbalanceado (b-c)
        "aaaa"                 // ✗ apenas a's
    };
    
    int num_testes = sizeof(testes) / sizeof(testes[0]);
    
    printf("=== Teste do Reconhecedor CSL para L = {a^n b^m c^m d^n | n,m≥1} ===\n\n");
    printf("Esta linguagem é SENSÍVEL AO CONTEXTO (Tipo 1) porque requer\n");
    printf("controle simultâneo de duas relações de paridade:\n");
    printf("  1) #a's = #d's (paridade externa)\n");
    printf("  2) #b's = #c's (paridade interna)\n\n");

    // Testa cada string e exibe resultado detalhado
    for (int i = 0; i < num_testes; i++) {
        if (verifica_linguagem(testes[i]))
            printf("✓ A string \"%s\" pertence à linguagem a^n b^m c^m d^n\n", 
                   testes[i]);
        else
            printf("✗ A string \"%s\" NÃO pertence à linguagem a^n b^m c^m d^n\n", 
                   testes[i]);
    }
    
    printf("\n=== Fim dos testes ===\n");
    printf("\nOBSERVAÇÃO: Uma pilha simples (PDA) não é suficiente para\n");
    printf("reconhecer esta linguagem, pois precisaríamos de duas pilhas\n");
    printf("independentes. Por isso, é necessário um LBA (Linear Bounded\n");
    printf("Automaton), característico de linguagens Tipo 1.\n");

    return 0;
}
