#include <stdio.h>
#include <string.h>

int eh_palavra_reservada(char s[]) {
    // Lista de palavras reservadas
    char *palavras_reservadas[] = {"if", "else", "while", "return", "int", "float", "char"};
    int tamanho = sizeof(palavras_reservadas) / sizeof(palavras_reservadas[0]);

    // Verifica se a string está na lista de palavras reservadas
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(s, palavras_reservadas[i]) == 0) {
            return 1; // É palavra reservada
        }
    }

    return 0; // Não é palavra reservada (identificador)
}

int main() {
    char entrada[50]; // Buffer para armazenar a entrada

    // Lendo a string de entrada
    scanf("%49s", entrada); // O %49s evita buffer overflow

    // Verifica e imprime o resultado
    if (eh_palavra_reservada(entrada)) {
        printf("Palavra Reservada\n");
    } else {
        printf("Identificador\n");
    }

    return 0;
}
