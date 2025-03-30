#include <stdio.h>   // Biblioteca padrão para entrada e saída
#include <stdlib.h>  // Biblioteca padrão para funções utilitárias
#include <regex.h>   // Biblioteca para manipulação de expressões regulares

// Função para validar se um endereço IP está no formato correto (IPv4)
int validar_ip(const char *ip) {
    regex_t regex;         // Estrutura para armazenar a regex compilada
    int resultado;         // Variável para armazenar o resultado da validação

    // Expressão regular para validar endereços IPv4
    // O padrão verifica:
    // - Quatro grupos de 1 a 3 dígitos (0-9), separados por pontos
    // - Cada grupo deve estar no formato "XXX."
    // - O último grupo não possui o ponto final
    const char *padrao = "^([0-9]{1,3}\\.){3}[0-9]{1,3}$";

    // Compilar a regex
    // regcomp compila o padrão fornecido em uma estrutura regex_t
    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) { // Se o resultado for diferente de 0, ocorreu um erro
        printf("Erro ao compilar regex\n");
        return 0; // Retorna 0 indicando que o IP não é válido
    }

    // Executar a regex no endereço IP fornecido
    // regexec verifica se o IP corresponde ao padrão compilado
    resultado = regexec(&regex, ip, 0, NULL, 0);

    // Liberar a memória alocada para a regex compilada
    regfree(&regex);

    // Retorna 1 se o IP for válido (resultado == 0), 0 caso contrário
    return resultado == 0;
}

int main() {
    // Lista de endereços IP para validação
    const char *ips[] = {
        "192.168.1.1",       // Válido: formato correto
        "255.255.255.255",   // Válido: formato correto
        "10.0.0.1",          // Válido: formato correto
        "300.400.500.600",   // Inválido: valores fora do intervalo permitido (0-255)
        "123.45.67.89"       // Válido: formato correto
    };

    // Loop para validar cada endereço IP da lista
    for (int i = 0; i < 5; i++) {
        // Chama a função validar_ip para verificar se o IP é válido
        if (validar_ip(ips[i])) {
            printf("Válido: %s\n", ips[i]); // Exibe se o IP é válido
        } else {
            printf("Inválido: %s\n", ips[i]); // Exibe se o IP é inválido
        }
    }

    return 0; // Indica que o programa terminou com sucesso
}