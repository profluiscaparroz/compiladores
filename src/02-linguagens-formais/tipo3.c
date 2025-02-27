#include <stdio.h>
#include <string.h>

// Função para verificar se a string pertence à linguagem a^n b^n
int verifica_anbn(const char *str) {
    int count_a = 0, count_b = 0;
    int i = 0, estado = 0;

    // Percorre a string verificando a sequência correta
    while (str[i] != '\0') {
        if (estado == 0) {
            if (str[i] == 'a') {
                count_a++;
            } else if (str[i] == 'b') {
                estado = 1; // Troca para estado de 'b's
                count_b++;
            } else {
                return 0; // Se encontrar outro caractere, rejeita
            }
        } else if (estado == 1) {
            if (str[i] == 'b') {
                count_b++;
            } else {
                return 0; // Se encontrar outro caractere, rejeita
            }
        }
        i++;
    }

    // A string é aceita se count_a == count_b e pelo menos um 'a' e um 'b' existirem
    return (count_a == count_b && count_a > 0);
}

int main() {
    // Testes com diferentes strings
    char *testes[] = { "ab", "aabb", "aaabbb", "aaaabbbb", "abb", "ba", "aabbb", "aaaaabb", "bbb", "" };
    int num_testes = sizeof(testes) / sizeof(testes[0]);

    for (int i = 0; i < num_testes; i++) {
        if (verifica_anbn(testes[i]))
            printf("A string \"%s\" pertence à linguagem a^n b^n\n", testes[i]);
        else
            printf("A string \"%s\" NÃO pertence à linguagem a^n b^n\n", testes[i]);
    }

    return 0;
}
