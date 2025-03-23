#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

int validar_email(const char *email) {
    regex_t regex;
    int resultado;

    // Expressão regular para validar e-mails
    const char *padrao = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";

    // Compilar a regex
    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) {
        printf("Erro ao compilar regex\n");
        return 0;
    }

    // Executar a regex no email fornecido
    resultado = regexec(&regex, email, 0, NULL, 0);

    // Liberar memória
    regfree(&regex);

    // Retorna 1 se o email for válido, 0 caso contrário
    return resultado == 0;
}

int main() {
    const char *emails[] = {
        "usuario@example.com",
        "email_invalido@",
        "teste@dominio.org",
        "@sem_usuario.com",
        "correto.email123@servidor.net"
    };

    for (int i = 0; i < 5; i++) {
        if (validar_email(emails[i])) {
            printf("Válido: %s\n", emails[i]);
        } else {
            printf("Inválido: %s\n", emails[i]);
        }
    }

    return 0;
}
