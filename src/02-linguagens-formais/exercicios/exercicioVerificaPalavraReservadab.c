#include <stdio.h>

// Função para comparar duas strings manualmente
int comparar_strings(char s1[], char s2[]) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            return 0; // As strings são diferentes
        }
        i++;
    }
    return (s1[i] == '\0' && s2[i] == '\0'); // Retorna 1 se forem iguais, 0 caso contrário
}

// Função para verificar se a string é uma palavra reservada
int eh_palavra_reservada(char s[]) {
    char *palavras_reservadas[] = {"if", "else", "while", "return", "int", "float", "char"};
    int tamanho = sizeof(palavras_reservadas) / sizeof(palavras_reservadas[0]);

    // Percorre a lista de palavras reservadas e compara manualmente
    for (int i = 0; i < tamanho; i++) {
        if (comparar_strings(s, palavras_reservadas[i])) {
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