#include <stdio.h>

int main() {
    int valor = 10;

    if (valor > 5) {
        printf("Valor maior do que cinco.\n");
    }

    valor = 10;
    if (valor % 2 == 0) {
        printf("O valor é par.\n");
    } else {
        printf("O valor é impar.\n");
    }

    // Example of if-else if-else statement
    if (valor < 0) {
        printf("The valor is negative.\n");
    } else if (valor == 0) {
        printf("The valor is zero.\n");
    } else {
        printf("The valor is positive.\n");
    }

    // Example of a for loop
    printf("For loop example:\n");
    for (int indice= 0; indice< 5; indice++) {
        printf("indice= %d\n", indice);
    }

    printf("Exempo de While loop:\n");
    int count = 0;
    while (count < 3) {
        printf("count = %d\n", count);
        count++;
    }

    // Example of a do-while loop
    printf("Do-while loop example:\n");
    int x = 0;
    do {
        printf("x = %d\n", x);
        x++;
    } while (x < 2);
    int t1, t2, t3, t4;
    int a = 1, b = 2, c = 3;
    t1 = a + b;
    t2 = t1 - c;
    t3 = t1; // reaproveitamento de t1
    t4 = t2; // reaproveitamento de t2


    t3 = a + b;
    t4 = t3 - c;

    t1 = a + b;
    t2 = t1 - c;

    return 0;
}

int funcao() {
    int indice = 0; // BB1
    int soma = 0; // BB1
    while (indice< 5) { // BB2
        if (indice% 2 == 0) { // BB3
            soma = soma + indice; // BB4
        }
        indice= indice+ 1; // BB5
    }

    printf("soma = %d\n", soma); // BB6
    return 0;
}

void funcao1(){
    int valor = 5;         // Bloco B1
    inicio:
        if (valor < 0)         // Bloco B2
        {
            printf("Valor é negativo.\n");  // Bloco B3
            goto fim;
        }
        else if (valor >= 0 && valor <= 10) // ainda B2
        {
            printf("Valor está entre 1 e 10.\n");  // Bloco B4
            goto fim;
        }
        else
        {
            printf("Valor é maior que 10.\n"); // Bloco B5
            goto fim;
        }
    fim:                       // Bloco B6 (fim)
        printf("Fim do exemplo.\n"); // Bloco B7
}


void reaproveitar(){

    int x = 4, y = 5, z = 6;
    int r1 = x * y;
    int r2 = r1 + z;
    int r3 = x * y;
    int r4 = r3 + z;
/*
    O que pode ser reaproveitado?

r1 = x * y e r3 = x * y → são iguais.
r2 = r1 + z e r4 = r3 + z → também são iguais.
*/
}