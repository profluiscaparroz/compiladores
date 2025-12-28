/*
 * Exemplos Básicos para Fundamentos de Compiladores
 * 
 * Este arquivo contém exemplos práticos dos conceitos fundamentais
 * apresentados no README.md
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ============================================================================
// 1. EXEMPLOS DE FUNÇÕES BÁSICAS
// ============================================================================

// Função simples que soma dois números
int somar(int a, int b) {
    return a + b;
}

// Função que calcula o fatorial (exemplo de recursão)
int fatorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * fatorial(n - 1);
}

// Função que verifica se um número é primo
int eh_primo(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

// ============================================================================
// 2. EXEMPLOS DE MANIPULAÇÃO DE ARRAYS
// ============================================================================

// Imprime um array
void imprimir_array(int arr[], int tamanho) {
    printf("[");
    for (int i = 0; i < tamanho; i++) {
        printf("%d", arr[i]);
        if (i < tamanho - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// Encontra o maior elemento em um array
int encontrar_maximo(int arr[], int tamanho) {
    if (tamanho <= 0) {
        return -1; // Erro
    }
    
    int max = arr[0];
    for (int i = 1; i < tamanho; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Inverte um array in-place
void inverter_array(int arr[], int tamanho) {
    for (int i = 0; i < tamanho / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[tamanho - 1 - i];
        arr[tamanho - 1 - i] = temp;
    }
}

// ============================================================================
// 3. EXEMPLOS DE MANIPULAÇÃO DE STRINGS
// ============================================================================

// Conta quantas vezes um caractere aparece em uma string
int contar_caractere(const char *str, char c) {
    int contador = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == c) {
            contador++;
        }
    }
    return contador;
}

// Converte string para maiúsculas
void para_maiuscula(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32; // Diferença entre 'a' e 'A' na tabela ASCII
        }
    }
}

// Verifica se uma string é palíndromo
int eh_palindromo(const char *str) {
    int inicio = 0;
    int fim = strlen(str) - 1;
    
    while (inicio < fim) {
        if (str[inicio] != str[fim]) {
            return 0; // Não é palíndromo
        }
        inicio++;
        fim--;
    }
    return 1; // É palíndromo
}

// ============================================================================
// 4. EXEMPLOS DE ALGORITMOS DE BUSCA
// ============================================================================

// Busca linear (funciona em array não ordenado)
int busca_linear(int arr[], int tamanho, int alvo) {
    for (int i = 0; i < tamanho; i++) {
        if (arr[i] == alvo) {
            return i; // Retorna o índice
        }
    }
    return -1; // Não encontrado
}

// Busca binária (requer array ordenado)
int busca_binaria(int arr[], int tamanho, int alvo) {
    int esquerda = 0;
    int direita = tamanho - 1;
    
    while (esquerda <= direita) {
        // Usamos esta forma em vez de (esquerda + direita) / 2 para evitar
        // possível overflow quando os índices são grandes, embora a lógica
        // seja equivalente ao exemplo do README (esq + dir) / 2.
        int meio = esquerda + (direita - esquerda) / 2;
        
        if (arr[meio] == alvo) {
            return meio; // Encontrado
        }
        
        if (arr[meio] < alvo) {
            esquerda = meio + 1; // Buscar na metade direita
        } else {
            direita = meio - 1; // Buscar na metade esquerda
        }
    }
    
    return -1; // Não encontrado
}

// ============================================================================
// 5. EXEMPLOS DE ALGORITMOS DE ORDENAÇÃO
// ============================================================================

// Bubble Sort - O(n²)
void bubble_sort(int arr[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Trocar elementos
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Selection Sort - O(n²)
void selection_sort(int arr[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        int min_idx = i;
        
        // Encontrar o menor elemento
        for (int j = i + 1; j < tamanho; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        
        // Trocar com a posição atual
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

// ============================================================================
// 6. EXEMPLOS DE USO DE PONTEIROS
// ============================================================================

// Troca valores usando ponteiros
void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Aloca e inicializa um array dinamicamente
int* criar_array(int tamanho, int valor_inicial) {
    int *arr = (int*) malloc(tamanho * sizeof(int));
    
    if (arr == NULL) {
        return NULL; // Erro de alocação
    }
    
    for (int i = 0; i < tamanho; i++) {
        arr[i] = valor_inicial;
    }
    
    return arr;
}

// ============================================================================
// 7. EXEMPLOS DE ESTRUTURAS
// ============================================================================

// Estrutura representando um ponto 2D
struct Ponto {
    float x;
    float y;
};

// Calcula distância entre dois pontos
float distancia(struct Ponto p1, struct Ponto p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

// Estrutura representando um estudante
struct Estudante {
    char nome[50];
    int matricula;
    float notas[3];
    float media;
};

// Calcula média das notas de um estudante
void calcular_media(struct Estudante *est) {
    float soma = 0;
    for (int i = 0; i < 3; i++) {
        soma += est->notas[i];
    }
    est->media = soma / 3.0;
}

// ============================================================================
// 8. FUNÇÃO PRINCIPAL COM DEMONSTRAÇÕES
// ============================================================================

int main() {
    printf("=== EXEMPLOS BÁSICOS DE C ===\n\n");
    
    // 1. Testando funções matemáticas
    printf("1. FUNÇÕES MATEMÁTICAS\n");
    printf("   Soma de 5 + 3 = %d\n", somar(5, 3));
    printf("   Fatorial de 5 = %d\n", fatorial(5));
    printf("   7 é primo? %s\n", eh_primo(7) ? "Sim" : "Não");
    printf("   10 é primo? %s\n\n", eh_primo(10) ? "Sim" : "Não");
    
    // 2. Testando manipulação de arrays
    printf("2. MANIPULAÇÃO DE ARRAYS\n");
    int numeros[] = {5, 2, 8, 1, 9, 3};
    int tamanho = sizeof(numeros) / sizeof(numeros[0]);
    
    printf("   Array original: ");
    imprimir_array(numeros, tamanho);
    
    printf("   Maior elemento: %d\n", encontrar_maximo(numeros, tamanho));
    
    inverter_array(numeros, tamanho);
    printf("   Array invertido: ");
    imprimir_array(numeros, tamanho);
    printf("\n");
    
    // 3. Testando manipulação de strings
    printf("3. MANIPULAÇÃO DE STRINGS\n");
    char texto[] = "programacao";
    printf("   String original: %s\n", texto);
    printf("   Quantidade de 'a': %d\n", contar_caractere(texto, 'a'));
    
    para_maiuscula(texto);
    printf("   Em maiúsculas: %s\n", texto);
    
    char palavra[] = "radar";
    printf("   '%s' é palíndromo? %s\n\n", palavra, 
           eh_palindromo(palavra) ? "Sim" : "Não");
    
    // 4. Testando algoritmos de busca
    printf("4. ALGORITMOS DE BUSCA\n");
    int arr_busca[] = {10, 23, 45, 70, 92, 101};
    int tam_busca = sizeof(arr_busca) / sizeof(arr_busca[0]);
    
    printf("   Array: ");
    imprimir_array(arr_busca, tam_busca);
    
    int alvo = 70;
    int pos = busca_linear(arr_busca, tam_busca, alvo);
    printf("   Busca linear de %d: posição %d\n", alvo, pos);
    
    pos = busca_binaria(arr_busca, tam_busca, alvo);
    printf("   Busca binária de %d: posição %d\n\n", alvo, pos);
    
    // 5. Testando ordenação
    printf("5. ALGORITMOS DE ORDENAÇÃO\n");
    int arr_sort[] = {64, 34, 25, 12, 22, 11, 90};
    int tam_sort = sizeof(arr_sort) / sizeof(arr_sort[0]);
    
    printf("   Array desordenado: ");
    imprimir_array(arr_sort, tam_sort);
    
    bubble_sort(arr_sort, tam_sort);
    printf("   Array ordenado (bubble sort): ");
    imprimir_array(arr_sort, tam_sort);
    printf("\n");
    
    // 6. Testando ponteiros
    printf("6. USO DE PONTEIROS\n");
    int x = 10, y = 20;
    printf("   Antes da troca: x = %d, y = %d\n", x, y);
    trocar(&x, &y);
    printf("   Depois da troca: x = %d, y = %d\n\n", x, y);
    
    // 7. Testando alocação dinâmica
    printf("7. ALOCAÇÃO DINÂMICA\n");
    int *arr_dinamico = criar_array(5, 100);
    if (arr_dinamico != NULL) {
        printf("   Array dinâmico criado: ");
        imprimir_array(arr_dinamico, 5);
        free(arr_dinamico); // SEMPRE liberar memória alocada!
        printf("   Memória liberada com sucesso\n\n");
    }
    
    // 8. Testando estruturas
    printf("8. ESTRUTURAS\n");
    struct Estudante aluno;
    strncpy(aluno.nome, "João Silva", sizeof(aluno.nome) - 1);
    aluno.nome[sizeof(aluno.nome) - 1] = '\0';
    aluno.matricula = 12345;
    aluno.notas[0] = 8.5;
    aluno.notas[1] = 7.0;
    aluno.notas[2] = 9.0;
    
    calcular_media(&aluno);
    
    printf("   Nome: %s\n", aluno.nome);
    printf("   Matrícula: %d\n", aluno.matricula);
    printf("   Notas: %.1f, %.1f, %.1f\n", 
           aluno.notas[0], aluno.notas[1], aluno.notas[2]);
    printf("   Média: %.2f\n\n", aluno.media);
    
    printf("=== FIM DOS EXEMPLOS ===\n");
    
    return 0;
}
