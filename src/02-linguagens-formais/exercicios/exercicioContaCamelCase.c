#include <stdio.h>

int contar_palavras(char s[]) {
    int contador = 1; // Começamos com 1 porque a primeira palavra é sempre minúscula

    // Percorre a string
    for (int i = 0; s[i] != '\0'; i++) {
        // Se o caractere for maiúsculo, é uma nova palavra
        if (s[i] >= 'A' && s[i] <= 'Z') {
            contador++;
        }
    }

    return contador;
}

int main() {
    char entrada[101]; // Define um array para armazenar a string (máximo 100 caracteres + '\0')

    // Lendo a string de entrada
    scanf("%100s", entrada); // O %100s evita buffer overflow

    // Chamando a função e imprimindo o resultado
    printf("%d\n", contar_palavras(entrada));

    return 0;
}