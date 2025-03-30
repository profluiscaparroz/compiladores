#include <stdio.h>
#include <ctype.h>

// Função que verifica se a string é um identificador válido
int automato_identificador(const char *entrada) {
    int estado = 0; // Estado inicial

    for (int i = 0; entrada[i] != '\0'; i++) {
        char c = entrada[i];

        switch (estado) {
            case 0: // Estado inicial (q0)
                if (isalpha(c) || c == '_') {
                    estado = 1; // Vai para q1 (identificador válido)
                } else {
                    estado = 2; // Vai para q2 (erro)
                }
                break;

            case 1: // Estado q1 (letras, números e _ são permitidos)
                if (isalnum(c) || c == '_') {
                    estado = 1; // Permanece em q1
                } else {
                    estado = 2; // Vai para q2 (erro)
                }
                break;

            case 2: // Estado de erro (q2)
                return 0; // Rejeita imediatamente
        }
    }

    // Se terminou no estado q1, a string é um identificador válido
    return (estado == 1);
}

int main() {
    const char *testes[] = {"variavel", "_contador", "x1y2z3", "123abc", "var#1", "hello world", "myVar"};
    int num_testes = sizeof(testes) / sizeof(testes[0]);

    for (int i = 0; i < num_testes; i++) {
        printf("Entrada '%s': %s\n", testes[i], automato_identificador(testes[i]) ? "Aceita" : "Rejeitada");
    }

    return 0;
}
