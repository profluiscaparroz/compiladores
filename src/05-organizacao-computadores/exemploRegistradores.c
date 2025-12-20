/**
 * Exemplo de uso de registradores em Assembly Inline
 * Demonstra diferentes operações com registradores
 */

#include <stdio.h>

int main() {
    int a = 10, b = 20, c = 0;
    
    printf("=== Exemplo 1: Operações Básicas com Registradores ===\n");
    printf("a = %d, b = %d\n", a, b);
    
    // Exemplo 1: Soma usando registradores
    __asm__ (
        "movl %1, %%eax;"      // EAX = a
        "movl %2, %%ebx;"      // EBX = b
        "addl %%ebx, %%eax;"   // EAX = EAX + EBX
        "movl %%eax, %0;"      // c = EAX
        : "=r" (c)              // output
        : "r" (a), "r" (b)      // inputs
        : "%eax", "%ebx"        // clobbered registers
    );
    printf("Soma: %d + %d = %d\n\n", a, b, c);
    
    // Exemplo 2: Multiplicação e flags
    printf("=== Exemplo 2: Multiplicação ===\n");
    int x = 15, y = 4, produto;
    
    __asm__ (
        "movl %1, %%eax;"
        "imull %2, %%eax;"     // EAX = EAX * y
        "movl %%eax, %0;"
        : "=r" (produto)
        : "r" (x), "r" (y)
        : "%eax"
    );
    printf("%d * %d = %d\n\n", x, y, produto);
    
    // Exemplo 3: Shift (deslocamento de bits)
    printf("=== Exemplo 3: Shift Operations (multiplicação/divisão por potências de 2) ===\n");
    int num = 8, resultado_shl, resultado_shr;
    
    // Multiplicação por 4 usando shift left
    __asm__ (
        "movl %1, %%eax;"
        "shl $2, %%eax;"       // EAX = EAX << 2 (multiplica por 4)
        "movl %%eax, %0;"
        : "=r" (resultado_shl)
        : "r" (num)
        : "%eax"
    );
    printf("%d << 2 = %d (equivalente a %d * 4)\n", num, resultado_shl, num);
    
    // Divisão por 2 usando shift right
    __asm__ (
        "movl %1, %%eax;"
        "shr $1, %%eax;"       // EAX = EAX >> 1 (divide por 2)
        "movl %%eax, %0;"
        : "=r" (resultado_shr)
        : "r" (num)
        : "%eax"
    );
    printf("%d >> 1 = %d (equivalente a %d / 2)\n\n", num, resultado_shr, num);
    
    // Exemplo 4: Operações lógicas
    printf("=== Exemplo 4: Operações Lógicas ===\n");
    int mask = 0xFF, value = 0x12AB, masked;
    
    __asm__ (
        "movl %1, %%eax;"
        "andl %2, %%eax;"      // EAX = EAX & mask
        "movl %%eax, %0;"
        : "=r" (masked)
        : "r" (value), "r" (mask)
        : "%eax"
    );
    printf("0x%X & 0x%X = 0x%X (extrai byte menos significativo)\n\n", value, mask, masked);
    
    // Exemplo 5: Zerando registrador com XOR (idiom comum)
    printf("=== Exemplo 5: Zerando Registrador com XOR ===\n");
    int zero_result;
    
    __asm__ (
        "xorl %%eax, %%eax;"   // EAX = EAX XOR EAX (sempre = 0)
        "movl %%eax, %0;"
        : "=r" (zero_result)
        :
        : "%eax"
    );
    printf("XOR EAX, EAX = %d (forma eficiente de zerar)\n\n", zero_result);
    
    // Exemplo 6: Incremento e Decremento
    printf("=== Exemplo 6: Incremento e Decremento ===\n");
    int counter = 5, inc_result, dec_result;
    
    __asm__ (
        "movl %1, %%eax;"
        "incl %%eax;"          // EAX++
        "movl %%eax, %0;"
        : "=r" (inc_result)
        : "r" (counter)
        : "%eax"
    );
    printf("%d++ = %d\n", counter, inc_result);
    
    __asm__ (
        "movl %1, %%eax;"
        "decl %%eax;"          // EAX--
        "movl %%eax, %0;"
        : "=r" (dec_result)
        : "r" (counter)
        : "%eax"
    );
    printf("%d-- = %d\n\n", counter, dec_result);
    
    printf("=== Observações Importantes ===\n");
    printf("1. Registradores são a memória mais rápida da CPU\n");
    printf("2. Operações com registradores levam ~1 ciclo\n");
    printf("3. x86-64 tem registradores de 64 bits (RAX, RBX, etc)\n");
    printf("4. EAX é a versão de 32 bits de RAX\n");
    printf("5. Compiladores fazem alocação de registradores automaticamente\n");
    
    return 0;
}
