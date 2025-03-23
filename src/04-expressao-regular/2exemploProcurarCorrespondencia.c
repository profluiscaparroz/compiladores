#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

void encontrar_todos(const char *padrao, const char *texto) {
    regex_t regex;
    regmatch_t match;
    int resultado;
    const char *ptr = texto;

    // Compilar a regex
    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) {
        printf("Erro ao compilar regex\n");
        return;
    }

    // Procurar múltiplas correspondências
    while (regexec(&regex, ptr, 1, &match, 0) == 0) {
        int inicio = match.rm_so;
        int fim = match.rm_eo;

        // Criar uma substring correspondente
        char encontrado[10];
        strncpy(encontrado, ptr + inicio, fim - inicio);
        encontrado[fim - inicio] = '\0';

        printf("Encontrado: %s\n", encontrado);
        
        // Mover o ponteiro para continuar a busca
        ptr += fim;
    }

    // Liberar memória
    regfree(&regex);
}

int main() {
    const char *padrao = "[0-9]{4}";  // Captura qualquer número de 4 dígitos
    const char *texto = "Os anos 1999, 2023 e 2024 foram marcantes.";

    encontrar_todos(padrao, texto);

    return 0;
}
