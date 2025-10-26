#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*
 * ============================================================================
 * EXEMPLO 3: AFD para Validação de Endereços de E-mail (Simplificado)
 * ============================================================================
 * 
 * Este programa implementa um AFD que valida endereços de e-mail básicos.
 * 
 * FORMATO ESPERADO:
 * ================
 * usuario@dominio.extensao
 * 
 * Onde:
 * - usuario: letras, dígitos, underscore ou ponto
 * - @ (arroba): separador obrigatório
 * - dominio: letras e dígitos
 * - . (ponto): separador obrigatório
 * - extensao: letras (ex: com, br, org)
 * 
 * NOTA: Esta é uma validação simplificada. E-mails reais têm regras mais
 * complexas definidas na RFC 5322.
 * 
 * DEFINIÇÃO FORMAL DO AFD:
 * ========================
 * M = (Q, Σ, δ, q0, F)
 * 
 * onde:
 * - Q = {q0, q1, q2, q3, q4, q5, q6}  (conjunto de estados)
 * - Σ = {a-z, A-Z, 0-9, @, ., _}      (alfabeto)
 * - q0 = 0                             (estado inicial)
 * - F = {q6}                           (estados finais/aceitação)
 * 
 * MÁQUINA DE ESTADOS:
 * ==================
 * q0: Estado inicial (antes do nome de usuário)
 * q1: Lendo nome de usuário (antes do @)
 * q2: Após o @ (esperando início do domínio)
 * q3: Lendo domínio (antes do .)
 * q5: Após o . (esperando extensão)
 * q6: Lendo extensão (estado de aceitação)
 * q4: Estado de erro
 * 
 * EXEMPLOS DE EXECUÇÃO:
 * ====================
 * "teste@email.com"     → ACEITA
 * "user123@site.org"    → ACEITA
 * "@gmail.com"          → REJEITA (falta usuário)
 * "usuario@.com"        → REJEITA (domínio vazio)
 * "user@dominio"        → REJEITA (falta extensão)
 */

/**
 * Função que verifica se a string é um e-mail válido usando AFD
 * 
 * @param entrada: String a ser verificada (deve terminar com '\0')
 * @return 1 se a string é um e-mail válido, 0 caso contrário
 * 
 * Complexidade: O(n) onde n é o tamanho da string
 */
int automato_email(const char *entrada) {
    // Estado inicial do autômato
    int estado = 0; // q0
    int i = 0;

    // Percorre cada caractere da string de entrada
    while (entrada[i] != '\0') {
        char c = entrada[i];

        // Máquina de estados: processa cada caractere baseado no estado atual
        switch (estado) {
            case 0: // Estado inicial (antes do '@')
                // Primeiro caractere do e-mail (parte do usuário)
                if (isalnum(c) || c == '_' || c == '.') {
                    estado = 1; // Transição: q0 --[valido]--> q1
                } else {
                    estado = 4; // Transição: q0 --[invalido]--> q4 (erro)
                }
                break;

            case 1: // Nome de usuário (antes do '@')
                // Caracteres válidos no nome de usuário
                if (isalnum(c) || c == '_' || c == '.') {
                    estado = 1; // Transição: q1 --[valido]--> q1 (loop)
                } else if (c == '@') {
                    estado = 2; // Transição: q1 --[@]--> q2 (passa para domínio)
                } else {
                    estado = 4; // Transição: q1 --[invalido]--> q4 (erro)
                }
                break;

            case 2: // Parte do domínio (após '@')
                // Primeiro caractere do domínio deve ser alfanumérico
                if (isalnum(c)) {
                    estado = 3; // Transição: q2 --[letra/digito]--> q3
                } else {
                    estado = 4; // Transição: q2 --[invalido]--> q4 (erro)
                }
                break;

            case 3: // Domínio (antes do '.')
                // Caracteres válidos no domínio ou transição para extensão
                if (isalnum(c)) {
                    estado = 3; // Transição: q3 --[letra/digito]--> q3 (loop)
                } else if (c == '.') {
                    estado = 5; // Transição: q3 --[.]--> q5 (passa para extensão)
                } else {
                    estado = 4; // Transição: q3 --[invalido]--> q4 (erro)
                }
                break;

            case 5: // Extensão do domínio (após '.')
                // Primeiro caractere da extensão deve ser letra
                if (isalpha(c)) {
                    estado = 6; // Transição: q5 --[letra]--> q6 (aceitação)
                } else {
                    estado = 4; // Transição: q5 --[invalido]--> q4 (erro)
                }
                break;

            case 6: // Finalizando a extensão (estado de aceitação)
                // Pode continuar lendo letras da extensão
                if (isalpha(c)) {
                    estado = 6; // Transição: q6 --[letra]--> q6 (loop)
                } else if (c == '.') {
                    // Permite múltiplos pontos (ex: .com.br)
                    estado = 5; // Transição: q6 --[.]--> q5
                } else {
                    estado = 4; // Transição: q6 --[invalido]--> q4 (erro)
                }
                break;

            case 4: // Estado de erro
                // Uma vez no estado de erro, rejeita imediatamente
                return 0; // Rejeita: e-mail inválido
        }

        i++; // Avança para o próximo caractere
    }

    // O e-mail só é válido se terminar no estado 6 (extensão válida)
    // Isso garante que temos: usuario@dominio.extensao
    return (estado == 6); // Retorna 1 se aceita, 0 se rejeita
}

/**
 * Função principal - testa o autômato com várias strings
 */
int main() {
    // Array de strings de teste
    const char *testes[] = {
        "teste@email.com",         // válido: formato completo
        "usuario_123@dominio.org", // válido: com underscore e números
        "meu.email@site.net",      // válido: ponto no usuário
        "@gmail.com",              // inválido: falta usuário
        "semarroba.com",           // inválido: falta @
        "nome@.com",               // inválido: domínio vazio
        "user@site"                // inválido: falta extensão
    };

    int num_testes = sizeof(testes) / sizeof(testes[0]);

    printf("=== TESTANDO AFD PARA VALIDAÇÃO DE E-MAILS ===\n\n");
    printf("Este programa verifica se strings são e-mails válidos.\n");
    printf("Formato esperado: usuario@dominio.extensao\n\n");

    // Testa cada string
    for (int i = 0; i < num_testes; i++) {
        int resultado = automato_email(testes[i]);
        
        printf("Teste %d: '%s'\n", i + 1, testes[i]);
        printf("  Resultado: %s\n", 
               resultado ? "✓ ACEITA (e-mail válido)" : "✗ REJEITADA (e-mail inválido)");
        
        // Explicação do resultado para casos inválidos
        if (!resultado) {
            if (testes[i][0] == '@') {
                printf("  Motivo: e-mail não pode começar com @\n");
            } else if (strchr(testes[i], '@') == NULL) {
                printf("  Motivo: falta o símbolo @ no e-mail\n");
            } else {
                printf("  Motivo: formato incompleto ou caractere inválido\n");
            }
        }
        printf("\n");
    }

    printf("=== ESTRUTURA DE UM E-MAIL VÁLIDO ===\n");
    printf("┌─────────┐   ┌────────┐   ┌─────────┐\n");
    printf("│ usuario │ @ │ dominio│ . │ extensao│\n");
    printf("└─────────┘   └────────┘   └─────────┘\n");
    printf("\n");
    printf("Onde:\n");
    printf("• usuario: letras, dígitos, underscore ou ponto\n");
    printf("• dominio: letras e dígitos\n");
    printf("• extensao: apenas letras (ex: com, br, org)\n");

    printf("\n=== EXEMPLOS DE E-MAILS VÁLIDOS ===\n");
    printf("• joao.silva@empresa.com\n");
    printf("• admin@site.com.br\n");
    printf("• user_123@exemplo.org\n");
    printf("• contato@mail.net\n");

    printf("\n=== NOTA IMPORTANTE ===\n");
    printf("Esta é uma validação simplificada para fins didáticos.\n");
    printf("E-mails reais podem ter formatos mais complexos definidos\n");
    printf("na RFC 5322, incluindo caracteres especiais e aspas.\n");

    return 0;
}
