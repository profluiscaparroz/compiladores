#include <stdio.h>
#include <stdbool.h>

// Função para comparar regex com texto
bool match(const char *regex, const char *text);

// Função auxiliar para o operador *
bool match_star(char c, const char *regex, const char *text);

// Implementação principal do regex simples
bool match(const char *regex, const char *text) {
    // Caso base: regex vazia
    if (regex[0] == '\0') {
        return true;
    }
    
    // Caso especial: $ no final da regex
    if (regex[0] == '$' && regex[1] == '\0') {
        return *text == '\0';
    }
    
    // Operador *: zero ou mais ocorrências
    if (regex[1] == '*') {
        return match_star(regex[0], regex + 2, text);
    }
    
    // Caractere curinga . ou caractere igual
    if (*text != '\0' && (regex[0] == '.' || regex[0] == *text)) {
        return match(regex + 1, text + 1);
    }
    
    return false;
}

// Implementação do operador *
bool match_star(char c, const char *regex, const char *text) {
    do {
        // Tenta casar o restante do padrão
        if (match(regex, text)) {
            return true;
        }
    } while (*text != '\0' && (*text++ == c || c == '.'));
    
    return false;
}

// Função wrapper para verificar se o texto inteiro casa com a regex
bool regex_match(const char *regex, const char *text) {
    // Se a regex começa com ^, verifica apenas no início
    if (regex[0] == '^') {
        return match(regex + 1, text);
    }
    
    // Caso contrário, verifica em qualquer posição do texto
    do {
        if (match(regex, text)) {
            return true;
        }
    } while (*text++ != '\0');
    
    return false;
}

// Função principal para teste
int main() {
    const char *regex = "a*b.c";
    const char *text = "aaaabXc";
    
    if (regex_match(regex, text)) {
        printf("'%s' corresponde ao padrão '%s'\n", text, regex);
    } else {
        printf("'%s' NÃO corresponde ao padrão '%s'\n", text, regex);
    }
    
    return 0;
}

// Explicacao do regex
/*
A expressão regular "a*b.c" tem o seguinte significado:

a*:

O caractere a pode aparecer zero ou mais vezes.
Exemplos válidos: "" (nenhum a), "a", "aa", "aaa", etc.
b:

O caractere b deve aparecer exatamente uma vez após os as.
.:

O ponto (.) é um curinga que corresponde a qualquer caractere único (exceto uma nova linha).
c:

O caractere c deve aparecer exatamente uma vez no final.
Resumo:
A regex "a*b.c" corresponde a:

Uma sequência de zero ou mais as, seguida por:
Um único caractere b, seguido por:
Qualquer caractere único, seguido por:
Um único caractere c.
Exemplos de correspondências válidas:
"bXc": Nenhum a, seguido de b, qualquer caractere (X), e c.
"abZc": Um a, seguido de b, qualquer caractere (Z), e c.
"aaaabQc": Quatro as, seguido de b, qualquer caractere (Q), e c.
Exemplos de correspondências inválidas:
"abc": Falta o caractere curinga entre b e c.
"aabcc": Há um caractere extra após c.
"aaaac": Falta o b antes do curinga.

*/