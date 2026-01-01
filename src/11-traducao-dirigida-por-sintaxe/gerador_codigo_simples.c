/*
 * Gerador de Código Intermediário Simplificado
 * 
 * Versão simplificada que evita problemas de memória
 * ao usar alocação estática para strings.
 * 
 * Compilação: gcc gerador_codigo_simples.c -o gerador_codigo_simples -std=c99
 * Uso: ./gerador_codigo_simples
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_NUM, TOKEN_ID, TOKEN_PLUS, TOKEN_MINUS,
    TOKEN_MULT, TOKEN_DIV, TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_ASSIGN, TOKEN_SEMI, TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[32];
} Token;

char* input;
int pos = 0;
Token current_token;

// Código gerado
#define MAX_CODE 100
char code_lines[MAX_CODE][100];
int code_count = 0;
int temp_count = 0;

void emit_code(const char* line) {
    if (code_count >= MAX_CODE) {
        fprintf(stderr, "Erro: número máximo de linhas de código (%d) excedido.\n", MAX_CODE);
        exit(EXIT_FAILURE);
    }
    strcpy(code_lines[code_count++], line);
}

char* new_temp() {
    /* Allocate a fresh buffer for each temporary name to avoid reuse issues */
    char *temp = (char *)malloc(16);
    if (temp == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para temporário.\n");
        exit(EXIT_FAILURE);
    }
    snprintf(temp, 16, "t%d", temp_count++);
    return temp;
}

Token next_token() {
    while (input[pos] == ' ' || input[pos] == '\t') pos++;
    
    Token tok;
    tok.lexeme[0] = '\0';
    
    if (input[pos] == '\0') {
        tok.type = TOKEN_EOF;
        return tok;
    }
    
    if (isdigit(input[pos])) {
        tok.type = TOKEN_NUM;
        int i = 0;
        while (isdigit(input[pos]) && i < 31) {
            tok.lexeme[i++] = input[pos++];
        }
        tok.lexeme[i] = '\0';
        return tok;
    }
    
    if (isalpha(input[pos]) || input[pos] == '_') {
        tok.type = TOKEN_ID;
        int i = 0;
        while ((isalnum(input[pos]) || input[pos] == '_') && i < 31) {
            tok.lexeme[i++] = input[pos++];
        }
        tok.lexeme[i] = '\0';
        return tok;
    }
    
    switch (input[pos++]) {
        case '+': tok.type = TOKEN_PLUS; strcpy(tok.lexeme, "+"); break;
        case '-': tok.type = TOKEN_MINUS; strcpy(tok.lexeme, "-"); break;
        case '*': tok.type = TOKEN_MULT; strcpy(tok.lexeme, "*"); break;
        case '/': tok.type = TOKEN_DIV; strcpy(tok.lexeme, "/"); break;
        case '(': tok.type = TOKEN_LPAREN; strcpy(tok.lexeme, "("); break;
        case ')': tok.type = TOKEN_RPAREN; strcpy(tok.lexeme, ")"); break;
        case '=': tok.type = TOKEN_ASSIGN; strcpy(tok.lexeme, "="); break;
        case ';': tok.type = TOKEN_SEMI; strcpy(tok.lexeme, ";"); break;
        default:
            fprintf(stderr, "Erro léxico: caractere inválido '%c'\n", input[pos - 1]);
            tok.type = TOKEN_EOF;
            tok.lexeme[0] = '\0';
            break;
    }
    
    return tok;
}

void advance() {
    current_token = next_token();
}

// Parser com geração de código
void parse_expr(char* result);
void parse_term(char* result);
void parse_factor(char* result);

void parse_factor(char* result) {
    if (current_token.type == TOKEN_NUM || current_token.type == TOKEN_ID) {
        strcpy(result, current_token.lexeme);
        advance();
        return;
    }
    
    if (current_token.type == TOKEN_LPAREN) {
        advance();
        parse_expr(result);
        if (current_token.type != TOKEN_RPAREN) {
            fprintf(stderr, "Erro: esperado ')'\n");
            exit(1);
        }
        advance();
        return;
    }
    
    fprintf(stderr, "Erro sintático\n");
    exit(1);
}

void parse_term(char* result) {
    char left[32];
    parse_factor(left);
    strcpy(result, left);
    
    while (current_token.type == TOKEN_MULT || current_token.type == TOKEN_DIV) {
        char op = (current_token.type == TOKEN_MULT) ? '*' : '/';
        advance();
        
        char right[32];
        parse_factor(right);
        
        char* temp = new_temp();
        char line[100];
        sprintf(line, "%s = %s %c %s", temp, result, op, right);
        emit_code(line);
        strcpy(result, temp);
    }
}

void parse_expr(char* result) {
    char left[32];
    parse_term(left);
    strcpy(result, left);
    
    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
        char op = (current_token.type == TOKEN_PLUS) ? '+' : '-';
        advance();
        
        char right[32];
        parse_term(right);
        
        char* temp = new_temp();
        char line[100];
        sprintf(line, "%s = %s %c %s", temp, result, op, right);
        emit_code(line);
        strcpy(result, temp);
    }
}

void parse_assignment() {
    if (current_token.type != TOKEN_ID) {
        fprintf(stderr, "Erro: esperado identificador\n");
        exit(1);
    }
    
    char id[32];
    strcpy(id, current_token.lexeme);
    advance();
    
    if (current_token.type != TOKEN_ASSIGN) {
        fprintf(stderr, "Erro: esperado '='\n");
        exit(1);
    }
    advance();
    
    char result[32];
    parse_expr(result);
    
    char line[100];
    sprintf(line, "%s = %s", id, result);
    emit_code(line);
    
    if (current_token.type != TOKEN_SEMI) {
        fprintf(stderr, "Erro: esperado ';'\n");
        exit(1);
    }
    advance();
}

int main() {
    printf("╔════════════════════════════════════════════╗\n");
    printf("║  GERADOR DE CÓDIGO COM SDT                 ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    printf("Digite atribuições: var = expr;\n");
    printf("Exemplo: x = a + b * 3;\n\n");
    
    char line[256];
    while (1) {
        printf(">>> ");
        fflush(stdout);
        
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, "sair") == 0 || strlen(line) == 0) break;
        
        code_count = 0;
        temp_count = 0;
        input = line;
        pos = 0;
        advance();
        
        parse_assignment();
        
        printf("\n=== CÓDIGO INTERMEDIÁRIO ===\n");
        for (int i = 0; i < code_count; i++) {
            printf("%s\n", code_lines[i]);
        }
        printf("\n");
    }
    
    return 0;
}
