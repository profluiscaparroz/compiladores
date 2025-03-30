#include <stdio.h>   // Biblioteca padrão para entrada e saída
#include <stdlib.h>  // Biblioteca padrão para funções utilitárias
#include <regex.h>   // Biblioteca para manipulação de expressões regulares

// Função para validar se um e-mail é válido com base em uma expressão regular
int validar_email(const char *email) {
    regex_t regex;         // Estrutura para armazenar a regex compilada
    int resultado;         // Variável para armazenar o resultado da validação

    // Expressão regular para validar e-mails
    // O padrão verifica:
    // - Início com letras, números ou caracteres especiais permitidos (._%+-)
    // - Presença obrigatória de um '@'
    // - Domínio com letras, números ou hífens
    // - Um ponto seguido de uma extensão de 2 ou mais letras
    const char *padrao = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";

    // Compilar a regex
    // regcomp compila o padrão fornecido em uma estrutura regex_t
    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) { // Se o resultado for diferente de 0, ocorreu um erro
        printf("Erro ao compilar regex\n");
        return 0; // Retorna 0 indicando que o e-mail não é válido
    }

    // Executar a regex no e-mail fornecido
    // regexec verifica se o e-mail corresponde ao padrão compilado
    resultado = regexec(&regex, email, 0, NULL, 0);

    // Liberar a memória alocada para a regex compilada
    regfree(&regex);

    // Retorna 1 se o e-mail for válido (resultado == 0), 0 caso contrário
    return resultado == 0;
}

int main() {
    // Lista de e-mails para validação
    const char *emails[] = {
        "usuario@example.com",       // E-mail válido
        "email_invalido@",           // Inválido: falta o domínio
        "teste@dominio.org",         // E-mail válido
        "@sem_usuario.com",          // Inválido: falta o nome do usuário
        "correto.email123@servidor.net" // E-mail válido
    };

    // Loop para validar cada e-mail da lista
    for (int i = 0; i < 5; i++) {
        // Chama a função validar_email para verificar se o e-mail é válido
        if (validar_email(emails[i])) {
            printf("Válido: %s\n", emails[i]); // Exibe se o e-mail é válido
        } else {
            printf("Inválido: %s\n", emails[i]); // Exibe se o e-mail é inválido
        }
    }

    return 0; // Indica que o programa terminou com sucesso
}