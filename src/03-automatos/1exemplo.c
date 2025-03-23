#include <stdio.h>

// Função que simula um AFD para verificar se a string contém apenas números

/*
Como funciona este AFD?
Ele começa no estado inicial (q0).
Se encontrar um dígito (0-9), permanece no estado q1.
Se encontrar qualquer outro caractere, vai para um estado de erro (q2) e rejeita a entrada.
Se chegar ao fim da string no estado q1, a entrada é aceita.
*/
int automato_numerico(const char *entrada) {
    int estado = 0; // Estado inicial (q0)
    
    for (int i = 0; entrada[i] != '\0'; i++) {
        char c = entrada[i];

        switch (estado) {
            case 0: // Estado inicial (q0)
                if (c >= '0' && c <= '9') {
                    estado = 1; // Vai para q1 (estado válido para números)
                } else {
                    estado = 2; // Vai para q2 (estado de erro)
                }
                break;

            case 1: // Estado q1 (número válido)
                if (c >= '0' && c <= '9') {
                    estado = 1; // Continua em q1
                } else {
                    estado = 2; // Vai para q2 (erro)
                }
                break;

            case 2: // Estado de erro (q2)
                return 0; // Rejeita imediatamente
        }
    }

    // Se terminou no estado q1, a string é um número válido
    return (estado == 1);
}

int main() {
    const char *testes[] = {"12345", "42", "98a7", "hello", "67890", "12.34"};
    int num_testes = sizeof(testes) / sizeof(testes[0]);

    for (int i = 0; i < num_testes; i++) {
        printf("Entrada '%s': %s\n", testes[i], automato_numerico(testes[i]) ? "Aceita" : "Rejeitada");
    }

    return 0;
}
