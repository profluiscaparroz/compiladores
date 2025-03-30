#include <stdio.h>   // Biblioteca padrão para entrada e saída
#include <stdlib.h>  // Biblioteca padrão para funções utilitárias
#include <regex.h>   // Biblioteca para manipulação de expressões regulares

// Função para verificar se um padrão de expressão regular é encontrado em um texto
void verificar_padrao(const char *padrao, const char *texto) {
    regex_t regex;   // Estrutura para armazenar a expressão regular compilada
    int resultado;   // Variável para armazenar o resultado das operações com regex

    // Compilar a expressão regular
    // regcomp compila o padrão fornecido em uma estrutura regex_t
    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) { // Se o resultado for diferente de 0, ocorreu um erro
        printf("Erro ao compilar regex\n");
        return; // Encerra a função em caso de erro
    }

    // Executar a regex no texto
    // regexec verifica se o texto corresponde ao padrão compilado
    resultado = regexec(&regex, texto, 0, NULL, 0);
    if (!resultado) { // Se o resultado for 0, houve uma correspondência
        printf("Padrão encontrado na string: \"%s\"\n", texto);
    } else { // Caso contrário, nenhuma correspondência foi encontrada
        printf("Nenhuma correspondência encontrada na string: \"%s\"\n", texto);
    }

    // Liberar a memória alocada para a regex compilada
    // É importante liberar a memória para evitar vazamentos
    regfree(&regex);
}

int main() {
    // Define o padrão de expressão regular
    // Neste caso, o padrão busca por exatamente 4 dígitos consecutivos ([0-9]{4})
    const char *padrao = "[0-9]{4}";  // Equivalente a \d{4} em outras linguagens

    // Define os textos que serão verificados
    const char *texto1 = "Ano 2024 está próximo."; // Contém 4 dígitos
    const char *texto2 = "Nenhum número aqui.";   // Não contém 4 dígitos consecutivos

    // Chama a função para verificar o padrão nos textos
    verificar_padrao(padrao, texto1); // Deve encontrar o padrão
    verificar_padrao(padrao, texto2); // Não deve encontrar o padrão

    return 0; // Indica que o programa terminou com sucesso
}