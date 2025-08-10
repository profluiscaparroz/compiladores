/**
 * Analisador Semântico Simplificado para Verificação de Tipos
 * 
 * Este programa demonstra verificações semânticas básicas:
 * - Declaração de variáveis antes do uso
 * - Compatibilidade de tipos em operações
 * - Verificação de tipos em atribuições
 * 
 * Tipos suportados: int, float, bool
 * 
 * Autor: Disciplina de Compiladores  
 * Data: 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARS 50

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_ERROR
} DataType;

typedef struct {
    char name[50];
    DataType type;
    int declared;
} Variable;

Variable variables[MAX_VARS];
int var_count = 0;

const char* type_name(DataType type) {
    switch(type) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float"; 
        case TYPE_BOOL: return "bool";
        default: return "error";
    }
}

void declare_variable(const char* name, DataType type) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            printf("ERRO: Variável '%s' já foi declarada\n", name);
            return;
        }
    }
    
    strcpy(variables[var_count].name, name);
    variables[var_count].type = type;
    variables[var_count].declared = 1;
    var_count++;
    printf("✓ Variável '%s' declarada como %s\n", name, type_name(type));
}

DataType get_variable_type(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].type;
        }
    }
    printf("ERRO: Variável '%s' não foi declarada\n", name);
    return TYPE_ERROR;
}

DataType check_binary_operation(DataType left, DataType right, const char* op) {
    printf("  Verificando operação: %s %s %s\n", 
           type_name(left), op, type_name(right));
    
    if (left == TYPE_ERROR || right == TYPE_ERROR) {
        return TYPE_ERROR;
    }
    
    // Operações aritméticas
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || 
        strcmp(op, "*") == 0 || strcmp(op, "/") == 0) {
        
        if (left == TYPE_BOOL || right == TYPE_BOOL) {
            printf("  ERRO: Operação aritmética com tipo bool\n");
            return TYPE_ERROR;
        }
        
        if (left == TYPE_FLOAT || right == TYPE_FLOAT) {
            printf("  Resultado: float (promoção de tipo)\n");
            return TYPE_FLOAT;
        }
        
        printf("  Resultado: int\n");
        return TYPE_INT;
    }
    
    // Operações de comparação
    if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {
        if (left == right) {
            printf("  Resultado: bool\n");
            return TYPE_BOOL;
        } else {
            printf("  ERRO: Comparação entre tipos diferentes\n");
            return TYPE_ERROR;
        }
    }
    
    // Operações lógicas
    if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
        if (left == TYPE_BOOL && right == TYPE_BOOL) {
            printf("  Resultado: bool\n");
            return TYPE_BOOL;
        } else {
            printf("  ERRO: Operação lógica requer tipo bool\n");
            return TYPE_ERROR;
        }
    }
    
    printf("  ERRO: Operador desconhecido '%s'\n", op);
    return TYPE_ERROR;
}

void check_assignment(const char* var_name, DataType expr_type) {
    DataType var_type = get_variable_type(var_name);
    
    if (var_type == TYPE_ERROR) {
        return;
    }
    
    printf("  Verificando atribuição: %s = %s\n", 
           type_name(var_type), type_name(expr_type));
    
    if (var_type == expr_type) {
        printf("  ✓ Atribuição válida\n");
    } else if (var_type == TYPE_FLOAT && expr_type == TYPE_INT) {
        printf("  ✓ Atribuição válida (int promovido para float)\n");
    } else {
        printf("  ERRO: Incompatibilidade de tipos na atribuição\n");
    }
}

void print_symbol_table() {
    printf("\n=== TABELA DE SÍMBOLOS ===\n");
    for (int i = 0; i < var_count; i++) {
        printf("  %s: %s\n", variables[i].name, type_name(variables[i].type));
    }
}

void demo_declarations() {
    printf("\n=== TESTE 1: DECLARAÇÕES DE VARIÁVEIS ===\n");
    
    declare_variable("x", TYPE_INT);
    declare_variable("y", TYPE_FLOAT);  
    declare_variable("flag", TYPE_BOOL);
    declare_variable("x", TYPE_INT); // Erro: redeclaração
    
    print_symbol_table();
}

void demo_type_checking() {
    printf("\n=== TESTE 2: VERIFICAÇÃO DE TIPOS EM OPERAÇÕES ===\n");
    
    // Operações válidas
    check_binary_operation(TYPE_INT, TYPE_INT, "+");
    check_binary_operation(TYPE_INT, TYPE_FLOAT, "*");
    check_binary_operation(TYPE_BOOL, TYPE_BOOL, "&&");
    check_binary_operation(TYPE_INT, TYPE_INT, "==");
    
    // Operações inválidas
    check_binary_operation(TYPE_BOOL, TYPE_INT, "+");
    check_binary_operation(TYPE_INT, TYPE_FLOAT, "==");
    check_binary_operation(TYPE_INT, TYPE_INT, "&&");
}

void demo_assignments() {
    printf("\n=== TESTE 3: VERIFICAÇÃO DE ATRIBUIÇÕES ===\n");
    
    // Atribuições válidas
    check_assignment("x", TYPE_INT);
    check_assignment("y", TYPE_FLOAT);
    check_assignment("y", TYPE_INT); // Promoção int->float
    check_assignment("flag", TYPE_BOOL);
    
    // Atribuições inválidas
    check_assignment("x", TYPE_FLOAT);
    check_assignment("flag", TYPE_INT);
    check_assignment("undefined_var", TYPE_INT); // Variável não declarada
}

void demo_complex_expressions() {
    printf("\n=== TESTE 4: EXPRESSÕES COMPLEXAS ===\n");
    
    printf("Simulando: int result = (x + 10) * y;\n");
    DataType expr1 = check_binary_operation(TYPE_INT, TYPE_INT, "+"); // x + 10
    DataType expr2 = check_binary_operation(expr1, TYPE_FLOAT, "*");  // (x+10) * y
    check_assignment("result", expr2);
    
    printf("\nSimulando: bool test = (x == 10) && flag;\n");
    DataType comp = check_binary_operation(TYPE_INT, TYPE_INT, "==");  // x == 10
    DataType logic = check_binary_operation(comp, TYPE_BOOL, "&&");   // (...) && flag
    check_assignment("test", logic);
}

int main() {
    printf("=== ANALISADOR SEMÂNTICO - VERIFICAÇÃO DE TIPOS ===\n");
    printf("Demonstração de verificações semânticas básicas em compiladores\n");
    
    demo_declarations();
    demo_type_checking();
    demo_assignments();
    demo_complex_expressions();
    
    printf("\n=== RESUMO DAS VERIFICAÇÕES SEMÂNTICAS ===\n");
    printf("✓ Declaração obrigatória de variáveis\n");
    printf("✓ Verificação de tipos em operações binárias\n");
    printf("✓ Compatibilidade de tipos em atribuições\n");
    printf("✓ Promoção automática de tipos (int -> float)\n");
    printf("✓ Verificação de redeclaração de variáveis\n");
    printf("✓ Tratamento de operações lógicas e aritméticas\n");
    
    printf("\n=== IMPORTÂNCIA DA ANÁLISE SEMÂNTICA ===\n");
    printf("• Detecta erros de tipo em tempo de compilação\n");
    printf("• Previne erros de execução relacionados a tipos\n");
    printf("• Permite otimizações baseadas em informações de tipo\n");
    printf("• Garante a correção do programa antes da execução\n");
    printf("• Base para análises mais avançadas (fluxo de dados, etc.)\n");
    
    return 0;
}