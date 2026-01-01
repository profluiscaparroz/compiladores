#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Exemplo Completo de Geração de Código Intermediário
 * Inclui: expressões, comandos de controle e otimizações básicas
 */

// ========== FUNÇÕES AUXILIARES ==========

char* my_strdup(const char* s) {
    if (!s) return NULL;
    char* copy = malloc(strlen(s) + 1);
    if (copy) strcpy(copy, s);
    return copy;
}

// ========== TIPOS ==========

typedef enum {
    TAC_ADD, TAC_SUB, TAC_MUL, TAC_DIV,
    TAC_COPY, TAC_LABEL, TAC_GOTO,
    TAC_IF_LT, TAC_IF_GT, TAC_IF_EQ,
    TAC_IF_LE, TAC_IF_GE, TAC_IF_NE
} TACOp;

typedef struct TACInstr {
    TACOp op;
    char* result;
    char* arg1;
    char* arg2;
    struct TACInstr* next;
} TACInstr;

typedef enum {
    NODE_NUMBER, NODE_IDENTIFIER, NODE_BINARY_OP
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* value;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

// ========== VARIÁVEIS GLOBAIS ==========

TACInstr* tac_head = NULL;
TACInstr* tac_tail = NULL;
int temp_count = 0;
int label_count = 0;

// ========== FUNÇÕES AUXILIARES ==========

char* new_temp() {
    char* temp = malloc(10);
    if (!temp) {
        fprintf(stderr, "Erro: falha na alocacao de memoria em new_temp().\n");
        exit(EXIT_FAILURE);
    }
    sprintf(temp, "t%d", temp_count++);
    return temp;
}
    char* temp = malloc(10);
    if (!temp) {
        fprintf(stderr, "Erro: falha na alocação de memória em new_temp.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(temp, "t%d", temp_count++);
    return temp;
}

char* new_label() {
    char* label = malloc(10);
    if (!label) {
        fprintf(stderr, "Erro: falha na alocação de memória em new_label.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(label, "L%d", label_count++);
    return label;
}

void emit(TACOp op, char* result, char* arg1, char* arg2) {
    TACInstr* instr = malloc(sizeof(TACInstr));
    if (!instr) {
        fprintf(stderr, "Erro: falha ao alocar memória para TACInstr.\n");
        exit(EXIT_FAILURE);
    }
    if (!instr) {
        fprintf(stderr, "Erro: falha na alocação de memória em emit.\n");
        exit(EXIT_FAILURE);
    }
    if (!instr) {
        fprintf(stderr, "Erro: falha na alocacao de memoria em emit().\n");
        exit(EXIT_FAILURE);
    }
    instr->op = op;
    instr->result = result;
    instr->arg1 = arg1;
    instr->arg2 = arg2;
    instr->next = NULL;
    
    if (tac_tail == NULL) {
        tac_head = tac_tail = instr;
    } else {
        tac_tail->next = instr;
        tac_tail = instr;
    }
}

const char* tac_op_name(TACOp op) {
    switch (op) {
        case TAC_ADD: return "+";
        case TAC_SUB: return "-";
        case TAC_MUL: return "*";
        case TAC_DIV: return "/";
        case TAC_COPY: return "=";
        case TAC_LABEL: return ":";
        case TAC_GOTO: return "goto";
        case TAC_IF_LT: return "<";
        case TAC_IF_GT: return ">";
        case TAC_IF_EQ: return "==";
        case TAC_IF_LE: return "<=";
        case TAC_IF_GE: return ">=";
        case TAC_IF_NE: return "!=";
        default: return "???";
    }
}

void print_tac() {
    printf("\n=== CÓDIGO DE TRÊS ENDEREÇOS ===\n");
    
    TACInstr* instr = tac_head;
    while (instr) {
        if (instr->op == TAC_LABEL) {
            printf("%s:\n", instr->result);
        } else if (instr->op == TAC_GOTO) {
            printf("  goto %s\n", instr->result);
        } else if (instr->op >= TAC_IF_LT && instr->op <= TAC_IF_NE) {
            printf("  if %s %s %s goto %s\n",
                   instr->arg1,
                   tac_op_name(instr->op),
                   instr->arg2,
                   instr->result);
        } else if (instr->op == TAC_COPY) {
            printf("  %s = %s\n", instr->result, instr->arg1);
        } else {
            printf("  %s = %s %s %s\n",
                   instr->result,
                   instr->arg1,
                   tac_op_name(instr->op),
                   instr->arg2);
        }
        
        instr = instr->next;
    }
}

// ========== CRIAÇÃO DE AST ==========

ASTNode* create_node(NodeType type, char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? my_strdup(value) : NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode* create_number(int value) {
    char buf[20];
    snprintf(buf, sizeof(buf), "%d", value);
    return create_node(NODE_NUMBER, buf);
}

ASTNode* create_binary_op(char* op, ASTNode* left, ASTNode* right) {
    ASTNode* node = create_node(NODE_BINARY_OP, op);
    node->left = left;
    node->right = right;
    return node;
}

// ========== TRADUÇÃO DE EXPRESSÕES ==========

typedef struct {
    char* place;
} ExprResult;

ExprResult* translate_expression(ASTNode* node);

ExprResult* translate_binary_op(ASTNode* node) {
    ExprResult* left = translate_expression(node->left);
    ExprResult* right = translate_expression(node->right);
    
    char* temp = new_temp();
    
    TACOp op;
    switch (node->value[0]) {
        case '+': op = TAC_ADD; break;
        case '-': op = TAC_SUB; break;
        case '*': op = TAC_MUL; break;
        case '/': op = TAC_DIV; break;
        default: 
            fprintf(stderr, "Operador desconhecido: %s\n", node->value);
            exit(1);
    }
    
    emit(op, temp, left->place, right->place);
    
    ExprResult* result = malloc(sizeof(ExprResult));
    result->place = temp;
    return result;
}

ExprResult* translate_expression(ASTNode* node) {
    ExprResult* result = malloc(sizeof(ExprResult));
    
    switch (node->type) {
        case NODE_NUMBER:
            result->place = my_strdup(node->value);
            break;
            
        case NODE_IDENTIFIER:
            result->place = my_strdup(node->value);
            break;
            
        case NODE_BINARY_OP:
            free(result);
            return translate_binary_op(node);
            
        default:
            fprintf(stderr, "Tipo de nó desconhecido\n");
            exit(1);
    }
    
    return result;
}

// ========== OTIMIZAÇÕES ==========

int is_constant(char* place) {
    if (!place) return 0;
    int i = (place[0] == '-') ? 1 : 0;
    while (place[i]) {
        if (place[i] < '0' || place[i] > '9') return 0;
        i++;
    }
    return place[0] != '\0';
}

ExprResult* optimize_constant_folding(ASTNode* node) {
    if (node->type == NODE_BINARY_OP) {
        ExprResult* left = translate_expression(node->left);
        ExprResult* right = translate_expression(node->right);
        
        if (is_constant(left->place) && is_constant(right->place)) {
            int val1 = atoi(left->place);
            int val2 = atoi(right->place);
            int result_val;
            
            switch (node->value[0]) {
                case '+': result_val = val1 + val2; break;
                case '-': result_val = val1 - val2; break;
                case '*': result_val = val1 * val2; break;
                case '/':
                    if (val2 != 0) result_val = val1 / val2;
                    else return translate_expression(node);
                    break;
                default: return translate_expression(node);
            }
            
            printf("  [Otimização: %d %s %d = %d]\n",
                   val1, node->value, val2, result_val);
            
            ExprResult* result = malloc(sizeof(ExprResult));
            if (!result) {
                /* Falha na alocação: volta para a tradução normal (sem otimização) */
                return translate_expression(node);
            }
            result->place = malloc(20);
            if (!result->place) {
                /* Falha na alocação do buffer: evita leak e volta para a tradução normal */
                free(result);
                return translate_expression(node);
            }
            sprintf(result->place, "%d", result_val);
            return result;
        }
    }
    
    return translate_expression(node);
}

// ========== COMANDOS DE CONTROLE ==========

void generate_if_then(ASTNode* cond, ASTNode* then_body) {
    char* L_then = new_label();
    char* L_end = new_label();
    
    ExprResult* cond_result = translate_expression(cond);
    emit(TAC_IF_GT, L_then, cond_result->place, "0");
    emit(TAC_GOTO, L_end, NULL, NULL);
    
    emit(TAC_LABEL, L_then, NULL, NULL);
    ExprResult* then_result = translate_expression(then_body);
    emit(TAC_COPY, "x", then_result->place, NULL);
    
    emit(TAC_LABEL, L_end, NULL, NULL);
}

void generate_while_loop(ASTNode* cond, ASTNode* body) {
    char* L_start = new_label();
    char* L_body = new_label();
    char* L_end = new_label();
    
    emit(TAC_LABEL, L_start, NULL, NULL);
    
    ExprResult* cond_result = translate_expression(cond);
    emit(TAC_IF_LE, L_body, cond_result->place, "10");
    emit(TAC_GOTO, L_end, NULL, NULL);
    
    emit(TAC_LABEL, L_body, NULL, NULL);
    ExprResult* body_result = translate_expression(body);
    emit(TAC_COPY, "i", body_result->place, NULL);
    emit(TAC_GOTO, L_start, NULL, NULL);
    
    emit(TAC_LABEL, L_end, NULL, NULL);
}

// ========== EXEMPLOS ==========

void exemplo_expressao_complexa() {
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  EXEMPLO 1: Expressão Complexa            ║\n");
    printf("║  Código: (a + b * 3) - (c / 2)            ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    temp_count = 0;
    label_count = 0;
    tac_head = tac_tail = NULL;
    
    // Construir AST: (a + b * 3) - (c / 2)
    ASTNode* mult = create_binary_op("*",
                                      create_node(NODE_IDENTIFIER, "b"),
                                      create_number(3));
    ASTNode* add = create_binary_op("+",
                                     create_node(NODE_IDENTIFIER, "a"),
                                     mult);
    ASTNode* div = create_binary_op("/",
                                     create_node(NODE_IDENTIFIER, "c"),
                                     create_number(2));
    ASTNode* sub = create_binary_op("-", add, div);
    
    ExprResult* result = translate_expression(sub);
    emit(TAC_COPY, "resultado", result->place, NULL);
    
    print_tac();
    printf("\nTemporários: %d | Rótulos: %d\n", temp_count, label_count);
}

void exemplo_comando_if() {
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  EXEMPLO 2: Comando if-then                ║\n");
    printf("║  Código: if (a > 0) x = a * 2              ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    temp_count = 0;
    label_count = 0;
    tac_head = tac_tail = NULL;
    
    ASTNode* mult = create_binary_op("*",
                                      create_node(NODE_IDENTIFIER, "a"),
                                      create_number(2));
    
    generate_if_then(create_node(NODE_IDENTIFIER, "a"), mult);
    
    print_tac();
    printf("\nTemporários: %d | Rótulos: %d\n", temp_count, label_count);
}

void exemplo_loop_while() {
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  EXEMPLO 3: Loop while                     ║\n");
    printf("║  Código: while (i <= 10) i = i + 1        ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    temp_count = 0;
    label_count = 0;
    tac_head = tac_tail = NULL;
    
    ASTNode* add = create_binary_op("+",
                                     create_node(NODE_IDENTIFIER, "i"),
                                     create_number(1));
    
    generate_while_loop(create_node(NODE_IDENTIFIER, "i"), add);
    
    print_tac();
    printf("\nTemporários: %d | Rótulos: %d\n", temp_count, label_count);
}

void exemplo_otimizacao() {
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  EXEMPLO 4: Otimização (Constant Folding) ║\n");
    printf("║  Código: x = 2 + 3 * 4                     ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    temp_count = 0;
    label_count = 0;
    tac_head = tac_tail = NULL;
    
    ASTNode* mult = create_binary_op("*", create_number(3), create_number(4));
    ASTNode* add = create_binary_op("+", create_number(2), mult);
    
    printf("\nSem otimização:\n");
    ExprResult* result1 = translate_expression(add);
    emit(TAC_COPY, "x", result1->place, NULL);
    print_tac();
    
    printf("\n\nCom otimização:\n");
    temp_count = 0;
    tac_head = tac_tail = NULL;
    
    // Reconstruir AST
    mult = create_binary_op("*", create_number(3), create_number(4));
    add = create_binary_op("+", create_number(2), mult);
    
    ExprResult* result2 = optimize_constant_folding(add);
    emit(TAC_COPY, "x", result2->place, NULL);
    print_tac();
    
    printf("\nTemporários economizados: 2 → 0\n");
}

// ========== MAIN ==========

int main() {
    printf("╔════════════════════════════════════════════╗\n");
    printf("║  Gerador de Código Intermediário          ║\n");
    printf("║  Exemplo Completo - TAC                    ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    exemplo_expressao_complexa();
    exemplo_comando_if();
    exemplo_loop_while();
    exemplo_otimizacao();
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  Fim dos Exemplos                          ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    
    return 0;
}
