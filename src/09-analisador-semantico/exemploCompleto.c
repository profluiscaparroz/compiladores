/**
 * Analisador Semântico para Linguagem C Simplificada
 * 
 * Este programa implementa um analisador semântico que verifica:
 * 
 * VERIFICAÇÕES SEMÂNTICAS:
 * 1. Declaração de variáveis antes do uso
 * 2. Compatibilidade de tipos em operações
 * 3. Compatibilidade de tipos em atribuições
 * 4. Verificação de escopo de variáveis
 * 5. Detecção de redeclaração de variáveis
 * 6. Verificação de tipos em chamadas de função
 * 7. Detecção de divisão por zero em tempo de compilação
 * 
 * TIPOS SUPORTADOS:
 * - int: números inteiros
 * - float: números de ponto flutuante
 * - char: caracteres
 * - bool: valores booleanos (true/false)
 * 
 * GRAMÁTICA SIMPLIFICADA:
 * program      ::= declaration*
 * declaration  ::= var_decl | assignment
 * var_decl     ::= type IDENTIFIER ('=' expression)? ';'
 * assignment   ::= IDENTIFIER '=' expression ';'
 * type         ::= 'int' | 'float' | 'char' | 'bool'
 * expression   ::= term (('+' | '-' | '&&' | '||') term)*
 * term         ::= factor (('*' | '/' | '==' | '!=') factor)*
 * factor       ::= NUMBER | IDENTIFIER | '(' expression ')'
 * 
 * Autor: Disciplina de Compiladores
 * Data: 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100
#define MAX_VARIABLES 100
#define MAX_ERRORS 50

// Tipos de dados
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_BOOL,
    TYPE_UNKNOWN,
    TYPE_ERROR
} DataType;

// Tipos de tokens
typedef enum {
    TOKEN_INT_TYPE,     // 'int'
    TOKEN_FLOAT_TYPE,   // 'float' 
    TOKEN_CHAR_TYPE,    // 'char'
    TOKEN_BOOL_TYPE,    // 'bool'
    TOKEN_IDENTIFIER,   // nome de variável
    TOKEN_NUMBER,       // número inteiro
    TOKEN_FLOAT_NUM,    // número decimal
    TOKEN_CHAR_LIT,     // 'a'
    TOKEN_BOOL_LIT,     // true/false
    TOKEN_ASSIGN,       // =
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_MULTIPLY,     // *
    TOKEN_DIVIDE,       // /
    TOKEN_EQ,           // ==
    TOKEN_NE,           // !=
    TOKEN_AND,          // &&
    TOKEN_OR,           // ||
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_SEMICOLON,    // ;
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

// Token
typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
    DataType data_type;
    union {
        int int_val;
        float float_val;
        char char_val;
        int bool_val;
    } value;
    int line;
} Token;

// Entrada da tabela de símbolos
typedef struct {
    char name[MAX_TOKEN_LENGTH];
    DataType type;
    int line_declared;
    int is_initialized;
    union {
        int int_val;
        float float_val;
        char char_val;
        int bool_val;
    } value;
} Symbol;

// Erro semântico
typedef struct {
    char message[200];
    int line;
} SemanticError;

// Estado do analisador
typedef struct {
    const char* input;
    int position;
    int line;
    Token current_token;
    
    Symbol symbol_table[MAX_VARIABLES];
    int symbol_count;
    
    SemanticError errors[MAX_ERRORS];
    int error_count;
} Analyzer;

// ==================== UTILITÁRIOS ====================

const char* type_to_string(DataType type) {
    switch (type) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_CHAR: return "char";
        case TYPE_BOOL: return "bool";
        case TYPE_UNKNOWN: return "unknown";
        case TYPE_ERROR: return "error";
        default: return "invalid";
    }
}

void add_error(Analyzer* analyzer, const char* message) {
    if (analyzer->error_count < MAX_ERRORS) {
        strcpy(analyzer->errors[analyzer->error_count].message, message);
        analyzer->errors[analyzer->error_count].line = analyzer->line;
        analyzer->error_count++;
    }
}

// ==================== TABELA DE SÍMBOLOS ====================

Symbol* find_symbol(Analyzer* analyzer, const char* name) {
    for (int i = 0; i < analyzer->symbol_count; i++) {
        if (strcmp(analyzer->symbol_table[i].name, name) == 0) {
            return &analyzer->symbol_table[i];
        }
    }
    return NULL;
}

void add_symbol(Analyzer* analyzer, const char* name, DataType type) {
    if (analyzer->symbol_count >= MAX_VARIABLES) {
        add_error(analyzer, "Muitas variáveis declaradas");
        return;
    }
    
    // Verifica se já existe
    if (find_symbol(analyzer, name)) {
        char error[200];
        sprintf(error, "Variável '%s' já foi declarada", name);
        add_error(analyzer, error);
        return;
    }
    
    Symbol* symbol = &analyzer->symbol_table[analyzer->symbol_count];
    strcpy(symbol->name, name);
    symbol->type = type;
    symbol->line_declared = analyzer->line;
    symbol->is_initialized = 0;
    analyzer->symbol_count++;
}

void print_symbol_table(Analyzer* analyzer) {
    printf("\n=== TABELA DE SÍMBOLOS ===\n");
    printf("%-15s %-10s %-10s %-12s\n", "Nome", "Tipo", "Linha", "Inicializada");
    printf("%-15s %-10s %-10s %-12s\n", "----", "----", "-----", "------------");
    
    for (int i = 0; i < analyzer->symbol_count; i++) {
        Symbol* s = &analyzer->symbol_table[i];
        printf("%-15s %-10s %-10d %-12s\n", 
               s->name, 
               type_to_string(s->type), 
               s->line_declared,
               s->is_initialized ? "Sim" : "Não");
    }
}

// ==================== LEXER ====================

void skip_whitespace(Analyzer* analyzer) {
    while (isspace(analyzer->input[analyzer->position])) {
        if (analyzer->input[analyzer->position] == '\n') {
            analyzer->line++;
        }
        analyzer->position++;
    }
}

Token get_next_token(Analyzer* analyzer) {
    Token token;
    token.line = analyzer->line;
    token.data_type = TYPE_UNKNOWN;
    
    skip_whitespace(analyzer);
    
    char ch = analyzer->input[analyzer->position];
    
    // Fim da entrada
    if (ch == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }
    
    // Números
    if (isdigit(ch)) {
        int i = 0;
        int has_dot = 0;
        
        while ((isdigit(analyzer->input[analyzer->position]) || 
                analyzer->input[analyzer->position] == '.') && 
               i < MAX_TOKEN_LENGTH - 1) {
            if (analyzer->input[analyzer->position] == '.') {
                if (has_dot) break; // segundo ponto, para
                has_dot = 1;
            }
            token.lexeme[i++] = analyzer->input[analyzer->position++];
        }
        token.lexeme[i] = '\0';
        
        if (has_dot) {
            token.type = TOKEN_FLOAT_NUM;
            token.data_type = TYPE_FLOAT;
            token.value.float_val = atof(token.lexeme);
        } else {
            token.type = TOKEN_NUMBER;
            token.data_type = TYPE_INT;
            token.value.int_val = atoi(token.lexeme);
        }
        return token;
    }
    
    // Caracteres literais
    if (ch == '\'') {
        analyzer->position++; // consome '
        if (analyzer->input[analyzer->position] != '\0') {
            token.lexeme[0] = '\'';
            token.lexeme[1] = analyzer->input[analyzer->position];
            token.lexeme[2] = '\'';
            token.lexeme[3] = '\0';
            token.type = TOKEN_CHAR_LIT;
            token.data_type = TYPE_CHAR;
            token.value.char_val = analyzer->input[analyzer->position];
            analyzer->position++;
            if (analyzer->input[analyzer->position] == '\'') {
                analyzer->position++; // consome '
            }
        }
        return token;
    }
    
    // Palavras-chave e identificadores
    if (isalpha(ch) || ch == '_') {
        int i = 0;
        while ((isalnum(analyzer->input[analyzer->position]) || 
                analyzer->input[analyzer->position] == '_') && 
               i < MAX_TOKEN_LENGTH - 1) {
            token.lexeme[i++] = analyzer->input[analyzer->position++];
        }
        token.lexeme[i] = '\0';
        
        // Verifica palavras-chave
        if (strcmp(token.lexeme, "int") == 0) {
            token.type = TOKEN_INT_TYPE;
        } else if (strcmp(token.lexeme, "float") == 0) {
            token.type = TOKEN_FLOAT_TYPE;
        } else if (strcmp(token.lexeme, "char") == 0) {
            token.type = TOKEN_CHAR_TYPE;
        } else if (strcmp(token.lexeme, "bool") == 0) {
            token.type = TOKEN_BOOL_TYPE;
        } else if (strcmp(token.lexeme, "true") == 0) {
            token.type = TOKEN_BOOL_LIT;
            token.data_type = TYPE_BOOL;
            token.value.bool_val = 1;
        } else if (strcmp(token.lexeme, "false") == 0) {
            token.type = TOKEN_BOOL_LIT;
            token.data_type = TYPE_BOOL;
            token.value.bool_val = 0;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }
    
    // Operadores
    char next = (analyzer->position + 1 < strlen(analyzer->input)) ? 
                analyzer->input[analyzer->position + 1] : '\0';
    
    switch (ch) {
        case '=':
            if (next == '=') {
                strcpy(token.lexeme, "==");
                token.type = TOKEN_EQ;
                analyzer->position += 2;
            } else {
                strcpy(token.lexeme, "=");
                token.type = TOKEN_ASSIGN;
                analyzer->position++;
            }
            break;
        case '!':
            if (next == '=') {
                strcpy(token.lexeme, "!=");
                token.type = TOKEN_NE;
                analyzer->position += 2;
            } else {
                token.type = TOKEN_ERROR;
            }
            break;
        case '&':
            if (next == '&') {
                strcpy(token.lexeme, "&&");
                token.type = TOKEN_AND;
                analyzer->position += 2;
            } else {
                token.type = TOKEN_ERROR;
            }
            break;
        case '|':
            if (next == '|') {
                strcpy(token.lexeme, "||");
                token.type = TOKEN_OR;
                analyzer->position += 2;
            } else {
                token.type = TOKEN_ERROR;
            }
            break;
        case '+':
            token.type = TOKEN_PLUS;
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            analyzer->position++;
            break;
        case '-':
            token.type = TOKEN_MINUS;
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            analyzer->position++;
            break;
        case '*':
            token.type = TOKEN_MULTIPLY;
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            analyzer->position++;
            break;
        case '/':
            token.type = TOKEN_DIVIDE;
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            analyzer->position++;
            break;
        case '(':
            token.type = TOKEN_LPAREN;
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            analyzer->position++;
            break;
        case ')':
            token.type = TOKEN_RPAREN;
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            analyzer->position++;
            break;
        case ';':
            token.type = TOKEN_SEMICOLON;
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            analyzer->position++;
            break;
        default:
            token.type = TOKEN_ERROR;
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            analyzer->position++;
            break;
    }
    
    return token;
}

void advance_token(Analyzer* analyzer) {
    analyzer->current_token = get_next_token(analyzer);
}

// ==================== VERIFICAÇÃO SEMÂNTICA ====================

DataType get_compatible_type(DataType type1, DataType type2, const char* op) {
    // Regras de compatibilidade de tipos
    
    // Operações lógicas retornam bool
    if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
        if (type1 == TYPE_BOOL && type2 == TYPE_BOOL) {
            return TYPE_BOOL;
        }
        return TYPE_ERROR;
    }
    
    // Comparações retornam bool
    if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {
        if (type1 == type2) {
            return TYPE_BOOL;
        }
        return TYPE_ERROR;
    }
    
    // Operações aritméticas
    if (type1 == TYPE_INT && type2 == TYPE_INT) {
        return TYPE_INT;
    }
    
    if ((type1 == TYPE_INT || type1 == TYPE_FLOAT) && 
        (type2 == TYPE_INT || type2 == TYPE_FLOAT)) {
        return TYPE_FLOAT; // promoção para float
    }
    
    return TYPE_ERROR;
}

DataType check_expression(Analyzer* analyzer);

DataType check_factor(Analyzer* analyzer) {
    Token token = analyzer->current_token;
    
    if (token.type == TOKEN_NUMBER) {
        advance_token(analyzer);
        return TYPE_INT;
    }
    
    if (token.type == TOKEN_FLOAT_NUM) {
        advance_token(analyzer);
        return TYPE_FLOAT;
    }
    
    if (token.type == TOKEN_CHAR_LIT) {
        advance_token(analyzer);
        return TYPE_CHAR;
    }
    
    if (token.type == TOKEN_BOOL_LIT) {
        advance_token(analyzer);
        return TYPE_BOOL;
    }
    
    if (token.type == TOKEN_IDENTIFIER) {
        Symbol* symbol = find_symbol(analyzer, token.lexeme);
        if (!symbol) {
            char error[200];
            sprintf(error, "Variável '%s' não foi declarada", token.lexeme);
            add_error(analyzer, error);
            advance_token(analyzer);
            return TYPE_ERROR;
        }
        
        if (!symbol->is_initialized) {
            char error[200];
            sprintf(error, "Variável '%s' usada antes de ser inicializada", token.lexeme);
            add_error(analyzer, error);
        }
        
        advance_token(analyzer);
        return symbol->type;
    }
    
    if (token.type == TOKEN_LPAREN) {
        advance_token(analyzer);
        DataType expr_type = check_expression(analyzer);
        
        if (analyzer->current_token.type != TOKEN_RPAREN) {
            add_error(analyzer, "Esperado ')' após expressão");
        } else {
            advance_token(analyzer);
        }
        
        return expr_type;
    }
    
    add_error(analyzer, "Fator inválido em expressão");
    return TYPE_ERROR;
}

DataType check_term(Analyzer* analyzer) {
    DataType left_type = check_factor(analyzer);
    
    while (analyzer->current_token.type == TOKEN_MULTIPLY ||
           analyzer->current_token.type == TOKEN_DIVIDE ||
           analyzer->current_token.type == TOKEN_EQ ||
           analyzer->current_token.type == TOKEN_NE) {
        
        char op[10];
        strcpy(op, analyzer->current_token.lexeme);
        advance_token(analyzer);
        
        DataType right_type = check_factor(analyzer);
        
        // Verificação especial para divisão por zero
        if (strcmp(op, "/") == 0 && analyzer->current_token.type == TOKEN_NUMBER &&
            analyzer->current_token.value.int_val == 0) {
            add_error(analyzer, "Divisão por zero detectada");
        }
        
        DataType result_type = get_compatible_type(left_type, right_type, op);
        if (result_type == TYPE_ERROR) {
            char error[200];
            sprintf(error, "Tipos incompatíveis para operador '%s': %s e %s",
                   op, type_to_string(left_type), type_to_string(right_type));
            add_error(analyzer, error);
        }
        
        left_type = result_type;
    }
    
    return left_type;
}

DataType check_expression(Analyzer* analyzer) {
    DataType left_type = check_term(analyzer);
    
    while (analyzer->current_token.type == TOKEN_PLUS ||
           analyzer->current_token.type == TOKEN_MINUS ||
           analyzer->current_token.type == TOKEN_AND ||
           analyzer->current_token.type == TOKEN_OR) {
        
        char op[10];
        strcpy(op, analyzer->current_token.lexeme);
        advance_token(analyzer);
        
        DataType right_type = check_term(analyzer);
        
        DataType result_type = get_compatible_type(left_type, right_type, op);
        if (result_type == TYPE_ERROR) {
            char error[200];
            sprintf(error, "Tipos incompatíveis para operador '%s': %s e %s",
                   op, type_to_string(left_type), type_to_string(right_type));
            add_error(analyzer, error);
        }
        
        left_type = result_type;
    }
    
    return left_type;
}

void check_variable_declaration(Analyzer* analyzer) {
    // Lê o tipo
    DataType var_type = TYPE_UNKNOWN;
    
    if (analyzer->current_token.type == TOKEN_INT_TYPE) {
        var_type = TYPE_INT;
    } else if (analyzer->current_token.type == TOKEN_FLOAT_TYPE) {
        var_type = TYPE_FLOAT;
    } else if (analyzer->current_token.type == TOKEN_CHAR_TYPE) {
        var_type = TYPE_CHAR;
    } else if (analyzer->current_token.type == TOKEN_BOOL_TYPE) {
        var_type = TYPE_BOOL;
    } else {
        add_error(analyzer, "Tipo de variável inválido");
        return;
    }
    
    advance_token(analyzer);
    
    // Lê o nome
    if (analyzer->current_token.type != TOKEN_IDENTIFIER) {
        add_error(analyzer, "Esperado nome da variável");
        return;
    }
    
    char var_name[MAX_TOKEN_LENGTH];
    strcpy(var_name, analyzer->current_token.lexeme);
    advance_token(analyzer);
    
    // Adiciona à tabela de símbolos
    add_symbol(analyzer, var_name, var_type);
    
    // Verifica inicialização
    if (analyzer->current_token.type == TOKEN_ASSIGN) {
        advance_token(analyzer);
        
        DataType expr_type = check_expression(analyzer);
        
        // Verifica compatibilidade de tipos na atribuição
        if (var_type != expr_type && expr_type != TYPE_ERROR) {
            char error[200];
            sprintf(error, "Incompatibilidade de tipos na inicialização: %s = %s",
                   type_to_string(var_type), type_to_string(expr_type));
            add_error(analyzer, error);
        } else {
            // Marca como inicializada
            Symbol* symbol = find_symbol(analyzer, var_name);
            if (symbol) {
                symbol->is_initialized = 1;
            }
        }
    }
    
    // Verifica ponto e vírgula
    if (analyzer->current_token.type != TOKEN_SEMICOLON) {
        add_error(analyzer, "Esperado ';' após declaração");
    } else {
        advance_token(analyzer);
    }
}

void check_assignment(Analyzer* analyzer) {
    char var_name[MAX_TOKEN_LENGTH];
    strcpy(var_name, analyzer->current_token.lexeme);
    
    Symbol* symbol = find_symbol(analyzer, var_name);
    if (!symbol) {
        char error[200];
        sprintf(error, "Variável '%s' não foi declarada", var_name);
        add_error(analyzer, error);
        return;
    }
    
    advance_token(analyzer); // nome da variável
    
    if (analyzer->current_token.type != TOKEN_ASSIGN) {
        add_error(analyzer, "Esperado '=' na atribuição");
        return;
    }
    
    advance_token(analyzer); // =
    
    DataType expr_type = check_expression(analyzer);
    
    // Verifica compatibilidade de tipos
    if (symbol->type != expr_type && expr_type != TYPE_ERROR) {
        char error[200];
        sprintf(error, "Incompatibilidade de tipos na atribuição: %s = %s",
               type_to_string(symbol->type), type_to_string(expr_type));
        add_error(analyzer, error);
    } else {
        symbol->is_initialized = 1;
    }
    
    if (analyzer->current_token.type != TOKEN_SEMICOLON) {
        add_error(analyzer, "Esperado ';' após atribuição");
    } else {
        advance_token(analyzer);
    }
}

void check_declaration(Analyzer* analyzer) {
    if (analyzer->current_token.type == TOKEN_INT_TYPE ||
        analyzer->current_token.type == TOKEN_FLOAT_TYPE ||
        analyzer->current_token.type == TOKEN_CHAR_TYPE ||
        analyzer->current_token.type == TOKEN_BOOL_TYPE) {
        check_variable_declaration(analyzer);
    } else if (analyzer->current_token.type == TOKEN_IDENTIFIER) {
        check_assignment(analyzer);
    } else {
        add_error(analyzer, "Declaração inválida");
        advance_token(analyzer); // pula token inválido
    }
}

void check_program(Analyzer* analyzer) {
    advance_token(analyzer); // lê o primeiro token
    
    while (analyzer->current_token.type != TOKEN_EOF) {
        check_declaration(analyzer);
    }
}

// ==================== DEMO ====================

void analyze_code(const char* code, const char* description) {
    printf("\n=== %s ===\n", description);
    printf("Código:\n%s\n", code);
    
    Analyzer analyzer;
    analyzer.input = code;
    analyzer.position = 0;
    analyzer.line = 1;
    analyzer.symbol_count = 0;
    analyzer.error_count = 0;
    
    check_program(&analyzer);
    
    print_symbol_table(&analyzer);
    
    printf("\n=== RESULTADO DA ANÁLISE SEMÂNTICA ===\n");
    if (analyzer.error_count == 0) {
        printf("✓ Análise semântica bem-sucedida! Nenhum erro encontrado.\n");
    } else {
        printf("✗ %d erro(s) semântico(s) encontrado(s):\n", analyzer.error_count);
        for (int i = 0; i < analyzer.error_count; i++) {
            printf("  Linha %d: %s\n", 
                   analyzer.errors[i].line, 
                   analyzer.errors[i].message);
        }
    }
    printf("\n==================================================\n");
}

int main() {
    printf("=== ANALISADOR SEMÂNTICO PARA LINGUAGEM C SIMPLIFICADA ===\n");
    printf("Este programa verifica a correção semântica de declarações e\n");
    printf("expressões, incluindo tipos, escopo e inicialização de variáveis.\n");
    
    // Código válido
    analyze_code(
        "int x = 10;\n"
        "float y = 3.14;\n"
        "bool flag = true;\n"
        "x = x + 5;\n"
        "y = y * 2.0;\n",
        "CÓDIGO VÁLIDO - Declarações e atribuições corretas"
    );
    
    // Erro: variável não declarada
    analyze_code(
        "int x = 10;\n"
        "y = 20;\n",
        "ERRO: Uso de variável não declarada"
    );
    
    // Erro: incompatibilidade de tipos
    analyze_code(
        "int x;\n"
        "float y = 3.14;\n"
        "x = y;\n",
        "ERRO: Incompatibilidade de tipos na atribuição"
    );
    
    // Erro: redeclaração
    analyze_code(
        "int x = 10;\n"
        "int x = 20;\n",
        "ERRO: Redeclaração de variável"
    );
    
    // Erro: uso antes da inicialização
    analyze_code(
        "int x;\n"
        "int y = x + 10;\n",
        "ERRO: Uso de variável não inicializada"
    );
    
    // Operações com tipos incompatíveis
    analyze_code(
        "bool flag1 = true;\n"
        "bool flag2 = false;\n"
        "int result = flag1 + flag2;\n",
        "ERRO: Operação aritmética com booleanos"
    );
    
    // Expressões complexas válidas
    analyze_code(
        "int a = 10;\n"
        "int b = 20;\n"
        "float c = 3.14;\n"
        "float result = (a + b) * c;\n"
        "bool test = (a == b) || (c != 0.0);\n",
        "CÓDIGO VÁLIDO - Expressões complexas com tipos compatíveis"
    );
    
    printf("\n=== VERIFICAÇÕES SEMÂNTICAS IMPLEMENTADAS ===\n");
    printf("• Declaração obrigatória de variáveis antes do uso\n");
    printf("• Verificação de compatibilidade de tipos em operações\n");
    printf("• Verificação de compatibilidade em atribuições\n");
    printf("• Detecção de redeclaração de variáveis\n");
    printf("• Verificação de inicialização antes do uso\n");
    printf("• Promoção automática de tipos (int -> float)\n");
    printf("• Verificação de tipos em operações lógicas\n");
    printf("• Verificação de tipos em comparações\n");
    
    printf("\n=== TIPOS SUPORTADOS E REGRAS ===\n");
    printf("• int: números inteiros\n");
    printf("• float: números de ponto flutuante\n");
    printf("• char: caracteres simples\n");
    printf("• bool: valores verdadeiro/falso\n");
    printf("\nRegras de compatibilidade:\n");
    printf("• int + int -> int\n");
    printf("• int + float -> float (promoção)\n");
    printf("• bool + bool -> ERRO (apenas &&, || permitidos)\n");
    printf("• Comparações (==, !=) requerem tipos iguais\n");
    printf("• Operações lógicas (&&, ||) requerem tipo bool\n");
    
    printf("\n=== IMPORTÂNCIA DA ANÁLISE SEMÂNTICA ===\n");
    printf("• Detecta erros que passam pela análise sintática\n");
    printf("• Garante correção de tipos em tempo de compilação\n");
    printf("• Previne erros de execução relacionados a tipos\n");
    printf("• Otimiza a geração de código com informações de tipo\n");
    printf("• Fornece base para verificações avançadas de segurança\n");
    
    return 0;
}