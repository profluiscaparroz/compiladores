#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Exemplo de código C com SSA (Static Single Assignment)
    // Cada variável é atribuída apenas uma vez
    // e não pode ser reatribuída.
    // Isso facilita a análise e otimização do código.
    int a = 5; // BB1
    int b = 3; // BB1

    if (a > b) { // BB2
        a = a + 1; // BB3
    } else {
        a = b + 1; // BB4
    }

    int c = a + b; // BB5
}

void exemploSSA() {
    int x = 10; // BB1
    int y = 20; // BB1

    if (x < y) { // BB2
        x = x - 2; // BB3
    } else {
        x = y - 5; // BB4
    }

    int z = x + y; // BB5
    printf("Resultado: %d\n", z);
}

// Exemplo de código C sem SSA
void equivalenteSSA(){
    int a1 = 5;         // primeira definição de a
    int b1 = 3;         // primeira definição de b

    int cond = a1 > b1;

    int a2, a3, a4;
    if (cond) {
        a2 = a1 + 1;    // segunda definição de a (via a2)
    } else {
        a3 = b1 + 1;    // terceira definição de a (via a3)
    }

    // aqui usamos a função φ para unir os dois caminhos
    // (na prática, o compilador decide qual valor usar com base no caminho seguido)
    if (cond) {
        a4 = a2;
    } else {
        a4 = a3;
    }

    int c1 = a4 + b1;   // resultado final usa a versão única de cada variável
    printf("Resultado: %d\n", c1);
// Aqui, a variável 'a' é reatribuída em dois lugares diferentes,
}