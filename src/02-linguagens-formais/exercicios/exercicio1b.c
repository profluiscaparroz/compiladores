#include <stdio.h>

void analisar_entrada(char s[]) {
    int tokens = 0, caracteres_sem_espaco = 0;
    int i = 0, palavra = 0, len = 0;
    int termina_com_pv = 0;

    // Percorre a string manualmente
    while (s[len] != '\0') {
        // Conta caracteres que não são espaço
        if (s[len] != ' ') {
            caracteres_sem_espaco++;
        }

        // Detecta se está em um token (nova palavra iniciando)
        if (s[len] != ' ' && palavra == 0) {
            tokens++;
            palavra = 1;
        } 
        // Se encontrar espaço, indica que a palavra terminou
        else if (s[len] == ' ') {
            palavra = 0;
        }

        // Verifica se o último caractere é ';'
        if (s[len + 1] == '\0' && s[len] == ';') {
            termina_com_pv = 1;
        }

        len++;
    }

    // Exibe a saída
    printf("%d %d %s\n", tokens, caracteres_sem_espaco, termina_com_pv ? "Estrutura válida" : "Estrutura inválida");
}

int main() {
    char entrada[1000];

    // Lendo a entrada
    fgets(entrada, sizeof(entrada), stdin);

    // Chamar a função de análise
    analisar_entrada(entrada);

    return 0;
}