#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

char keywords[4][7] = {"if", "else", "while", "return"};

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_SYMBOL,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
} Token;

int str_equals(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return (*a == 0 && *b == 0);
}

int is_keyword(const char* str) {
    for (int i = 0; i < 4; i++) {
        if (str_equals(str, keywords[i])) return 1;
    }
    return 0;
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

int read_char(int* index, const char* input) {
    return input[(*index)++];
}

void unget_char(int* index) {
    (*index)--;
}

Token get_next_token(const char* input, int* index) {
    char ch;
    Token token;
    int i = 0;

    // Skip whitespaces
    do {
        ch = read_char(index, input);
    } while (is_space(ch));

    if (ch == 0) {
        token.type = TOKEN_EOF;
        token.lexeme[0] = 0;
        return token;
    }

    if (is_alpha(ch) || ch == '_') {
        token.lexeme[i++] = ch;
        ch = read_char(index, input);
        while (is_alnum(ch) || ch == '_') {
            token.lexeme[i++] = ch;
            ch = read_char(index, input);
        }
        token.lexeme[i] = 0;
        unget_char(index);

        if (is_keyword(token.lexeme)) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }

    if (is_digit(ch)) {
        token.lexeme[i++] = ch;
        ch = read_char(index, input);
        while (is_digit(ch)) {
            token.lexeme[i++] = ch;
            ch = read_char(index, input);
        }
        token.lexeme[i] = 0;
        unget_char(index);
        token.type = TOKEN_NUMBER;
        return token;
    }

    // Simple symbols
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == ';' || ch == '(' || ch == ')') {
        token.lexeme[0] = ch;
        token.lexeme[1] = 0;
        token.type = TOKEN_SYMBOL;
        return token;
    }

    token.lexeme[0] = ch;
    token.lexeme[1] = 0;
    token.type = TOKEN_UNKNOWN;
    return token;
}

void print_token(Token t) {
    char* type;
    switch (t.type) {
        case TOKEN_KEYWORD: type = "KEYWORD"; break;
        case TOKEN_IDENTIFIER: type = "IDENTIFIER"; break;
        case TOKEN_NUMBER: type = "NUMBER"; break;
        case TOKEN_SYMBOL: type = "SYMBOL"; break;
        case TOKEN_EOF: type = "EOF"; break;
        default: type = "UNKNOWN"; break;
    }

    // Simples print com putchar
    char* p = "<";
    while (*p) putchar(*p++);
    p = type;
    while (*p) putchar(*p++);
    putchar(',');
    putchar(' ');
    putchar('\'');
    p = t.lexeme;
    while (*p) putchar(*p++);
    putchar('\'');
    putchar('>');
    putchar('\n');
}

int main() {
    const char* code = "if (x == 10) return x;";
    int idx = 0;
    Token t;

    do {
        t = get_next_token(code, &idx);
        print_token(t);
    } while (t.type != TOKEN_EOF);

    return 0;
}
