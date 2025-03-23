#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

int validar_ip(const char *ip) {
    regex_t regex;
    int resultado;

    const char *padrao = "^([0-9]{1,3}\\.){3}[0-9]{1,3}$";

    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) {
        printf("Erro ao compilar regex\n");
        return 0;
    }

    resultado = regexec(&regex, ip, 0, NULL, 0);
    regfree(&regex);

    return resultado == 0;
}

int main() {
    const char *ips[] = {
        "192.168.1.1",
        "255.255.255.255",
        "10.0.0.1",
        "300.400.500.600",
        "123.45.67.89"
    };

    for (int i = 0; i < 5; i++) {
        if (validar_ip(ips[i])) {
            printf("Válido: %s\n", ips[i]);
        } else {
            printf("Inválido: %s\n", ips[i]);
        }
    }

    return 0;
}
