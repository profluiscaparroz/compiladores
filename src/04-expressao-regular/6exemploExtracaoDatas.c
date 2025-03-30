#include <stdio.h>   // Biblioteca padrão para entrada e saída
#include <stdlib.h>  // Biblioteca padrão para funções utilitárias
#include <regex.h>   // Biblioteca para manipulação de expressões regulares
#include <string.h>  // Biblioteca para manipulação de strings

// Função para extrair todas as datas no formato DD/MM/AAAA de um texto
void extrair_datas(const char *texto) {
    regex_t regex;         // Estrutura para armazenar a regex compilada
    regmatch_t match;      // Estrutura para armazenar informações sobre a correspondência
    int resultado;         // Variável para armazenar o resultado das operações com regex
    const char *padrao = "[0-9]{2}/[0-9]{2}/[0-9]{4}"; // Padrão para datas no formato DD/MM/AAAA
    const char *ptr = texto; // Ponteiro para percorrer o texto original

    // Compilar a expressão regular
    // regcomp compila o padrão fornecido em uma estrutura regex_t
    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) { // Se o resultado for diferente de 0, ocorreu um erro
        printf("Erro ao compilar regex\n");
        return; // Encerra a função em caso de erro na compilação
    }

    // Procurar todas as ocorrências do padrão no texto
    while (regexec(&regex, ptr, 1, &match, 0) == 0) { // Enquanto encontrar correspondências
        int inicio = match.rm_so; // Índice inicial da correspondência
        int fim = match.rm_eo;    // Índice final da correspondência

        // Criar uma substring correspondente à data encontrada
        char data[11]; // Buffer para armazenar a data (formato DD/MM/AAAA + caractere nulo)
        strncpy(data, ptr + inicio, fim - inicio); // Copia a correspondência para o buffer
        data[fim - inicio] = '\0'; // Adiciona o caractere nulo para finalizar a string

        // Exibe a data encontrada
        printf("Data encontrada: %s\n", data);

        // Move o ponteiro para continuar a busca após a correspondência atual
        ptr += fim;
    }

    // Liberar a memória alocada para a regex compilada
    regfree(&regex);
}

int main() {
    // Texto de entrada contendo datas no formato DD/MM/AAAA
    const char *texto = "Eventos: 10/04/2023, 25/12/2024, 01/01/2025.";

    // Chama a função para extrair todas as datas do texto
    extrair_datas(texto);

    return 0; // Indica que o programa terminou com sucesso
}