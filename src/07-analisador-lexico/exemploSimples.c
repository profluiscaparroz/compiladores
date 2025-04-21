#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

// Lista de palavras-chave
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

// Verifica se é uma palavra-chave
int is_keyword(const char* str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Lê próximo token
Token get_next_token(FILE* fp) {
    char ch;
    Token token;
    int i = 0;

    // Pular espaços em branco
    while ((ch = fgetc(fp)) != EOF && isspace(ch));

    // Verificar e ignorar comentários
    if (ch == '/') {
        char next = fgetc(fp);
        if (next == '/') {
            // Comentário de linha - ignora até fim da linha
            while ((ch = fgetc(fp)) != EOF && ch != '\n');
            return get_next_token(fp); // chama recursivamente para buscar próximo token
        } else if (next == '*') {
            // Comentário de bloco - ignora até */
            char prev = 0;
            while ((ch = fgetc(fp)) != EOF) {
                if (prev == '*' && ch == '/') {
                    break;
                }
                prev = ch;
            }
            return get_next_token(fp); // continua buscando tokens
        } else {
            // Não era comentário, é divisão ou outro operador
            ungetc(next, fp);
        }
    }
    
    // String literal
    if (ch == '"') {
        token.type = TOKEN_STRING;
        i = 0;
        token.lexeme[i++] = ch; // abre aspas

        while ((ch = fgetc(fp)) != EOF) {
            token.lexeme[i++] = ch;

            if (ch == '\\') {
                // Lê o caractere escapado (ex: \" ou \\)
                char esc = fgetc(fp);
                if (esc == EOF) break;
                token.lexeme[i++] = esc;
            } else if (ch == '"') {
                // Fechou a string
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

    // Identificadores ou palavras-chave
    if (isalpha(ch) || ch == '_') {
        token.lexeme[i++] = ch;
        while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
            token.lexeme[i++] = ch;
        }
        token.lexeme[i] = '\0';
        ungetc(ch, fp); // devolve o último caractere lido

        if (is_keyword(token.lexeme)) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }

    // Números
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

    // Símbolos simples
    if (strchr("+-*/=;()", ch)) {
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        token.type = TOKEN_SYMBOL;
        return token;
    }

    // Token desconhecido
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
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main() {
    FILE* fp = fopen("entrada.txt", "r");
    if (!fp) {
        printf("Erro ao abrir arquivo.\n");
        return 1;
    }

    Token token;
    do {
        token = get_next_token(fp);
        printf("<%s, '%s'>\n", token_type_to_string(token.type), token.lexeme);
    } while (token.type != TOKEN_EOF);

    fclose(fp);
    return 0;
}