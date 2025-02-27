#include <stdio.h>
#include <string.h>

// Função que verifica se uma string pertence à linguagem ww^R
int verifica_wwR(const char *str) {
    int len = strlen(str);
    
    // Se o comprimento for ímpar, não pode ser da forma ww^R
    if (len % 2 != 0)
        return 0;

    int mid = len / 2; // Ponto central da string

    // Comparação da primeira metade com o reverso da segunda metade
    for (int i = 0; i < mid; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0; // Se houver uma diferença, a string não pertence à linguagem
        }
    }

    return 1; // A string pertence à linguagem
}

int main() {
    // Testes para validar a implementação
    char *testes[] = {
        "abbaabba",  // Aceito (w = abba, w^R = abba)
        "aabbbaa",   // Aceito (w = aabb, w^R = baa)
        "bababbab",  // Aceito (w = babab, w^R = babab)
        "abcba",     // Rejeitado (ímpar)
        "aabb",      // Rejeitado (não é ww^R)
        "bbaa",      // Rejeitado (não é ww^R)
        "abababab",  // Aceito (w = abab, w^R = abab)
        ""           // Rejeitado (vazio)
    };

    int num_testes = sizeof(testes) / sizeof(testes[0]);

    for (int i = 0; i < num_testes; i++) {
        if (verifica_wwR(testes[i]))
            printf("A string \"%s\" pertence à linguagem ww^R\n", testes[i]);
        else
            printf("A string \"%s\" NÃO pertence à linguagem ww^R\n", testes[i]);
    }

    return 0;
}
