/**
 * Exemplo de Calling Convention e Stack Frames
 * Demonstra como funções são chamadas e como a pilha funciona
 */

#include <stdio.h>
#include <stdlib.h>

// Variável global para demonstração
int global_var = 999;

// Função simples que será analisada
int soma_tres(int a, int b, int c) {
    int resultado = a + b + c;
    return resultado;
}

// Função que demonstra variáveis locais
int calcula_area_retangulo(int largura, int altura) {
    int area = largura * altura;
    int perimetro = 2 * (largura + altura);
    return area;  // Ignora perímetro para simplicidade
}

// Função recursiva para demonstrar stack frames
int fatorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * fatorial(n - 1);
}

// Função com inline assembly mostrando convenção de chamada
int soma_com_assembly(int x, int y) {
    int resultado;
    __asm__ (
        // x está em EDI (primeiro argumento)
        // y está em ESI (segundo argumento)
        "movl %%edi, %%eax;"   // EAX = x
        "addl %%esi, %%eax;"   // EAX += y
        "movl %%eax, %0;"      // resultado = EAX
        : "=r" (resultado)
        :
        : "%eax"
    );
    return resultado;
}

void demonstra_stack() {
    printf("=== Demonstração de Stack Frame ===\n\n");
    
    printf("1. CALLING CONVENTION (x86-64 System V ABI)\n");
    printf("   Argumentos passados em registradores (ordem):\n");
    printf("   1º: RDI (EDI para 32-bit)\n");
    printf("   2º: RSI (ESI para 32-bit)\n");
    printf("   3º: RDX (EDX para 32-bit)\n");
    printf("   4º: RCX (ECX para 32-bit)\n");
    printf("   5º: R8\n");
    printf("   6º: R9\n");
    printf("   7º em diante: Stack (pilha)\n");
    printf("   Retorno: RAX (EAX para 32-bit)\n\n");
    
    int resultado1 = soma_tres(10, 20, 30);
    printf("   soma_tres(10, 20, 30) = %d\n", resultado1);
    printf("   (10 em EDI, 20 em ESI, 30 em EDX)\n\n");
    
    printf("2. STACK FRAME LAYOUT\n");
    printf("   Cada chamada de função cria um frame:\n\n");
    printf("   High Addresses\n");
    printf("   ┌──────────────────────┐\n");
    printf("   │   Argumentos 7+      │  (se houver)\n");
    printf("   ├──────────────────────┤\n");
    printf("   │   Return Address     │  ← Onde retornar após função\n");
    printf("   ├──────────────────────┤\n");
    printf("   │   Saved RBP          │  ← Frame pointer anterior\n");
    printf("   ├──────────────────────┤  ← RBP (base do frame atual)\n");
    printf("   │   Variáveis Locais   │\n");
    printf("   ├──────────────────────┤\n");
    printf("   │   Saved Registers    │  (se necessário)\n");
    printf("   ├──────────────────────┤\n");
    printf("   │   Temp Space         │\n");
    printf("   └──────────────────────┘  ← RSP (topo da pilha)\n");
    printf("   Low Addresses\n\n");
    
    printf("3. FUNÇÃO COM VARIÁVEIS LOCAIS\n");
    int area = calcula_area_retangulo(5, 3);
    printf("   calcula_area_retangulo(5, 3) = %d\n", area);
    printf("   Assembly gerado (aproximado):\n");
    printf("   calcula_area_retangulo:\n");
    printf("       push   rbp              ; Salva frame pointer\n");
    printf("       mov    rbp, rsp         ; Setup novo frame\n");
    printf("       sub    rsp, 16          ; Aloca espaço para locais\n");
    printf("       mov    DWORD PTR [rbp-4], edi   ; largura (1º arg)\n");
    printf("       mov    DWORD PTR [rbp-8], esi   ; altura (2º arg)\n");
    printf("       mov    eax, [rbp-4]             ; EAX = largura\n");
    printf("       imul   eax, [rbp-8]             ; EAX *= altura\n");
    printf("       mov    [rbp-12], eax            ; area = EAX\n");
    printf("       mov    eax, [rbp-12]            ; return area\n");
    printf("       leave                           ; Restaura frame\n");
    printf("       ret                             ; Retorna\n\n");
    
    printf("4. RECURSÃO E STACK FRAMES\n");
    int fat = fatorial(5);
    printf("   fatorial(5) = %d\n", fat);
    printf("   Pilha durante execução:\n\n");
    printf("   fatorial(5)  ┌─────────────┐\n");
    printf("                │  n=5        │\n");
    printf("                │  ret addr   │\n");
    printf("                ├─────────────┤\n");
    printf("   fatorial(4)  │  n=4        │\n");
    printf("                │  ret addr   │\n");
    printf("                ├─────────────┤\n");
    printf("   fatorial(3)  │  n=3        │\n");
    printf("                │  ret addr   │\n");
    printf("                ├─────────────┤\n");
    printf("   fatorial(2)  │  n=2        │\n");
    printf("                │  ret addr   │\n");
    printf("                ├─────────────┤\n");
    printf("   fatorial(1)  │  n=1        │\n");
    printf("                │  ret addr   │ ← RSP aqui no ponto mais fundo\n");
    printf("                └─────────────┘\n\n");
    printf("   Cada chamada empilha um novo frame\n");
    printf("   Retorno desempilha frames na ordem inversa\n\n");
    
    printf("5. DEMONSTRAÇÃO COM INLINE ASSEMBLY\n");
    int soma_asm = soma_com_assembly(15, 25);
    printf("   soma_com_assembly(15, 25) = %d\n", soma_asm);
    printf("   Função recebe argumentos diretamente em EDI e ESI\n\n");
    
    printf("6. REGISTRADORES PRESERVADOS\n");
    printf("   Callee-saved (função deve preservar):\n");
    printf("   • RBX, RBP, R12-R15\n");
    printf("   • Função deve salvar no início e restaurar no fim\n\n");
    printf("   Caller-saved (chamador deve preservar se necessário):\n");
    printf("   • RAX, RCX, RDX, RSI, RDI, R8-R11\n");
    printf("   • Podem ser destruídos pela função chamada\n\n");
    
    printf("7. EXEMPLO DE PRESERVAÇÃO DE REGISTRADORES\n");
    printf("   função_que_usa_rbx:\n");
    printf("       push   rbx              ; Salva RBX\n");
    printf("       mov    rbx, rdi         ; Usa RBX\n");
    printf("       ; ... código ...\n");
    printf("       pop    rbx              ; Restaura RBX\n");
    printf("       ret\n\n");
}

// Função que mostra layout de memória
void mostra_layout_memoria() {
    printf("=== Layout de Memória do Processo ===\n\n");
    
    static int var_estatica = 100;
    int var_local = 200;
    int *heap_var = (int*)malloc(sizeof(int));
    *heap_var = 300;
    
    printf("Endereços de memória (ponteiros):\n\n");
    printf("TEXT (código):        ~0x400000 (baixo)\n");
    printf("DATA (globais):       ~0x600000\n");
    printf("  global_var:         %p\n", (void*)&global_var);
    printf("BSS (não-inicializ):  ~0x600000\n");
    printf("  var_estatica:       %p\n", (void*)&var_estatica);
    printf("HEAP (malloc):        ~0x1000000 (cresce ↑)\n");
    printf("  heap_var:           %p\n", (void*)heap_var);
    printf("  *heap_var = %d\n", *heap_var);
    printf("STACK (locais):       ~0x7fff... (alto, cresce ↓)\n");
    printf("  var_local:          %p\n", (void*)&var_local);
    printf("  var_local = %d\n\n", var_local);
    
    printf("Observações:\n");
    printf("• Endereços podem variar devido a ASLR (Address Space Layout Randomization)\n");
    printf("• Stack e Heap crescem em direções opostas\n");
    printf("• Stack overflow ocorre quando Stack cresce demais\n");
    printf("• Cada thread tem sua própria stack\n");
    
    free(heap_var);
}

int main() {
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║  CALLING CONVENTIONS E STACK FRAMES - DEMONSTRAÇÃO   ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n\n");
    
    demonstra_stack();
    
    printf("\n");
    mostra_layout_memoria();
    
    printf("\n=== RESUMO PARA COMPILADORES ===\n");
    printf("1. Compilador precisa seguir calling convention da plataforma\n");
    printf("2. Deve gerar prólogo/epílogo corretos para cada função\n");
    printf("3. Alocar espaço correto para variáveis locais\n");
    printf("4. Preservar registradores callee-saved\n");
    printf("5. Alinhar stack (16 bytes em x86-64 antes de CALL)\n");
    printf("6. Gerenciar red-zone (128 bytes abaixo de RSP não podem ser sobrescritos)\n");
    printf("7. Otimizar: omitir frame pointer quando possível (-fomit-frame-pointer)\n");
    
    return 0;
}
