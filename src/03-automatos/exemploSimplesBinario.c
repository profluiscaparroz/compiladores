#include <stdio.h>

/*
 * ============================================================================
 * EXEMPLO SIMPLES: AFD para Validar Strings Binárias
 * ============================================================================
 * 
 * Este programa demonstra um AFD que aceita apenas strings compostas
 * por '0' e '1' (strings binárias válidas).
 * 
 * PROBLEMA:
 * =========
 * Aceitar strings que contenham apenas os caracteres '0' e '1'.
 * Qualquer outro caractere torna a string inválida.
 * 
 * EXEMPLOS:
 * =========
 * "0"        → ACEITA (binário válido)
 * "1"        → ACEITA (binário válido)
 * "101010"   → ACEITA (binário válido)
 * "110011"   → ACEITA (binário válido)
 * "102"      → REJEITA (contém '2')
 * "1a0"      → REJEITA (contém 'a')
 * ""         → REJEITA (string vazia)
 * 
 * DEFINIÇÃO FORMAL:
 * =================
 * M = (Q, Σ, δ, q0, F)
 * 
 * - Q = {q0, q1, q2}      (3 estados)
 * - Σ = {0, 1}            (alfabeto binário)
 * - q0 = 0                (estado inicial)
 * - F = {q1}              (estado de aceitação)
 * 
 * ESTADOS:
 * ========
 * q0: Estado inicial (ainda não leu nada)
 * q1: Estado válido (lendo dígitos binários)
 * q2: Estado de erro (encontrou caractere inválido)
 * 
 * DIAGRAMA DE TRANSIÇÃO:
 * ======================
 * 
 *         [0,1]        [0,1]
 *    q0 ---------> q1 <------
 *     |            |^       |
 *     |            ||       |
 *  [outros]     [outros]    |
 *     |            ||       |
 *     v            v        |
 *    q2 <------------------'
 *  (erro)
 */

/**
 * Função que valida se a string é binária (apenas 0s e 1s)
 * 
 * @param entrada: String a ser validada
 * @return 1 se é binária válida, 0 caso contrário
 */
int validar_binario(const char *entrada) {
    int estado = 0;  // Estado inicial
    
    printf("\n  ┌─────────────────────────────────────────┐\n");
    printf("  │ Simulação Passo a Passo do Autômato    │\n");
    printf("  └─────────────────────────────────────────┘\n\n");
    printf("  Estado inicial: q0\n");
    printf("  String: \"%s\"\n\n", entrada);
    
    int posicao = 0;
    
    // Processa cada caractere
    while (entrada[posicao] != '\0') {
        char c = entrada[posicao];
        
        printf("  Passo %d: Lendo '%c' no estado q%d\n", posicao + 1, c, estado);
        
        switch (estado) {
            case 0:  // Estado inicial
                if (c == '0' || c == '1') {
                    estado = 1;
                    printf("           → Caractere válido! Transição: q0 → q1\n");
                } else {
                    estado = 2;
                    printf("           → Caractere inválido! Transição: q0 → q2\n");
                }
                break;
                
            case 1:  // Estado válido (aceitação)
                if (c == '0' || c == '1') {
                    // Permanece em q1
                    printf("           → Caractere válido! Permanece em: q1\n");
                } else {
                    estado = 2;
                    printf("           → Caractere inválido! Transição: q1 → q2\n");
                }
                break;
                
            case 2:  // Estado de erro
                printf("           → Permanece em estado de erro: q2\n");
                return 0;  // Rejeita imediatamente
        }
        
        posicao++;
    }
    
    printf("\n  Estado final: q%d\n", estado);
    
    // Aceita apenas se terminou em q1 (e não está vazio)
    return (estado == 1);
}

/**
 * Função auxiliar para contar bits
 */
void analisar_binario(const char *str) {
    int zeros = 0, uns = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '0') zeros++;
        else if (str[i] == '1') uns++;
    }
    
    printf("  Análise da string:\n");
    printf("    • Quantidade de 0s: %d\n", zeros);
    printf("    • Quantidade de 1s: %d\n", uns);
    printf("    • Tamanho total: %d\n", zeros + uns);
    
    if (zeros + uns > 0) {
        // Converte binário para decimal (apenas para demonstração)
        int decimal = 0;
        int potencia = 1;
        for (int i = zeros + uns - 1; i >= 0; i--) {
            if (str[i] == '1') {
                decimal += potencia;
            }
            potencia *= 2;
        }
        printf("    • Valor em decimal: %d\n", decimal);
    }
}

/**
 * Função principal
 */
int main() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║     AFD: Validador de Strings Binárias                    ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    printf("\nEste autômato aceita apenas strings compostas por 0s e 1s.\n");
    printf("Qualquer outro caractere torna a string inválida.\n");
    
    // Strings de teste
    const char *testes[] = {
        "0",           // válido
        "1",           // válido
        "101010",      // válido
        "110011",      // válido
        "102",         // inválido (contém '2')
        "1a0",         // inválido (contém 'a')
        "",            // inválido (vazio)
        "11111111"     // válido
    };
    
    int num_testes = sizeof(testes) / sizeof(testes[0]);
    
    // Executa os testes
    for (int i = 0; i < num_testes; i++) {
        printf("\n");
        printf("═════════════════════════════════════════════════════════════\n");
        printf("TESTE #%d\n", i + 1);
        printf("═════════════════════════════════════════════════════════════\n");
        
        int resultado = validar_binario(testes[i]);
        
        printf("\n  ┌─────────────────────────────────────────┐\n");
        printf("  │ RESULTADO                                │\n");
        printf("  └─────────────────────────────────────────┘\n\n");
        
        if (resultado) {
            printf("  ✓ ACEITA - String binária válida!\n\n");
            analisar_binario(testes[i]);
        } else {
            printf("  ✗ REJEITA - String inválida!\n");
            if (testes[i][0] == '\0') {
                printf("  Motivo: String vazia não é aceita\n");
            } else {
                printf("  Motivo: Contém caracteres que não são 0 ou 1\n");
            }
        }
        
        printf("\n");
    }
    
    printf("═════════════════════════════════════════════════════════════\n");
    printf("RESUMO DO AUTÔMATO:\n");
    printf("═════════════════════════════════════════════════════════════\n\n");
    
    printf("TABELA DE TRANSIÇÕES:\n");
    printf("┌────────┬──────────┬───────────┬────────────┐\n");
    printf("│ Estado │ Lê '0'   │ Lê '1'    │ Lê outros  │\n");
    printf("├────────┼──────────┼───────────┼────────────┤\n");
    printf("│  q0    │  → q1    │  → q1     │  → q2      │\n");
    printf("│  q1*   │  → q1    │  → q1     │  → q2      │\n");
    printf("│  q2    │  → q2    │  → q2     │  → q2      │\n");
    printf("└────────┴──────────┴───────────┴────────────┘\n");
    printf("* = estado de aceitação\n\n");
    
    printf("APLICAÇÕES PRÁTICAS:\n");
    printf("• Validação de entrada em sistemas digitais\n");
    printf("• Verificação de números binários\n");
    printf("• Processamento de dados em baixo nível\n");
    printf("• Base para conversores binário-decimal\n");
    printf("• Parte de analisadores léxicos em compiladores\n\n");
    
    return 0;
}
