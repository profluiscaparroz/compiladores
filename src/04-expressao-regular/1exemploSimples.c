#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

void verificar_padrao(const char *padrao, const char *texto) {
    regex_t regex;
    int resultado;

    // Compilar a expressão regular
    resultado = regcomp(&regex, padrao, REG_EXTENDED);
    if (resultado) {
        printf("Erro ao compilar regex\n");
        return;
    }

    // Executar a regex no texto
    resultado = regexec(&regex, texto, 0, NULL, 0);
    if (!resultado) {
        printf("Padrão encontrado na string: \"%s\"\n", texto);
    } else {
        printf("Nenhuma correspondência encontrada na string: \"%s\"\n", texto);
    }

    // Liberar a memória da regex compilada
    regfree(&regex);
}

int main() {
    const char *padrao = "[0-9]{4}";  // Equivalente a \d{4} em outras linguagens
    const char *texto1 = "Ano 2024 está próximo.";
    const char *texto2 = "Nenhum número aqui.";

    verificar_padrao(padrao, texto1);
    verificar_padrao(padrao, texto2);

    return 0;
}
