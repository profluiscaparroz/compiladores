/**
 * Analisador Léxico Completo para Linguagem C
 * 
 * Este exemplo demonstra um analisador léxico mais avançado que reconhece:
 * - Palavras-chave de C (int, float, if, else, while, for, return, etc.)
 * - Identificadores (nomes de variáveis e funções)
 * - Números inteiros e de ponto flutuante
 * - Strings literais
 * - Caracteres literais
 * - Operadores compostos (==, !=, <=, >=, ++, --, &&, ||)
 * - Comentários de linha e de bloco
 * - Diretivas de pré-processador
 * - Símbolos de pontuação e delimitadores
 * 
 * Autor: Disciplina de Compiladores
 * Data: 2024
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 256
#define MAX_KEYWORDS 32

// Lista completa de palavras-chave da linguagem C
const char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};
const int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

// Tipos de tokens expandidos
typedef enum {
    TOKEN_KEYWORD,          // Palavras-chave (if, while, int, etc.)
    TOKEN_IDENTIFIER,       // Identificadores (nomes de variáveis/funções)
    TOKEN_INTEGER,          // Números inteiros (123, 0x1A, 077)
    TOKEN_FLOAT,            // Números de ponto flutuante (3.14, 2.5e-3)
    TOKEN_STRING,           // String literal ("hello world")
    TOKEN_CHAR,             // Caractere literal ('a', '\n')
    TOKEN_OPERATOR,         // Operadores (+, -, ==, !=, etc.)
    TOKEN_PUNCTUATION,      // Pontuação (;, {, }, (, ), etc.)
    TOKEN_PREPROCESSOR,     // Diretivas de pré-processador (#include, #define)
    TOKEN_COMMENT,          // Comentários
    TOKEN_WHITESPACE,       // Espaços em branco (normalmente ignorados)
    TOKEN_UNKNOWN,          // Caractere não reconhecido
    TOKEN_EOF               // Fim do arquivo
} TokenType;

// Estrutura do token com informações adicionais
typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
    int line;               // Linha onde o token foi encontrado
    int column;             // Coluna onde o token inicia
} Token;

// Estado do analisador léxico
typedef struct {
    const char* input;      // String de entrada
    int position;           // Posição atual na string
    int line;               // Linha atual
    int column;             // Coluna atual
    int length;             // Tamanho total da entrada
} LexerState;

/**
 * Verifica se uma string é uma palavra-chave da linguagem C
 */
int is_keyword(const char* str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * Converte tipo de token para string legível
 */
const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER: return "INTEGER";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_STRING: return "STRING";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        case TOKEN_PREPROCESSOR: return "PREPROCESSOR";
        case TOKEN_COMMENT: return "COMMENT";
        case TOKEN_WHITESPACE: return "WHITESPACE";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

/**
 * Avança o ponteiro do lexer e atualiza linha/coluna
 */
char advance(LexerState* lexer) {
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

/**
 * Olha o próximo caractere sem avançar o ponteiro
 */
char peek(LexerState* lexer) {
    if (lexer->position >= lexer->length) {
        return '\0';
    }
    return lexer->input[lexer->position];
}

/**
 * Olha o caractere na posição atual + offset
 */
char peek_ahead(LexerState* lexer, int offset) {
    int pos = lexer->position + offset;
    if (pos >= lexer->length) {
        return '\0';
    }
    return lexer->input[pos];
}

/**
 * Pula espaços em branco
 */
void skip_whitespace(LexerState* lexer) {
    while (isspace(peek(lexer))) {
        advance(lexer);
    }
}

/**
 * Lê um identificador ou palavra-chave
 */
Token read_identifier(LexerState* lexer) {
    Token token;
    token.line = lexer->line;
    token.column = lexer->column;
    
    int i = 0;
    char ch;
    
    // Primeiro caractere deve ser letra ou underscore
    ch = advance(lexer);
    token.lexeme[i++] = ch;
    
    // Próximos caracteres podem ser letras, dígitos ou underscore
    while (isalnum(peek(lexer)) || peek(lexer) == '_') {
        if (i >= MAX_TOKEN_LENGTH - 1) break;
        token.lexeme[i++] = advance(lexer);
    }
    
    token.lexeme[i] = '\0';
    
    // Verifica se é palavra-chave ou identificador
    token.type = is_keyword(token.lexeme) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    
    return token;
}

/**
 * Lê um número (inteiro ou ponto flutuante)
 */
Token read_number(LexerState* lexer) {
    Token token;
    token.line = lexer->line;
    token.column = lexer->column;
    token.type = TOKEN_INTEGER;
    
    int i = 0;
    char ch;
    
    // Lê dígitos iniciais
    while (isdigit(peek(lexer))) {
        if (i >= MAX_TOKEN_LENGTH - 1) break;
        token.lexeme[i++] = advance(lexer);
    }
    
    // Verifica se é número de ponto flutuante
    if (peek(lexer) == '.' && isdigit(peek_ahead(lexer, 1))) {
        token.type = TOKEN_FLOAT;
        token.lexeme[i++] = advance(lexer); // consome o '.'
        
        while (isdigit(peek(lexer))) {
            if (i >= MAX_TOKEN_LENGTH - 1) break;
            token.lexeme[i++] = advance(lexer);
        }
    }
    
    // Verifica notação científica (e/E)
    if (peek(lexer) == 'e' || peek(lexer) == 'E') {
        token.type = TOKEN_FLOAT;
        token.lexeme[i++] = advance(lexer);
        
        if (peek(lexer) == '+' || peek(lexer) == '-') {
            token.lexeme[i++] = advance(lexer);
        }
        
        while (isdigit(peek(lexer))) {
            if (i >= MAX_TOKEN_LENGTH - 1) break;
            token.lexeme[i++] = advance(lexer);
        }
    }
    
    token.lexeme[i] = '\0';
    return token;
}

/**
 * Lê uma string literal
 */
Token read_string(LexerState* lexer) {
    Token token;
    token.type = TOKEN_STRING;
    token.line = lexer->line;
    token.column = lexer->column;
    
    int i = 0;
    char ch;
    
    // Consome a aspas de abertura
    token.lexeme[i++] = advance(lexer);
    
    while (peek(lexer) != '"' && peek(lexer) != '\0') {
        if (i >= MAX_TOKEN_LENGTH - 2) break;
        
        ch = advance(lexer);
        token.lexeme[i++] = ch;
        
        // Trata caracteres de escape
        if (ch == '\\' && peek(lexer) != '\0') {
            token.lexeme[i++] = advance(lexer);
        }
    }
    
    // Consome a aspas de fechamento
    if (peek(lexer) == '"') {
        token.lexeme[i++] = advance(lexer);
    }
    
    token.lexeme[i] = '\0';
    return token;
}

/**
 * Lê um caractere literal
 */
Token read_char(LexerState* lexer) {
    Token token;
    token.type = TOKEN_CHAR;
    token.line = lexer->line;
    token.column = lexer->column;
    
    int i = 0;
    char ch;
    
    // Consome a aspas simples de abertura
    token.lexeme[i++] = advance(lexer);
    
    // Lê o caractere (pode ser escape)
    if (peek(lexer) != '\'' && peek(lexer) != '\0') {
        ch = advance(lexer);
        token.lexeme[i++] = ch;
        
        if (ch == '\\' && peek(lexer) != '\0') {
            token.lexeme[i++] = advance(lexer);
        }
    }
    
    // Consome a aspas simples de fechamento
    if (peek(lexer) == '\'') {
        token.lexeme[i++] = advance(lexer);
    }
    
    token.lexeme[i] = '\0';
    return token;
}

/**
 * Lê um operador (pode ser composto)
 */
Token read_operator(LexerState* lexer) {
    Token token;
    token.type = TOKEN_OPERATOR;
    token.line = lexer->line;
    token.column = lexer->column;
    
    char ch = advance(lexer);
    char next = peek(lexer);
    
    token.lexeme[0] = ch;
    token.lexeme[1] = '\0';
    
    // Verifica operadores compostos
    switch (ch) {
        case '=':
            if (next == '=') {
                token.lexeme[1] = advance(lexer);
                token.lexeme[2] = '\0';
            }
            break;
        case '!':
            if (next == '=') {
                token.lexeme[1] = advance(lexer);
                token.lexeme[2] = '\0';
            }
            break;
        case '<':
            if (next == '=' || next == '<') {
                token.lexeme[1] = advance(lexer);
                token.lexeme[2] = '\0';
            }
            break;
        case '>':
            if (next == '=' || next == '>') {
                token.lexeme[1] = advance(lexer);
                token.lexeme[2] = '\0';
            }
            break;
        case '+':
            if (next == '+' || next == '=') {
                token.lexeme[1] = advance(lexer);
                token.lexeme[2] = '\0';
            }
            break;
        case '-':
            if (next == '-' || next == '=' || next == '>') {
                token.lexeme[1] = advance(lexer);
                token.lexeme[2] = '\0';
            }
            break;
        case '*':
        case '/':
        case '%':
            if (next == '=') {
                token.lexeme[1] = advance(lexer);
                token.lexeme[2] = '\0';
            }
            break;
        case '&':
            if (next == '&' || next == '=') {
                token.lexeme[1] = advance(lexer);
                token.lexeme[2] = '\0';
            }
            break;
        case '|':
            if (next == '|' || next == '=') {
                token.lexeme[1] = advance(lexer);
                token.lexeme[2] = '\0';
            }
            break;
        case '^':
            if (next == '=') {
                token.lexeme[1] = advance(lexer);
                token.lexeme[2] = '\0';
            }
            break;
    }
    
    return token;
}

/**
 * Lê um comentário
 */
Token read_comment(LexerState* lexer) {
    Token token;
    token.type = TOKEN_COMMENT;
    token.line = lexer->line;
    token.column = lexer->column;
    
    int i = 0;
    char ch = advance(lexer); // primeiro '/'
    token.lexeme[i++] = ch;
    
    char next = peek(lexer);
    
    if (next == '/') {
        // Comentário de linha
        token.lexeme[i++] = advance(lexer);
        
        while (peek(lexer) != '\n' && peek(lexer) != '\0') {
            if (i >= MAX_TOKEN_LENGTH - 1) break;
            token.lexeme[i++] = advance(lexer);
        }
    } else if (next == '*') {
        // Comentário de bloco
        token.lexeme[i++] = advance(lexer);
        
        while (!(peek(lexer) == '*' && peek_ahead(lexer, 1) == '/') && peek(lexer) != '\0') {
            if (i >= MAX_TOKEN_LENGTH - 3) break;
            token.lexeme[i++] = advance(lexer);
        }
        
        if (peek(lexer) == '*') {
            token.lexeme[i++] = advance(lexer);
            token.lexeme[i++] = advance(lexer);
        }
    }
    
    token.lexeme[i] = '\0';
    return token;
}

/**
 * Função principal do analisador léxico
 */
Token get_next_token(LexerState* lexer) {
    Token token;
    
    // Pula espaços em branco
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
    } else if (ch == '\'') {
        return read_char(lexer);
    } else if (ch == '/' && (peek_ahead(lexer, 1) == '/' || peek_ahead(lexer, 1) == '*')) {
        return read_comment(lexer);
    } else if (ch == '#') {
        // Diretiva de pré-processador
        token.type = TOKEN_PREPROCESSOR;
        token.line = lexer->line;
        token.column = lexer->column;
        
        int i = 0;
        while (peek(lexer) != '\n' && peek(lexer) != '\0') {
            if (i >= MAX_TOKEN_LENGTH - 1) break;
            token.lexeme[i++] = advance(lexer);
        }
        token.lexeme[i] = '\0';
        return token;
    } else if (strchr("+-*/%=<>!&|^~", ch)) {
        return read_operator(lexer);
    } else if (strchr("(){}[];,.", ch)) {
        // Pontuação
        token.type = TOKEN_PUNCTUATION;
        token.line = lexer->line;
        token.column = lexer->column;
        token.lexeme[0] = advance(lexer);
        token.lexeme[1] = '\0';
        return token;
    } else {
        // Caractere desconhecido
        token.type = TOKEN_UNKNOWN;
        token.line = lexer->line;
        token.column = lexer->column;
        token.lexeme[0] = advance(lexer);
        token.lexeme[1] = '\0';
        return token;
    }
}

/**
 * Inicializa o estado do lexer
 */
void init_lexer(LexerState* lexer, const char* input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
}

/**
 * Função de demonstração
 */
int main() {
    // Exemplo de código C para análise
    const char* code = 
        "#include <stdio.h>\n"
        "\n"
        "// Função principal\n"
        "int main() {\n"
        "    int x = 42;\n"
        "    float pi = 3.14159;\n"
        "    char letra = 'A';\n"
        "    char* mensagem = \"Hello, World!\";\n"
        "\n"
        "    /* Comentário de bloco\n"
        "       múltiplas linhas */\n"
        "    if (x >= 40 && x <= 50) {\n"
        "        printf(\"x está no intervalo\\n\");\n"
        "    }\n"
        "\n"
        "    return 0;\n"
        "}\n";
    
    printf("=== ANALISADOR LÉXICO COMPLETO ===\n");
    printf("Código de entrada:\n%s\n", code);
    printf("=== TOKENS IDENTIFICADOS ===\n");
    
    LexerState lexer;
    init_lexer(&lexer, code);
    
    Token token;
    int token_count = 0;
    
    do {
        token = get_next_token(&lexer);
        
        // Não exibe tokens de espaço em branco
        if (token.type != TOKEN_WHITESPACE) {
            printf("%3d. [%2d:%2d] %-12s '%s'\n", 
                   ++token_count,
                   token.line, 
                   token.column,
                   token_type_to_string(token.type), 
                   token.lexeme);
        }
        
    } while (token.type != TOKEN_EOF);
    
    printf("\nTotal de tokens processados: %d\n", token_count);
    
    return 0;
}