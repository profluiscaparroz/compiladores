#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

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
    TOKEN_COMMENT
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
} Token;

// Funções auxiliares no lugar de <string.h> e <ctype.h>
int str_equals(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return (*a == '\0' && *b == '\0');
}

int str_copy(char* dest, const char* src) {
    int i = 0;
    while (src[i]) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_alnum(char c) {
    return is_alpha(c) || is_digit(c);
}

int is_space(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

int is_keyword(const char* str) {
    for (int i = 0; i < num_keywords; i++) {
        if (str_equals(str, keywords[i])) return 1;
    }
    return 0;
}

Token get_next_token(FILE* fp) {
    char ch;
    Token token;
    int i = 0;

    // Pula espaços
    while ((ch = fgetc(fp)) != EOF && is_space(ch));

    if (ch == EOF) {
        token.type = TOKEN_EOF;
        str_copy(token.lexeme, "EOF");
        return token;
    }

    // Strings
    if (ch == '"') {
        token.type = TOKEN_STRING;
        token.lexeme[i++] = ch;
        while ((ch = fgetc(fp)) != EOF) {
            token.lexeme[i++] = ch;
            if (ch == '"') break;
            if (i >= MAX_TOKEN_LENGTH - 1) {
                printf("String muito longa!\n");
                exit(1);
            }
        }
        token.lexeme[i] = '\0';
        return token;
    }

    // Comentários
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

    // Identificadores ou palavras-chave
    if (is_alpha(ch) || ch == '_') {
        token.lexeme[i++] = ch;
        while ((ch = fgetc(fp)) != EOF && (is_alnum(ch) || ch == '_')) {
            token.lexeme[i++] = ch;
        }
        token.lexeme[i] = '\0';
        ungetc(ch, fp);
        token.type = is_keyword(token.lexeme) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        return token;
    }

    // Números
    if (is_digit(ch)) {
        token.lexeme[i++] = ch;
        while ((ch = fgetc(fp)) != EOF && is_digit(ch)) {
            token.lexeme[i++] = ch;
        }
        token.lexeme[i] = '\0';
        ungetc(ch, fp);
        token.type = TOKEN_NUMBER;
        return token;
    }

    // Símbolos simples
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
        ch == '=' || ch == ';' || ch == '(' || ch == ')') {
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        token.type = TOKEN_SYMBOL;
        return token;
    }

    // Desconhecido
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
        case TOKEN_STRING: return "STRING";
        case TOKEN_COMMENT: return "COMMENT";
        default: return "UNKNOWN";
    }
}

int main() {
    FILE* fp = fopen("entrada.txt", "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo.\n");
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
