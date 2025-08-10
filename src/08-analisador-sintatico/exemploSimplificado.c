/**
 * Analisador Sintático Simplificado para Expressões Aritméticas
 * 
 * Este programa implementa um parser descendente recursivo que reconhece
 * expressões aritméticas com a seguinte gramática:
 * 
 * GRAMÁTICA:
 * expression   ::= term (('+' | '-') term)*
 * term         ::= factor (('*' | '/') factor)*
 * factor       ::= number | identifier | '(' expression ')'
 * number       ::= [0-9]+
 * identifier   ::= [a-zA-Z_][a-zA-Z0-9_]*
 * 
 * Características:
 * - Parser descendente recursivo
 * - Tratamento correto de precedência de operadores
 * - Construção de árvore sintática abstrata (AST)
 * - Avaliação de expressões
 * - Detecção de erros sintáticos
 * 
 * Exemplos de entrada válida:
 * - "2 + 3 * 4"
 * - "x + y - z"
 * - "(a + b) * c"
 * - "10 / (2 + 3)"
 * 
 * Autor: Disciplina de Compiladores
 * Data: 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100

// Tipos de tokens
typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

// Estrutura do token
typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
    double value;  // Para números
} Token;

// Tipos de nós da AST
typedef enum {
    NODE_NUMBER,
    NODE_IDENTIFIER,
    NODE_BINARY_OP,
    NODE_UNARY_OP
} NodeType;

// Nó da árvore sintática
typedef struct ASTNode {
    NodeType type;
    char op;                    // Para operadores
    double value;               // Para números
    char name[MAX_TOKEN_LENGTH]; // Para identificadores
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

// Estado do parser
typedef struct {
    const char* input;
    int position;
    Token current_token;
    int has_error;
    char error_message[200];
} Parser;

// Tabela de símbolos simples para variáveis
typedef struct {
    char name[MAX_TOKEN_LENGTH];
    double value;
} Variable;

Variable variables[100];
int var_count = 0;

// ==================== LEXER ====================

void skip_whitespace(Parser* parser) {
    while (isspace(parser->input[parser->position])) {
        parser->position++;
    }
}

Token get_next_token(Parser* parser) {
    Token token;
    skip_whitespace(parser);
    
    char ch = parser->input[parser->position];
    
    // Fim da entrada
    if (ch == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }
    
    // Números
    if (isdigit(ch)) {
        int i = 0;
        while (isdigit(parser->input[parser->position]) && i < MAX_TOKEN_LENGTH - 1) {
            token.lexeme[i++] = parser->input[parser->position++];
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_NUMBER;
        token.value = atof(token.lexeme);
        return token;
    }
    
    // Identificadores
    if (isalpha(ch) || ch == '_') {
        int i = 0;
        while ((isalnum(parser->input[parser->position]) || parser->input[parser->position] == '_') 
               && i < MAX_TOKEN_LENGTH - 1) {
            token.lexeme[i++] = parser->input[parser->position++];
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_IDENTIFIER;
        return token;
    }
    
    // Operadores e delimitadores
    token.lexeme[0] = ch;
    token.lexeme[1] = '\0';
    parser->position++;
    
    switch (ch) {
        case '+': token.type = TOKEN_PLUS; break;
        case '-': token.type = TOKEN_MINUS; break;
        case '*': token.type = TOKEN_MULTIPLY; break;
        case '/': token.type = TOKEN_DIVIDE; break;
        case '(': token.type = TOKEN_LPAREN; break;
        case ')': token.type = TOKEN_RPAREN; break;
        default:
            token.type = TOKEN_ERROR;
            sprintf(parser->error_message, "Caractere inesperado: '%c'", ch);
            parser->has_error = 1;
            break;
    }
    
    return token;
}

void advance_token(Parser* parser) {
    parser->current_token = get_next_token(parser);
}

// ==================== AST ====================

ASTNode* create_number_node(double value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode* create_identifier_node(const char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_IDENTIFIER;
    strcpy(node->name, name);
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode* create_binary_op_node(char op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BINARY_OP;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

void print_ast(ASTNode* node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; i++) printf("  ");
    
    switch (node->type) {
        case NODE_NUMBER:
            printf("NUMBER: %.2f\n", node->value);
            break;
        case NODE_IDENTIFIER:
            printf("IDENTIFIER: %s\n", node->name);
            break;
        case NODE_BINARY_OP:
            printf("BINARY_OP: %c\n", node->op);
            break;
        case NODE_UNARY_OP:
            printf("UNARY_OP: %c\n", node->op);
            break;
    }
    
    if (node->left) print_ast(node->left, depth + 1);
    if (node->right) print_ast(node->right, depth + 1);
}

// ==================== PARSER ====================

// Forward declarations
ASTNode* parse_expression(Parser* parser);

ASTNode* parse_factor(Parser* parser) {
    Token token = parser->current_token;
    
    if (token.type == TOKEN_NUMBER) {
        advance_token(parser);
        return create_number_node(token.value);
    }
    
    if (token.type == TOKEN_IDENTIFIER) {
        advance_token(parser);
        return create_identifier_node(token.lexeme);
    }
    
    if (token.type == TOKEN_LPAREN) {
        advance_token(parser); // consome '('
        ASTNode* node = parse_expression(parser);
        
        if (parser->current_token.type != TOKEN_RPAREN) {
            sprintf(parser->error_message, "Esperado ')' mas encontrou '%s'", 
                    parser->current_token.lexeme);
            parser->has_error = 1;
            return NULL;
        }
        
        advance_token(parser); // consome ')'
        return node;
    }
    
    sprintf(parser->error_message, "Esperado número, identificador ou '(' mas encontrou '%s'", 
            token.lexeme);
    parser->has_error = 1;
    return NULL;
}

ASTNode* parse_term(Parser* parser) {
    ASTNode* left = parse_factor(parser);
    
    while (parser->current_token.type == TOKEN_MULTIPLY || 
           parser->current_token.type == TOKEN_DIVIDE) {
        
        char op = (parser->current_token.type == TOKEN_MULTIPLY) ? '*' : '/';
        advance_token(parser);
        
        ASTNode* right = parse_factor(parser);
        left = create_binary_op_node(op, left, right);
    }
    
    return left;
}

ASTNode* parse_expression(Parser* parser) {
    ASTNode* left = parse_term(parser);
    
    while (parser->current_token.type == TOKEN_PLUS || 
           parser->current_token.type == TOKEN_MINUS) {
        
        char op = (parser->current_token.type == TOKEN_PLUS) ? '+' : '-';
        advance_token(parser);
        
        ASTNode* right = parse_term(parser);
        left = create_binary_op_node(op, left, right);
    }
    
    return left;
}

ASTNode* parse(Parser* parser) {
    advance_token(parser); // lê o primeiro token
    ASTNode* ast = parse_expression(parser);
    
    if (parser->current_token.type != TOKEN_EOF && !parser->has_error) {
        sprintf(parser->error_message, "Token inesperado no final: '%s'", 
                parser->current_token.lexeme);
        parser->has_error = 1;
    }
    
    return ast;
}

// ==================== AVALIADOR ====================

double get_variable_value(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    return 0.0; // valor padrão se variável não encontrada
}

void set_variable_value(const char* name, double value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            return;
        }
    }
    
    // Adiciona nova variável
    if (var_count < 100) {
        strcpy(variables[var_count].name, name);
        variables[var_count].value = value;
        var_count++;
    }
}

double evaluate_ast(ASTNode* node) {
    if (!node) return 0.0;
    
    switch (node->type) {
        case NODE_NUMBER:
            return node->value;
            
        case NODE_IDENTIFIER:
            return get_variable_value(node->name);
            
        case NODE_BINARY_OP:
            {
                double left_val = evaluate_ast(node->left);
                double right_val = evaluate_ast(node->right);
                
                switch (node->op) {
                    case '+': return left_val + right_val;
                    case '-': return left_val - right_val;
                    case '*': return left_val * right_val;
                    case '/': 
                        if (right_val == 0.0) {
                            printf("Erro: Divisão por zero!\n");
                            return 0.0;
                        }
                        return left_val / right_val;
                    default: return 0.0;
                }
            }
            
        default:
            return 0.0;
    }
}

// ==================== DEMO ====================

void test_expression(const char* expression) {
    printf("\n=== TESTANDO EXPRESSÃO: %s ===\n", expression);
    
    Parser parser;
    parser.input = expression;
    parser.position = 0;
    parser.has_error = 0;
    
    ASTNode* ast = parse(&parser);
    
    if (parser.has_error) {
        printf("✗ ERRO SINTÁTICO: %s\n", parser.error_message);
        return;
    }
    
    printf("✓ Análise sintática bem-sucedida!\n");
    printf("\nÁrvore Sintática Abstrata:\n");
    print_ast(ast, 0);
    
    double result = evaluate_ast(ast);
    printf("\nResultado da avaliação: %.2f\n", result);
    
    // Libera memória (simplificado)
    // Em um programa real, implementaríamos free_ast(ast);
}

void demonstrate_precedence() {
    printf("\n=== DEMONSTRAÇÃO DE PRECEDÊNCIA DE OPERADORES ===\n");
    printf("As seguintes expressões mostram como a precedência é tratada:\n\n");
    
    test_expression("2 + 3 * 4");      // Deve resultar em 14, não 20
    test_expression("10 - 6 / 2");     // Deve resultar em 7, não 2
    test_expression("(2 + 3) * 4");    // Deve resultar em 20
    test_expression("10 / (2 + 3)");   // Deve resultar em 2
}

void demonstrate_variables() {
    printf("\n=== DEMONSTRAÇÃO COM VARIÁVEIS ===\n");
    printf("Definindo variáveis: x = 5, y = 3, z = 2\n");
    
    set_variable_value("x", 5.0);
    set_variable_value("y", 3.0);
    set_variable_value("z", 2.0);
    
    test_expression("x + y");
    test_expression("x * y - z");
    test_expression("(x + y) / z");
}

void demonstrate_error_handling() {
    printf("\n=== DEMONSTRAÇÃO DE TRATAMENTO DE ERROS ===\n");
    printf("As seguintes expressões contêm erros sintáticos:\n");
    
    test_expression("2 + + 3");        // Operador duplo
    test_expression("2 + 3)");         // Parêntese desbalanceado
    test_expression("(2 + 3");         // Parêntese não fechado
    test_expression("2 +");            // Expressão incompleta
    test_expression("+ 2 3");          // Operador no início
}

int main() {
    printf("=== ANALISADOR SINTÁTICO PARA EXPRESSÕES ARITMÉTICAS ===\n");
    printf("Este programa demonstra um parser descendente recursivo que\n");
    printf("reconhece e avalia expressões aritméticas com precedência correta.\n");
    
    printf("\n=== GRAMÁTICA SUPORTADA ===\n");
    printf("expression ::= term (('+' | '-') term)*\n");
    printf("term       ::= factor (('*' | '/') factor)*\n");
    printf("factor     ::= number | identifier | '(' expression ')'\n");
    printf("number     ::= [0-9]+\n");
    printf("identifier ::= [a-zA-Z_][a-zA-Z0-9_]*\n");
    
    // Testes de expressões válidas
    test_expression("42");
    test_expression("2 + 3");
    test_expression("10 - 5");
    test_expression("4 * 7");
    test_expression("15 / 3");
    test_expression("2 + 3 * 4 - 1");
    test_expression("(2 + 3) * (4 - 1)");
    
    // Demonstrações especiais
    demonstrate_precedence();
    demonstrate_variables();
    demonstrate_error_handling();
    
    printf("\n=== CARACTERÍSTICAS DO PARSER ===\n");
    printf("• Método: Descendente recursivo\n");
    printf("• Precedência: * e / têm precedência maior que + e -\n");
    printf("• Associatividade: Operadores são associativos à esquerda\n");
    printf("• Parênteses: Suportados para alterar precedência\n");
    printf("• Variáveis: Suportadas com tabela de símbolos simples\n");
    printf("• Tratamento de erros: Detecção e relatório de erros sintáticos\n");
    
    printf("\n=== APLICAÇÕES ===\n");
    printf("• Calculadoras científicas\n");
    printf("• Interpretadores de linguagens de programação\n");
    printf("• Sistemas de planilhas eletrônicas\n");
    printf("• Compiladores (fase de análise sintática)\n");
    printf("• Avaliadores de fórmulas matemáticas\n");
    
    return 0;
}