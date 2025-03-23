#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

void dividir_string(const char *texto, const char *padrao) {
    regex_t regex;
    regmatch_t match;
    const char *ptr = texto;
    int inicio, fim;

    if (regcomp(&regex, padrao, REG_EXTENDED)) {
        printf("Erro ao compilar regex\n");
        return;
    }

    while (regexec(&regex, ptr, 1, &match, 0) == 0) {
        inicio = match.rm_so;
        fim = match.rm_eo;

        char token[256];
        strncpy(token, ptr, inicio);
        token[inicio] = '\0';

        if (strlen(token) > 0) {
            printf("Parte: %s\n", token);
        }

        ptr += fim;
    }

    if (*ptr) {
        printf("Parte: %s\n", ptr);
    }

    regfree(&regex);
}

int main() {
    const char *texto = "banana,maçã;laranja|pera";
    const char *padrao = "[,;|]";  // Delimitadores: , ; |

    dividir_string(texto, padrao);

    return 0;
}
