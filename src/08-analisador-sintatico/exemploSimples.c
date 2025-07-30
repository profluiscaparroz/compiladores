#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


// Define o tamanho máximo de um lexema (texto associado a um token)
#define MAX_TOKEN_LENGTH 100

// Lista de palavras-chave reconhecidas pelo analisador léxico
const char* keywords[] = {"if", "else", "while", "return"};
const int num_keywords = 4; // Número de palavras-chave

// Enumeração que define os tipos de tokens que podem ser reconhecidos
typedef enum {
    TOKEN_KEYWORD,       // Palavra-chave (ex: if, else)
    TOKEN_IDENTIFIER,    // Identificador (ex: nomes de variáveis ou funções)
    TOKEN_NUMBER,        // Número (ex: 123)
    TOKEN_SYMBOL,        // Símbolo (ex: +, -, *, /, =, ;, etc.)
    TOKEN_UNKNOWN,       // Token desconhecido (caractere não reconhecido)
    TOKEN_EOF,           // Fim do arquivo
    TOKEN_STRING,        // String literal (ex: "texto")
    TOKEN_COMMENT,       // Comentário (ex: // ou /* */)
    TOKEN_CHAR,          // Caractere literal (ex: 'a')
    TOKEN_OPERATOR,      // Operador (ex: ==, <=, >=, etc.)
    TOKEN_PREPROCESSOR   // Diretiva de pré-processador (ex: #include)
} TokenType;

// Estrutura que representa um token identificado
typedef struct {
    TokenType type;               // Tipo do token
    char lexeme[MAX_TOKEN_LENGTH]; // Texto associado ao token
} Token;

// Função que verifica se uma string é uma palavra-chave
int is_keyword(const char* str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) { // Compara a string com cada palavra-chave
            return 1; // Retorna 1 se for uma palavra-chave
        }
    }
    return 0; // Retorna 0 caso contrário
}

// Função que lê o próximo token do arquivo
Token get_next_token(FILE* fp) {
    char ch; // Armazena o caractere atual
    Token token; // Estrutura para armazenar o token identificado
    int i = 0; // Índice para construir o lexema

    // Ignora espaços em branco
    while ((ch = fgetc(fp)) != EOF && isspace(ch));

    // Verifica e ignora comentários
    if (ch == '/') {
        char next = fgetc(fp);
        if (next == '/') { // Comentário de linha
            while ((ch = fgetc(fp)) != EOF && ch != '\n'); // Ignora até o fim da linha
            return get_next_token(fp); // Continua buscando o próximo token
        } else if (next == '*') { // Comentário de bloco
            char prev = 0;
            while ((ch = fgetc(fp)) != EOF) {
                if (prev == '*' && ch == '/') break; // Encontra o final do comentário
                prev = ch;
            }
            return get_next_token(fp); // Continua buscando o próximo token
        } else {
            ungetc(next, fp); // Não era comentário, devolve o caractere
        }
    }

    // Verifica strings literais
    if (ch == '"') {
        token.type = TOKEN_STRING; // Define o tipo como string
        i = 0;
        token.lexeme[i++] = ch; // Adiciona a abertura da string
        while ((ch = fgetc(fp)) != EOF) {
            token.lexeme[i++] = ch;
            if (ch == '\\') { // Trata caracteres escapados
                char esc = fgetc(fp);
                if (esc == EOF) break;
                token.lexeme[i++] = esc;
            } else if (ch == '"') { // Fecha a string
                break;
            }
            if (i >= MAX_TOKEN_LENGTH - 2) { // Verifica se a string é muito longa
                printf("String muito longa!\n");
                exit(1);
            }
        }
        token.lexeme[i] = '\0'; // Finaliza o lexema
        return token;
    }

    // Verifica o fim do arquivo
    if (ch == EOF) {
        token.type = TOKEN_EOF; // Define o tipo como EOF
        strcpy(token.lexeme, "EOF"); // Define o lexema como "EOF"
        return token;
    }

    // Identifica palavras-chave ou identificadores
    if (isalpha(ch) || ch == '_') { // Começa com uma letra ou '_'
        token.lexeme[i++] = ch;
        while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
            token.lexeme[i++] = ch;
        }
        token.lexeme[i] = '\0'; // Finaliza o lexema
        ungetc(ch, fp); // Devolve o último caractere lido
        token.type = is_keyword(token.lexeme) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER; // Verifica se é palavra-chave ou identificador
        return token;
    }

    // Identifica números
    if (isdigit(ch)) { // Começa com um dígito
        token.lexeme[i++] = ch;
        while ((ch = fgetc(fp)) != EOF && isdigit(ch)) {
            token.lexeme[i++] = ch;
        }
        token.lexeme[i] = '\0'; // Finaliza o lexema
        ungetc(ch, fp); // Devolve o último caractere lido
        token.type = TOKEN_NUMBER; // Define o tipo como número
        return token;
    }

    // Identifica símbolos simples
    if (strchr("+-*/=;(){}", ch)) { // Verifica se o caractere é um símbolo
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0'; // Finaliza o lexema
        token.type = TOKEN_SYMBOL; // Define o tipo como símbolo
        return token;
    }

    // Token desconhecido
    token.lexeme[0] = ch;
    token.lexeme[1] = '\0'; // Finaliza o lexema
    token.type = TOKEN_UNKNOWN; // Define o tipo como desconhecido
    return token;
}

// Função que converte o tipo de token para uma string legível
const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_SYMBOL: return "SYMBOL";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

// ====================== PARSER =======================


// Token atual sendo analisado
Token current_token;

// Nível de indentação para rastreamento
int indent_level = 0;

// Função para imprimir a indentação atual
void print_indent() {
    for (int i = 0; i < indent_level; i++) printf("  ");
}

// Função para rastrear a entrada em uma regra
void trace_enter(const char* rule_name) {
    print_indent();
    printf(">> Entrando em %s (token atual: '%s')\n", rule_name, current_token.lexeme);
    indent_level++;
}

// Função para rastrear a saída de uma regra
void trace_exit(const char* rule_name) {
    indent_level--;
    print_indent();
    printf("<< Saindo de %s\n", rule_name);
}

// Avança para o próximo token
void advance(FILE* fp) {
    current_token = get_next_token(fp);
}

// Verifica se o token atual é o esperado
void expect(TokenType type, const char* expected, FILE* fp) {
    if (current_token.type == type && strcmp(current_token.lexeme, expected) == 0) {
        advance(fp); // Avança para o próximo token
    } else {
        printf("Erro de sintaxe: Esperado '%s', encontrado '%s'\n", expected, current_token.lexeme);
        exit(1);
    }
}

// <expression> ::= <term> (('+' | '-') <term>)*
// Declaração da função que analisa expressões. A implementação está abaixo.
void parse_expression(FILE* fp);

// <term> ::= <factor> (('*' | '/') <factor>)*
// Declaração da função que analisa termos. A implementação está abaixo.
void parse_term(FILE* fp);

// <factor> ::= NUMBER | IDENTIFIER | '(' <expression> ')'
// Implementação da função que analisa fatores.
void parse_factor(FILE* fp) {
    trace_enter("parse_factor"); // Rastreia a entrada na regra <factor>.
    if (current_token.type == TOKEN_NUMBER || current_token.type == TOKEN_IDENTIFIER) {
        // Se o token atual for um número ou identificador, consome o fator.
        print_indent(); // Imprime a indentação para rastreamento.
        printf("Consumindo fator: '%s'\n", current_token.lexeme); // Exibe o fator consumido.
        advance(fp); // Avança para o próximo token.
    } else if (strcmp(current_token.lexeme, "(") == 0) {
        // Se o token atual for '(', consome e analisa uma expressão dentro dos parênteses.
        advance(fp); // Avança para o próximo token.
        parse_expression(fp); // Analisa a expressão dentro dos parênteses.
        expect(TOKEN_SYMBOL, ")", fp); // Espera o fechamento do parêntese ')'.
    } else {
        // Caso contrário, ocorre um erro de sintaxe.
        printf("Erro de sintaxe: Fator inválido '%s'\n", current_token.lexeme);
        exit(1); // Encerra o programa com erro.
    }
    trace_exit("parse_factor"); // Rastreia a saída da regra <factor>.
}

// Implementação da função que analisa termos.
// <term> ::= <factor> (('*' | '/') <factor>)*
void parse_term(FILE* fp) {
    trace_enter("parse_term"); // Rastreia a entrada na regra <term>.
    parse_factor(fp); // Analisa o primeiro fator.
    while (strcmp(current_token.lexeme, "*") == 0 || strcmp(current_token.lexeme, "/") == 0) {
        // Enquanto o token atual for '*' ou '/', consome o operador e o próximo fator.
        print_indent(); // Imprime a indentação para rastreamento.
        printf("Operador multiplicativo: '%s'\n", current_token.lexeme); // Exibe o operador consumido.
        advance(fp); // Avança para o próximo token.
        parse_factor(fp); // Analisa o próximo fator.
    }
    trace_exit("parse_term"); // Rastreia a saída da regra <term>.
}

// Implementação da função que analisa expressões.
// <expression> ::= <term> (('+' | '-') <term>)*
void parse_expression(FILE* fp) {
    trace_enter("parse_expression"); // Rastreia a entrada na regra <expression>.
    parse_term(fp); // Analisa o primeiro termo.
    while (strcmp(current_token.lexeme, "+") == 0 || strcmp(current_token.lexeme, "-") == 0) {
        // Enquanto o token atual for '+' ou '-', consome o operador e o próximo termo.
        print_indent(); // Imprime a indentação para rastreamento.
        printf("Operador aditivo: '%s'\n", current_token.lexeme); // Exibe o operador consumido.
        advance(fp); // Avança para o próximo token.
        parse_term(fp); // Analisa o próximo termo.
    }
    trace_exit("parse_expression"); // Rastreia a saída da regra <expression>.
}

// Implementação da função que analisa declarações.
// <statement> ::= "if" '(' <expression> ')' <statement> | <expression> ';'
void parse_statement(FILE* fp) {
    trace_enter("parse_statement"); // Rastreia a entrada na regra <statement>.
    if (current_token.type == TOKEN_KEYWORD && strcmp(current_token.lexeme, "if") == 0) {
        // Se o token atual for a palavra-chave "if", analisa a estrutura condicional.
        print_indent(); // Imprime a indentação para rastreamento.
        printf("Reconhecido: if\n"); // Exibe que a palavra-chave "if" foi reconhecida.
        advance(fp); // Avança para o próximo token.
        expect(TOKEN_SYMBOL, "(", fp); // Espera o parêntese de abertura '('.
        parse_expression(fp); // Analisa a expressão dentro dos parênteses.
        expect(TOKEN_SYMBOL, ")", fp); // Espera o parêntese de fechamento ')'.
        parse_statement(fp); // Analisa a declaração associada ao "if".
    } else {
        // Caso contrário, analisa uma expressão seguida de ';'.
        parse_expression(fp); // Analisa a expressão.
        expect(TOKEN_SYMBOL, ";", fp); // Espera o ponto e vírgula ';'.
    }
    trace_exit("parse_statement"); // Rastreia a saída da regra <statement>.
}

// Implementação da função que analisa o programa.
// <program> ::= <statement>*
void parse_program(FILE* fp) {
    trace_enter("parse_program"); // Rastreia a entrada na regra <program>.
    advance(fp); // Avança para o primeiro token.
    while (current_token.type != TOKEN_EOF) {
        // Enquanto não chegar ao fim do arquivo, analisa declarações.
        parse_statement(fp); // Analisa uma declaração.
    }
    trace_exit("parse_program"); // Rastreia a saída da regra <program>.
}


// ====================== MAIN =======================

// Funções para analisar expressões, termos, fatores, etc.
// (detalhes omitidos para brevidade)

// Função principal
int main() {
    FILE* fp = fopen("entrada.txt", "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    parse_program(fp);

    printf("Análise sintática completa!\n");
    fclose(fp);
    return 0;
}
