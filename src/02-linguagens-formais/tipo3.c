#include <stdio.h>
#include <string.h>

/**
 * TIPO 3 - LINGUAGENS REGULARES (Hierarquia de Chomsky)
 * 
 * Este programa implementa um reconhecedor para a linguagem a^n b^n,
 * demonstrando as características de uma linguagem regular (Tipo 3).
 * 
 * NOTA IMPORTANTE: Tecnicamente, a^n b^n NÃO é uma linguagem regular,
 * mas sim livre de contexto (Tipo 2). Este código foi mantido como está
 * no repositório original, mas serve para ilustrar uma abordagem de
 * autômato finito com contadores.
 * 
 * LINGUAGEM: L = { a^n b^n | n ≥ 1 }
 * - Aceita strings com número igual de 'a's seguidos de 'b's
 * - Exemplos aceitos: "ab", "aabb", "aaabbb"
 * - Exemplos rejeitados: "ba", "abb", "aab"
 * 
 * MÉTODO: Autômato finito com estados e contadores
 * - Estado 0: Lendo 'a's
 * - Estado 1: Lendo 'b's
 * 
 * Referências:
 * - Sipser, M. (2013). Introduction to the Theory of Computation.
 * - Hopcroft, J. E., et al. (2001). Introduction to Automata Theory.
 */

/**
 * Função: verifica_anbn
 * 
 * Verifica se a string de entrada pertence à linguagem a^n b^n
 * 
 * @param str - String a ser verificada (terminada em null)
 * @return 1 se a string pertence à linguagem, 0 caso contrário
 * 
 * Algoritmo:
 * 1. Usa uma máquina de estados com 2 estados:
 *    - Estado 0: Processa todos os 'a's
 *    - Estado 1: Processa todos os 'b's
 * 2. Conta separadamente os 'a's e 'b's
 * 3. Verifica se as quantidades são iguais ao final
 * 4. Rejeita se encontrar caracteres fora de ordem ou inválidos
 */
int verifica_anbn(const char *str) {
    int count_a = 0, count_b = 0;  // Contadores para 'a' e 'b'
    int i = 0, estado = 0;          // Índice e estado atual
    
    // Percorre a string caractere por caractere
    while (str[i] != '\0') {
        if (estado == 0) {  // Estado 0: Esperando 'a's ou primeira ocorrência de 'b'
            if (str[i] == 'a') {
                count_a++;  // Incrementa contador de 'a's
            } else if (str[i] == 'b') {
                estado = 1;  // Transição para estado 1 (lendo 'b's)
                count_b++;   // Incrementa contador de 'b's
            } else {
                return 0;  // Caractere inválido - rejeita a string
            }
        } else if (estado == 1) {  // Estado 1: Esperando apenas 'b's
            if (str[i] == 'b') {
                count_b++;  // Incrementa contador de 'b's
            } else {
                return 0;  // Encontrou 'a' após 'b' ou caractere inválido - rejeita
            }
        }
        i++;  // Avança para o próximo caractere
    }
    
    // Aceita se: número de 'a's == número de 'b's E pelo menos um de cada
    // Garante que não aceita string vazia
    return (count_a == count_b && count_a > 0);
}

/**
 * Função: main
 * 
 * Testa o reconhecedor com várias strings de exemplo
 * 
 * Casos de teste incluem:
 * - Strings válidas: "ab", "aabb", "aaabbb", "aaaabbbb"
 * - Strings inválidas: "abb" (falta 'a'), "ba" (ordem errada),
 *   "aabbb" (mais 'b's), "aaaaabb" (mais 'a's), "bbb" (só 'b's), "" (vazia)
 */
int main() {
    // Array de strings para teste
    // Inclui casos positivos (aceitos) e negativos (rejeitados)
    char *testes[] = { 
        "ab",        // Válido: n=1
        "aabb",      // Válido: n=2
        "aaabbb",    // Válido: n=3
        "aaaabbbb",  // Válido: n=4
        "abb",       // Inválido: falta um 'a'
        "ba",        // Inválido: ordem invertida
        "aabbb",     // Inválido: 'b's em excesso
        "aaaaabb",   // Inválido: 'a's em excesso
        "bbb",       // Inválido: apenas 'b's
        ""           // Inválido: string vazia
    };
    
    // Calcula o número de testes no array
    int num_testes = sizeof(testes) / sizeof(testes[0]);
    
    // Itera sobre todos os casos de teste
    for (int i = 0; i < num_testes; i++) {
        // Chama a função de verificação e imprime o resultado
        if (verifica_anbn(testes[i]))
            printf("✓ A string \"%s\" pertence à linguagem a^n b^n\n", testes[i]);
        else
            printf("✗ A string \"%s\" NÃO pertence à linguagem a^n b^n\n", testes[i]);
    }
    
    return 0;  // Indica execução bem-sucedida
}
