/*
 * Conversor de Notação Infixa para Pós-fixa (RPN)
 * 
 * Implementa um tradutor que converte expressões em notação infixa
 * para notação pós-fixa (Reverse Polish Notation - RPN) usando SDT.
 * 
 * Gramática S-atribuída para conversão:
 *   E → E + T  { E.posfixa = E₁.posfixa || T.posfixa || "+" }
 *   E → E - T  { E.posfixa = E₁.posfixa || T.posfixa || "-" }
 *   E → T      { E.posfixa = T.posfixa }
 *   T → T * F  { T.posfixa = T₁.posfixa || F.posfixa || "*" }
 *   T → T / F  { T.posfixa = T₁.posfixa || F.posfixa || "/" }
 *   T → F      { T.posfixa = F.posfixa }
 *   F → (E)    { F.posfixa = E.posfixa }
 *   F → id     { F.posfixa = id.lexeme }
 *   F → num    { F.posfixa = num.lexeme }
 * 
 * Exemplos:
 *   Infixa:     a + b * c
 *   Pós-fixa:   a b c * +
 * 
 *   Infixa:     (a + b) * c
 *   Pós-fixa:   a b + c *
 * 
 * Compilação: gcc conversor_notacao.c -o conversor_notacao -std=c99
 * Uso: ./conversor_notacao
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ========== DEFINIÇÃO DE TOKENS ==========
typedef enum {
    TOKEN_NUM,
    TOKEN_ID,
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
    char lexeme[32];
} Token;

// ========== VARIÁVEIS GLOBAIS DO LEXER ==========
char* input;
int pos = 0;
Token current_token;

// ========== BUFFER PARA SAÍDA PÓS-FIXA ==========
#define MAX_OUTPUT 256
char postfix_output[MAX_OUTPUT];

void append_postfix(const char* str) {
    size_t len = strlen(postfix_output);

    /* Adiciona espaço separador apenas se já houver algo e houver espaço suficiente */
    if (len > 0 && len < MAX_OUTPUT - 1) {
        postfix_output[len] = ' ';
        postfix_output[len + 1] = '\0';
        len++;
    }

    /* Calcula o espaço restante para o próximo token (inclui espaço para '\0') */
    if (len < MAX_OUTPUT - 1) {
        size_t remaining = MAX_OUTPUT - 1 - len;
        strncat(postfix_output, str, remaining);
    }
}

// ========== ANALISADOR LÉXICO ==========

Token next_token() {
    // Pular espaços em branco
    while (input[pos] == ' ' || input[pos] == '\t') {
        pos++;
    }
    
    Token tok;
    tok.lexeme[0] = '\0';
    
    // Fim da entrada
    if (input[pos] == '\0') {
        tok.type = TOKEN_EOF;
        return tok;
    }
    
    // Números
    if (isdigit(input[pos])) {
        tok.type = TOKEN_NUM;
        int i = 0;
        while (isdigit(input[pos]) && i < 30) {
            tok.lexeme[i++] = input[pos++];
        }
        tok.lexeme[i] = '\0';
        return tok;
    }
    
    // Identificadores
    if (isalpha(input[pos]) || input[pos] == '_') {
        tok.type = TOKEN_ID;
        int i = 0;
        while ((isalnum(input[pos]) || input[pos] == '_') && i < 30) {
            tok.lexeme[i++] = input[pos++];
        }
        tok.lexeme[i] = '\0';
        return tok;
    }
    
    // Operadores e parênteses
    switch (input[pos++]) {
        case '+': tok.type = TOKEN_PLUS; strcpy(tok.lexeme, "+"); break;
        case '-': tok.type = TOKEN_MINUS; strcpy(tok.lexeme, "-"); break;
        case '*': tok.type = TOKEN_MULT; strcpy(tok.lexeme, "*"); break;
        case '/': tok.type = TOKEN_DIV; strcpy(tok.lexeme, "/"); break;
        case '(': tok.type = TOKEN_LPAREN; strcpy(tok.lexeme, "("); break;
        case ')': tok.type = TOKEN_RPAREN; strcpy(tok.lexeme, ")"); break;
        default:
            fprintf(stderr, "Erro léxico: caractere inválido '%c'\n", input[pos-1]);
            exit(1);
    }
    
    return tok;
}

void advance() {
    current_token = next_token();
}

// ========== ANALISADOR SINTÁTICO COM CONVERSÃO ==========

// Declarações forward
void parse_expr();
void parse_term();
void parse_factor();

/*
 * F → id | num | (E)
 * Ação semântica: adiciona operando ou processa subexpressão
 */
void parse_factor() {
    if (current_token.type == TOKEN_NUM) {
        // F → num { F.posfixa = num.lexeme }
        append_postfix(current_token.lexeme);
        advance();
        return;
    }
    
    if (current_token.type == TOKEN_ID) {
        // F → id { F.posfixa = id.lexeme }
        append_postfix(current_token.lexeme);
        advance();
        return;
    }
    
    if (current_token.type == TOKEN_LPAREN) {
        // F → (E) { F.posfixa = E.posfixa }
        advance();  // consome '('
        parse_expr();  // E.posfixa já foi adicionada
        
        if (current_token.type != TOKEN_RPAREN) {
            fprintf(stderr, "Erro sintático: esperado ')'\n");
            exit(1);
        }
        advance();  // consome ')'
        return;
    }
    
    fprintf(stderr, "Erro sintático: esperado identificador, número ou '('\n");
    exit(1);
}

/*
 * T → F T'
 * T' → * F T' | / F T' | ε
 * Ação semântica: adiciona operador após operandos
 */
void parse_term() {
    // T → F
    parse_factor();
    
    // T' → * F T' | / F T' | ε
    while (current_token.type == TOKEN_MULT || 
           current_token.type == TOKEN_DIV) {
        char op[2];
        op[0] = (current_token.type == TOKEN_MULT) ? '*' : '/';
        op[1] = '\0';
        advance();
        
        parse_factor();
        
        // T → T₁ op F { T.posfixa = T₁.posfixa || F.posfixa || op }
        append_postfix(op);
    }
}

/*
 * E → T E'
 * E' → + T E' | - T E' | ε
 * Ação semântica: adiciona operador após operandos
 */
void parse_expr() {
    // E → T
    parse_term();
    
    // E' → + T E' | - T E' | ε
    while (current_token.type == TOKEN_PLUS || 
           current_token.type == TOKEN_MINUS) {
        char op[2];
        op[0] = (current_token.type == TOKEN_PLUS) ? '+' : '-';
        op[1] = '\0';
        advance();
        
        parse_term();
        
        // E → E₁ op T { E.posfixa = E₁.posfixa || T.posfixa || op }
        append_postfix(op);
    }
}

// ========== AVALIADOR DE NOTAÇÃO PÓS-FIXA ==========

int evaluate_postfix(const char* postfix) {
    int stack[100];
    int top = -1;
    
    char expr[MAX_OUTPUT];
    strcpy(expr, postfix);
    
    char* token = strtok(expr, " ");
    
    while (token != NULL) {
        // Se é número, empilha
        if (isdigit(token[0])) {
            if (top + 1 >= 100) {
                fprintf(stderr, "Erro: pilha cheia\n");
                return 0;
            }
            stack[++top] = atoi(token);
        }
        // Se é operador, desempilha dois operandos e calcula
        else if (strlen(token) == 1 && strchr("+-*/", token[0])) {
            if (top < 1) {
                fprintf(stderr, "Erro: expressão pós-fixa inválida\n");
                return 0;
            }
            
            int b = stack[top--];
            int a = stack[top--];
            int result;
            
            switch (token[0]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/':
                    if (b == 0) {
                        fprintf(stderr, "Erro: divisão por zero\n");
                        return 0;
                    }
                    result = a / b;
                    break;
                default: result = 0;
            }
            
            if (top + 1 >= 100) {
                fprintf(stderr, "Erro: pilha cheia\n");
                return 0;
            }
            stack[++top] = result;
        }
        // Se é identificador, não pode avaliar
        else if (isalpha(token[0])) {
            fprintf(stderr, "Aviso: não é possível avaliar expressão com variáveis\n");
            return 0;
        }
        
        token = strtok(NULL, " ");
    }
    
    if (top != 0) {
        fprintf(stderr, "Erro: expressão pós-fixa inválida\n");
        return 0;
    }
    
    return stack[0];
}

// ========== FUNÇÃO PRINCIPAL ==========

int main() {
    printf("╔════════════════════════════════════════════╗\n");
    printf("║  CONVERSOR INFIXA → PÓS-FIXA (RPN)        ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    printf("Converte expressões da notação infixa para pós-fixa.\n");
    printf("Exemplos:\n");
    printf("  Infixa:      a + b * c\n");
    printf("  Pós-fixa:    a b c * +\n\n");
    printf("  Infixa:      (a + b) * (c - d)\n");
    printf("  Pós-fixa:    a b + c d - *\n\n");
    printf("Digite 'sair' ou Ctrl+D para encerrar.\n\n");
    
    char line[256];
    
    while (1) {
        printf("Infixa >>> ");
        fflush(stdout);
        
        if (!fgets(line, sizeof(line), stdin)) {
            break;  // EOF
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
        
        // Reset
        postfix_output[0] = '\0';
        input = line;
        pos = 0;
        advance();  // Carregar primeiro token
        
        // Converter para pós-fixa
        parse_expr();
        
        // Verificar se consumiu toda a entrada
        if (current_token.type != TOKEN_EOF) {
            fprintf(stderr, "Aviso: caracteres extras após expressão válida\n");
        }
        
        // Imprimir resultado
        printf("Pós-fixa:  %s\n", postfix_output);
        
        // Se for expressão apenas com números, tentar avaliar
        int has_variables = 0;
        for (int i = 0; postfix_output[i]; i++) {
            if (isalpha(postfix_output[i])) {
                has_variables = 1;
                break;
            }
        }
        
        if (!has_variables) {
            int result = evaluate_postfix(postfix_output);
            printf("Valor:     %d\n", result);
        }
        
        printf("\n");
    }
    
    printf("Encerrando conversor.\n");
    return 0;
}
