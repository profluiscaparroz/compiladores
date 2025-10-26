#include <stdio.h>
#include <ctype.h>

/*
 * ============================================================================
 * EXEMPLO 2: AFD para Reconhecimento de Identificadores
 * ============================================================================
 * 
 * Este programa implementa um AFD que valida identificadores válidos na
 * linguagem C, seguindo as regras:
 * 
 * REGRAS DE IDENTIFICADORES EM C:
 * ===============================
 * 1. Deve começar com uma letra (a-z, A-Z) ou underscore (_)
 * 2. Pode conter letras, dígitos (0-9) ou underscores nas posições seguintes
 * 3. Não pode começar com dígito
 * 4. Não pode conter espaços ou caracteres especiais
 * 
 * DEFINIÇÃO FORMAL DO AFD:
 * ========================
 * M = (Q, Σ, δ, q0, F)
 * 
 * onde:
 * - Q = {q0, q1, q2}          (conjunto de estados)
 * - Σ = {a-z, A-Z, 0-9, _}    (alfabeto: letras, dígitos, underscore)
 * - q0 = 0                     (estado inicial)
 * - F = {q1}                   (estados finais/aceitação)
 * 
 * FUNÇÃO DE TRANSIÇÃO δ:
 * =====================
 * Estado q0 (inicial):
 *   - Se ler letra [a-zA-Z] ou _ → vai para q1
 *   - Se ler dígito [0-9] ou outro → vai para q2 (erro)
 * 
 * Estado q1 (aceitação):
 *   - Se ler letra, dígito ou _ → permanece em q1
 *   - Se ler outro caractere → vai para q2 (erro)
 * 
 * Estado q2 (erro):
 *   - Qualquer entrada → permanece em q2 (rejeita)
 * 
 * DIAGRAMA DE TRANSIÇÃO:
 * =====================
 *        [a-zA-Z_]    [a-zA-Z0-9_]
 *   q0 -----------> q1 <-----------
 *    |              | ^           |
 *    |              | |           |
 *    |[0-9,outros]  | |           |
 *    |              |[outros]     |
 *    v              v             |
 *   q2 <---------------------------
 *  (erro)
 * 
 * EXEMPLOS DE EXECUÇÃO:
 * ====================
 * "variavel"   → q0 → q1 → q1 → ... → q1 (ACEITA)
 * "_contador"  → q0 → q1 → q1 → ... → q1 (ACEITA)
 * "123abc"     → q0 → q2 (REJEITA - começa com dígito)
 * "var#1"      → q0 → q1 → q1 → q1 → q2 (REJEITA - contém #)
 */

/**
 * Função que verifica se a string é um identificador válido em C
 * 
 * @param entrada: String a ser verificada (deve terminar com '\0')
 * @return 1 se a string é um identificador válido, 0 caso contrário
 * 
 * Usa funções da biblioteca <ctype.h>:
 * - isalpha(c): verifica se c é letra (a-z, A-Z)
 * - isalnum(c): verifica se c é letra ou dígito (a-z, A-Z, 0-9)
 * 
 * Complexidade: O(n) onde n é o tamanho da string
 */
int automato_identificador(const char *entrada) {
    // Estado inicial do autômato
    int estado = 0; // q0
    // Percorre cada caractere da string de entrada
    for (int i = 0; entrada[i] != '\0'; i++) {
        char c = entrada[i];

        // Máquina de estados: processa cada caractere baseado no estado atual
        switch (estado) {
            case 0: // Estado inicial (q0)
                // Primeiro caractere deve ser letra ou underscore
                if (isalpha(c) || c == '_') {
                    estado = 1; // Transição: q0 --[letra,_]--> q1
                } else {
                    // Qualquer outro caractere (incluindo dígitos) é inválido no início
                    estado = 2; // Transição: q0 --[outros]--> q2
                }
                break;

            case 1: // Estado q1 (identificador válido até agora)
                // Caracteres seguintes podem ser letra, dígito ou underscore
                if (isalnum(c) || c == '_') {
                    estado = 1; // Transição: q1 --[letra,digito,_]--> q1 (loop)
                } else {
                    // Espaços ou caracteres especiais são inválidos
                    estado = 2; // Transição: q1 --[outros]--> q2 (erro)
                }
                break;

            case 2: // Estado de erro (q2)
                // Uma vez no estado de erro, rejeita imediatamente
                return 0; // Rejeita: identificador inválido
        }
    }

    // Verifica se terminou em um estado de aceitação
    // Apenas q1 é estado final: significa que o primeiro caractere era
    // letra/underscore e todos os seguintes eram letras, dígitos ou underscores
    return (estado == 1); // Retorna 1 se aceita, 0 se rejeita
}

/**
 * Função principal - testa o autômato com várias strings
 */
int main() {
    // Array de strings de teste
    const char *testes[] = {
        "variavel",      // válido: começa com letra
        "_contador",     // válido: começa com underscore
        "x1y2z3",        // válido: letras e números
        "123abc",        // inválido: começa com dígito
        "var#1",         // inválido: contém #
        "hello world",   // inválido: contém espaço
        "myVar"          // válido: camelCase
    };
    int num_testes = sizeof(testes) / sizeof(testes[0]);

    printf("=== TESTANDO AFD PARA RECONHECIMENTO DE IDENTIFICADORES ===\n\n");
    printf("Este programa verifica se strings são identificadores válidos em C.\n");
    printf("Regras: deve começar com letra ou '_', seguido de letras, dígitos ou '_'.\n\n");

    // Testa cada string
    for (int i = 0; i < num_testes; i++) {
        int resultado = automato_identificador(testes[i]);
        
        printf("Teste %d: '%s'\n", i + 1, testes[i]);
        printf("  Resultado: %s\n", 
               resultado ? "✓ ACEITA (identificador válido)" : "✗ REJEITADA (identificador inválido)");
        
        // Explicação do resultado
        if (!resultado) {
            if (testes[i][0] >= '0' && testes[i][0] <= '9') {
                printf("  Motivo: identificador não pode começar com dígito\n");
            } else {
                printf("  Motivo: contém caractere inválido (espaço ou especial)\n");
            }
        }
        printf("\n");
    }

    printf("=== EXEMPLOS DE IDENTIFICADORES VÁLIDOS ===\n");
    printf("• contador\n");
    printf("• _temp\n");
    printf("• variavel1\n");
    printf("• myVariable\n");
    printf("• MAX_VALUE\n");
    printf("• x2_y3\n");

    printf("\n=== EXEMPLOS DE IDENTIFICADORES INVÁLIDOS ===\n");
    printf("• 1variavel (começa com dígito)\n");
    printf("• minha-variavel (contém hífen)\n");
    printf("• valor$ (contém caractere especial)\n");
    printf("• duas palavras (contém espaço)\n");

    return 0;
}
