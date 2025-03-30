#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Função que verifica se a string é um e-mail válido usando AFD
int automato_email(const char *entrada) {
    int estado = 0; // Estado inicial
    int i = 0;

    while (entrada[i] != '\0') {
        char c = entrada[i];

        switch (estado) {
            case 0: // Estado inicial (antes do '@')
                if (isalnum(c) || c == '_' || c == '.') {
                    estado = 1; // Continua no nome de usuário
                } else {
                    estado = 4; // Estado de erro
                }
                break;

            case 1: // Nome de usuário
                if (isalnum(c) || c == '_' || c == '.') {
                    estado = 1; // Continua no nome de usuário
                } else if (c == '@') {
                    estado = 2; // Passa para o domínio
                } else {
                    estado = 4; // Estado de erro
                }
                break;

            case 2: // Parte do domínio (após '@')
                if (isalnum(c)) {
                    estado = 3; // Continua no domínio
                } else {
                    estado = 4; // Estado de erro
                }
                break;

            case 3: // Domínio antes do '.'
                if (isalnum(c) || c == '.') {
                    estado = 3; // Continua no domínio
                } else if (c == '.') {
                    estado = 5; // Passa para a extensão
                } else {
                    estado = 4; // Estado de erro
                }
                break;

            case 5: // Extensão do domínio (após '.')
                if (isalpha(c)) {
                    estado = 6; // Continua na extensão
                } else {
                    estado = 4; // Estado de erro
                }
                break;

            case 6: // Finalizando a extensão
                if (isalpha(c)) {
                    estado = 6; // Continua na extensão
                } else {
                    estado = 4; // Estado de erro
                }
                break;

            case 4: // Estado de erro
                return 0; // Rejeita imediatamente
        }

        i++;
    }

    // O e-mail só é válido se terminar no estado 6 (extensão válida)
    return (estado == 6);
}

int main() {
    const char *testes[] = {
        "teste@email.com",
        "usuario_123@dominio.org",
        "meu.email@site.net",
        "@gmail.com",
        "semarroba.com",
        "nome@.com",
        "user@site"
    };

    int num_testes = sizeof(testes) / sizeof(testes[0]);

    for (int i = 0; i < num_testes; i++) {
        printf("Entrada '%s': %s\n", testes[i], automato_email(testes[i]) ? "Aceita" : "Rejeitada");
    }

    return 0;
}
