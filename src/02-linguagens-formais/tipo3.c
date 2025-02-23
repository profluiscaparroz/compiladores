#include <stdio.h>
#include <string.h>

// Função para simular o AFD
int isAccepted(char *word) {
    int state = 0; // Começa no estado q0
    int len = strlen(word);

    for (int i = 0; i < len; i++) {
        char c = word[i];

        // Transições baseadas no estado atual e no símbolo de entrada
        if (state == 0) {
            if (c == 'a') {
                state = 0; // Fica em q0 com 'a'
            } else if (c == 'b') {
                state = 1; // Vai para q1 com 'b'
            } else {
                return 0; // Símbolo inválido
            }
        } else if (state == 1) {
            if (c == 'a' || c == 'b') {
                state = 1; // Fica em q1 com 'a' ou 'b'
            } else {
                return 0; // Símbolo inválido
            }
        }
    }

    // A palavra é aceita se terminar em um estado de aceitação (q1)
    return (state == 1);
}

int main() {
    char word[100];

    printf("Digite uma palavra para verificar se é aceita pelo AFD: ");
    scanf("%s", word);

    if (isAccepted(word)) {
        printf("A palavra '%s' é aceita pelo AFD.\n", word);
    } else {
        printf("A palavra '%s' não é aceita pelo AFD.\n", word);
    }

    return 0;
}
