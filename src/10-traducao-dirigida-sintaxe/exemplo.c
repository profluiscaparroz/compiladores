#include <stdio.h>
#include <ctype.h>

char *input; // ponteiro para a entrada

// Função que lê um número da entrada
int parse_number() {
    int num = 0;

    // Ignora espaços
    while (*input == ' ') input++;

    // Verifica se é dígito
    if (!isdigit(*input)) {
        printf("Erro: Esperado um número\n");
        return 0;
    }

    while (isdigit(*input)) {
        num = num * 10 + (*input - '0');
        input++;
    }

    // Ignora espaços
    while (*input == ' ') input++;

    return num;
}

// Função recursiva para Expr
int parse_expr() {
    int val = parse_number();  // Term → número

    while (*input == '+') {
        input++; // consome '+'
        int right = parse_expr();  // Expr → Term + Expr
        val = val + right;
    }

    return val;
}

int main() {
    char expr[100];

    printf("Digite uma expressão (ex: 2 + 3 + 4): ");
    fgets(expr, sizeof(expr), stdin);

    input = expr;

    int resultado = parse_expr();

    printf("Resultado = %d\n", resultado);

    return 0;
}
