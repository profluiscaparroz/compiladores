#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

void substituir(const char *texto, const char *padrao, const char *substituto) {
    regex_t regex;
    regmatch_t match;
    char buffer[1024];
    char resultado[1024] = "";
    const char *ptr = texto;
    int offset = 0;

    // Compilar a regex
    if (regcomp(&regex, padrao, REG_EXTENDED) != 0) {
        printf("Erro ao compilar regex\n");
        return;
    }

    // Procurar a primeira ocorrência
    while (regexec(&regex, ptr, 1, &match, 0) == 0) {
        // Copiar parte antes da correspondência
        strncat(resultado, ptr, match.rm_so);

        // Adicionar a substituição
        strcat(resultado, substituto);

        // Avançar no texto
        ptr += match.rm_eo;
    }

    // Adicionar o restante do texto
    strcat(resultado, ptr);

    printf("Texto original: %s\n", texto);
    printf("Texto modificado: %s\n", resultado);

    regfree(&regex);
}

int main() {
    const char *texto = "O gato pulou sobre o muro.";
    const char *padrao = "gato";  // Palavra a ser substituída
    const char *substituto = "cachorro";  // Nova palavra

    substituir(texto, padrao, substituto);

    return 0;
}
