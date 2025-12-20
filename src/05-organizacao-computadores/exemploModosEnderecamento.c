/**
 * Exemplo de modos de endereçamento
 * Demonstra como diferentes modos de endereçamento são usados
 * para acessar memória de formas diferentes
 */

#include <stdio.h>
#include <stdlib.h>

// Estrutura para demonstrar acesso a campos
struct Ponto {
    int x;
    int y;
    int z;
};

// Variável global para demonstrar endereçamento direto
int global_var = 42;

int main() {
    printf("=== Demonstração de Modos de Endereçamento ===\n\n");
    
    // 1. Endereçamento Imediato
    printf("1. ENDEREÇAMENTO IMEDIATO\n");
    printf("   Valor constante codificado na instrução\n");
    int valor_imediato;
    __asm__ (
        "movl $100, %%eax;"    // Move constante 100 para EAX
        "movl %%eax, %0;"
        : "=r" (valor_imediato)
        :
        : "%eax"
    );
    printf("   Resultado: %d\n\n", valor_imediato);
    
    // 2. Endereçamento por Registrador
    printf("2. ENDEREÇAMENTO POR REGISTRADOR\n");
    printf("   Operando está em um registrador\n");
    int a = 50, b = 30, soma;
    __asm__ (
        "movl %1, %%eax;"      // EAX = a (registrador)
        "addl %2, %%eax;"      // EAX += b (registrador)
        "movl %%eax, %0;"
        : "=r" (soma)
        : "r" (a), "r" (b)
        : "%eax"
    );
    printf("   %d + %d = %d\n\n", a, b, soma);
    
    // 3. Endereçamento Direto
    printf("3. ENDEREÇAMENTO DIRETO/ABSOLUTO\n");
    printf("   Endereço de memória especificado diretamente\n");
    printf("   Variável global = %d\n", global_var);
    printf("   (Endereço: %p)\n\n", (void*)&global_var);
    
    // 4. Endereçamento Indireto por Registrador
    printf("4. ENDEREÇAMENTO INDIRETO\n");
    printf("   Registrador contém o endereço do operando\n");
    int valor = 77;
    int *ponteiro = &valor;
    int resultado_indireto;
    
    __asm__ (
        "movl (%1), %%eax;"    // EAX = *ponteiro (acesso indireto)
        "movl %%eax, %0;"
        : "=r" (resultado_indireto)
        : "r" (ponteiro)
        : "%eax"
    );
    printf("   Valor original: %d\n", valor);
    printf("   Através do ponteiro: %d\n\n", resultado_indireto);
    
    // 5. Endereçamento Base + Deslocamento
    printf("5. ENDEREÇAMENTO BASE + DESLOCAMENTO\n");
    printf("   Usado para acessar campos de estruturas\n");
    struct Ponto p = {10, 20, 30};
    int coord_y;
    
    __asm__ (
        "movl 4(%1), %%eax;"   // EAX = *(p + 4 bytes) = p.y
        "movl %%eax, %0;"
        : "=r" (coord_y)
        : "r" (&p)
        : "%eax"
    );
    printf("   Estrutura: p.x=%d, p.y=%d, p.z=%d\n", p.x, p.y, p.z);
    printf("   Acesso via [base+4]: %d (p.y)\n", coord_y);
    printf("   Offsets: x=0, y=4, z=8 bytes\n\n");
    
    // 6. Endereçamento Indexado
    printf("6. ENDEREÇAMENTO INDEXADO\n");
    printf("   Usado para acessar arrays\n");
    printf("   Fórmula: endereço = base + índice * escala + deslocamento\n");
    
    int array[5] = {100, 200, 300, 400, 500};
    int indice = 2;
    int elemento;
    
    printf("   Array: [100, 200, 300, 400, 500]\n");
    printf("   Acessando índice %d:\n", indice);
    
    // Em C: array[indice]
    // Em Assembly: [base + indice*4] (4 = sizeof(int))
    // Usando forma mais simples que funciona em diferentes arquiteturas
    elemento = array[indice];  // Deixar compilador fazer por nós para este exemplo
    printf("   array[%d] = %d\n\n", indice, elemento);
    
    // 7. Exemplo Complexo: Matriz 2D
    printf("7. EXEMPLO AVANÇADO: Acesso a Matriz 2D\n");
    printf("   matriz[linha][coluna] = base + (linha*colunas + coluna)*sizeof(elemento)\n");
    
    int matriz[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    
    int linha = 1, coluna = 2;
    int elemento_matriz;
    int offset = linha * 4 + coluna;  // 4 colunas
    
    // Para portabilidade, usamos C direto aqui
    elemento_matriz = matriz[linha][coluna];
    
    printf("   Matriz 3x4:\n");
    for (int i = 0; i < 3; i++) {
        printf("   ");
        for (int j = 0; j < 4; j++) {
            printf("%3d ", matriz[i][j]);
        }
        printf("\n");
    }
    printf("   matriz[%d][%d] = %d\n\n", linha, coluna, elemento_matriz);
    
    // 8. Comparação de Performance
    printf("8. COMPARAÇÃO DE PERFORMANCE\n");
    printf("   Modos mais rápidos para mais lentos:\n");
    printf("   1. Imediato (0 acessos à memória)\n");
    printf("   2. Registrador (~1 ciclo)\n");
    printf("   3. Base+Deslocamento (~3-4 ciclos se em cache)\n");
    printf("   4. Indexado (~3-4 ciclos se em cache)\n");
    printf("   5. Indireto (~3-4 ciclos + latência de load)\n");
    printf("   6. Múltiplos níveis de indireção (muito lento)\n\n");
    
    printf("=== Implicações para Compiladores ===\n");
    printf("• Compiladores escolhem modo de endereçamento baseado no contexto\n");
    printf("• Variáveis locais frequentes → Registradores\n");
    printf("• Acesso a structs → Base + Deslocamento\n");
    printf("• Acesso a arrays → Indexado\n");
    printf("• Ponteiros → Indireto\n");
    printf("• Otimizações podem eliminar acessos à memória\n");
    
    return 0;
}
