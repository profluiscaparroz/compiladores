#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

void extrair_datas(const char *texto) {
    regex_t regex;
    regmatch_t match;
    int resultado;
    const char *padrao = "[0-9]{2}/[0-9]{2}/[0-9]{4}";
    const char *ptr = texto;

    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) {
        printf("Erro ao compilar regex\n");
        return;
    }

    while (regexec(&regex, ptr, 1, &match, 0) == 0) {
        int inicio = match.rm_so;
        int fim = match.rm_eo;

        char data[11];
        strncpy(data, ptr + inicio, fim - inicio);
        data[fim - inicio] = '\0';

        printf("Data encontrada: %s\n", data);
        ptr += fim;
    }

    regfree(&regex);
}

int main() {
    const char *texto = "Eventos: 10/04/2023, 25/12/2024, 01/01/2025.";
    extrair_datas(texto);
    return 0;
}
