#include "../include/lexer.h"

// Lê arquivo completo para string
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro: não foi possível abrir o arquivo '%s'\n", filename);
        return NULL;
    }
    
    // Obtém tamanho do arquivo
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Aloca memória e lê o arquivo
    char* content = malloc(size + 1);
    if (!content) {
        fprintf(stderr, "Erro: não foi possível alocar memória\n");
        fclose(file);
        return NULL;
    }
    
    fread(content, 1, size, file);
    content[size] = '\0';
    
    fclose(file);
    return content;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        printf("Exemplo: %s tests/exemplo1.tl\n", argv[0]);
        return 1;
    }
    
    // Lê o arquivo de entrada
    char* input = read_file(argv[1]);
    if (!input) {
        return 1;
    }
    
    printf("Analisando arquivo: %s\n", argv[1]);
    printf("----------------------------------------\n");
    printf("%-15s %-12s %s\n", "TIPO", "LEXEMA", "LINHA:COLUNA");
    printf("----------------------------------------\n");
    
    // Inicializa o lexer
    LexerState lexer;
    init_lexer(&lexer, input);
    
    // Processa todos os tokens
    Token token;
    do {
        token = get_next_token(&lexer);
        if (token.type != TOKEN_EOF) {
            print_token(&token);
        }
    } while (token.type != TOKEN_EOF);
    
    printf("----------------------------------------\n");
    printf("Análise concluída.\n");
    
    free(input);
    return 0;
}