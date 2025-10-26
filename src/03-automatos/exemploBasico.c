#include <stdio.h>

/*
 * ============================================================================
 * EXEMPLO BÁSICO: AFD Simples para Iniciantes
 * ============================================================================
 * 
 * Este é o exemplo mais simples possível de um Autômato Finito Determinístico.
 * 
 * PROBLEMA:
 * =========
 * Verificar se uma string contém um número PAR de letras 'a'.
 * 
 * Exemplos:
 * - "" (string vazia)     → PAR (0 é par) → ACEITA
 * - "a"                   → ÍMPAR (1 é ímpar) → REJEITA
 * - "aa"                  → PAR (2 é par) → ACEITA
 * - "aba"                 → PAR (2 é par) → ACEITA
 * - "aaa"                 → ÍMPAR (3 é ímpar) → REJEITA
 * - "abab"                → PAR (2 é par) → ACEITA
 * 
 * DEFINIÇÃO FORMAL:
 * =================
 * M = (Q, Σ, δ, q0, F)
 * 
 * - Q = {q0, q1}          (apenas 2 estados!)
 * - Σ = {a, b, ...}       (alfabeto: todas as letras)
 * - q0 = 0                (estado inicial: número par de 'a's)
 * - F = {q0}              (estado final: q0, pois começamos com 0 'a's que é par)
 * 
 * COMO FUNCIONA:
 * ==============
 * - Começamos no estado q0 (par)
 * - Cada vez que lemos 'a', alternamos entre par e ímpar
 * - Outros caracteres não afetam o estado
 * - No final, aceitamos se estamos em q0 (par)
 * 
 * DIAGRAMA:
 * =========
 *        'a'          'a'
 *   q0 <------> q1
 *  (par)      (ímpar)
 *   [✓]         [ ]
 *    ^           ^
 *    |           |
 *  [outros]   [outros]
 *    └───┘       └───┘
 */

/**
 * Função que verifica se a string tem número par de 'a's
 * 
 * @param entrada: String a ser verificada
 * @return 1 se tem número par de 'a's, 0 se tem número ímpar
 */
int tem_par_de_as(const char *entrada) {
    // Estado 0 = par de 'a's
    // Estado 1 = ímpar de 'a's
    int estado = 0;  // Começamos no estado "par" (0 é par)
    
    printf("  Estado inicial: q%d (par)\n", estado);
    
    // Lê cada caractere da string
    for (int i = 0; entrada[i] != '\0'; i++) {
        char c = entrada[i];
        
        // Se o caractere é 'a', alternamos entre par e ímpar
        if (c == 'a') {
            // Alterna: par → ímpar ou ímpar → par
            estado = 1 - estado;
            printf("  Leu '%c': transição para q%d (%s)\n", 
                   c, estado, estado == 0 ? "par" : "ímpar");
        } else {
            // Qualquer outro caractere mantém o estado
            printf("  Leu '%c': permanece em q%d (%s)\n", 
                   c, estado, estado == 0 ? "par" : "ímpar");
        }
    }
    
    printf("  Estado final: q%d (%s)\n", estado, estado == 0 ? "par" : "ímpar");
    
    // Aceita se terminou em estado 0 (par)
    return (estado == 0);
}

/**
 * Função principal com testes
 */
int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║   AFD BÁSICO: Strings com número PAR de 'a's              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Este é o exemplo mais simples de um autômato finito!\n");
    printf("Ele conta quantas letras 'a' existem na string.\n");
    printf("Se o número for PAR (0, 2, 4, ...), a string é ACEITA.\n");
    printf("Se o número for ÍMPAR (1, 3, 5, ...), a string é REJEITADA.\n\n");
    
    // Array de strings de teste
    const char *testes[] = {
        "",        // 0 'a's → par
        "a",       // 1 'a'  → ímpar
        "aa",      // 2 'a's → par
        "aba",     // 2 'a's → par
        "aaa",     // 3 'a's → ímpar
        "abab",    // 2 'a's → par
        "bbb",     // 0 'a's → par
        "aaaa"     // 4 'a's → par
    };
    
    int num_testes = sizeof(testes) / sizeof(testes[0]);
    
    // Testa cada string
    for (int i = 0; i < num_testes; i++) {
        printf("─────────────────────────────────────────────────────────────\n");
        printf("TESTE %d: \"%s\"\n", i + 1, testes[i]);
        printf("─────────────────────────────────────────────────────────────\n");
        
        int resultado = tem_par_de_as(testes[i]);
        
        // Conta quantos 'a's existem
        int count_a = 0;
        for (int j = 0; testes[i][j] != '\0'; j++) {
            if (testes[i][j] == 'a') count_a++;
        }
        
        printf("\n  ANÁLISE:\n");
        printf("  • Total de 'a's: %d\n", count_a);
        printf("  • %d é %s\n", count_a, count_a % 2 == 0 ? "PAR" : "ÍMPAR");
        printf("  • Resultado: %s\n\n", 
               resultado ? "✓ ACEITA" : "✗ REJEITA");
    }
    
    printf("═════════════════════════════════════════════════════════════\n");
    printf("CONCEITOS IMPORTANTES:\n");
    printf("═════════════════════════════════════════════════════════════\n\n");
    
    printf("1. ESTADOS:\n");
    printf("   • q0 = número par de 'a's (estado de ACEITAÇÃO)\n");
    printf("   • q1 = número ímpar de 'a's (estado de REJEIÇÃO)\n\n");
    
    printf("2. TRANSIÇÕES:\n");
    printf("   • Ao ler 'a': muda de estado (par↔ímpar)\n");
    printf("   • Ao ler outro caractere: mantém o estado\n\n");
    
    printf("3. DECISÃO FINAL:\n");
    printf("   • Se terminar em q0 → ACEITA (número par)\n");
    printf("   • Se terminar em q1 → REJEITA (número ímpar)\n\n");
    
    printf("4. POR QUE ISSO É ÚTIL?\n");
    printf("   • Validar padrões em texto\n");
    printf("   • Verificar paridade (par/ímpar)\n");
    printf("   • Base para autômatos mais complexos\n");
    printf("   • Usado em compiladores e processadores de texto\n\n");
    
    return 0;
}
