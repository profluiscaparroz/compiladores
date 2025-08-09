/**
 * Analisador Sintático (Parser) Descendente Recursivo para Linguagem C Simplificada
 * 
 * Este programa implementa um analisador sintático completo que reconhece:
 * 
 * GRAMÁTICA SUPORTADA:
 * program      ::= declaration_list
 * declaration  ::= var_declaration | func_declaration
 * var_declaration ::= type IDENTIFIER ('=' expression)? ';'
 * func_declaration ::= type IDENTIFIER '(' param_list ')' compound_stmt
 * param_list   ::= (parameter (',' parameter)*)?
 * parameter    ::= type IDENTIFIER
 * type         ::= 'int' | 'float' | 'char'
 * compound_stmt ::= '{' statement_list '}'
 * statement_list ::= statement*
 * statement    ::= expression_stmt | compound_stmt | if_stmt | while_stmt | return_stmt
 * if_stmt      ::= 'if' '(' expression ')' statement ('else' statement)?
 * while_stmt   ::= 'while' '(' expression ')' statement
 * return_stmt  ::= 'return' expression? ';'
 * expression_stmt ::= expression? ';'
 * expression   ::= assignment_expr
 * assignment_expr ::= logical_or_expr ('=' assignment_expr)?
 * logical_or_expr ::= logical_and_expr ('||' logical_and_expr)*
 * logical_and_expr ::= equality_expr ('&&' equality_expr)*
 * equality_expr ::= relational_expr (('==' | '!=') relational_expr)*
 * relational_expr ::= additive_expr (('<' | '>' | '<=' | '>=') additive_expr)*
 * additive_expr ::= multiplicative_expr (('+' | '-') multiplicative_expr)*
 * multiplicative_expr ::= unary_expr (('*' | '/' | '%') unary_expr)*
 * unary_expr   ::= ('+' | '-' | '!')? primary_expr
 * primary_expr ::= IDENTIFIER | NUMBER | '(' expression ')' | func_call
 * func_call    ::= IDENTIFIER '(' argument_list ')'
 * argument_list ::= (expression (',' expression)*)?
 * 
 * Características:
 * - Parser descendente recursivo
 * - Tratamento de precedência de operadores
 * - Construção de árvore sintática abstrata (AST)
 * - Detecção e relatório de erros sintáticos
 * - Suporte a funções e declarações de variáveis
 * 
 * Autor: Disciplina de Compiladores
 * Data: 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100
#define MAX_TOKENS 1000

// Tipos de tokens expandidos
typedef enum {
    // Literals
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    
    // Palavras-chave
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_RETURN,
    
    // Operadores
    TOKEN_ASSIGN,       // =
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_MULTIPLY,     // *
    TOKEN_DIVIDE,       // /
    TOKEN_MODULO,       // %
    TOKEN_EQ,           // ==
    TOKEN_NE,           // !=
    TOKEN_LT,           // <
    TOKEN_LE,           // <=
    TOKEN_GT,           // >
    TOKEN_GE,           // >=
    TOKEN_AND,          // &&
    TOKEN_OR,           // ||
    TOKEN_NOT,          // !
    
    // Delimitadores
    TOKEN_SEMICOLON,    // ;
    TOKEN_COMMA,        // ,
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LBRACE,       // {
    TOKEN_RBRACE,       // }
    
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

// Estrutura do token
typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
    int line;
    int column;
} Token;

// Tipos de nós da AST
typedef enum {
    NODE_PROGRAM,
    NODE_VAR_DECL,
    NODE_FUNC_DECL,
    NODE_PARAM,
    NODE_COMPOUND_STMT,
    NODE_IF_STMT,
    NODE_WHILE_STMT,
    NODE_RETURN_STMT,
    NODE_EXPRESSION_STMT,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_ASSIGN,
    NODE_FUNC_CALL,
    NODE_IDENTIFIER,
    NODE_NUMBER
} NodeType;

// Estrutura de nó da AST
typedef struct ASTNode {
    NodeType type;
    char* value;
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* children[10];  // Para nós com múltiplos filhos
    int child_count;
    int line;
} ASTNode;

// Estado do lexer
typedef struct {
    char* input;
    int position;
    int line;
    int column;
    int length;
} Lexer;

// Estado do parser
typedef struct {
    Token* tokens;
    int token_count;
    int current_token;
    ASTNode* ast_root;
    int error_count;
} Parser;

// ==================== LEXER ====================

char* keywords[] = {"int", "float", "char", "if", "else", "while", "return"};
TokenType keyword_tokens[] = {TOKEN_INT, TOKEN_FLOAT, TOKEN_CHAR, TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_RETURN};
int num_keywords = 7;

void init_lexer(Lexer* lexer, char* input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(input);
}

char peek_char(Lexer* lexer) {
    if (lexer->position >= lexer->length) return '\0';
    return lexer->input[lexer->position];
}

char advance_char(Lexer* lexer) {
    if (lexer->position >= lexer->length) return '\0';
    char ch = lexer->input[lexer->position++];
    if (ch == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return ch;
}

void skip_whitespace(Lexer* lexer) {
    while (isspace(peek_char(lexer))) {
        advance_char(lexer);
    }
}

void skip_comment(Lexer* lexer) {
    if (peek_char(lexer) == '/' && lexer->input[lexer->position + 1] == '/') {
        // Comentário de linha
        while (peek_char(lexer) != '\n' && peek_char(lexer) != '\0') {
            advance_char(lexer);
        }
    } else if (peek_char(lexer) == '/' && lexer->input[lexer->position + 1] == '*') {
        // Comentário de bloco
        advance_char(lexer); // /
        advance_char(lexer); // *
        while (peek_char(lexer) != '\0') {
            char ch = advance_char(lexer);
            if (ch == '*' && peek_char(lexer) == '/') {
                advance_char(lexer);
                break;
            }
        }
    }
}

TokenType get_keyword_token(char* word) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return keyword_tokens[i];
        }
    }
    return TOKEN_IDENTIFIER;
}

Token get_next_token(Lexer* lexer) {
    Token token;
    token.line = lexer->line;
    token.column = lexer->column;
    
    // Pula espaços e comentários
    while (1) {
        skip_whitespace(lexer);
        if (peek_char(lexer) == '/' && 
           (lexer->input[lexer->position + 1] == '/' || lexer->input[lexer->position + 1] == '*')) {
            skip_comment(lexer);
        } else {
            break;
        }
    }
    
    char ch = peek_char(lexer);
    
    // Fim do arquivo
    if (ch == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }
    
    // Identificadores e palavras-chave
    if (isalpha(ch) || ch == '_') {
        int i = 0;
        while ((isalnum(peek_char(lexer)) || peek_char(lexer) == '_') && i < MAX_TOKEN_LENGTH - 1) {
            token.lexeme[i++] = advance_char(lexer);
        }
        token.lexeme[i] = '\0';
        token.type = get_keyword_token(token.lexeme);
        return token;
    }
    
    // Números
    if (isdigit(ch)) {
        int i = 0;
        while (isdigit(peek_char(lexer)) && i < MAX_TOKEN_LENGTH - 1) {
            token.lexeme[i++] = advance_char(lexer);
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_NUMBER;
        return token;
    }
    
    // Strings
    if (ch == '"') {
        int i = 0;
        token.lexeme[i++] = advance_char(lexer); // aspas de abertura
        while (peek_char(lexer) != '"' && peek_char(lexer) != '\0' && i < MAX_TOKEN_LENGTH - 2) {
            char c = advance_char(lexer);
            token.lexeme[i++] = c;
            if (c == '\\' && peek_char(lexer) != '\0') {
                token.lexeme[i++] = advance_char(lexer);
            }
        }
        if (peek_char(lexer) == '"') {
            token.lexeme[i++] = advance_char(lexer); // aspas de fechamento
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_STRING;
        return token;
    }
    
    // Operadores e delimitadores
    char next = (lexer->position + 1 < lexer->length) ? lexer->input[lexer->position + 1] : '\0';
    
    switch (ch) {
        case '=':
            if (next == '=') {
                strcpy(token.lexeme, "==");
                advance_char(lexer);
                advance_char(lexer);
                token.type = TOKEN_EQ;
            } else {
                strcpy(token.lexeme, "=");
                advance_char(lexer);
                token.type = TOKEN_ASSIGN;
            }
            break;
        case '!':
            if (next == '=') {
                strcpy(token.lexeme, "!=");
                advance_char(lexer);
                advance_char(lexer);
                token.type = TOKEN_NE;
            } else {
                strcpy(token.lexeme, "!");
                advance_char(lexer);
                token.type = TOKEN_NOT;
            }
            break;
        case '<':
            if (next == '=') {
                strcpy(token.lexeme, "<=");
                advance_char(lexer);
                advance_char(lexer);
                token.type = TOKEN_LE;
            } else {
                strcpy(token.lexeme, "<");
                advance_char(lexer);
                token.type = TOKEN_LT;
            }
            break;
        case '>':
            if (next == '=') {
                strcpy(token.lexeme, ">=");
                advance_char(lexer);
                advance_char(lexer);
                token.type = TOKEN_GE;
            } else {
                strcpy(token.lexeme, ">");
                advance_char(lexer);
                token.type = TOKEN_GT;
            }
            break;
        case '&':
            if (next == '&') {
                strcpy(token.lexeme, "&&");
                advance_char(lexer);
                advance_char(lexer);
                token.type = TOKEN_AND;
            } else {
                token.type = TOKEN_ERROR;
            }
            break;
        case '|':
            if (next == '|') {
                strcpy(token.lexeme, "||");
                advance_char(lexer);
                advance_char(lexer);
                token.type = TOKEN_OR;
            } else {
                token.type = TOKEN_ERROR;
            }
            break;
        case '+': token.type = TOKEN_PLUS; break;
        case '-': token.type = TOKEN_MINUS; break;
        case '*': token.type = TOKEN_MULTIPLY; break;
        case '/': token.type = TOKEN_DIVIDE; break;
        case '%': token.type = TOKEN_MODULO; break;
        case ';': token.type = TOKEN_SEMICOLON; break;
        case ',': token.type = TOKEN_COMMA; break;
        case '(': token.type = TOKEN_LPAREN; break;
        case ')': token.type = TOKEN_RPAREN; break;
        case '{': token.type = TOKEN_LBRACE; break;
        case '}': token.type = TOKEN_RBRACE; break;
        default:
            token.type = TOKEN_ERROR;
            break;
    }
    
    if (token.type != TOKEN_ERROR && token.type != TOKEN_AND && token.type != TOKEN_OR) {
        token.lexeme[0] = advance_char(lexer);
        token.lexeme[1] = '\0';
    }
    
    return token;
}

Token* tokenize(char* input, int* token_count) {
    Lexer lexer;
    init_lexer(&lexer, input);
    
    Token* tokens = malloc(MAX_TOKENS * sizeof(Token));
    *token_count = 0;
    
    Token token;
    do {
        token = get_next_token(&lexer);
        if (token.type != TOKEN_ERROR) {
            tokens[*token_count] = token;
            (*token_count)++;
        }
    } while (token.type != TOKEN_EOF && *token_count < MAX_TOKENS);
    
    return tokens;
}

// ==================== AST ====================

ASTNode* create_node(NodeType type, char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = NULL;
    node->right = NULL;
    node->child_count = 0;
    node->line = 0;
    return node;
}

void add_child(ASTNode* parent, ASTNode* child) {
    if (parent->child_count < 10) {
        parent->children[parent->child_count++] = child;
    }
}

void print_ast(ASTNode* node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; i++) printf("  ");
    
    switch (node->type) {
        case NODE_PROGRAM: printf("PROGRAM\n"); break;
        case NODE_VAR_DECL: printf("VAR_DECL: %s\n", node->value); break;
        case NODE_FUNC_DECL: printf("FUNC_DECL: %s\n", node->value); break;
        case NODE_BINARY_OP: printf("BINARY_OP: %s\n", node->value); break;
        case NODE_IDENTIFIER: printf("ID: %s\n", node->value); break;
        case NODE_NUMBER: printf("NUM: %s\n", node->value); break;
        case NODE_IF_STMT: printf("IF\n"); break;
        case NODE_WHILE_STMT: printf("WHILE\n"); break;
        case NODE_RETURN_STMT: printf("RETURN\n"); break;
        case NODE_COMPOUND_STMT: printf("COMPOUND\n"); break;
        default: printf("NODE_%d\n", node->type); break;
    }
    
    if (node->left) print_ast(node->left, depth + 1);
    if (node->right) print_ast(node->right, depth + 1);
    for (int i = 0; i < node->child_count; i++) {
        print_ast(node->children[i], depth + 1);
    }
}

// ==================== PARSER ====================

void init_parser(Parser* parser, Token* tokens, int token_count) {
    parser->tokens = tokens;
    parser->token_count = token_count;
    parser->current_token = 0;
    parser->ast_root = NULL;
    parser->error_count = 0;
}

Token current_token(Parser* parser) {
    if (parser->current_token < parser->token_count) {
        return parser->tokens[parser->current_token];
    }
    Token eof = {TOKEN_EOF, "EOF", 0, 0};
    return eof;
}

void advance_token(Parser* parser) {
    if (parser->current_token < parser->token_count) {
        parser->current_token++;
    }
}

void error(Parser* parser, char* message) {
    Token token = current_token(parser);
    printf("Erro sintático na linha %d, coluna %d: %s (token: '%s')\n", 
           token.line, token.column, message, token.lexeme);
    parser->error_count++;
}

int match(Parser* parser, TokenType type) {
    return current_token(parser).type == type;
}

void consume(Parser* parser, TokenType type, char* error_msg) {
    if (match(parser, type)) {
        advance_token(parser);
    } else {
        error(parser, error_msg);
    }
}

// Forward declarations
ASTNode* parse_expression(Parser* parser);
ASTNode* parse_statement(Parser* parser);

// Parsing functions
ASTNode* parse_primary(Parser* parser) {
    Token token = current_token(parser);
    
    if (match(parser, TOKEN_NUMBER)) {
        advance_token(parser);
        return create_node(NODE_NUMBER, token.lexeme);
    }
    
    if (match(parser, TOKEN_IDENTIFIER)) {
        advance_token(parser);
        
        // Verifica se é uma chamada de função
        if (match(parser, TOKEN_LPAREN)) {
            ASTNode* func_call = create_node(NODE_FUNC_CALL, token.lexeme);
            advance_token(parser); // (
            
            // Lista de argumentos
            if (!match(parser, TOKEN_RPAREN)) {
                add_child(func_call, parse_expression(parser));
                while (match(parser, TOKEN_COMMA)) {
                    advance_token(parser);
                    add_child(func_call, parse_expression(parser));
                }
            }
            
            consume(parser, TOKEN_RPAREN, "Esperado ')'");
            return func_call;
        }
        
        return create_node(NODE_IDENTIFIER, token.lexeme);
    }
    
    if (match(parser, TOKEN_LPAREN)) {
        advance_token(parser);
        ASTNode* expr = parse_expression(parser);
        consume(parser, TOKEN_RPAREN, "Esperado ')'");
        return expr;
    }
    
    error(parser, "Esperado número, identificador ou '('");
    return NULL;
}

ASTNode* parse_unary(Parser* parser) {
    if (match(parser, TOKEN_PLUS) || match(parser, TOKEN_MINUS) || match(parser, TOKEN_NOT)) {
        Token op = current_token(parser);
        advance_token(parser);
        ASTNode* node = create_node(NODE_UNARY_OP, op.lexeme);
        node->left = parse_unary(parser);
        return node;
    }
    
    return parse_primary(parser);
}

ASTNode* parse_multiplicative(Parser* parser) {
    ASTNode* left = parse_unary(parser);
    
    while (match(parser, TOKEN_MULTIPLY) || match(parser, TOKEN_DIVIDE) || match(parser, TOKEN_MODULO)) {
        Token op = current_token(parser);
        advance_token(parser);
        ASTNode* node = create_node(NODE_BINARY_OP, op.lexeme);
        node->left = left;
        node->right = parse_unary(parser);
        left = node;
    }
    
    return left;
}

ASTNode* parse_additive(Parser* parser) {
    ASTNode* left = parse_multiplicative(parser);
    
    while (match(parser, TOKEN_PLUS) || match(parser, TOKEN_MINUS)) {
        Token op = current_token(parser);
        advance_token(parser);
        ASTNode* node = create_node(NODE_BINARY_OP, op.lexeme);
        node->left = left;
        node->right = parse_multiplicative(parser);
        left = node;
    }
    
    return left;
}

ASTNode* parse_relational(Parser* parser) {
    ASTNode* left = parse_additive(parser);
    
    while (match(parser, TOKEN_LT) || match(parser, TOKEN_LE) || 
           match(parser, TOKEN_GT) || match(parser, TOKEN_GE)) {
        Token op = current_token(parser);
        advance_token(parser);
        ASTNode* node = create_node(NODE_BINARY_OP, op.lexeme);
        node->left = left;
        node->right = parse_additive(parser);
        left = node;
    }
    
    return left;
}

ASTNode* parse_equality(Parser* parser) {
    ASTNode* left = parse_relational(parser);
    
    while (match(parser, TOKEN_EQ) || match(parser, TOKEN_NE)) {
        Token op = current_token(parser);
        advance_token(parser);
        ASTNode* node = create_node(NODE_BINARY_OP, op.lexeme);
        node->left = left;
        node->right = parse_relational(parser);
        left = node;
    }
    
    return left;
}

ASTNode* parse_logical_and(Parser* parser) {
    ASTNode* left = parse_equality(parser);
    
    while (match(parser, TOKEN_AND)) {
        Token op = current_token(parser);
        advance_token(parser);
        ASTNode* node = create_node(NODE_BINARY_OP, op.lexeme);
        node->left = left;
        node->right = parse_equality(parser);
        left = node;
    }
    
    return left;
}

ASTNode* parse_logical_or(Parser* parser) {
    ASTNode* left = parse_logical_and(parser);
    
    while (match(parser, TOKEN_OR)) {
        Token op = current_token(parser);
        advance_token(parser);
        ASTNode* node = create_node(NODE_BINARY_OP, op.lexeme);
        node->left = left;
        node->right = parse_logical_and(parser);
        left = node;
    }
    
    return left;
}

ASTNode* parse_assignment(Parser* parser) {
    ASTNode* left = parse_logical_or(parser);
    
    if (match(parser, TOKEN_ASSIGN)) {
        advance_token(parser);
        ASTNode* node = create_node(NODE_ASSIGN, "=");
        node->left = left;
        node->right = parse_assignment(parser);
        return node;
    }
    
    return left;
}

ASTNode* parse_expression(Parser* parser) {
    return parse_assignment(parser);
}

ASTNode* parse_compound_statement(Parser* parser) {
    ASTNode* compound = create_node(NODE_COMPOUND_STMT, NULL);
    
    consume(parser, TOKEN_LBRACE, "Esperado '{'");
    
    while (!match(parser, TOKEN_RBRACE) && !match(parser, TOKEN_EOF)) {
        ASTNode* stmt = parse_statement(parser);
        if (stmt) add_child(compound, stmt);
    }
    
    consume(parser, TOKEN_RBRACE, "Esperado '}'");
    
    return compound;
}

ASTNode* parse_if_statement(Parser* parser) {
    ASTNode* if_stmt = create_node(NODE_IF_STMT, NULL);
    
    advance_token(parser); // if
    consume(parser, TOKEN_LPAREN, "Esperado '(' após 'if'");
    add_child(if_stmt, parse_expression(parser));
    consume(parser, TOKEN_RPAREN, "Esperado ')' após condição");
    add_child(if_stmt, parse_statement(parser));
    
    if (match(parser, TOKEN_ELSE)) {
        advance_token(parser);
        add_child(if_stmt, parse_statement(parser));
    }
    
    return if_stmt;
}

ASTNode* parse_while_statement(Parser* parser) {
    ASTNode* while_stmt = create_node(NODE_WHILE_STMT, NULL);
    
    advance_token(parser); // while
    consume(parser, TOKEN_LPAREN, "Esperado '(' após 'while'");
    add_child(while_stmt, parse_expression(parser));
    consume(parser, TOKEN_RPAREN, "Esperado ')' após condição");
    add_child(while_stmt, parse_statement(parser));
    
    return while_stmt;
}

ASTNode* parse_return_statement(Parser* parser) {
    ASTNode* return_stmt = create_node(NODE_RETURN_STMT, NULL);
    
    advance_token(parser); // return
    
    if (!match(parser, TOKEN_SEMICOLON)) {
        add_child(return_stmt, parse_expression(parser));
    }
    
    consume(parser, TOKEN_SEMICOLON, "Esperado ';' após return");
    
    return return_stmt;
}

ASTNode* parse_expression_statement(Parser* parser) {
    ASTNode* expr_stmt = create_node(NODE_EXPRESSION_STMT, NULL);
    
    if (!match(parser, TOKEN_SEMICOLON)) {
        add_child(expr_stmt, parse_expression(parser));
    }
    
    consume(parser, TOKEN_SEMICOLON, "Esperado ';'");
    
    return expr_stmt;
}

ASTNode* parse_statement(Parser* parser) {
    if (match(parser, TOKEN_LBRACE)) {
        return parse_compound_statement(parser);
    }
    
    if (match(parser, TOKEN_IF)) {
        return parse_if_statement(parser);
    }
    
    if (match(parser, TOKEN_WHILE)) {
        return parse_while_statement(parser);
    }
    
    if (match(parser, TOKEN_RETURN)) {
        return parse_return_statement(parser);
    }
    
    return parse_expression_statement(parser);
}

ASTNode* parse_variable_declaration(Parser* parser) {
    // Pula o tipo (int, float, char)
    Token type_token = current_token(parser);
    advance_token(parser);
    
    Token name_token = current_token(parser);
    consume(parser, TOKEN_IDENTIFIER, "Esperado nome da variável");
    
    ASTNode* var_decl = create_node(NODE_VAR_DECL, name_token.lexeme);
    var_decl->left = create_node(NODE_IDENTIFIER, type_token.lexeme);
    
    if (match(parser, TOKEN_ASSIGN)) {
        advance_token(parser);
        var_decl->right = parse_expression(parser);
    }
    
    consume(parser, TOKEN_SEMICOLON, "Esperado ';' após declaração");
    
    return var_decl;
}

ASTNode* parse_function_declaration(Parser* parser) {
    // Pula o tipo de retorno
    Token return_type = current_token(parser);
    advance_token(parser);
    
    Token name_token = current_token(parser);
    consume(parser, TOKEN_IDENTIFIER, "Esperado nome da função");
    
    ASTNode* func_decl = create_node(NODE_FUNC_DECL, name_token.lexeme);
    func_decl->left = create_node(NODE_IDENTIFIER, return_type.lexeme);
    
    consume(parser, TOKEN_LPAREN, "Esperado '(' após nome da função");
    
    // Parâmetros
    if (!match(parser, TOKEN_RPAREN)) {
        // Parseia parâmetros...
        // Simplificado para este exemplo
        while (!match(parser, TOKEN_RPAREN) && !match(parser, TOKEN_EOF)) {
            advance_token(parser); // simplificado
        }
    }
    
    consume(parser, TOKEN_RPAREN, "Esperado ')'");
    func_decl->right = parse_compound_statement(parser);
    
    return func_decl;
}

ASTNode* parse_declaration(Parser* parser) {
    if (match(parser, TOKEN_INT) || match(parser, TOKEN_FLOAT) || match(parser, TOKEN_CHAR)) {
        // Lookahead para decidir se é função ou variável
        int saved_pos = parser->current_token;
        advance_token(parser); // tipo
        advance_token(parser); // nome
        
        if (match(parser, TOKEN_LPAREN)) {
            // É uma função
            parser->current_token = saved_pos;
            return parse_function_declaration(parser);
        } else {
            // É uma variável
            parser->current_token = saved_pos;
            return parse_variable_declaration(parser);
        }
    }
    
    return parse_statement(parser);
}

ASTNode* parse_program(Parser* parser) {
    ASTNode* program = create_node(NODE_PROGRAM, NULL);
    
    while (!match(parser, TOKEN_EOF)) {
        ASTNode* decl = parse_declaration(parser);
        if (decl) add_child(program, decl);
    }
    
    return program;
}

// ==================== MAIN ====================

const char* sample_code = 
    "int factorial(int n) {\n"
    "    if (n <= 1) {\n"
    "        return 1;\n"
    "    } else {\n"
    "        return n * factorial(n - 1);\n"
    "    }\n"
    "}\n"
    "\n"
    "int main() {\n"
    "    int result;\n"
    "    result = factorial(5);\n"
    "    return 0;\n"
    "}\n";

int main() {
    printf("=== ANALISADOR SINTÁTICO DESCENDENTE RECURSIVO ===\n");
    printf("Código de entrada:\n%s\n", sample_code);
    
    // Tokenização
    printf("=== PHASE 1: ANÁLISE LÉXICA ===\n");
    int token_count;
    Token* tokens = tokenize((char*)sample_code, &token_count);
    
    printf("Tokens identificados (%d):\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("  %2d: %-15s '%s'\n", i+1, 
               tokens[i].type == TOKEN_IDENTIFIER ? "IDENTIFIER" :
               tokens[i].type == TOKEN_NUMBER ? "NUMBER" :
               tokens[i].type == TOKEN_INT ? "INT" :
               tokens[i].type == TOKEN_IF ? "IF" :
               tokens[i].type == TOKEN_ELSE ? "ELSE" :
               tokens[i].type == TOKEN_RETURN ? "RETURN" :
               tokens[i].type == TOKEN_ASSIGN ? "ASSIGN" :
               tokens[i].type == TOKEN_EQ ? "EQ" :
               tokens[i].type == TOKEN_LE ? "LE" :
               tokens[i].type == TOKEN_MULTIPLY ? "MULTIPLY" :
               tokens[i].type == TOKEN_MINUS ? "MINUS" :
               tokens[i].type == TOKEN_SEMICOLON ? "SEMICOLON" :
               tokens[i].type == TOKEN_LPAREN ? "LPAREN" :
               tokens[i].type == TOKEN_RPAREN ? "RPAREN" :
               tokens[i].type == TOKEN_LBRACE ? "LBRACE" :
               tokens[i].type == TOKEN_RBRACE ? "RBRACE" :
               tokens[i].type == TOKEN_EOF ? "EOF" : "OTHER",
               tokens[i].lexeme);
    }
    
    // Análise sintática
    printf("\n=== PHASE 2: ANÁLISE SINTÁTICA ===\n");
    Parser parser;
    init_parser(&parser, tokens, token_count);
    
    ASTNode* ast = parse_program(&parser);
    
    if (parser.error_count == 0) {
        printf("✓ Análise sintática completada com sucesso!\n");
        printf("\n=== ÁRVORE SINTÁTICA ABSTRATA ===\n");
        print_ast(ast, 0);
    } else {
        printf("✗ Análise sintática falhou com %d erro(s).\n", parser.error_count);
    }
    
    printf("\n=== INFORMAÇÕES SOBRE A GRAMÁTICA ===\n");
    printf("Características da gramática implementada:\n");
    printf("• Precedência de operadores: () > !,+,- > *,/,%% > +,- > <,<=,>,>= > ==,!= > && > || > =\n");
    printf("• Associatividade: Operadores são associativos à esquerda (exceto =)\n");
    printf("• Estruturas suportadas: if-else, while, funções, declarações de variáveis\n");
    printf("• Tipos suportados: int, float, char\n");
    printf("• Expressões: Aritméticas, lógicas, relacionais, atribuição, chamadas de função\n");
    
    printf("\n=== APLICAÇÕES DO PARSER ===\n");
    printf("• Validação de sintaxe em IDEs\n");
    printf("• Construção de compiladores e interpretadores\n");
    printf("• Análise de código fonte para refatoração\n");
    printf("• Geração de documentação automática\n");
    printf("• Verificação de conformidade com padrões de codificação\n");
    
    // Cleanup
    free(tokens);
    
    return 0;
}