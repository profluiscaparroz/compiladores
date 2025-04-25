#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

// Define os tipos de tokens que o analisador pode identificar
typedef enum {
    TOKEN_KEYWORD,    // Representa palavras-chave, como "int"
    TOKEN_IDENTIFIER, // Representa identificadores, como nomes de variáveis
    TOKEN_NUMBER,     // Representa números inteiros
    TOKEN_SYMBOL,     // Representa símbolos específicos, como '=' ou ';'
    TOKEN_UNKNOWN     // Representa qualquer outro caractere não reconhecido
} TokenType;

// Estrutura que representa um token identificado
typedef struct {
    TokenType type;               // Tipo do token
    char lexeme[MAX_TOKEN_LENGTH]; // Texto associado ao token
} Token;

// Função que verifica se uma string é uma palavra-chave
int is_keyword(const char* str) {
    // Atualmente, só reconhece "int" como palavra-chave
    return strcmp(str, "int") == 0;
}

// Função que imprime um token no formato <TIPO, 'LEXEMA'>
void print_token(Token token) {
    const char* types[] = {"KEYWORD", "IDENTIFIER", "NUMBER", "SYMBOL", "UNKNOWN"};
    printf("<%s, '%s'>\n", types[token.type], token.lexeme);
}

// Função principal que analisa uma linha de código e identifica tokens
void analyze_line(const char* line) {
    const char* ptr = line; // Ponteiro para percorrer a linha
    Token token;           // Estrutura para armazenar o token atual
    int i;                 // Índice auxiliar para construir o lexema

    // Loop principal: percorre a linha até o final
    while (*ptr != '\0') {
        // Ignora espaços em branco
        while (isspace(*ptr)) ptr++;

        // Identifica palavras-chave ou identificadores
        if (isalpha(*ptr)) { // Começa com uma letra
            i = 0;
            // Continua enquanto for letra ou número
            while (isalnum(*ptr)) {
                token.lexeme[i++] = *ptr++;
            }
            token.lexeme[i] = '\0'; // Finaliza a string do lexema
            // Verifica se é uma palavra-chave ou identificador
            token.type = is_keyword(token.lexeme) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            print_token(token); // Imprime o token identificado
        }

        // Identifica números
        else if (isdigit(*ptr)) { // Começa com um dígito
            i = 0;
            // Continua enquanto for dígito
            while (isdigit(*ptr)) {
                token.lexeme[i++] = *ptr++;
            }
            token.lexeme[i] = '\0'; // Finaliza a string do lexema
            token.type = TOKEN_NUMBER; // Define o tipo como número
            print_token(token); // Imprime o token identificado
        }

        // Identifica símbolos específicos
        else if (strchr("=;", *ptr)) { // Verifica se o caractere é '=' ou ';'
            token.lexeme[0] = *ptr++; // Armazena o símbolo
            token.lexeme[1] = '\0';   // Finaliza a string do lexema
            token.type = TOKEN_SYMBOL; // Define o tipo como símbolo
            print_token(token); // Imprime o token identificado
        }

        // Identifica caracteres desconhecidos
        else if (*ptr != '\0') { // Qualquer outro caractere não reconhecido
            token.lexeme[0] = *ptr++; // Armazena o caractere
            token.lexeme[1] = '\0';   // Finaliza a string do lexema
            token.type = TOKEN_UNKNOWN; // Define o tipo como desconhecido
            print_token(token); // Imprime o token identificado
        }
    }
}

int main() {
    // Linha de código a ser analisada
    const char* linha = "int idade = 10;";
    printf("Analisando: %s\n", linha);

    // Chama a função para analisar a linha
    analyze_line(linha);

    int idade = 10;
    printf("Idade: %d\n", idade);

    return 0;
}