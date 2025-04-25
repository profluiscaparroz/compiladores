#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

// Lista de palavras-chave
const char* keywords[] = {"if", "else", "while", "return"};
const int num_keywords = 4;

// Enumeração para os tipos de tokens
// Define os diferentes tipos de tokens que podem ser reconhecidos pelo analisador léxico.
typedef enum {
    TOKEN_KEYWORD,       // Palavra-chave (ex: if, else, while)
    TOKEN_IDENTIFIER,    // Identificador (ex: nomes de variáveis ou funções)
    TOKEN_NUMBER,        // Número (ex: 123, 45)
    TOKEN_SYMBOL,        // Símbolo (ex: +, -, *, /, =, ;, etc.)
    TOKEN_UNKNOWN,       // Token desconhecido (caractere não reconhecido)
    TOKEN_EOF,           // Fim do arquivo
    TOKEN_STRING,        // String literal (ex: "texto")
    TOKEN_COMMENT,       // Comentário (ex: // ou /* */)
    TOKEN_CHAR,          // Caractere literal (ex: 'a')
    TOKEN_OPERATOR,      // Operador (ex: ==, <=, >=, etc.)
    TOKEN_PREPROCESSOR   // Diretiva de pré-processador (ex: #include, #define)
} TokenType;

// Estrutura para representar um token
// Contém o tipo do token e o lexema associado a ele.
typedef struct {
    TokenType type;               // Tipo do token
    char lexeme[MAX_TOKEN_LENGTH]; // Lexema (texto do token)
} Token;

// Verifica se é uma palavra-chave
// Esta função recebe uma string e verifica se ela está na lista de palavras-chave.
// Retorna 1 se for uma palavra-chave, caso contrário retorna 0.
int is_keyword(const char* str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Lê o próximo token do arquivo
// Esta função analisa o arquivo caractere por caractere para identificar tokens.
// Ela reconhece diferentes tipos de tokens, como palavras-chave, identificadores, números, strings, símbolos e comentários.
// Retorna um token identificado.
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
            // Comentário de linha - ignora até o fim da linha
            while ((ch = fgetc(fp)) != EOF && ch != '\n');
            return get_next_token(fp); // chama recursivamente para buscar próximo token
        } else if (next == '*') {
            // Comentário de bloco - ignora até encontrar */
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

    // Verifica se chegou ao final do arquivo
    if (ch == EOF) {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }

    // Identificadores ou palavras-chave
    // Identifica palavras que começam com letras ou '_'.
    // Verifica se é uma palavra-chave ou um identificador.
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
    // Identifica sequências de dígitos como números.
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
    // Identifica símbolos como operadores ou pontuações.
    if (strchr("+-*/=;()", ch)) {
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        token.type = TOKEN_SYMBOL;
        return token;
    }

    // Token desconhecido
    // Caso nenhum dos casos anteriores seja atendido, o caractere é considerado desconhecido.
    token.lexeme[0] = ch;
    token.lexeme[1] = '\0';
    token.type = TOKEN_UNKNOWN;
    return token;
}

// Converte o tipo de token para uma string
// Esta função recebe um tipo de token e retorna uma string representando o tipo.
// É útil para exibir os tokens de forma legível.
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

// Função principal
// Abre o arquivo de entrada, lê os tokens usando `get_next_token` e os exibe no console.
// Fecha o arquivo ao final.
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