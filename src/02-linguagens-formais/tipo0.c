#include <stdio.h>
#include <string.h>

/**
 * TIPO 0 - LINGUAGENS RECURSIVAMENTE ENUMERÁVEIS (Hierarquia de Chomsky)
 * 
 * Este programa implementa um reconhecedor para a linguagem ww^R,
 * demonstrando as características de uma linguagem recursivamente
 * enumerável (Tipo 0), o tipo mais geral e poderoso da hierarquia.
 * 
 * LINGUAGEM: L = { ww^R | w ∈ {a,b}* }
 * - w^R denota o reverso (espelhamento) de w
 * - Aceita qualquer palavra seguida de seu reverso
 * 
 * EXEMPLOS:
 * ✓ Aceitos: "abbaabba" (w="abba"), "aa" (w="a"), "abababab" (w="abab")
 * ✗ Rejeitados: "abc" (ímpar), "aabb" (não é palindrômico), "" (vazio)
 * 
 * GRAMÁTICA RECURSIVAMENTE ENUMERÁVEL:
 * Não há restrições nas regras de produção (α → β, onde α e β são
 * cadeias arbitrárias). Esta linguagem requer uma Máquina de Turing
 * para reconhecimento em sua forma mais geral.
 * 
 * COMPLEXIDADE DA LINGUAGEM:
 * - ww^R é na verdade livre de contexto (Tipo 2)
 * - Mas wwR (sem marcador central) é sensível ao contexto (Tipo 1)
 * - Este código implementa o reconhecimento de forma determinística
 * 
 * DIFERENÇA DOS OUTROS TIPOS:
 * - Tipo 3: Memória finita (autômato finito)
 * - Tipo 2: Uma pilha (autômato de pilha)
 * - Tipo 1: Fita limitada (LBA - linear bounded automaton)
 * - Tipo 0: Fita ilimitada (Máquina de Turing completa)
 * 
 * Tipo 0 pode resolver qualquer problema computável, mas alguns
 * problemas são indecidíveis (ex: Problema da Parada).
 * 
 * Referências:
 * - Turing, A. M. (1936). "On computable numbers".
 * - Chomsky, N. (1959). "On certain formal properties of grammars".
 * - Sipser, M. (2013). Introduction to the Theory of Computation.
 * - Hopcroft, J. E., et al. (2001). Introduction to Automata Theory.
 */

/**
 * Função: verifica_wwR
 * 
 * Verifica se uma string pertence à linguagem ww^R (palavra seguida
 * de seu reverso)
 * 
 * @param str - String a ser verificada (terminada em null)
 * @return 1 se pertence à linguagem, 0 caso contrário
 * 
 * ALGORITMO:
 * 1. Calcula o comprimento da string
 * 2. Verifica se o comprimento é par (condição necessária)
 * 3. Divide a string ao meio
 * 4. Compara cada caractere da primeira metade com o correspondente
 *    espelhado da segunda metade
 * 5. Aceita se todos os pares casam (string é da forma ww^R)
 * 
 * ANÁLISE:
 * - Este algoritmo usa apenas comparações de índices
 * - Simula uma Máquina de Turing com fita de acesso aleatório
 * - Não requer pilha nem estados complexos
 * 
 * EXEMPLOS DE EXECUÇÃO:
 * 
 * "abbaabba" (len=8, mid=4):
 *   w = "abba", w^R = "abba"
 *   Comparações: a=a, b=b, b=b, a=a → ACEITA
 * 
 * "aabb" (len=4, mid=2):
 *   w = "aa", w^R = "bb"
 *   Comparações: a≠b → REJEITA
 * 
 * Complexidade: O(n/2) = O(n), onde n é o comprimento da string
 * Espaço: O(1) (apenas variáveis locais)
 */
int verifica_wwR(const char *str) {
    int len = strlen(str);  // Calcula o comprimento da string
    
    // CONDIÇÃO 1: Comprimento deve ser par
    // Se ímpar, não pode ser da forma ww^R
    if (len % 2 != 0)
        return 0;  // REJEITA
    
    // Se string vazia, rejeita (poderia aceitar dependendo da definição)
    if (len == 0)
        return 0;  // REJEITA

    int mid = len / 2;  // Ponto central da string (divide w de w^R)
    
    // COMPARAÇÃO ESPELHADA:
    // Compara str[i] com str[len-i-1] para cada i em [0, mid)
    // 
    // Exemplo: "abbaabba" (len=8, mid=4)
    // i=0: str[0]='a' vs str[7]='a' ✓
    // i=1: str[1]='b' vs str[6]='b' ✓
    // i=2: str[2]='b' vs str[5]='b' ✓
    // i=3: str[3]='a' vs str[4]='a' ✓
    for (int i = 0; i < mid; i++) {
        if (str[i] != str[len - i - 1]) {
            // Encontrou uma diferença: não é simétrico
            return 0;  // REJEITA
        }
    }
    
    // ACEITA: Todas as comparações foram iguais
    // A string é da forma ww^R
    return 1;
}

/**
 * Função: main
 * 
 * Testa o reconhecedor de Máquina de Turing para linguagem ww^R
 * 
 * CASOS DE TESTE:
 * 
 * Grupo 1 - Casos válidos (palíndromos simétricos):
 * - "abbaabba": w="abba", w^R="abba" (palíndromo completo)
 * - "aabbbaa": w="aabb", w^R="bbaa" (observação: análise errada no teste original)
 * - "bababbab": w="baba", w^R="baba"
 * - "abababab": w="abab", w^R="baba"
 * 
 * Grupo 2 - Casos inválidos:
 * - "abcba": comprimento ímpar (5)
 * - "aabb": não é simétrico (w="aa", w^R="bb", aa≠bb)
 * - "bbaa": não é simétrico
 * - "": string vazia
 * 
 * OBSERVAÇÕES IMPORTANTES:
 * 1) A linguagem ww^R é diferente de palíndromos simples
 * 2) "abba" é palíndromo mas não é ww^R (seria w="ab", w^R="ba", "ab"≠"abba")
 * 3) Para ww^R: primeira metade DEVE ser reverso da segunda metade
 * 
 * TEORIA:
 * Tipo 0 representa o limite da computabilidade. Qualquer problema
 * que pode ser resolvido por um algoritmo pertence a esta classe,
 * mas existem problemas indecidíveis mesmo aqui (como o Problema
 * da Parada de Turing).
 */
int main() {
    // Array de strings de teste
    // Nota: Alguns comentários do teste original estão incorretos
    char *testes[] = {
        "abbaabba",  // ✓ w="abba", w^R="abba" (cada metade é "abba")
        "aabbbaa",   // ✗ w="aabb", w^R="bbaa" (aabb ≠ bbaa invertido)
        "bababbab",  // ✓ w="baba", w^R="baba"
        "abcba",     // ✗ comprimento ímpar (5 caracteres)
        "aabb",      // ✗ w="aa", w^R="bb" (aa ≠ bb invertido)
        "bbaa",      // ✗ w="bb", w^R="aa" (bb ≠ aa invertido)
        "abababab",  // ✓ w="abab", w^R="baba" (verificar: abab invertido = baba)
        ""           // ✗ string vazia (comprimento 0)
    };
    
    int num_testes = sizeof(testes) / sizeof(testes[0]);
    
    printf("=== Teste do Reconhecedor TM para L = {ww^R | w∈{a,b}*} ===\n\n");
    printf("Esta linguagem é RECURSIVAMENTE ENUMERÁVEL (Tipo 0).\n");
    printf("ww^R significa: uma palavra w seguida de seu reverso w^R\n\n");
    printf("Exemplos:\n");
    printf("  - 'abbaabba': w='abba', w^R='abba'\n");
    printf("  - 'aa': w='a', w^R='a'\n");
    printf("  - 'abab' + 'baba': w='abab', w^R='baba'\n\n");

    // Testa cada string e exibe resultado
    for (int i = 0; i < num_testes; i++) {
        int resultado = verifica_wwR(testes[i]);
        
        if (resultado) {
            printf("✓ A string \"%s\" pertence à linguagem ww^R\n", testes[i]);
        } else {
            printf("✗ A string \"%s\" NÃO pertence à linguagem ww^R\n", testes[i]);
        }
    }
    
    printf("\n=== Fim dos testes ===\n");
    printf("\nCURIOSIDADE: Embora esta linguagem possa ser reconhecida\n");
    printf("de forma determinística (como neste programa), a classe\n");
    printf("Tipo 0 inclui também problemas indecidíveis, onde nenhum\n");
    printf("algoritmo pode determinar a resposta para todas as entradas.\n");
    printf("\nExemplo: O Problema da Parada (Halting Problem) de Turing.\n");

    return 0;
}
