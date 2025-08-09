// Arquivo de teste para o analisador léxico
#include <stdio.h>
#define MAX_SIZE 100

int main() {
    int idade = 25;
    float salario = 1500.50;
    char nome[] = "João";
    char inicial = 'J';
    
    /* Verificação de idade */
    if (idade >= 18 && idade <= 65) {
        printf("Idade válida: %d\n", idade);
        salario += 200.0;
    } else {
        printf("Idade inválida!\n");
    }
    
    // Loop de exemplo
    for (int i = 0; i < 5; i++) {
        printf("Contador: %d\n", i);
    }
    
    return 0;
}