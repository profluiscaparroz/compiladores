/*
 * Gerador de Código Intermediário com SDT
 * 
 * Implementa um tradutor que gera código de três endereços
 * para atribuições com expressões aritméticas.
 * 
 * Gramática com geração de código:
 *   S → id = E ;    { emit(id, "=", E.addr) }
 *   E → E + T       { t = new_temp(); emit(t, "=", E.addr, "+", T.addr); E.addr = t }
 *   E → T           { E.addr = T.addr }
 *   T → T * F       { t = new_temp(); emit(t, "=", T.addr, "*", F.addr); T.addr = t }
 *   T → F           { T.addr = F.addr }
 *   F → (E)         { F.addr = E.addr }
 *   F → id          { F.addr = id.lexeme }
 *   F → num         { F.addr = num.lexeme }
 * 
 * Compilação: gcc gerador_codigo.c -o gerador_codigo -std=c99
 * Uso: ./gerador_codigo
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ========== DEFINIÇÃO DE TOKENS ==========
typedef enum {
    TOKEN_NUM,
    TOKEN_ID,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULT,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_ASSIGN,
    TOKEN_SEMI,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[32];
    int value;
} Token;

// ========== CÓDIGO DE TRÊS ENDEREÇOS ==========
typedef struct {
    char op;        // '+', '-', '*', '/', '='
    char arg1[32];  // Primeiro argumento
    char arg2[32];  // Segundo argumento
    char result[32];// Resultado (temporário ou variável)
} Instruction;

#define MAX_INSTRUCTIONS 100
Instruction code[MAX_INSTRUCTIONS];
int code_count = 0;

int temp_count = 0;

/*
 * Cria um novo temporário
 * Retorna: nome do temporário (ex: "t0", "t1", ...)
 */
char* new_temp() {
    /* Aloca um novo buffer para cada temporário para evitar sobrescrever um buffer estático */
    char *temp = malloc(16);
    if (temp == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para temporário\n");
        exit(1);
    }
    snprintf(temp, 16, "t%d", temp_count++);
    return temp;
}

/*
 * Emite uma instrução de três endereços
 */
void emit(char op, const char* arg1, const char* arg2, const char* result) {
    if (code_count >= MAX_INSTRUCTIONS) {
        fprintf(stderr, "Erro: limite de instruções excedido\n");
        exit(1);
    }
    
    code[code_count].op = op;
    strcpy(code[code_count].arg1, arg1);
    strcpy(code[code_count].arg2, arg2);
    strcpy(code[code_count].result, result);
    code_count++;
}

/*
 * Imprime o código intermediário gerado
 */
void print_code() {
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║  CÓDIGO INTERMEDIÁRIO GERADO               ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    
    for (int i = 0; i < code_count; i++) {
        if (code[i].op == '=') {
            // Atribuição simples
            if (strlen(code[i].arg2) > 0) {
                // Atribuição com operação: t = a op b
                printf("%s = %s %c %s\n", 
                       code[i].result,
                       code[i].arg1,
                       code[i].arg2[0],  // operador em arg2
                       code[i].arg2 + 1); // segundo operando
            } else {
                // Atribuição direta: x = t
                printf("%s = %s\n", code[i].result, code[i].arg1);
            }
        } else {
            // Operação aritmética: t = a op b
            printf("%s = %s %c %s\n", 
                   code[i].result,
                   code[i].arg1,
                   code[i].op,
                   code[i].arg2);
        }
    }
    printf("\n");
}

// ========== VARIÁVEIS GLOBAIS DO LEXER ==========
char* input;
int pos = 0;
Token current_token;

// ========== ANALISADOR LÉXICO ==========

Token next_token() {
    // Pular espaços em branco
    while (input[pos] == ' ' || input[pos] == '\t') {
        pos++;
    }
    
    Token tok;
    tok.lexeme[0] = '\0';
    
    // Fim da entrada
    if (input[pos] == '\0') {
        tok.type = TOKEN_EOF;
        return tok;
    }
    
    // Números
    if (isdigit(input[pos])) {
        tok.type = TOKEN_NUM;
        tok.value = 0;
        int i = 0;
        while (isdigit(input[pos])) {
            tok.lexeme[i++] = input[pos];
            tok.value = tok.value * 10 + (input[pos] - '0');
            pos++;
        }
        tok.lexeme[i] = '\0';
        return tok;
    }
    
    // Identificadores
    if (isalpha(input[pos]) || input[pos] == '_') {
        tok.type = TOKEN_ID;
        int i = 0;
        while (isalnum(input[pos]) || input[pos] == '_') {
            tok.lexeme[i++] = input[pos++];
        }
        tok.lexeme[i] = '\0';
        return tok;
    }
    
    // Operadores e pontuação
    switch (input[pos++]) {
        case '+': tok.type = TOKEN_PLUS; strcpy(tok.lexeme, "+"); break;
        case '-': tok.type = TOKEN_MINUS; strcpy(tok.lexeme, "-"); break;
        case '*': tok.type = TOKEN_MULT; strcpy(tok.lexeme, "*"); break;
        case '/': tok.type = TOKEN_DIV; strcpy(tok.lexeme, "/"); break;
        case '(': tok.type = TOKEN_LPAREN; strcpy(tok.lexeme, "("); break;
        case ')': tok.type = TOKEN_RPAREN; strcpy(tok.lexeme, ")"); break;
        case '=': tok.type = TOKEN_ASSIGN; strcpy(tok.lexeme, "="); break;
        case ';': tok.type = TOKEN_SEMI; strcpy(tok.lexeme, ";"); break;
        default:
            fprintf(stderr, "Erro léxico: caractere inválido '%c'\n", input[pos-1]);
            exit(1);
    }
    
    return tok;
}

void advance() {
    current_token = next_token();
}

// ========== ANALISADOR SINTÁTICO COM GERAÇÃO DE CÓDIGO ==========

// Declarações forward
char* parse_expr();
char* parse_term();
char* parse_factor();

/*
 * F → id | num | (E)
 * Retorna: endereço do resultado (atributo sintetizado)
 */
char* parse_factor() {
    if (current_token.type == TOKEN_NUM) {
        // F → num { F.addr = num.lexeme }
        char* addr = current_token.lexeme;
        advance();
        return addr;
    }
    
    if (current_token.type == TOKEN_ID) {
        // F → id { F.addr = id.lexeme }
        char* addr = current_token.lexeme;
        advance();
        return addr;
    }
    
    if (current_token.type == TOKEN_LPAREN) {
        // F → (E) { F.addr = E.addr }
        advance();  // consome '('
        char* addr = parse_expr();
        
        if (current_token.type != TOKEN_RPAREN) {
            fprintf(stderr, "Erro sintático: esperado ')'\n");
            exit(1);
        }
        advance();  // consome ')'
        
        return addr;
    }
    
    fprintf(stderr, "Erro sintático: esperado identificador, número ou '('\n");
    exit(1);
}

/*
 * T → F T'
 * T' → * F T' | / F T' | ε
 * Retorna: endereço do resultado (atributo sintetizado)
 */
char* parse_term() {
    // T → F { T.addr = F.addr }
    char* addr = parse_factor();
    
    // T' → * F T' | / F T' | ε
    while (current_token.type == TOKEN_MULT || 
           current_token.type == TOKEN_DIV) {
        char op = (current_token.type == TOKEN_MULT) ? '*' : '/';
        advance();
        char* addr2 = parse_factor();
        
        // Ação semântica: gerar código
        // T → T₁ op F { t = new_temp(); emit(t, "=", T₁.addr, op, F.addr); T.addr = t }
        char* temp = new_temp();
        emit(op, addr, addr2, temp);
        addr = temp;
    }
    
    return addr;  // Atributo sintetizado
}

/*
 * E → T E'
 * E' → + T E' | - T E' | ε
 * Retorna: endereço do resultado (atributo sintetizado)
 */
char* parse_expr() {
    // E → T { E.addr = T.addr }
    char* addr = parse_term();
    
    // E' → + T E' | - T E' | ε
    while (current_token.type == TOKEN_PLUS || 
           current_token.type == TOKEN_MINUS) {
        char op = (current_token.type == TOKEN_PLUS) ? '+' : '-';
        advance();
        char* addr2 = parse_term();
        
        // Ação semântica: gerar código
        // E → E₁ op T { t = new_temp(); emit(t, "=", E₁.addr, op, T.addr); E.addr = t }
        char* temp = new_temp();
        emit(op, addr, addr2, temp);
        addr = temp;
    }
    
    return addr;  // Atributo sintetizado
}

/*
 * S → id = E ;
 * Traduz uma atribuição e gera código intermediário
 */
void parse_assignment() {
    // id = E ;
    if (current_token.type != TOKEN_ID) {
        fprintf(stderr, "Erro sintático: esperado identificador\n");
        exit(1);
    }
    
    char id[32];
    if (current_token.lexeme == NULL) {
        fprintf(stderr, "Erro de memória: identificador nulo\n");
        exit(1);
    }
    strcpy(id, current_token.lexeme);
    advance();
    
    if (current_token.type != TOKEN_ASSIGN) {
        fprintf(stderr, "Erro sintático: esperado '='\n");
        exit(1);
    }
    advance();
    
    char* addr = parse_expr();
    
    // Ação semântica: gerar código de atribuição
    // S → id = E { emit(id, "=", E.addr) }
    Instruction instr;
    instr.op = '=';
    strcpy(instr.arg1, addr);
    instr.arg2[0] = '\0';
    strcpy(instr.result, id);
    code[code_count++] = instr;
    
    if (current_token.type != TOKEN_SEMI) {
        fprintf(stderr, "Erro sintático: esperado ';'\n");
        exit(1);
    }
    advance();
}

// ========== FUNÇÃO PRINCIPAL ==========

int main() {
    printf("╔════════════════════════════════════════════╗\n");
    printf("║  GERADOR DE CÓDIGO COM SDT                 ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    printf("Digite atribuições na forma: var = expressão;\n");
    printf("Exemplos:\n");
    printf("  • x = a + b * 3;\n");
    printf("  • resultado = (p + q) * r - s;\n");
    printf("  • y = 10 + 20 * 30;\n");
    printf("\nDigite 'sair' ou Ctrl+D para encerrar.\n\n");
    
    char line[256];
    
    while (1) {
        printf(">>> ");
        fflush(stdout);
        
        if (!fgets(line, sizeof(line), stdin)) {
            break;  // EOF
        }
        
        // Remover newline
        line[strcspn(line, "\n")] = 0;
        
        // Verificar comando de saída
        if (strcmp(line, "sair") == 0 || strcmp(line, "exit") == 0) {
            break;
        }
        
        // Linha vazia
        if (strlen(line) == 0) {
            continue;
        }
        
        // Reset
        code_count = 0;
        temp_count = 0;
        input = line;
        pos = 0;
        advance();  // Carregar primeiro token
        
        // Traduzir atribuição
        parse_assignment();
        
        // Imprimir código gerado
        print_code();
    }
    
    printf("Encerrando gerador de código.\n");
    return 0;
}
