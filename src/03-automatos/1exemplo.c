#include <stdio.h>

/*
 * ============================================================================
 * EXEMPLO 1: AFD para Reconhecimento de Números
 * ============================================================================
 * 
 * Este programa implementa um Autômato Finito Determinístico (AFD) simples
 * que reconhece strings compostas apenas por dígitos numéricos (0-9).
 * 
 * DEFINIÇÃO FORMAL DO AFD:
 * ========================
 * M = (Q, Σ, δ, q0, F)
 * 
 * onde:
 * - Q = {q0, q1, q2}          (conjunto de estados)
 * - Σ = {0, 1, 2, ..., 9}     (alfabeto: dígitos)
 * - q0 = 0                     (estado inicial)
 * - F = {q1}                   (estados finais/aceitação)
 * 
 * FUNÇÃO DE TRANSIÇÃO δ:
 * =====================
 * Estado q0 (inicial):
 *   - Se ler dígito [0-9] → vai para q1
 *   - Se ler outro caractere → vai para q2 (erro)
 * 
 * Estado q1 (aceitação):
 *   - Se ler dígito [0-9] → permanece em q1
 *   - Se ler outro caractere → vai para q2 (erro)
 * 
 * Estado q2 (erro):
 *   - Qualquer entrada → permanece em q2 (rejeita)
 * 
 * DIAGRAMA DE TRANSIÇÃO:
 * =====================
 *          [0-9]         [0-9]
 *   q0 -----------> q1 <---------
 *    |              | ^         |
 *    |              | |         |
 *    |[outros]      | |         |
 *    |              |[outros]   |
 *    v              v           |
 *   q2 <------------------------'
 *  (erro)
 * 
 * EXEMPLOS DE EXECUÇÃO:
 * ====================
 * "123"   → q0 → q1 → q1 → q1 (ACEITA)
 * "42"    → q0 → q1 → q1 (ACEITA)
 * "12a"   → q0 → q1 → q1 → q2 (REJEITA)
 * "hello" → q0 → q2 (REJEITA)
 */

/**
 * Função que simula um AFD para verificar se a string contém apenas números
 * 
 * @param entrada: String a ser verificada (deve terminar com '\0')
 * @return 1 se a string é aceita (contém apenas dígitos), 0 caso contrário
 * 
 * Complexidade: O(n) onde n é o tamanho da string
 */
int automato_numerico(const char *entrada) {
    // Estado inicial do autômato
    int estado = 0; // q0
    
    // Percorre cada caractere da string de entrada
    for (int i = 0; entrada[i] != '\0'; i++) {
        char c = entrada[i];

        // Máquina de estados: processa cada caractere baseado no estado atual
        switch (estado) {
            case 0: // Estado inicial (q0)
                // Verifica se o caractere é um dígito
                if (c >= '0' && c <= '9') {
                    estado = 1; // Transição: q0 --[0-9]--> q1
                } else {
                    estado = 2; // Transição: q0 --[outros]--> q2
                }
                break;

            case 1: // Estado q1 (número válido encontrado até agora)
                // Continua verificando se todos os caracteres são dígitos
                if (c >= '0' && c <= '9') {
                    estado = 1; // Transição: q1 --[0-9]--> q1 (loop)
                } else {
                    estado = 2; // Transição: q1 --[outros]--> q2 (erro)
                }
                break;

            case 2: // Estado de erro (q2)
                // Uma vez no estado de erro, rejeita imediatamente
                // Não há transições que saem do estado de erro
                return 0; // Rejeita: string inválida
        }
    }

    // Verifica se terminou em um estado de aceitação
    // Apenas q1 é estado final: significa que lemos pelo menos um dígito
    // e todos os caracteres eram dígitos
    return (estado == 1); // Retorna 1 se aceita, 0 se rejeita
}

/**
 * Função principal - testa o autômato com várias strings
 */
int main() {
    // Array de strings de teste
    const char *testes[] = {"12345", "42", "98a7", "hello", "67890", "12.34"};
    int num_testes = sizeof(testes) / sizeof(testes[0]);

    printf("=== TESTANDO AFD PARA RECONHECIMENTO DE NÚMEROS ===\n\n");
    printf("Este programa verifica se strings contêm apenas dígitos (0-9).\n\n");

    // Testa cada string
    for (int i = 0; i < num_testes; i++) {
        int resultado = automato_numerico(testes[i]);
        
        printf("Teste %d: '%s'\n", i + 1, testes[i]);
        printf("  Resultado: %s\n", resultado ? "✓ ACEITA (número válido)" : "✗ REJEITADA (não é número)");
        printf("\n");
    }

    printf("=== EXPLICAÇÃO DOS RESULTADOS ===\n");
    printf("• Strings aceitas: contêm apenas dígitos 0-9\n");
    printf("• Strings rejeitadas: contêm letras, espaços ou caracteres especiais\n");
    printf("• O ponto decimal '.' não é aceito pois não está no alfabeto do AFD\n");

    return 0;
}
