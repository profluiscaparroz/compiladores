#include <stdio.h>

// Função para verificar se a string contém apenas 'a' e 'b'
int validar_string(char s[]) {
    int i = 0;

    // Se a string estiver vazia, retorna inválida
    if (s[i] == '\0') {
        return 0;
    }

    // Percorre a string verificando cada caractere
    while (s[i] != '\0') {
        if (s[i] != 'a' && s[i] != 'b') {
            return 0; // Se encontrar algo diferente de 'a' ou 'b', retorna inválido
        }
        i++;
    }

    return 1; // Se passou por toda a string, é válida
}

int main() {
    char entrada[100]; // Buffer para armazenar a entrada

    // Lendo a string de entrada
    printf("Digite a palavra: ");
    scanf("%99s", entrada); // Lendo no máximo 99 caracteres para evitar overflow

    // Validação e exibição do resultado
    if (validar_string(entrada)) {
        printf("Válido\n");
    } else {
        printf("Inválido\n");
    }

    return 0;
}