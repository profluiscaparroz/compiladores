#include <stdio.h>   // Biblioteca padrão para entrada e saída
#include <stdlib.h>  // Biblioteca padrão para funções utilitárias
#include <regex.h>   // Biblioteca para manipulação de expressões regulares
#include <string.h>  // Biblioteca para manipulação de strings

// Função para dividir uma string com base em delimitadores definidos por uma expressão regular
void dividir_string(const char *texto, const char *padrao) {
    regex_t regex;         // Estrutura para armazenar a regex compilada
    regmatch_t match;      // Estrutura para armazenar informações sobre a correspondência
    const char *ptr = texto; // Ponteiro para percorrer o texto original
    int inicio, fim;       // Variáveis para armazenar os índices de início e fim da correspondência

    // Compilar a expressão regular
    // regcomp compila o padrão fornecido em uma estrutura regex_t
    if (regcomp(&regex, padrao, REG_EXTENDED)) {
        printf("Erro ao compilar regex\n");
        return; // Encerra a função em caso de erro na compilação
    }

    // Procurar todas as ocorrências do padrão no texto
    while (regexec(&regex, ptr, 1, &match, 0) == 0) { // Enquanto encontrar correspondências
        inicio = match.rm_so; // Índice inicial da correspondência
        fim = match.rm_eo;    // Índice final da correspondência

        // Criar um token com a parte do texto antes da correspondência
        char token[256]; // Buffer para armazenar o token
        strncpy(token, ptr, inicio); // Copia a parte antes do delimitador para o buffer
        token[inicio] = '\0'; // Adiciona o caractere nulo para finalizar a string

        // Exibe o token, se ele não estiver vazio
        if (strlen(token) > 0) {
            printf("Parte: %s\n", token);
        }

        // Avança o ponteiro no texto após a correspondência
        ptr += fim;
    }

    // Exibe a parte restante do texto (após a última correspondência), se existir
    if (*ptr) {
        printf("Parte: %s\n", ptr);
    }

    // Liberar a memória alocada para a regex compilada
    regfree(&regex);
}

int main() {
    // Texto de entrada que será dividido
    const char *texto = "banana,maçã;laranja|pera";

    // Padrão de regex que define os delimitadores (, ; |)
    const char *padrao = "[,;|]";  // Delimitadores: vírgula, ponto e vírgula, barra vertical

    // Chama a função para dividir a string com base nos delimitadores
    dividir_string(texto, padrao);

    return 0; // Indica que o programa terminou com sucesso
}