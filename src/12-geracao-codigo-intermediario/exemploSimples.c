#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Exemplo Simples de Geração de Código Intermediário
 * Gera código de três endereços (TAC) para expressões aritméticas simples
 */

// ========== TIPOS ==========

typedef enum {
    TAC_ADD, TAC_SUB, TAC_MUL, TAC_DIV,
    TAC_COPY
} TACOp;

typedef struct TACInstr {
    TACOp op;
    char* result;
    char* arg1;
    char* arg2;
    struct TACInstr* next;
} TACInstr;

// ========== VARIÁVEIS GLOBAIS ==========

TACInstr* tac_head = NULL;
TACInstr* tac_tail = NULL;
int temp_count = 0;

// ========== FUNÇÕES AUXILIARES ==========

char* new_temp() {
    char* temp = malloc(10);
    if (temp == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para temporário.\n");
        exit(EXIT_FAILURE);
    /* Determine required buffer size for "t<temp_count>" (excluding '\0') */
    int len = snprintf(NULL, 0, "t%d", temp_count);
    char* temp = malloc((size_t)len + 1);
    if (temp == NULL) {
        /* In a small example program, exiting is a simple way to handle OOM */
        fprintf(stderr, "Erro ao alocar memória para temporário.\n");
        exit(EXIT_FAILURE);
    char* temp = malloc(10);
    if (temp == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para temp.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(temp, "t%d", temp_count++);
    return temp;
}

void emit(TACOp op, char* result, char* arg1, char* arg2) {
    TACInstr* instr = malloc(sizeof(TACInstr));
    if (instr == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para instrução TAC.\n");
        exit(EXIT_FAILURE);
    }
    if (instr == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para instrução TAC.\n");
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
        default: return "???";
    }
}

void print_tac() {
    printf("\n=== CÓDIGO DE TRÊS ENDEREÇOS ===\n");
    
    TACInstr* instr = tac_head;
    while (instr) {
        if (instr->op == TAC_COPY) {
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
    
    printf("\nTemporários usados: %d\n", temp_count);
}

// ========== GERAÇÃO DE CÓDIGO ==========

// Exemplo 1: (a + b) * c
void exemplo1() {
    printf("\n=== EXEMPLO 1: (a + b) * c ===\n");
    
    char* t1 = new_temp();
    emit(TAC_ADD, t1, "a", "b");
    
    char* t2 = new_temp();
    emit(TAC_MUL, t2, t1, "c");
    
    emit(TAC_COPY, "resultado", t2, NULL);
    
    print_tac();
}

// Exemplo 2: a + b * c - d
void exemplo2() {
    printf("\n=== EXEMPLO 2: a + b * c - d ===\n");
    
    // Reiniciar contadores
    temp_count = 0;
    tac_head = tac_tail = NULL;
    
    char* t1 = new_temp();
    emit(TAC_MUL, t1, "b", "c");
    
    char* t2 = new_temp();
    emit(TAC_ADD, t2, "a", t1);
    
    char* t3 = new_temp();
    emit(TAC_SUB, t3, t2, "d");
    
    emit(TAC_COPY, "resultado", t3, NULL);
    
    print_tac();
}

// Exemplo 3: (a - b) / (c + d)
void exemplo3() {
    printf("\n=== EXEMPLO 3: (a - b) / (c + d) ===\n");
    
    // Reiniciar contadores
    temp_count = 0;
    tac_head = tac_tail = NULL;
    
    char* t1 = new_temp();
    emit(TAC_SUB, t1, "a", "b");
    
    char* t2 = new_temp();
    emit(TAC_ADD, t2, "c", "d");
    
    char* t3 = new_temp();
    emit(TAC_DIV, t3, t1, t2);
    
    emit(TAC_COPY, "resultado", t3, NULL);
    
    print_tac();
}

// ========== MAIN ==========

int main() {
    printf("╔════════════════════════════════════════════╗\n");
    printf("║  Gerador de Código Intermediário - TAC    ║\n");
    printf("║  Exemplo Simples                           ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    exemplo1();
    exemplo2();
    exemplo3();
    
    printf("\n=== FIM ===\n\n");
    
    return 0;
}
