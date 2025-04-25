#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

// Simples tabela de símbolos para variáveis "declaradas"
#define MAX_SYMBOLS 100

char symbol_table[MAX_SYMBOLS][MAX_TOKEN_LENGTH];
int symbol_count = 0;

// Palavras-chave
const char* keywords[] = {"if", "else", "while", "return"};
const int num_keywords = 4;

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_SYMBOL,
    TOKEN_UNKNOWN,
    TOKEN_EOF,
    TOKEN_STRING,
    TOKEN_COMMENT,
    TOKEN_CHAR,
    TOKEN_OPERATOR,
    TOKEN_PREPROCESSOR
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
} Token;

// Verifica se um identificador está na tabela de símbolos
int is_declared(const char* name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i], name) == 0) return 1;
    }
    return 0;
}

// Adiciona um identificador na tabela (declaração implícita)
void declare(const char* name) {
    if (!is_declared(name)) {
        strcpy(symbol_table[symbol_count++], name);
    }
}

int is_keyword(const char* str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

Token get_next_token(FILE* fp) {
    char ch;
    Token token;
    int i = 0;

    while ((ch = fgetc(fp)) != EOF && isspace(ch));

    if (ch == '/') {
        char next = fgetc(fp);
        if (next == '/') {
            while ((ch = fgetc(fp)) != EOF && ch != '\n');
            return get_next_token(fp);
        } else if (next == '*') {
            char prev = 0;
            while ((ch = fgetc(fp)) != EOF) {
                if (prev == '*' && ch == '/') break;
                prev = ch;
            }
            return get_next_token(fp);
        } else {
            ungetc(next, fp);
        }
    }

    if (ch == '"') {
        token.type = TOKEN_STRING;
        i = 0;
        token.lexeme[i++] = ch;
        while ((ch = fgetc(fp)) != EOF) {
            token.lexeme[i++] = ch;
            if (ch == '\\') {
                char esc = fgetc(fp);
                if (esc == EOF) break;
                token.lexeme[i++] = esc;
            } else if (ch == '"') {
                break;
            }
            if (i >= MAX_TOKEN_LENGTH - 2) {
                printf("String muito longa!\n");
                exit(1);
            }
        }
        token.lexeme[i] = '\0';
        return token;
    }

    if (ch == EOF) {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }

    if (isalpha(ch) || ch == '_') {
        token.lexeme[i++] = ch;
        while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
            token.lexeme[i++] = ch;
        }
        token.lexeme[i] = '\0';
        ungetc(ch, fp);
        token.type = is_keyword(token.lexeme) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        return token;
    }

    if (isdigit(ch)) {
        token.lexeme[i++] = ch;
        while ((ch = fgetc(fp)) != EOF && isdigit(ch)) {
            token.lexeme[i++] = ch;
        }
        token.lexeme[i] = '\0';
        ungetc(ch, fp);
        token.type = TOKEN_NUMBER;
        return token;
    }

    if (strchr("+-*/=;(){}", ch)) {
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        token.type = TOKEN_SYMBOL;
        return token;
    }

    token.lexeme[0] = ch;
    token.lexeme[1] = '\0';
    token.type = TOKEN_UNKNOWN;
    return token;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_SYMBOL: return "SYMBOL";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

// ====================== PARSER =======================

Token current_token;

void advance(FILE* fp) {
    current_token = get_next_token(fp);
}

void expect(TokenType type, const char* expected, FILE* fp) {
    if (current_token.type == type && strcmp(current_token.lexeme, expected) == 0) {
        advance(fp);
    } else {
        printf("Erro de sintaxe: Esperado '%s', encontrado '%s'\n", expected, current_token.lexeme);
        exit(1);
    }
}

// <expression> ::= <term> (('+' | '-') <term>)*
void parse_expression(FILE* fp);

// <term> ::= <factor> (('*' | '/') <factor>)*
void parse_term(FILE* fp);

// <factor> ::= NUMBER | IDENTIFIER | '(' <expression> ')'
void parse_factor(FILE* fp) {
    if (current_token.type == TOKEN_IDENTIFIER) {
        if (!is_declared(current_token.lexeme)) {
            printf("Erro semântico: Variável '%s' usada antes da declaração\n", current_token.lexeme);
            exit(1);
        }
        advance(fp);
    } else if (current_token.type == TOKEN_NUMBER) {
        advance(fp);
    } else if (strcmp(current_token.lexeme, "(") == 0) {
        advance(fp);
        parse_expression(fp);
        expect(TOKEN_SYMBOL, ")", fp);
    } else {
        printf("Erro de sintaxe: Fator inválido '%s'\n", current_token.lexeme);
        exit(1);
    }
}

// Novo: expressão de atribuição com análise semântica
// <assign_expr> ::= IDENTIFIER '=' <expression>
void parse_assign_expr(FILE* fp) {
    if (current_token.type != TOKEN_IDENTIFIER) {
        printf("Erro de sintaxe: Esperado identificador para atribuição, encontrado '%s'\n", current_token.lexeme);
        exit(1);
    }

    // Declara variável implicitamente (como em C)
    declare(current_token.lexeme);
    advance(fp);

    expect(TOKEN_SYMBOL, "=", fp);
    parse_expression(fp);
}
// <statement> ::= "if" '(' <expression> ')' <statement> | <expression> ';'

void parse_term(FILE* fp) {
    parse_factor(fp);
    while (strcmp(current_token.lexeme, "*") == 0 || strcmp(current_token.lexeme, "/") == 0) {
        advance(fp);
        parse_factor(fp);
    }
}

void parse_expression(FILE* fp) {
    parse_term(fp);
    while (strcmp(current_token.lexeme, "+") == 0 || strcmp(current_token.lexeme, "-") == 0) {
        advance(fp);
        parse_term(fp);
    }
}

// <statement> ::= "if" '(' <expression> ')' <statement> | <expression> ';'
void parse_statement(FILE* fp) {
    if (current_token.type == TOKEN_KEYWORD && strcmp(current_token.lexeme, "if") == 0) {
        advance(fp);
        expect(TOKEN_SYMBOL, "(", fp);
        parse_expression(fp);
        expect(TOKEN_SYMBOL, ")", fp);
        parse_statement(fp);
    }else if (current_token.type == TOKEN_IDENTIFIER) {
        Token next = get_next_token(fp);
        if (strcmp(next.lexeme, "=") == 0) {
            push_token_to_buffer(next); // Push the token back to the buffer
            parse_assign_expr(fp);
            expect(TOKEN_SYMBOL, ";", fp);
        } else {
            push_token_to_buffer(next); // Push the token back to the buffer
            parse_expression(fp);
            expect(TOKEN_SYMBOL, ";", fp);
        }
    } else {
        parse_expression(fp);
        expect(TOKEN_SYMBOL, ";", fp);
    }
}

// <program> ::= <statement>*
void parse_program(FILE* fp) {
    advance(fp);
    while (current_token.type != TOKEN_EOF) {
        parse_statement(fp);
    }
}

// ====================== MAIN =======================
int main() {
    FILE* fp = fopen("entrada.txt", "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    parse_program(fp);

    printf("Análise sintática completa!\n");
    fclose(fp);
    return 0;
}
