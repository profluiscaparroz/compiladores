#include <stdio.h>
#include <string.h>
#include <ctype.h>

void analisar_entrada(char *s) {
    int tokens = 0, caracteres_sem_espaco = 0;
    int len = strlen(s);
    int termina_com_pv = (len > 0 && s[len - 1] == ';');
    
    // Contar tokens
    char *token = strtok(s, " ");
    while (token != NULL) {
        tokens++;
        for (int i = 0; token[i] != '\0'; i++) {
            if (!isspace(token[i])) {
                caracteres_sem_espaco++;
            }
        }
        token = strtok(NULL, " ");
    }

    // Imprimir resultado
    printf("%d %d %s\n", tokens, caracteres_sem_espaco, termina_com_pv ? "Estrutura válida" : "Estrutura inválida");
}

int main() {
    char entrada[1000];

    // Lendo a entrada
    fgets(entrada, sizeof(entrada), stdin);

    // Remover o '\n' da entrada (caso exista)
    entrada[strcspn(entrada, "\n")] = 0;

    // Chamar a função de análise
    analisar_entrada(entrada);

    return 0;
}
