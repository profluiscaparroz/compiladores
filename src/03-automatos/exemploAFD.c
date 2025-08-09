/**
 * Implementação de Autômatos Finitos Determinísticos (AFD) em C
 * 
 * Este programa demonstra a implementação prática de AFDs para reconhecimento
 * de padrões em strings. Inclui exemplos de:
 * 
 * 1. AFD que reconhece números binários divisíveis por 3
 * 2. AFD que reconhece identificadores válidos em C
 * 3. AFD que reconhece strings que terminam com "ab"
 * 4. AFD que reconhece comentários C++ (começam com //)
 * 
 * Cada autômato é implementado usando tabelas de transição e funções
 * de simulação que processam strings caractere por caractere.
 * 
 * Autor: Disciplina de Compiladores
 * Data: 2024
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STATES 10
#define MAX_ALPHABET 256
#define MAX_STRING 1000

// Estrutura para representar um AFD
typedef struct {
    int num_states;                              // Número de estados
    int alphabet_size;                           // Tamanho do alfabeto
    int transition[MAX_STATES][MAX_ALPHABET];    // Tabela de transição
    int initial_state;                           // Estado inicial
    int accepting_states[MAX_STATES];            // Estados de aceitação (1=aceita, 0=rejeita)
    char description[100];                       // Descrição do autômato
} DFA;

/**
 * Inicializa um AFD com valores padrão
 */
void init_dfa(DFA* dfa, const char* description) {
    dfa->num_states = 0;
    dfa->alphabet_size = 0;
    dfa->initial_state = 0;
    strcpy(dfa->description, description);
    
    // Inicializa tabela de transição com -1 (estado inválido)
    for (int i = 0; i < MAX_STATES; i++) {
        dfa->accepting_states[i] = 0;
        for (int j = 0; j < MAX_ALPHABET; j++) {
            dfa->transition[i][j] = -1;
        }
    }
}

/**
 * Adiciona uma transição ao AFD
 */
void add_transition(DFA* dfa, int from_state, char symbol, int to_state) {
    if (from_state < MAX_STATES && to_state < MAX_STATES) {
        dfa->transition[from_state][(unsigned char)symbol] = to_state;
    }
}

/**
 * Define um estado como estado de aceitação
 */
void set_accepting_state(DFA* dfa, int state) {
    if (state < MAX_STATES) {
        dfa->accepting_states[state] = 1;
    }
}

/**
 * Simula a execução do AFD para uma string de entrada
 */
int simulate_dfa(DFA* dfa, const char* input) {
    int current_state = dfa->initial_state;
    int length = strlen(input);
    
    printf("  Simulação do AFD: %s\n", dfa->description);
    printf("  String de entrada: \"%s\"\n", input);
    printf("  Sequência de estados: %d", current_state);
    
    for (int i = 0; i < length; i++) {
        char symbol = input[i];
        int next_state = dfa->transition[current_state][(unsigned char)symbol];
        
        if (next_state == -1) {
            printf(" -> ERRO (transição inválida para '%c')\n", symbol);
            return 0; // Rejeita
        }
        
        current_state = next_state;
        printf(" -> %d", current_state);
    }
    
    int accepted = dfa->accepting_states[current_state];
    printf("\n  Estado final: %d (%s)\n", 
           current_state, 
           accepted ? "ACEITA" : "REJEITA");
    
    return accepted;
}

/**
 * Cria AFD que reconhece números binários divisíveis por 3
 * 
 * Estados:
 * 0: resto 0 (divisível por 3) - estado de aceitação
 * 1: resto 1
 * 2: resto 2
 */
DFA create_divisible_by_3_dfa() {
    DFA dfa;
    init_dfa(&dfa, "Números binários divisíveis por 3");
    
    dfa.num_states = 3;
    dfa.initial_state = 0;
    
    // Estado 0 é de aceitação (resto 0)
    set_accepting_state(&dfa, 0);
    
    // Transições baseadas no resto da divisão por 3
    // Quando lemos '0': novo_resto = (resto_atual * 2) % 3
    // Quando lemos '1': novo_resto = (resto_atual * 2 + 1) % 3
    
    add_transition(&dfa, 0, '0', 0);  // 0*2 % 3 = 0
    add_transition(&dfa, 0, '1', 1);  // 0*2+1 % 3 = 1
    add_transition(&dfa, 1, '0', 2);  // 1*2 % 3 = 2  
    add_transition(&dfa, 1, '1', 0);  // 1*2+1 % 3 = 0
    add_transition(&dfa, 2, '0', 1);  // 2*2 % 3 = 1
    add_transition(&dfa, 2, '1', 2);  // 2*2+1 % 3 = 2
    
    return dfa;
}

/**
 * Cria AFD que reconhece identificadores válidos em C
 * 
 * Estados:
 * 0: estado inicial
 * 1: identificador válido (aceita) - primeiro char é letra ou _
 * 2: estado de erro (rejeita)
 */
DFA create_identifier_dfa() {
    DFA dfa;
    init_dfa(&dfa, "Identificadores válidos em C");
    
    dfa.num_states = 3;
    dfa.initial_state = 0;
    
    // Estado 1 é de aceitação
    set_accepting_state(&dfa, 1);
    
    // Transições do estado inicial (0)
    for (int c = 'a'; c <= 'z'; c++) add_transition(&dfa, 0, c, 1);
    for (int c = 'A'; c <= 'Z'; c++) add_transition(&dfa, 0, c, 1);
    add_transition(&dfa, 0, '_', 1);
    
    // Dígitos no início levam ao estado de erro
    for (int c = '0'; c <= '9'; c++) add_transition(&dfa, 0, c, 2);
    
    // Transições do estado de aceitação (1)
    for (int c = 'a'; c <= 'z'; c++) add_transition(&dfa, 1, c, 1);
    for (int c = 'A'; c <= 'Z'; c++) add_transition(&dfa, 1, c, 1);
    for (int c = '0'; c <= '9'; c++) add_transition(&dfa, 1, c, 1);
    add_transition(&dfa, 1, '_', 1);
    
    // Estado de erro (2) vai para ele mesmo para qualquer entrada
    for (int c = 0; c < 256; c++) {
        if (dfa.transition[2][c] == -1) {
            add_transition(&dfa, 2, c, 2);
        }
    }
    
    return dfa;
}

/**
 * Cria AFD que reconhece strings que terminam com "ab"
 * 
 * Estados:
 * 0: estado inicial
 * 1: último caractere lido foi 'a'
 * 2: string termina com "ab" (aceita)
 */
DFA create_ends_with_ab_dfa() {
    DFA dfa;
    init_dfa(&dfa, "Strings que terminam com 'ab'");
    
    dfa.num_states = 3;
    dfa.initial_state = 0;
    
    // Estado 2 é de aceitação
    set_accepting_state(&dfa, 2);
    
    // Transições do estado 0
    add_transition(&dfa, 0, 'a', 1);
    add_transition(&dfa, 0, 'b', 0);
    
    // Transições do estado 1 (após ler 'a')
    add_transition(&dfa, 1, 'a', 1);  // permanece no estado 1
    add_transition(&dfa, 1, 'b', 2);  // forma "ab"
    
    // Transições do estado 2 (após ler "ab")
    add_transition(&dfa, 2, 'a', 1);  // reinicia a busca por "ab"
    add_transition(&dfa, 2, 'b', 0);  // volta ao início
    
    return dfa;
}

/**
 * Cria AFD que reconhece comentários C++ (//)
 * 
 * Estados:
 * 0: estado inicial
 * 1: após ler primeira '/'
 * 2: comentário válido (aceita) - após ler "//"
 */
DFA create_cpp_comment_dfa() {
    DFA dfa;
    init_dfa(&dfa, "Comentários C++ que começam com '//'");
    
    dfa.num_states = 3;
    dfa.initial_state = 0;
    
    // Estado 2 é de aceitação
    set_accepting_state(&dfa, 2);
    
    // Transições
    add_transition(&dfa, 0, '/', 1);  // primeira barra
    add_transition(&dfa, 1, '/', 2);  // segunda barra - comentário válido
    
    // Qualquer outro caractere no estado 1 volta ao estado 0
    for (int c = 0; c < 256; c++) {
        if (c != '/' && isprint(c)) {
            add_transition(&dfa, 1, c, 0);
        }
    }
    
    // No estado 2, qualquer caractere mantém no estado 2
    for (int c = 0; c < 256; c++) {
        if (isprint(c) || c == ' ' || c == '\t') {
            add_transition(&dfa, 2, c, 2);
        }
    }
    
    return dfa;
}

/**
 * Testa um AFD com múltiplas strings
 */
void test_dfa(DFA* dfa, const char* test_strings[], int num_tests) {
    printf("\n=== TESTANDO AFD: %s ===\n", dfa->description);
    
    for (int i = 0; i < num_tests; i++) {
        printf("\nTeste %d:\n", i + 1);
        int result = simulate_dfa(dfa, test_strings[i]);
        printf("  Resultado: %s\n", result ? "✓ ACEITA" : "✗ REJEITA");
    }
    
    printf("\n==================================================\n");
}

/**
 * Função principal de demonstração
 */
int main() {
    printf("=== DEMONSTRAÇÃO DE AUTÔMATOS FINITOS DETERMINÍSTICOS ===\n");
    printf("Este programa mostra como implementar e usar AFDs para\n");
    printf("reconhecimento de padrões em strings.\n");
    
    // Teste 1: Números binários divisíveis por 3
    {
        DFA dfa = create_divisible_by_3_dfa();
        const char* tests[] = {"0", "11", "110", "1001", "1010", "1111"};
        test_dfa(&dfa, tests, 6);
    }
    
    // Teste 2: Identificadores válidos em C
    {
        DFA dfa = create_identifier_dfa();
        const char* tests[] = {"x", "var1", "_temp", "2invalid", "valid_name", "test123"};
        test_dfa(&dfa, tests, 6);
    }
    
    // Teste 3: Strings que terminam com "ab"
    {
        DFA dfa = create_ends_with_ab_dfa();
        const char* tests[] = {"ab", "aab", "bab", "abb", "abab", "ba"};
        test_dfa(&dfa, tests, 6);
    }
    
    // Teste 4: Comentários C++
    {
        DFA dfa = create_cpp_comment_dfa();
        const char* tests[] = {"//", "// comentario", "/ nao", "/// triplo", "/", "//x"};
        test_dfa(&dfa, tests, 6);
    }
    
    printf("\n=== APLICAÇÕES PRÁTICAS DOS AFDs ===\n");
    printf("• Análise léxica em compiladores\n");
    printf("• Validação de entrada (emails, CPFs, etc.)\n");
    printf("• Protocolos de rede\n");
    printf("• Processamento de texto\n");
    printf("• Controle de sistemas embarcados\n");
    
    return 0;
}

/**
 * Exemplo de uso adicional: Validação de CEP brasileiro
 * Formato: XXXXX-XXX onde X é dígito
 */
DFA create_cep_validator() {
    DFA dfa;
    init_dfa(&dfa, "Validador de CEP brasileiro (XXXXX-XXX)");
    
    dfa.num_states = 10;
    dfa.initial_state = 0;
    set_accepting_state(&dfa, 9); // Estado final após ler 8 dígitos + hífen
    
    // Estados 0-4: primeiros 5 dígitos
    for (int state = 0; state < 5; state++) {
        for (char c = '0'; c <= '9'; c++) {
            add_transition(&dfa, state, c, state + 1);
        }
    }
    
    // Estado 5: deve receber hífen
    add_transition(&dfa, 5, '-', 6);
    
    // Estados 6-8: últimos 3 dígitos
    for (int state = 6; state < 9; state++) {
        for (char c = '0'; c <= '9'; c++) {
            add_transition(&dfa, state, c, state + 1);
        }
    }
    
    return dfa;
}

/**
 * Função utilitária para visualizar a tabela de transição de um AFD
 */
void print_transition_table(DFA* dfa) {
    printf("\nTabela de Transição do AFD: %s\n", dfa->description);
    printf("Estado | Entrada | Próximo Estado\n");
    printf("-------|---------|---------------\n");
    
    for (int state = 0; state < dfa->num_states; state++) {
        for (int symbol = 0; symbol < 256; symbol++) {
            if (dfa->transition[state][symbol] != -1) {
                if (isprint(symbol)) {
                    printf("  %2d   |   '%c'   |      %2d\n", 
                           state, symbol, dfa->transition[state][symbol]);
                } else {
                    printf("  %2d   |  %3d    |      %2d\n", 
                           state, symbol, dfa->transition[state][symbol]);
                }
            }
        }
    }
    
    printf("\nEstados de aceitação: ");
    for (int i = 0; i < dfa->num_states; i++) {
        if (dfa->accepting_states[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
}