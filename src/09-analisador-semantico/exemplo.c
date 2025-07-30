#include <stdio.h>



int funcao_usa_soma(int a, int b) {
    return soma(a, b);
}

int soma(int a, int b) {
    return a + b;
}


int main() {
    int *ptr = NULL; // Ponteiro nulo
    int valor;
    valor = 15;
    
    if(valor == 10) {
        int valor3 = 10;
        printf("%d", valor3); // Erro: valor3 não está definido fora do escopo do if
    }


    if(valor == 10){
        ptr = &valor; // Atribui o endereço de valor ao ponteiro
    } else {
        ptr = NULL; // Mantém o ponteiro nulo se a condição não for atendida
    }

    // Tentativa de acessar o valor do ponteiro nulo
    // Isso causa um erro de semântica e pode levar a um comportamento indefinido.
    printf("Valor: %d\n", *ptr);

    return 0;
}