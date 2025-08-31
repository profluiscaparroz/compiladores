#include "../include/lexer.h"

// Tabela de palavras-chave
static const Keyword keywords[] = {
    {"int", TOKEN_INT},
    {"float", TOKEN_FLOAT},
    {"if", TOKEN_IF},
    {"else", TOKEN_ELSE},
    {"while", TOKEN_WHILE},
    {"for", TOKEN_FOR},
    {"return", TOKEN_RETURN}
};

static const int num_keywords = sizeof(keywords) / sizeof(Keyword);

// Inicializa o estado do lexer
void init_lexer(LexerState *lexer, const char *input) {
    lexer->input = (char*)input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
}

// Retorna o caractere atual sem avançar
char peek(LexerState *lexer) {
    if (lexer->position >= lexer->length) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

// Retorna o caractere atual e avança para o próximo
char advance(LexerState *lexer) {
    if (lexer->position >= lexer->length) {
        return '\0';
    }
    
    char ch = lexer->input[lexer->position++];
    
    if (ch == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    
    return ch;
}

// Pula espaços em branco
void skip_whitespace(LexerState *lexer) {
    while (isspace(peek(lexer))) {
        advance(lexer);
    }
}

// Verifica se uma palavra é palavra-chave
bool is_keyword(const char *word, TokenType *token_type) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(word, keywords[i].word) == 0) {
            *token_type = keywords[i].token;
            return true;
        }
    }
    return false;
}

// Lê um identificador ou palavra-chave
Token read_identifier(LexerState *lexer) {
    Token token;
    token.line = lexer->line;
    token.column = lexer->column;
    
    int i = 0;
    while ((isalnum(peek(lexer)) || peek(lexer) == '_') && i < MAX_LEXEME_LENGTH - 1) {
        token.lexeme[i++] = advance(lexer);
    }
    token.lexeme[i] = '\0';
    
    // Verifica se é palavra-chave
    if (!is_keyword(token.lexeme, &token.type)) {
        token.type = TOKEN_ID;
    }
    
    return token;
}

// Lê um número
Token read_number(LexerState *lexer) {
    Token token;
    token.type = TOKEN_NUMBER;
    token.line = lexer->line;
    token.column = lexer->column;
    
    int i = 0;
    bool has_dot = false;
    
    while ((isdigit(peek(lexer)) || (peek(lexer) == '.' && !has_dot)) && i < MAX_LEXEME_LENGTH - 1) {
        if (peek(lexer) == '.') {
            has_dot = true;
        }
        token.lexeme[i++] = advance(lexer);
    }
    token.lexeme[i] = '\0';
    
    return token;
}

// Lê uma string literal
Token read_string(LexerState *lexer) {
    Token token;
    token.type = TOKEN_STRING;
    token.line = lexer->line;
    token.column = lexer->column;
    
    advance(lexer); // pula a primeira aspas
    
    int i = 0;
    while (peek(lexer) != '"' && peek(lexer) != '\0' && i < MAX_LEXEME_LENGTH - 1) {
        if (peek(lexer) == '\\') {
            advance(lexer); // pula o escape
            if (peek(lexer) != '\0') {
                token.lexeme[i++] = advance(lexer);
            }
        } else {
            token.lexeme[i++] = advance(lexer);
        }
    }
    
    if (peek(lexer) == '"') {
        advance(lexer); // pula a última aspas
    }
    
    token.lexeme[i] = '\0';
    return token;
}

// Pula comentário de linha
void skip_line_comment(LexerState *lexer) {
    while (peek(lexer) != '\n' && peek(lexer) != '\0') {
        advance(lexer);
    }
}

// Pula comentário de bloco
void skip_block_comment(LexerState *lexer) {
    advance(lexer); // pula '/'
    advance(lexer); // pula '*'
    
    while (peek(lexer) != '\0') {
        if (peek(lexer) == '*') {
            advance(lexer);
            if (peek(lexer) == '/') {
                advance(lexer);
                break;
            }
        } else {
            advance(lexer);
        }
    }
}

// Função principal do analisador léxico
Token get_next_token(LexerState *lexer) {
    Token token;
    
    skip_whitespace(lexer);
    
    // Verifica fim do arquivo
    if (peek(lexer) == '\0') {
        token.type = TOKEN_EOF;
        token.line = lexer->line;
        token.column = lexer->column;
        strcpy(token.lexeme, "EOF");
        return token;
    }
    
    char ch = peek(lexer);
    
    // Identifica tipo do próximo token
    if (isalpha(ch) || ch == '_') {
        return read_identifier(lexer);
    } else if (isdigit(ch)) {
        return read_number(lexer);
    } else if (ch == '"') {
        return read_string(lexer);
    } else if (ch == '/') {
        char next = lexer->position + 1 < lexer->length ? lexer->input[lexer->position + 1] : '\0';
        if (next == '/') {
            skip_line_comment(lexer);
            return get_next_token(lexer); // recursão para próximo token
        } else if (next == '*') {
            skip_block_comment(lexer);
            return get_next_token(lexer); // recursão para próximo token
        } else {
            token.type = TOKEN_DIVIDE;
            token.line = lexer->line;
            token.column = lexer->column;
            token.lexeme[0] = advance(lexer);
            token.lexeme[1] = '\0';
            return token;
        }
    }
    
    // Operadores compostos
    token.line = lexer->line;
    token.column = lexer->column;
    
    switch (ch) {
        case '=':
            advance(lexer);
            if (peek(lexer) == '=') {
                advance(lexer);
                token.type = TOKEN_EQUAL;
                strcpy(token.lexeme, "==");
            } else {
                token.type = TOKEN_ASSIGN;
                strcpy(token.lexeme, "=");
            }
            break;
            
        case '!':
            advance(lexer);
            if (peek(lexer) == '=') {
                advance(lexer);
                token.type = TOKEN_NOT_EQUAL;
                strcpy(token.lexeme, "!=");
            } else {
                token.type = TOKEN_ERROR;
                strcpy(token.lexeme, "!");
            }
            break;
            
        case '<':
            advance(lexer);
            if (peek(lexer) == '=') {
                advance(lexer);
                token.type = TOKEN_LESS_EQUAL;
                strcpy(token.lexeme, "<=");
            } else {
                token.type = TOKEN_LESS;
                strcpy(token.lexeme, "<");
            }
            break;
            
        case '>':
            advance(lexer);
            if (peek(lexer) == '=') {
                advance(lexer);
                token.type = TOKEN_GREATER_EQUAL;
                strcpy(token.lexeme, ">=");
            } else {
                token.type = TOKEN_GREATER;
                strcpy(token.lexeme, ">");
            }
            break;
            
        case '+':
            token.type = TOKEN_PLUS;
            token.lexeme[0] = advance(lexer);
            token.lexeme[1] = '\0';
            break;
            
        case '-':
            token.type = TOKEN_MINUS;
            token.lexeme[0] = advance(lexer);
            token.lexeme[1] = '\0';
            break;
            
        case '*':
            token.type = TOKEN_MULTIPLY;
            token.lexeme[0] = advance(lexer);
            token.lexeme[1] = '\0';
            break;
            
        case '(':
            token.type = TOKEN_LPAREN;
            token.lexeme[0] = advance(lexer);
            token.lexeme[1] = '\0';
            break;
            
        case ')':
            token.type = TOKEN_RPAREN;
            token.lexeme[0] = advance(lexer);
            token.lexeme[1] = '\0';
            break;
            
        case '{':
            token.type = TOKEN_LBRACE;
            token.lexeme[0] = advance(lexer);
            token.lexeme[1] = '\0';
            break;
            
        case '}':
            token.type = TOKEN_RBRACE;
            token.lexeme[0] = advance(lexer);
            token.lexeme[1] = '\0';
            break;
            
        case ';':
            token.type = TOKEN_SEMICOLON;
            token.lexeme[0] = advance(lexer);
            token.lexeme[1] = '\0';
            break;
            
        case ',':
            token.type = TOKEN_COMMA;
            token.lexeme[0] = advance(lexer);
            token.lexeme[1] = '\0';
            break;
            
        default:
            token.type = TOKEN_ERROR;
            token.lexeme[0] = advance(lexer);
            token.lexeme[1] = '\0';
            break;
    }
    
    return token;
}

// Converte tipo de token para string
const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_INT: return "TOKEN_INT";
        case TOKEN_FLOAT: return "TOKEN_FLOAT";
        case TOKEN_IF: return "TOKEN_IF";
        case TOKEN_ELSE: return "TOKEN_ELSE";
        case TOKEN_WHILE: return "TOKEN_WHILE";
        case TOKEN_FOR: return "TOKEN_FOR";
        case TOKEN_RETURN: return "TOKEN_RETURN";
        case TOKEN_ID: return "TOKEN_ID";
        case TOKEN_NUMBER: return "TOKEN_NUMBER";
        case TOKEN_STRING: return "TOKEN_STRING";
        case TOKEN_CHAR: return "TOKEN_CHAR";
        case TOKEN_PLUS: return "TOKEN_PLUS";
        case TOKEN_MINUS: return "TOKEN_MINUS";
        case TOKEN_MULTIPLY: return "TOKEN_MULTIPLY";
        case TOKEN_DIVIDE: return "TOKEN_DIVIDE";
        case TOKEN_ASSIGN: return "TOKEN_ASSIGN";
        case TOKEN_EQUAL: return "TOKEN_EQUAL";
        case TOKEN_NOT_EQUAL: return "TOKEN_NOT_EQUAL";
        case TOKEN_LESS: return "TOKEN_LESS";
        case TOKEN_GREATER: return "TOKEN_GREATER";
        case TOKEN_LESS_EQUAL: return "TOKEN_LESS_EQUAL";
        case TOKEN_GREATER_EQUAL: return "TOKEN_GREATER_EQUAL";
        case TOKEN_LPAREN: return "TOKEN_LPAREN";
        case TOKEN_RPAREN: return "TOKEN_RPAREN";
        case TOKEN_LBRACE: return "TOKEN_LBRACE";
        case TOKEN_RBRACE: return "TOKEN_RBRACE";
        case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_ERROR: return "TOKEN_ERROR";
        case TOKEN_UNKNOWN: return "TOKEN_UNKNOWN";
        default: return "UNKNOWN_TOKEN";
    }
}

// Imprime um token
void print_token(const Token *token) {
    printf("%-15s \"%-10s\" %d:%d\n", 
           token_type_to_string(token->type), 
           token->lexeme, 
           token->line, 
           token->column);
}