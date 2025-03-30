#include <stdio.h>   // Biblioteca padrão para entrada e saída
#include <stdlib.h>  // Biblioteca padrão para funções utilitárias
#include <regex.h>   // Biblioteca para manipulação de expressões regulares
#include <string.h>  // Biblioteca para manipulação de strings

// Função para substituir todas as ocorrências de um padrão em um texto por um substituto
void substituir(const char *texto, const char *padrao, const char *substituto) {
    regex_t regex;         // Estrutura para armazenar a regex compilada
    regmatch_t match;      // Estrutura para armazenar informações sobre a correspondência
    char buffer[1024];     // Buffer temporário para manipulação de strings (não utilizado neste código)
    char resultado[1024] = ""; // String para armazenar o texto modificado
    const char *ptr = texto;   // Ponteiro para percorrer o texto original
    int offset = 0;            // Variável para rastrear o deslocamento (não utilizada neste código)

    // Compilar a expressão regular
    // regcomp compila o padrão fornecido em uma estrutura regex_t
    if (regcomp(&regex, padrao, REG_EXTENDED) != 0) {
        printf("Erro ao compilar regex\n");
        return; // Encerra a função em caso de erro na compilação
    }

    // Procurar todas as ocorrências do padrão no texto
    while (regexec(&regex, ptr, 1, &match, 0) == 0) { // Enquanto encontrar correspondências
        // Copiar a parte do texto antes da correspondência para o resultado
        strncat(resultado, ptr, match.rm_so);

        // Adicionar o texto substituto no lugar da correspondência
        strcat(resultado, substituto);

        // Avançar o ponteiro no texto após a correspondência
        ptr += match.rm_eo;
    }

    // Adicionar o restante do texto (após a última correspondência) ao resultado
    strcat(resultado, ptr);

    // Exibir o texto original e o texto modificado
    printf("Texto original: %s\n", texto);
    printf("Texto modificado: %s\n", resultado);

    // Liberar a memória alocada para a regex compilada
    regfree(&regex);
}

int main() {
    // Texto original onde as substituições serão feitas
    const char *texto = "O gato pulou sobre o muro.";

    // Padrão de regex que será substituído
    const char *padrao = "gato";  // Palavra a ser substituída

    // Texto que substituirá o padrão encontrado
    const char *substituto = "cachorro";  // Nova palavra

    // Chama a função para realizar a substituição
    substituir(texto, padrao, substituto);

    return 0; // Indica que o programa terminou com sucesso
}