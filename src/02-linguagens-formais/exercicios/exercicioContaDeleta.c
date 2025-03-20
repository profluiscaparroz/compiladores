#include <stdio.h>

int contar_delecoes(char s[]) {
    int quantidadeDeExclusao = 0;

    // Percorre a string e conta caracteres consecutivos iguais
    for (int indice = 1; s[indice] != '\0'; indice++) {
        if (s[indice] == s[indice - 1]) {
            quantidadeDeExclusao++;
        }
    }

    return quantidadeDeExclusao;
}

int main() {
    char entrada[1000];

    // Lendo a entrada
    scanf("%s", entrada);

    // Chamando a função e imprimindo o resultado
    printf("%d\n", contar_delecoes(entrada));

    return 0;
}