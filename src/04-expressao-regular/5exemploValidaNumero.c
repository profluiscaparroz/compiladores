#include <stdio.h>   // Biblioteca padrão para entrada e saída
#include <stdlib.h>  // Biblioteca padrão para funções utilitárias
#include <regex.h>   // Biblioteca para manipulação de expressões regulares

// Função para validar se um número de telefone está no formato correto
int validar_telefone(const char *telefone) {
    regex_t regex;         // Estrutura para armazenar a regex compilada
    int resultado;         // Variável para armazenar o resultado da validação

    // Expressão regular para validar números de telefone
    // O padrão verifica:
    // - Início com parênteses contendo dois dígitos: (XX)
    // - Um espaço após os parênteses
    // - Quatro ou cinco dígitos seguidos de um hífen
    // - Quatro dígitos no final
    const char *padrao = "^\\([0-9]{2}\\) [0-9]{4,5}-[0-9]{4}$";

    // Compilar a regex
    // regcomp compila o padrão fornecido em uma estrutura regex_t
    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) { // Se o resultado for diferente de 0, ocorreu um erro
        printf("Erro ao compilar regex\n");
        return 0; // Retorna 0 indicando que o número não é válido
    }

    // Executar a regex no número de telefone fornecido
    // regexec verifica se o número corresponde ao padrão compilado
    resultado = regexec(&regex, telefone, 0, NULL, 0);

    // Liberar a memória alocada para a regex compilada
    regfree(&regex);

    // Retorna 1 se o número for válido (resultado == 0), 0 caso contrário
    return resultado == 0;
}

int main() {
    // Lista de números de telefone para validação
    const char *telefones[] = {
        "(11) 98765-4321", // Válido: formato correto com 5 dígitos antes do hífen
        "(21) 3456-7890",  // Válido: formato correto com 4 dígitos antes do hífen
        "1234-5678",       // Inválido: falta o código de área e o formato está incorreto
        "(99) 123-4567",   // Inválido: número antes do hífen tem menos de 4 dígitos
        "(31) 12345-6789"  // Válido: formato correto com 5 dígitos antes do hífen
    };

    // Loop para validar cada número de telefone da lista
    for (int i = 0; i < 5; i++) {
        // Chama a função validar_telefone para verificar se o número é válido
        if (validar_telefone(telefones[i])) {
            printf("Válido: %s\n", telefones[i]); // Exibe se o número é válido
        } else {
            printf("Inválido: %s\n", telefones[i]); // Exibe se o número é inválido
        }
    }

    return 0; // Indica que o programa terminou com sucesso
}