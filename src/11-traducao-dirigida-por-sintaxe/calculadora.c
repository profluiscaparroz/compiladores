/*
 * Calculadora com Tradução Dirigida por Sintaxe
 * 
 * Implementa uma calculadora simples que avalia expressões aritméticas
 * usando tradução dirigida por sintaxe com atributos sintetizados.
 * 
 * Gramática S-atribuída:
 *   E → E + T  { E.val = E₁.val + T.val }
 *   E → E - T  { E.val = E₁.val - T.val }
 *   E → T      { E.val = T.val }
 *   T → T * F  { T.val = T₁.val * F.val }
 *   T → T / F  { T.val = T₁.val / F.val }
 *   T → F      { T.val = F.val }
 *   F → (E)    { F.val = E.val }
 *   F → num    { F.val = num.value }
 * 
 * Compilação: gcc calculadora.c -o calculadora -std=c99
 * Uso: ./calculadora
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ========== DEFINIÇÃO DE TOKENS ==========
typedef enum {
    TOKEN_NUM,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULT,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    int value;  // Para números
} Token;

// ========== VARIÁVEIS GLOBAIS DO LEXER ==========
char* input;
int pos = 0;
Token current_token;

// ========== ANALISADOR LÉXICO ==========

Token next_token() {
    // Pular espaços em branco
    while (input[pos] == ' ' || input[pos] == '\t') {
        pos++;
    }
    
    Token tok;
    
    // Fim da entrada
    if (input[pos] == '\0') {
        tok.type = TOKEN_EOF;
        return tok;
    }
    
    // Números
    if (isdigit(input[pos])) {
        tok.type = TOKEN_NUM;
        tok.value = 0;
        while (isdigit(input[pos])) {
            tok.value = tok.value * 10 + (input[pos] - '0');
            pos++;
        }
        return tok;
    }
    
    // Operadores e parênteses
    switch (input[pos++]) {
        case '+': tok.type = TOKEN_PLUS; break;
        case '-': tok.type = TOKEN_MINUS; break;
        case '*': tok.type = TOKEN_MULT; break;
        case '/': tok.type = TOKEN_DIV; break;
        case '(': tok.type = TOKEN_LPAREN; break;
        case ')': tok.type = TOKEN_RPAREN; break;
        default:
            fprintf(stderr, "Erro léxico: caractere inválido '%c'\n", input[pos-1]);
            exit(1);
    }
    
    return tok;
}

void advance() {
    current_token = next_token();
}

// ========== ANALISADOR SINTÁTICO COM SDT ==========

// Declarações forward
int parse_expr();
int parse_term();
int parse_factor();

/*
 * F → num | (E)
 * Retorna: valor do fator (atributo sintetizado)
 */
int parse_factor() {
    if (current_token.type == TOKEN_NUM) {
        // F → num { F.val = num.value }
        int val = current_token.value;
        advance();
        return val;  // Atributo sintetizado
    }
    
    if (current_token.type == TOKEN_LPAREN) {
        // F → (E) { F.val = E.val }
        advance();  // consome '('
        int val = parse_expr();  // E.val
        
        if (current_token.type != TOKEN_RPAREN) {
            fprintf(stderr, "Erro sintático: esperado ')'\n");
            exit(1);
        }
        advance();  // consome ')'
        
        return val;  // F.val = E.val
    }
    
    fprintf(stderr, "Erro sintático: esperado número ou '('\n");
    exit(1);
}

/*
 * T → F T'
 * T' → * F T' | / F T' | ε
 * Retorna: valor do termo (atributo sintetizado)
 */
int parse_term() {
    // T → F { T.val = F.val }
    int val = parse_factor();
    
    // T' → * F T' | / F T' | ε
    while (current_token.type == TOKEN_MULT || 
           current_token.type == TOKEN_DIV) {
        TokenType op = current_token.type;
        advance();
        int right = parse_factor();
        
        // Ação semântica: calcular novo valor
        if (op == TOKEN_MULT) {
            // T → T₁ * F { T.val = T₁.val * F.val }
            val = val * right;
        } else {
            // T → T₁ / F { T.val = T₁.val / F.val }
            if (right == 0) {
                fprintf(stderr, "Erro: divisão por zero\n");
                exit(1);
            }
            val = val / right;
        }
    }
    
    return val;  // Atributo sintetizado
}

/*
 * E → T E'
 * E' → + T E' | - T E' | ε
 * Retorna: valor da expressão (atributo sintetizado)
 */
int parse_expr() {
    // E → T { E.val = T.val }
    int val = parse_term();
    
    // E' → + T E' | - T E' | ε
    while (current_token.type == TOKEN_PLUS || 
           current_token.type == TOKEN_MINUS) {
        TokenType op = current_token.type;
        advance();
        int right = parse_term();
        
        // Ação semântica: calcular novo valor
        if (op == TOKEN_PLUS) {
            // E → E₁ + T { E.val = E₁.val + T.val }
            val = val + right;
        } else {
            // E → E₁ - T { E.val = E₁.val - T.val }
            val = val - right;
        }
    }
    
    return val;  // Atributo sintetizado
}

// ========== FUNÇÃO PRINCIPAL ==========

int main() {
    printf("╔════════════════════════════════════════════╗\n");
    printf("║  CALCULADORA COM SDT (S-ATRIBUÍDA)         ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    printf("Digite expressões aritméticas.\n");
    printf("Operadores: + - * / ( )\n");
    printf("Exemplos:\n");
    printf("  • 3 + 4 * 5\n");
    printf("  • (10 + 20) / 3\n");
    printf("  • 2 * (3 + 4) - 5\n");
    printf("\nDigite 'sair' ou Ctrl+D para encerrar.\n\n");
    
    char line[256];
    
    while (1) {
        printf(">>> ");
        fflush(stdout);
        
        if (!fgets(line, sizeof(line), stdin)) {
            break;  // EOF (Ctrl+D)
        }
        
        // Remover newline
        line[strcspn(line, "\n")] = 0;
        
        // Verificar comando de saída
        if (strcmp(line, "sair") == 0 || strcmp(line, "exit") == 0) {
            break;
        }
        
        // Linha vazia
        if (strlen(line) == 0) {
            continue;
        }
        
        // Inicializar parser
        input = line;
        pos = 0;
        advance();  // Carregar primeiro token
        
        // Avaliar expressão
        int resultado = parse_expr();
        
        // Verificar se consumiu toda a entrada
        if (current_token.type != TOKEN_EOF) {
            fprintf(stderr, "Aviso: caracteres extras após expressão válida\n");
        }
        
        printf("Resultado: %d\n\n", resultado);
    }
    
    printf("\nEncerrando calculadora.\n");
    return 0;
}
