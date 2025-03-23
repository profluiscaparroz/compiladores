#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

int validar_telefone(const char *telefone) {
    regex_t regex;
    int resultado;

    // Regex para números no formato (XX) XXXX-XXXX ou (XX) XXXXX-XXXX
    const char *padrao = "^\\([0-9]{2}\\) [0-9]{4,5}-[0-9]{4}$";

    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) {
        printf("Erro ao compilar regex\n");
        return 0;
    }

    resultado = regexec(&regex, telefone, 0, NULL, 0);
    regfree(&regex);

    return resultado == 0;
}

int main() {
    const char *telefones[] = {
        "(11) 98765-4321",
        "(21) 3456-7890",
        "1234-5678",
        "(99) 123-4567",
        "(31) 12345-6789"
    };

    for (int i = 0; i < 5; i++) {
        if (validar_telefone(telefones[i])) {
            printf("Válido: %s\n", telefones[i]);
        } else {
            printf("Inválido: %s\n", telefones[i]);
        }
    }

    return 0;
}
