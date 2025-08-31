#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LEXEME_LENGTH 256
#define MAX_LINE_LENGTH 1024

// Tipos de tokens
typedef enum {
    // Palavras-chave
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_RETURN,
    
    // Identificadores e literais
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_CHAR,
    
    // Operadores
    TOKEN_PLUS,        // +
    TOKEN_MINUS,       // -
    TOKEN_MULTIPLY,    // *
    TOKEN_DIVIDE,      // /
    TOKEN_ASSIGN,      // =
    TOKEN_EQUAL,       // ==
    TOKEN_NOT_EQUAL,   // !=
    TOKEN_LESS,        // <
    TOKEN_GREATER,     // >
    TOKEN_LESS_EQUAL,  // <=
    TOKEN_GREATER_EQUAL, // >=
    
    // Delimitadores
    TOKEN_LPAREN,      // (
    TOKEN_RPAREN,      // )
    TOKEN_LBRACE,      // {
    TOKEN_RBRACE,      // }
    TOKEN_SEMICOLON,   // ;
    TOKEN_COMMA,       // ,
    
    // Especiais
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_UNKNOWN
} TokenType;

// Estrutura do token
typedef struct {
    TokenType type;
    char lexeme[MAX_LEXEME_LENGTH];
    int line;
    int column;
} Token;

// Estado do lexer
typedef struct {
    char *input;
    int position;
    int line;
    int column;
    int length;
} LexerState;

// Palavras-chave
typedef struct {
    char *word;
    TokenType token;
} Keyword;

// Funções do lexer
void init_lexer(LexerState *lexer, const char *input);
Token get_next_token(LexerState *lexer);
const char* token_type_to_string(TokenType type);
void print_token(const Token *token);

// Funções auxiliares
char peek(LexerState *lexer);
char advance(LexerState *lexer);
void skip_whitespace(LexerState *lexer);
bool is_keyword(const char *word, TokenType *token_type);

#endif // LEXER_H