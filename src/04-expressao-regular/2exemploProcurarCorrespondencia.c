#include <stdio.h>   // Biblioteca padrão para entrada e saída
#include <stdlib.h>  // Biblioteca padrão para funções utilitárias
#include <regex.h>   // Biblioteca para manipulação de expressões regulares
#include <string.h>  // Biblioteca para manipulação de strings

// Função para encontrar todas as correspondências de um padrão em um texto
void encontrar_todos(const char *padrao, const char *texto) {
    regex_t regex;         // Estrutura para armazenar a regex compilada
    regmatch_t match;      // Estrutura para armazenar informações sobre a correspondência
    int resultado;         // Variável para armazenar o resultado das operações com regex
    const char *ptr = texto; // Ponteiro para percorrer o texto

    // Compilar a expressão regular
    // regcomp compila o padrão fornecido em uma estrutura regex_t
    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) { // Se o resultado for diferente de 0, ocorreu um erro
        printf("Erro ao compilar regex\n");
        return; // Encerra a função em caso de erro
    }

    // Procurar múltiplas correspondências no texto
    while (regexec(&regex, ptr, 1, &match, 0) == 0) { // Enquanto encontrar correspondências
        int inicio = match.rm_so; // Índice inicial da correspondência
        int fim = match.rm_eo;    // Índice final da correspondência

        // Criar uma substring correspondente à correspondência encontrada
        char encontrado[10]; // Buffer para armazenar a correspondência (limite de 10 caracteres)
        strncpy(encontrado, ptr + inicio, fim - inicio); // Copia a correspondência para o buffer
        encontrado[fim - inicio] = '\0'; // Adiciona o caractere nulo para finalizar a string

        // Exibe a correspondência encontrada
        printf("Encontrado: %s\n", encontrado);
        
        // Move o ponteiro para continuar a busca após a correspondência atual
        ptr += fim;
    }

    // Liberar a memória alocada para a regex compilada
    regfree(&regex);
}

int main() {
    // Define o padrão de expressão regular
    // Neste caso, o padrão captura qualquer sequência de exatamente 4 dígitos consecutivos
    const char *padrao = "[0-9]{4}";

    // Define o texto onde as correspondências serão procuradas
    const char *texto = "Os anos 1999, 2023 e 2024 foram marcantes.";

    // Chama a função para encontrar todas as correspondências do padrão no texto
    encontrar_todos(padrao, texto);

    return 0; // Indica que o programa terminou com sucesso
}