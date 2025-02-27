#include <stdio.h>
#include <string.h>

// Função para verificar se a string pertence à linguagem a^n b^m c^m d^n
int verifica_linguagem(const char *str) {
    int count_a = 0, count_b = 0, count_c = 0, count_d = 0;
    int i = 0, estado = 0;

    // Percorre a string verificando os caracteres e contando
    while (str[i] != '\0') {
        if (estado == 0) { // Contando 'a's
            if (str[i] == 'a') count_a++;
            else if (str[i] == 'b') { estado = 1; count_b++; }
            else return 0;  // Erro de sequência ou caractere inválido
        }
        else if (estado == 1) { // Contando 'b's
            if (str[i] == 'b') count_b++;
            else if (str[i] == 'c') { estado = 2; count_c++; }
            else return 0;
        }
        else if (estado == 2) { // Contando 'c's
            if (str[i] == 'c') count_c++;
            else if (str[i] == 'd') { estado = 3; count_d++; }
            else return 0;
        }
        else if (estado == 3) { // Contando 'd's
            if (str[i] == 'd') count_d++;
            else return 0; // Qualquer outro caractere é inválido
        }
        i++;
    }

    // Verifica se os números seguem a regra n = d e b = c
    return (count_a == count_d && count_b == count_c && count_a >= 1 && count_b >= 1);
}

int main() {
    // Casos de teste
    char *testes[] = { "abbcdd", "aabbccdd", "aaabbcccddd", "abc", "aabbcddd", "aaaabbbbccccdddd", "aabbcccdd", "aaaa" };
    int num_testes = sizeof(testes) / sizeof(testes[0]);

    for (int i = 0; i < num_testes; i++) {
        if (verifica_linguagem(testes[i]))
            printf("A string \"%s\" pertence à linguagem a^n b^m c^m d^n\n", testes[i]);
        else
            printf("A string \"%s\" NÃO pertence à linguagem a^n b^m c^m d^n\n", testes[i]);
    }

    return 0;
}
