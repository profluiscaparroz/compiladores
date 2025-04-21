#include <stdio.h>

int main() {
    int a = 5;
    int b = 7;
    int resultado;

    // Assembly Inline
    __asm__ (
        "movl %1, %%eax;"      // Move 'a' para EAX (registrador A)
        "addl %2, %%eax;"      // Soma 'b' com EAX (registrador B)
        "movl %%eax, %0;"      // Move o resultado para 'resultado'
        : "=r" (resultado)     // saída: %0
        : "r" (a), "r" (b)      // entradas: %1 = a, %2 = b
        : "%eax"               // registrador modificado
    );

    printf("Resultado da soma: %d\n", resultado);
    return 0;
}
