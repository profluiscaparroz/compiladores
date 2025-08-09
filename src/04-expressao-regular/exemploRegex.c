/**
 * Implementação Básica de Expressões Regulares em C
 * 
 * Este programa demonstra os conceitos fundamentais de expressões regulares
 * através de uma implementação simples que suporta:
 * 
 * - Correspondência literal de caracteres
 * - Metacaractere '.' (qualquer caractere)
 * - Quantificador '*' (zero ou mais ocorrências)
 * - Âncora '^' (início da string)
 * - Âncora '$' (fim da string)
 * - Classes de caracteres '[abc]'
 * - Negação de classes '[^abc]'
 * 
 * O algoritmo utiliza recursão e backtracking para encontrar correspondências,
 * demonstrando como funcionam internamente os engines de regex.
 * 
 * Autor: Disciplina de Compiladores
 * Data: 2024
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PATTERN 100
#define MAX_TEXT 1000

/**
 * Estrutura para representar uma expressão regular compilada
 */
typedef struct {
    char pattern[MAX_PATTERN];
    int length;
    char description[200];
} Regex;

// Forward declarations
int match_here(const char* pattern, const char* text);
int match_star(char c, const char* pattern, const char* text);

/**
 * Verifica se um caractere está em uma classe de caracteres [abc]
 */
int match_char_class(const char* pattern, char c) {
    int negated = 0;
    
    // Verifica se é negação [^abc]
    if (*pattern == '^') {
        negated = 1;
        pattern++;
    }
    
    int found = 0;
    while (*pattern && *pattern != ']') {
        if (*pattern == c) {
            found = 1;
            break;
        }
        pattern++;
    }
    
    return negated ? !found : found;
}

/**
 * Verifica se um único caractere corresponde ao padrão
 */
int match_char(const char* pattern, char c) {
    if (*pattern == '.') {
        return c != '\0';  // . corresponde a qualquer caractere exceto fim da string
    }
    
    if (*pattern == '[') {
        return match_char_class(pattern + 1, c);
    }
    
    return *pattern == c;
}

/**
 * Implementação recursiva do matching de regex
 */
int match_here(const char* pattern, const char* text) {
    // Caso base: padrão vazio
    if (*pattern == '\0') {
        return 1;
    }
    
    // Verifica âncora de fim de string
    if (*pattern == '$' && *(pattern + 1) == '\0') {
        return *text == '\0';
    }
    
    // Verifica quantificador * (zero ou mais)
    if (*(pattern + 1) == '*') {
        return match_star(*pattern, pattern + 2, text);
    }
    
    // Pula classe de caracteres para encontrar o próximo ']'
    if (*pattern == '[') {
        const char* end = pattern + 1;
        while (*end && *end != ']') end++;
        if (*end == ']') {
            if (*text != '\0' && match_char(pattern, *text)) {
                return match_here(end + 1, text + 1);
            }
            return 0;
        }
    }
    
    // Correspondência normal de caractere
    if (*text != '\0' && match_char(pattern, *text)) {
        return match_here(pattern + 1, text + 1);
    }
    
    return 0;
}

/**
 * Implementa o quantificador * (zero ou mais ocorrências)
 */
int match_star(char c, const char* pattern, const char* text) {
    // Tenta correspondência com zero ocorrências
    if (match_here(pattern, text)) {
        return 1;
    }
    
    // Tenta correspondência com uma ou mais ocorrências
    while (*text != '\0' && match_char(&c, *text)) {
        text++;
        if (match_here(pattern, text)) {
            return 1;
        }
    }
    
    return 0;
}

/**
 * Função principal de matching que trata âncoras
 */
int regex_match(const char* pattern, const char* text) {
    // Verifica âncora de início
    if (*pattern == '^') {
        return match_here(pattern + 1, text);
    }
    
    // Tenta correspondência em todas as posições do texto
    do {
        if (match_here(pattern, text)) {
            return 1;
        }
    } while (*text++ != '\0');
    
    return 0;
}

/**
 * Cria um objeto regex com padrão e descrição
 */
Regex create_regex(const char* pattern, const char* description) {
    Regex r;
    strncpy(r.pattern, pattern, MAX_PATTERN - 1);
    r.pattern[MAX_PATTERN - 1] = '\0';
    r.length = strlen(r.pattern);
    strncpy(r.description, description, 199);
    r.description[199] = '\0';
    return r;
}

/**
 * Testa um regex contra múltiplas strings
 */
void test_regex(Regex* regex, const char* test_strings[], int num_tests) {
    printf("\n=== TESTANDO REGEX: %s ===\n", regex->description);
    printf("Padrão: /%s/\n", regex->pattern);
    
    for (int i = 0; i < num_tests; i++) {
        int match = regex_match(regex->pattern, test_strings[i]);
        printf("  %-20s -> %s\n", 
               test_strings[i], 
               match ? "✓ CORRESPONDE" : "✗ NÃO CORRESPONDE");
    }
    
    printf("\n");
}

/**
 * Demonstração de validação de email simples
 */
int validate_simple_email(const char* email) {
    // Padrão simples: letras@letras.letras
    const char* pattern = "^[a-zA-Z][a-zA-Z0-9]*@[a-zA-Z][a-zA-Z0-9]*\\.[a-zA-Z][a-zA-Z]*$";
    
    // Implementação simplificada para demonstração
    const char* at = strchr(email, '@');
    const char* dot = strrchr(email, '.');
    
    if (!at || !dot || at > dot) return 0;
    if (at == email || at == email + strlen(email) - 1) return 0;
    if (dot == email + strlen(email) - 1) return 0;
    
    // Verifica se tem caracteres válidos antes do @
    for (const char* p = email; p < at; p++) {
        if (!isalnum(*p) && *p != '_' && *p != '.') return 0;
    }
    
    // Verifica se tem caracteres válidos após o @
    for (const char* p = at + 1; p < dot; p++) {
        if (!isalnum(*p) && *p != '_') return 0;
    }
    
    // Verifica se tem caracteres válidos após o .
    for (const char* p = dot + 1; *p; p++) {
        if (!isalpha(*p)) return 0;
    }
    
    return 1;
}

/**
 * Demonstração de validação de CPF
 */
int validate_cpf_format(const char* cpf) {
    // Formato: XXX.XXX.XXX-XX
    if (strlen(cpf) != 14) return 0;
    
    // Verifica posições dos separadores
    if (cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-') return 0;
    
    // Verifica se os outros caracteres são dígitos
    for (int i = 0; i < 14; i++) {
        if (i != 3 && i != 7 && i != 11) {
            if (!isdigit(cpf[i])) return 0;
        }
    }
    
    return 1;
}

/**
 * Demonstração de extração de números de um texto
 */
void extract_numbers(const char* text) {
    printf("Extraindo números do texto: \"%s\"\n", text);
    printf("Números encontrados: ");
    
    const char* p = text;
    while (*p) {
        if (isdigit(*p)) {
            // Encontrou o início de um número
            const char* start = p;
            while (isdigit(*p)) p++;
            
            // Imprime o número encontrado
            printf("%.*s ", (int)(p - start), start);
        } else {
            p++;
        }
    }
    printf("\n\n");
}

/**
 * Demonstração de substituição de padrões
 */
void simple_replace(const char* text, char find, char replace) {
    char result[MAX_TEXT];
    int i, j = 0;
    
    printf("Substituindo '%c' por '%c' em: \"%s\"\n", find, replace, text);
    
    for (i = 0; text[i] && j < MAX_TEXT - 1; i++) {
        if (text[i] == find) {
            result[j++] = replace;
        } else {
            result[j++] = text[i];
        }
    }
    result[j] = '\0';
    
    printf("Resultado: \"%s\"\n\n", result);
}

/**
 * Função principal de demonstração
 */
int main() {
    printf("=== DEMONSTRAÇÃO DE EXPRESSÕES REGULARES ===\n");
    printf("Implementação básica de regex em C para fins educacionais\n");
    
    // Teste 1: Correspondência literal
    {
        Regex r = create_regex("hello", "Correspondência literal da palavra 'hello'");
        const char* tests[] = {"hello", "hello world", "say hello", "Help", "Hell", "helloworld"};
        test_regex(&r, tests, 6);
    }
    
    // Teste 2: Metacaractere . (ponto)
    {
        Regex r = create_regex("h.llo", "Palavra 'hello' com qualquer caractere no meio");
        const char* tests[] = {"hello", "hallo", "hxllo", "h1llo", "hllo", "heello"};
        test_regex(&r, tests, 6);
    }
    
    // Teste 3: Quantificador *
    {
        Regex r = create_regex("a*b", "Zero ou mais 'a's seguidos de 'b'");
        const char* tests[] = {"b", "ab", "aab", "aaab", "aaaab", "ba", "bb"};
        test_regex(&r, tests, 7);
    }
    
    // Teste 4: Âncoras ^ e $
    {
        Regex r = create_regex("^hello$", "Linha que contém exatamente 'hello'");
        const char* tests[] = {"hello", "hello world", "say hello", "hello!", "Hello", "HELLO"};
        test_regex(&r, tests, 6);
    }
    
    // Teste 5: Classes de caracteres [abc]
    {
        Regex r = create_regex("[aeiou]", "Contém pelo menos uma vogal");
        const char* tests[] = {"hello", "xyz", "aaa", "bcdfg", "rhythm", "programming"};
        test_regex(&r, tests, 6);
    }
    
    printf("=== APLICAÇÕES PRÁTICAS ===\n\n");
    
    // Demonstração de validação de email
    {
        printf("VALIDAÇÃO DE EMAIL:\n");
        const char* emails[] = {
            "user@domain.com",
            "test.email@example.org", 
            "@invalid.com",
            "no-at-sign.com",
            "user@.com",
            "valid_user@test.net"
        };
        
        for (int i = 0; i < 6; i++) {
            int valid = validate_simple_email(emails[i]);
            printf("  %-25s -> %s\n", 
                   emails[i], 
                   valid ? "✓ VÁLIDO" : "✗ INVÁLIDO");
        }
        printf("\n");
    }
    
    // Demonstração de validação de CPF
    {
        printf("VALIDAÇÃO DE FORMATO CPF:\n");
        const char* cpfs[] = {
            "123.456.789-00",
            "111.222.333-44",
            "12345678900",
            "123.456.789.00",
            "123-456-789-00",
            "abc.def.ghi-jk"
        };
        
        for (int i = 0; i < 6; i++) {
            int valid = validate_cpf_format(cpfs[i]);
            printf("  %-20s -> %s\n", 
                   cpfs[i], 
                   valid ? "✓ FORMATO VÁLIDO" : "✗ FORMATO INVÁLIDO");
        }
        printf("\n");
    }
    
    // Demonstração de extração de dados
    extract_numbers("O preço é R$ 29,90 e a idade é 25 anos. CEP: 12345-678");
    
    // Demonstração de substituição
    simple_replace("hello world", 'l', 'L');
    simple_replace("programming", 'm', 'M');
    
    printf("=== EQUIVALÊNCIA COM AUTÔMATOS FINITOS ===\n");
    printf("Cada expressão regular pode ser convertida em um AFD:\n");
    printf("• Regex 'a*'     -> AFD com 2 estados\n");
    printf("• Regex 'ab'     -> AFD com 3 estados\n");
    printf("• Regex 'a|b'    -> AFD com 2 estados\n");
    printf("• Regex '(ab)*'  -> AFD com 3 estados\n\n");
    
    printf("=== APLICAÇÕES EM COMPILADORES ===\n");
    printf("• Análise léxica: reconhecimento de tokens\n");
    printf("• Validação de identificadores e literais\n");
    printf("• Pré-processamento de código\n");
    printf("• Geração automática de scanners\n");
    printf("• Otimização de buscas em código\n");
    
    return 0;
}

/**
 * Função adicional: converte regex simples para descrição em português
 */
void explain_regex(const char* pattern) {
    printf("Explicação do padrão /%s/:\n", pattern);
    
    const char* p = pattern;
    while (*p) {
        switch (*p) {
            case '^':
                printf("  - Âncora de início (deve começar com)\n");
                break;
            case '$':
                printf("  - Âncora de fim (deve terminar com)\n");
                break;
            case '.':
                printf("  - Ponto (qualquer caractere)\n");
                break;
            case '*':
                printf("  - Asterisco (zero ou mais do anterior)\n");
                break;
            case '[':
                printf("  - Início de classe de caracteres\n");
                while (*p && *p != ']') p++;
                printf("  - Fim de classe de caracteres\n");
                break;
            case '\\':
                printf("  - Escape (próximo caractere é literal)\n");
                if (*(p+1)) p++; // pula o próximo caractere
                break;
            default:
                printf("  - Caractere literal: '%c'\n", *p);
                break;
        }
        p++;
    }
    printf("\n");
}