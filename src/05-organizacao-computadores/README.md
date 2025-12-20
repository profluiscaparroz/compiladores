# 05 — Organização de Computadores no Contexto de Compiladores

## 📚 Índice

1. [Introdução e Objetivos](#introdução-e-objetivos)
2. [Fundamentos Teóricos](#fundamentos-teóricos)
3. [Arquitetura de von Neumann](#arquitetura-de-von-neumann)
4. [A CPU e Seus Componentes](#a-cpu-e-seus-componentes)
5. [Registradores](#registradores)
6. [Memória e Hierarquia](#memória-e-hierarquia)
7. [Conjunto de Instruções (ISA)](#conjunto-de-instruções-isa)
8. [Modos de Endereçamento](#modos-de-endereçamento)
9. [Pipeline e Paralelismo](#pipeline-e-paralelismo)
10. [Convenções de Chamada de Função](#convenções-de-chamada-de-função)
11. [Compilação e Geração de Código](#compilação-e-geração-de-código)
12. [Arquiteturas RISC vs CISC](#arquiteturas-risc-vs-cisc)
13. [Exemplos Práticos](#exemplos-práticos)
14. [Exercícios Acadêmicos](#exercícios-acadêmicos)
15. [Perguntas para Pensar](#perguntas-para-pensar)
16. [Referências e Leitura Adicional](#referências-e-leitura-adicional)

---

## Introdução e Objetivos

### 🎯 Objetivos de Aprendizagem

- **Compreender** a ponte fundamental entre compilação e arquitetura de computadores
- **Dominar** conceitos de registradores, memória e conjunto de instruções
- **Relacionar** a geração de código com características específicas da arquitetura alvo
- **Analisar** como decisões de design de arquitetura impactam o trabalho do compilador
- **Implementar** traduções de código de alto nível para assembly
- **Otimizar** código considerando características do hardware

### 🔗 Por Que Organização de Computadores é Essencial para Compiladores?

Um compilador é, essencialmente, um **tradutor** que converte linguagens de alto nível em instruções de máquina. Para fazer isso eficientemente, o compilador precisa:

1. **Conhecer a arquitetura alvo**: Quantos registradores existem? Quais instruções estão disponíveis?
2. **Entender custos**: Algumas operações são mais caras que outras (divisão vs. multiplicação, acesso à memória vs. registrador)
3. **Explorar características**: Pipeline, cache, paralelismo em nível de instrução (ILP)
4. **Respeitar convenções**: ABIs (Application Binary Interfaces), calling conventions

Sem entender a organização do computador, um compilador geraria código **correto mas ineficiente**.

---

## Fundamentos Teóricos

### 🧠 O Modelo Abstrato de Máquina

Historicamente, o conceito de "máquina" na computação tem raízes teóricas profundas:

#### Máquina de Turing (1936)
Alan Turing propôs um modelo abstrato com:
- **Fita infinita** (memória)
- **Cabeçote de leitura/escrita**
- **Conjunto finito de estados**
- **Função de transição**

Este modelo estabelece os **limites teóricos da computação** — o que é computável.

#### Arquitetura de von Neumann (1945)
John von Neumann propôs uma arquitetura prática baseada em:
- **Programa armazenado**: Instruções e dados na mesma memória
- **Processamento sequencial**: Uma instrução por vez (ciclo fetch-decode-execute)
- **Barramentos compartilhados**: Comunicação entre componentes

**Diferença chave**: A Máquina de Turing é um modelo **teórico** (fita infinita), enquanto von Neumann é uma arquitetura **prática** (memória finita, implementável em hardware).

### 🔄 Do Abstrato ao Concreto

```
Máquina de Turing (Abstrata)
        ↓
Arquitetura de von Neumann (Modelo)
        ↓
x86, ARM, RISC-V, MIPS (Implementações)
        ↓
Intel Core i7, AMD Ryzen, Apple M1 (Hardware Real)
```

Um **compilador** precisa traduzir programas para **arquiteturas concretas**, mas usa princípios teóricos para garantir correção.

---

## Arquitetura de von Neumann

### 📐 Componentes Principais

A arquitetura de von Neumann consiste em 5 componentes fundamentais:

```
┌──────────────────────────────────────────────────────┐
│                                                      │
│  ┌──────────┐      ┌───────────────────┐           │
│  │  INPUT   │─────▶│                   │           │
│  │ DEVICES  │      │                   │           │
│  └──────────┘      │      MEMÓRIA      │           │
│                    │     PRINCIPAL     │           │
│  ┌──────────┐      │    (RAM/ROM)      │           │
│  │  OUTPUT  │◀─────│                   │           │
│  │ DEVICES  │      │                   │           │
│  └──────────┘      └─────────┬─────────┘           │
│                              │                      │
│                    ┌─────────▼─────────┐            │
│                    │                   │            │
│                    │     UNIDADE       │            │
│                    │    DE CONTROLE    │            │
│                    │       (UC)        │            │
│                    │                   │            │
│                    └─────────┬─────────┘            │
│                              │                      │
│                    ┌─────────▼─────────┐            │
│                    │                   │            │
│                    │     UNIDADE       │            │
│                    │   LÓGICA E        │            │
│                    │   ARITMÉTICA      │            │
│                    │      (ULA)        │            │
│                    │                   │            │
│                    └───────────────────┘            │
│                                                      │
│                     CPU                              │
└──────────────────────────────────────────────────────┘
```

#### 1. **Unidade Central de Processamento (CPU)**
O "cérebro" que executa instruções. Composta por:
- **UC (Unidade de Controle)**: Busca, decodifica e coordena execução
- **ULA (Unidade Lógica e Aritmética)**: Executa operações matemáticas e lógicas

#### 2. **Memória Principal**
Armazena:
- **Instruções do programa** (code segment)
- **Dados** (variáveis, constantes)
- **Stack** (chamadas de função, variáveis locais)
- **Heap** (alocação dinâmica)

#### 3. **Dispositivos de Entrada/Saída**
Comunicação com o mundo externo:
- Entrada: teclado, mouse, sensores
- Saída: monitor, impressora
- Ambos: disco, rede

#### 4. **Barramentos**
Canais de comunicação que conectam os componentes:
- **Barramento de Dados**: Transporta dados (8, 16, 32, 64 bits)
- **Barramento de Endereços**: Especifica localizações na memória
- **Barramento de Controle**: Sinais de controle (leitura, escrita, interrupções)

### �� O Ciclo de Instrução (Fetch-Decode-Execute)

Todo processador von Neumann opera em um ciclo fundamental:

```
1. FETCH (Busca)
   ┌─────────────────────────────────┐
   │ PC ──▶ Memória ──▶ IR           │
   │ (Busca instrução apontada por PC)│
   └─────────────────────────────────┘
            ↓
2. DECODE (Decodificação)
   ┌─────────────────────────────────┐
   │ UC interpreta o opcode           │
   │ Identifica operandos             │
   └─────────────────────────────────┘
            ↓
3. EXECUTE (Execução)
   ┌─────────────────────────────────┐
   │ ULA executa a operação           │
   │ Ou UC realiza transferência      │
   └─────────────────────────────────┘
            ↓
4. WRITE-BACK (Armazenamento)
   ┌─────────────────────────────────┐
   │ Resultado ──▶ Registrador/Memória│
   └─────────────────────────────────┘
            ↓
5. UPDATE PC
   ┌─────────────────────────────────┐
   │ PC = PC + tamanho_da_instrução   │
   │ (ou novo endereço em caso de jump)│
   └─────────────────────────────────┘
            ↓
        (Volta para FETCH)
```

**Exemplo Concreto**: Executando `ADD R1, R2, R3` (R1 = R2 + R3)

1. **FETCH**: Busca instrução da memória no endereço apontado por PC
2. **DECODE**: UC identifica: opcode=ADD, dest=R1, src1=R2, src2=R3
3. **EXECUTE**: ULA soma conteúdo de R2 e R3
4. **WRITE-BACK**: Resultado é escrito em R1
5. **UPDATE PC**: PC += 4 (assumindo instruções de 32 bits)

### ⚡ Velocidade e Clock

O **clock** determina a velocidade do ciclo:
- **Frequência**: Medida em Hz (ciclos por segundo)
  - 1 Hz = 1 ciclo/segundo
  - 1 GHz = 1.000.000.000 ciclos/segundo
- **Período**: Tempo de um ciclo = 1/frequência
  - CPU de 3 GHz: período = 1/3.000.000.000 ≈ 0,33 nanosegundos

**Importante**: Uma instrução pode levar **múltiplos ciclos de clock** para executar (especialmente em arquiteturas CISC). Pipeline moderno permite iniciar uma nova instrução a cada ciclo.

---

## A CPU e Seus Componentes

### 🧩 Anatomia Detalhada da CPU

#### 1. Unidade de Controle (UC)

**Responsabilidades**:
- **Buscar** instruções da memória
- **Decodificar** instruções (interpretar opcode e operandos)
- **Gerar sinais de controle** para coordenar outros componentes
- **Gerenciar** o fluxo de execução (branches, jumps)

**Componentes Internos**:
- **Program Counter (PC)**: Registrador que aponta para a próxima instrução
- **Instruction Register (IR)**: Armazena a instrução atual
- **Decodificador**: Circuito lógico que interpreta opcodes
- **Sequenciador**: Gera sinais de controle na ordem correta

#### 2. Unidade Lógica e Aritmética (ULA / ALU)

**Operações Aritméticas**:
- Adição, subtração
- Multiplicação, divisão (em ALUs mais complexas)
- Incremento, decremento

**Operações Lógicas**:
- AND, OR, XOR, NOT
- Deslocamentos (shift left/right)
- Rotações

**Operações de Comparação**:
- Igual, diferente
- Maior, menor, maior ou igual, menor ou igual

**Flags (Registrador de Status)**:
- **Zero (Z)**: Resultado foi zero
- **Carry (C)**: Houve carry-out no bit mais significativo
- **Overflow (V)**: Overflow em aritmética com sinal
- **Negative (N)**: Resultado é negativo
- **Parity (P)**: Paridade do resultado

**Exemplo**: Adição 8 + 7 = 15

```
  00001000  (8)
+ 00000111  (7)
-----------
  00001111  (15)

Flags após operação:
Z = 0 (resultado não é zero)
C = 0 (sem carry)
N = 0 (resultado positivo)
V = 0 (sem overflow)
```

#### 3. Registradores

Memória **ultra-rápida** dentro da CPU. Explicados em detalhes na próxima seção.

### 🔬 Implementação: Microarquitetura

A **arquitetura** define o que o processador faz (ISA visível ao programador).
A **microarquitetura** define como ele faz (implementação interna).

**Exemplo**: x86-64 é uma arquitetura. Intel Core, AMD Ryzen são microarquiteturas diferentes que implementam x86-64.

Diferentes microarquiteturas podem ter:
- Número diferente de estágios de pipeline
- Caches de tamanhos diferentes
- Unidades de execução diferentes (múltiplas ALUs, FPUs)
- Previsores de branch diferentes

---

## Registradores

### 📊 O Que São Registradores?

Registradores são **pequenas memórias** localizadas **dentro da CPU**, com as seguintes características:

| Característica | Registrador | Cache L1 | RAM | Disco |
|----------------|-------------|----------|-----|-------|
| **Tamanho** | ~32-256 bytes | ~32-64 KB | GB | TB |
| **Velocidade** | 1 ciclo | 3-4 ciclos | 100+ ciclos | milhões de ciclos |
| **Localização** | Dentro da CPU | Na CPU | Fora da CPU | Dispositivo I/O |
| **Custo** | Muito alto | Alto | Médio | Baixo |

### 🎯 Tipos de Registradores

#### 1. **Registradores de Propósito Geral (GPR)**

Usados para armazenar dados temporários durante computações.

**Arquitetura x86-64 (64 bits)**:
```
RAX, RBX, RCX, RDX   ← Registradores principais (64 bits)
RSI, RDI              ← Índices para strings
R8-R15                ← Registradores adicionais
```

**Compatibilidade com tamanhos menores**:
```
RAX (64 bits) : |--------------------------------|
EAX (32 bits) :                 |----------------|
 AX (16 bits) :                          |------|
 AH (8 bits)  :                          |----|
 AL (8 bits)  :                              |--|
```

**Arquitetura MIPS (32 registradores)**:
```
$zero ($0)   : Sempre contém 0
$at ($1)     : Reservado para assembler
$v0-$v1      : Valores de retorno
$a0-$a3      : Argumentos de função
$t0-$t9      : Temporários (não preservados)
$s0-$s7      : Salvos (devem ser preservados)
$k0-$k1      : Reservados para kernel
$gp          : Global pointer
$sp          : Stack pointer
$fp          : Frame pointer
$ra          : Return address
```

#### 2. **Registradores Especiais**

**Program Counter (PC) / Instruction Pointer (IP)**:
- Armazena o **endereço da próxima instrução**
- Incrementado automaticamente após cada instrução
- Modificado por instruções de controle (JMP, CALL, RET)

```assembly
; Exemplo de modificação do PC
0x1000: MOV RAX, 42    ; PC = 0x1000
0x1004: ADD RAX, 8     ; PC = 0x1004  
0x1008: JMP 0x2000     ; PC = 0x2000 (salto)
0x2000: SUB RAX, 10    ; PC = 0x2000
```

**Stack Pointer (SP)**:
- Aponta para o **topo da pilha**
- Decrementado em PUSH, incrementado em POP
- Essencial para chamadas de função

```assembly
; Exemplo de operações com pilha
PUSH RAX        ; SP -= 8, [SP] = RAX
PUSH RBX        ; SP -= 8, [SP] = RBX
POP RCX         ; RCX = [SP], SP += 8
```

**Frame Pointer (FP/BP)**:
- Aponta para a **base do frame** da função atual
- Facilita acesso a parâmetros e variáveis locais
- Em x86-64: RBP

**Status Register / Flags**:
- Armazena **condições** resultantes de operações
- Usado por instruções condicionais

#### 3. **Registradores de Ponto Flutuante**

Para operações com números reais:

**x86-64**:
```
XMM0-XMM15  : Registradores SSE (128 bits)
YMM0-YMM15  : Registradores AVX (256 bits)
ZMM0-ZMM31  : Registradores AVX-512 (512 bits)
```

Permitem **SIMD** (Single Instruction, Multiple Data) — uma instrução processa múltiplos dados:
```assembly
; Somar 4 floats de uma vez
ADDPS XMM0, XMM1   ; XMM0[0-3] += XMM1[0-3]
```

### 🎪 Alocação de Registradores no Compilador

Um dos **problemas mais importantes** na geração de código é decidir **quais variáveis** ficam em **quais registradores**.

#### Problema da Coloração de Grafos

1. **Construir grafo de interferência**: Nós são variáveis, arestas conectam variáveis que estão "vivas" simultaneamente
2. **Colorir o grafo**: Atribuir cores (registradores) aos nós tal que nós adjacentes tenham cores diferentes
3. **Spilling**: Se não há registradores suficientes, algumas variáveis vão para memória

**Exemplo Simples**:
```c
int a = 1;
int b = 2;
int c = a + b;  // a e b vivos
int d = c + 5;  // c vivo, a e b mortos
```

Grafo de interferência:
```
a ---- c
|      |
|      |
b -----+
```

Com 2 registradores (R1, R2):
- a → R1
- b → R2
- c → R1 (a já morreu)
- d → R2 (b já morreu)

**Custo de Spilling**:
Se tivéssemos apenas 1 registrador, precisaríamos fazer:
```assembly
; Sem registradores suficientes
MOV R1, 1
MOV [mem_a], R1   ; spill a
MOV R1, 2
MOV [mem_b], R1   ; spill b
MOV R1, [mem_a]   ; reload a
ADD R1, [mem_b]   ; c = a + b
; ... muito mais lento!
```

---

## Memória e Hierarquia

### 🗄️ Por Que Hierarquia de Memória?

**Princípios**:
1. **Localidade Temporal**: Dados usados recentemente serão usados novamente em breve
2. **Localidade Espacial**: Dados próximos a dados usados recentemente serão usados em breve

**Trade-off Fundamental**: 
- Memória rápida é **cara e pequena**
- Memória lenta é **barata e grande**

**Solução**: Hierarquia com múltiplos níveis!

```
┌────────────────────────────────────────────────┐
│          Hierarquia de Memória                 │
│                                                │
│  Registradores  ← Mais rápido, menor, mais caro│
│       ↕         ~1 ciclo                       │
│    Cache L1     ~64 KB, 3-4 ciclos             │
│       ↕                                        │
│    Cache L2     ~256 KB-1 MB, 10-20 ciclos     │
│       ↕                                        │
│    Cache L3     ~8-32 MB, 40-75 ciclos         │
│       ↕                                        │
│      RAM        ~8-64 GB, 100-300 ciclos       │
│       ↕                                        │
│     SSD         ~256 GB-4 TB, milhões ciclos   │
│       ↕                                        │
│     HDD         ~1-10 TB, mais milhões         │
│                ← Mais lento, maior, mais barato│
└────────────────────────────────────────────────┘
```

### 💾 Cache

**Como funciona**:
1. CPU precisa de dado no endereço X
2. Verifica se X está no cache (**cache hit**)
   - Se sim: retorna imediatamente (rápido!)
   - Se não (**cache miss**): busca da memória e carrega no cache (lento)

**Tipos de Cache Miss**:
- **Compulsory miss**: Primeira vez que dado é acessado
- **Capacity miss**: Cache está cheio
- **Conflict miss**: Múltiplos dados mapeiam para mesma posição

**Políticas de Substituição**:
- **LRU** (Least Recently Used): Remove o menos usado recentemente
- **LFU** (Least Frequently Used): Remove o menos frequentemente usado
- **Random**: Remove aleatoriamente

**Mapeamento de Cache**:

**Direct-Mapped**:
```
Endereço de memória → Uma única posição no cache
Simples, mas causa muitos conflict misses
```

**Fully Associative**:
```
Endereço de memória → Qualquer posição no cache
Flexível, mas hardware complexo e caro
```

**Set-Associative** (N-way):
```
Compromisso: Endereço → Um conjunto com N posições
Exemplo: 4-way set-associative é comum
```

### 🎯 Implicações para o Compilador

**Otimizações de Cache**:

1. **Loop Tiling/Blocking**: Dividir loops grandes para caber no cache
```c
// Ruim para cache
for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
        for (k = 0; k < N; k++)
            C[i][j] += A[i][k] * B[k][j];

// Melhor para cache (blocked)
for (ii = 0; ii < N; ii += BLOCK)
    for (jj = 0; jj < N; jj += BLOCK)
        for (kk = 0; kk < N; kk += BLOCK)
            for (i = ii; i < min(ii+BLOCK, N); i++)
                for (j = jj; j < min(jj+BLOCK, N); j++)
                    for (k = kk; k < min(kk+BLOCK, N); k++)
                        C[i][j] += A[i][k] * B[k][j];
```

2. **Alinhamento de Dados**: Alinhar estruturas em múltiplos da linha de cache (64 bytes típico)

3. **Prefetching**: Compiladores podem inserir instruções de prefetch para carregar dados antecipadamente

### 📏 Layout de Memória de um Programa

```
┌─────────────────────────┐ Endereços Altos (0xFFFFFFFF...)
│         KERNEL          │
│  (Espaço do S.O.)       │
├─────────────────────────┤
│         STACK           │ ← Cresce para baixo
│  (variáveis locais,     │   (SP aponta aqui)
│   parâmetros, endereços │
│   de retorno)           │
│           ↓             │
│           ·             │
│           ·             │
│           ↑             │
│         HEAP            │ ← Cresce para cima
│  (malloc, new)          │
├─────────────────────────┤
│         BSS             │
│  (variáveis não         │
│   inicializadas)        │
├─────────────────────────┤
│         DATA            │
│  (variáveis globais     │
│   inicializadas)        │
├─────────────────────────┤
│         TEXT            │
│  (código do programa)   │
└─────────────────────────┘ Endereços Baixos (0x00000000)
```

**Segmentos**:
- **TEXT**: Código executável (read-only)
- **DATA**: Variáveis globais/estáticas inicializadas
- **BSS**: Variáveis globais/estáticas não inicializadas (inicializadas com 0)
- **HEAP**: Alocação dinâmica (malloc/free, new/delete)
- **STACK**: Chamadas de função, variáveis locais

---

## Conjunto de Instruções (ISA)

### 🎮 O Que é ISA?

**ISA (Instruction Set Architecture)** é a interface entre software e hardware. Define:
- **Instruções** disponíveis
- **Tipos de dados** suportados
- **Registradores** visíveis
- **Modos de endereçamento**
- **Formato de instruções**

ISA é o "contrato" que o processador oferece ao software.

### 📋 Categorias de Instruções

#### 1. **Instruções Aritméticas e Lógicas**

```assembly
; x86-64
ADD RAX, RBX        ; RAX = RAX + RBX
SUB RAX, 10         ; RAX = RAX - 10
MUL RCX             ; RAX = RAX * RCX (unsigned)
DIV R8              ; RAX = RAX / R8, RDX = RAX % R8

AND RAX, 0xFF       ; RAX = RAX & 0xFF (máscara)
OR  RAX, RBX        ; RAX = RAX | RBX
XOR RAX, RAX        ; RAX = 0 (idiom comum para zerar)
NOT RAX             ; RAX = ~RAX

SHL RAX, 2          ; RAX = RAX << 2 (multiplica por 4)
SHR RAX, 1          ; RAX = RAX >> 1 (divide por 2)
```

```assembly
; MIPS
add  $t0, $t1, $t2  # $t0 = $t1 + $t2
sub  $t0, $t1, $t2  # $t0 = $t1 - $t2
addi $t0, $t1, 100  # $t0 = $t1 + 100 (imediato)
mult $t1, $t2       # Hi:Lo = $t1 * $t2
div  $t1, $t2       # Lo = $t1/$t2, Hi = $t1%$t2

and  $t0, $t1, $t2  # $t0 = $t1 & $t2
or   $t0, $t1, $t2  # $t0 = $t1 | $t2
sll  $t0, $t1, 3    # $t0 = $t1 << 3
```

#### 2. **Instruções de Transferência de Dados**

```assembly
; x86-64
MOV RAX, RBX        ; RAX = RBX
MOV RAX, [RBX]      ; RAX = memória[RBX] (load)
MOV [RAX], RBX      ; memória[RAX] = RBX (store)
MOV RAX, 42         ; RAX = 42 (imediato)

LEA RAX, [RBX+RCX*4+8]  ; RAX = RBX + RCX*4 + 8 (endereço)

PUSH RAX            ; [--SP] = RAX
POP RBX             ; RBX = [SP++]
```

```assembly
; MIPS
lw   $t0, 0($t1)    # $t0 = mem[$t1 + 0]
sw   $t0, 4($t1)    # mem[$t1 + 4] = $t0
lb   $t0, 0($t1)    # Load byte
sb   $t0, 0($t1)    # Store byte
lui  $t0, 0x1234    # Load upper immediate
```

#### 3. **Instruções de Controle de Fluxo**

```assembly
; x86-64
JMP label           ; PC = label (incondicional)
JE  label           ; Jump if Equal (Z=1)
JNE label           ; Jump if Not Equal (Z=0)
JL  label           ; Jump if Less (signed)
JG  label           ; Jump if Greater (signed)

CALL função         ; PUSH PC; JMP função
RET                 ; POP PC

CMP RAX, RBX        ; Compara (RAX - RBX), atualiza flags
TEST RAX, RAX       ; Testa (RAX & RAX), atualiza flags
```

```assembly
; MIPS
j    label          # Jump
jr   $ra            # Jump register (return)
jal  função         # Jump and link (call)

beq  $t0, $t1, label # Branch if equal
bne  $t0, $t1, label # Branch if not equal
blt  $t0, $t1, label # Branch if less than
```

#### 4. **Instruções do Sistema**

```assembly
; x86-64
NOP                 ; No operation
HLT                 ; Halt (para o processador)
INT n               ; Software interrupt
SYSCALL             ; System call
```

### 🔢 Formato de Instruções

**Tipo R (Register)** - Operações entre registradores:
```
┌────────┬────────┬────────┬────────┬────────┬────────┐
│ opcode │   rs   │   rt   │   rd   │  shamt │  funct │
│ 6 bits │ 5 bits │ 5 bits │ 5 bits │ 5 bits │ 6 bits │
└────────┴────────┴────────┴────────┴────────┴────────┘
Exemplo: add $t0, $t1, $t2
```

**Tipo I (Immediate)** - Operações com valor imediato:
```
┌────────┬────────┬────────┬─────────────────┐
│ opcode │   rs   │   rt   │    immediate    │
│ 6 bits │ 5 bits │ 5 bits │     16 bits     │
└────────┴────────┴────────┴─────────────────┘
Exemplo: addi $t0, $t1, 100
```

**Tipo J (Jump)** - Saltos:
```
┌────────┬──────────────────────────────┐
│ opcode │          address             │
│ 6 bits │          26 bits             │
└────────┴──────────────────────────────┘
Exemplo: j label
```

---

## Modos de Endereçamento

### 🎯 Por Que Modos de Endereçamento Importam?

Diferentes formas de **especificar onde os operandos estão** permitem código mais compacto e expressivo.

### 📍 Tipos Principais

#### 1. **Imediato**
O operando é uma **constante** na própria instrução.

```assembly
MOV RAX, 42         ; RAX = 42
ADDI $t0, $t1, 100  ; $t0 = $t1 + 100
```

**Vantagem**: Rápido, não precisa acessar memória
**Desvantagem**: Valor limitado pelo tamanho do campo imediato

#### 2. **Registrador**
O operando está em um **registrador**.

```assembly
MOV RAX, RBX        ; RAX = RBX
ADD $t0, $t1, $t2   ; $t0 = $t1 + $t2
```

**Vantagem**: Muito rápido (1 ciclo)
**Uso**: Variáveis temporárias, acumuladores

#### 3. **Direto/Absoluto**
O endereço de memória está **diretamente na instrução**.

```assembly
MOV RAX, [0x1000]   ; RAX = mem[0x1000]
LW $t0, 0x2000      ; $t0 = mem[0x2000]
```

**Vantagem**: Simples
**Uso**: Variáveis globais, endereços fixos

#### 4. **Indireto por Registrador**
O registrador **contém o endereço** do operando.

```assembly
MOV RAX, [RBX]      ; RAX = mem[RBX]
LW $t0, 0($t1)      ; $t0 = mem[$t1]
```

**Vantagem**: Flexível, permite ponteiros
**Uso**: Ponteiros, acessos indiretos

#### 5. **Base + Deslocamento**
Endereço = Registrador base + deslocamento constante.

```assembly
MOV RAX, [RBX + 8]     ; RAX = mem[RBX + 8]
LW $t0, 12($t1)        ; $t0 = mem[$t1 + 12]
```

**Vantagem**: Acesso a campos de estruturas
**Uso**: 
```c
struct Point { int x, y; };
struct Point *p;
// p->y compilado como: MOV RAX, [RBP + 4]  (assumindo RBP aponta para p)
```

#### 6. **Indexado**
Endereço = Base + Índice × Escala + Deslocamento.

```assembly
; x86-64: [base + index*scale + disp]
MOV RAX, [RBX + RCX*4 + 8]
; RAX = mem[RBX + RCX*4 + 8]
```

**Vantagem**: Ideal para arrays
**Uso**:
```c
int array[100];
int i = 10;
int x = array[i];  // MOV RAX, [array_base + RCX*4]
```

**Tabela comparativa**:

| Modo | Sintaxe | Cálculo do Endereço | Uso Principal |
|------|---------|---------------------|---------------|
| Imediato | `MOV RAX, 42` | N/A (constante) | Literais |
| Registrador | `MOV RAX, RBX` | N/A (registrador) | Temporários |
| Direto | `MOV RAX, [0x1000]` | 0x1000 | Globais |
| Indireto | `MOV RAX, [RBX]` | RBX | Ponteiros |
| Base+Desl | `MOV RAX, [RBX+8]` | RBX + 8 | Structs |
| Indexado | `MOV RAX, [RBX+RCX*4]` | RBX + RCX*4 | Arrays |

### 🎓 Exemplo Acadêmico: Compilando Acesso a Array

```c
int sum = 0;
int arr[10];
for (int i = 0; i < 10; i++) {
    sum += arr[i];
}
```

**Assembly gerado**:
```assembly
; Suponha:
; RBX = endereço base de arr
; RCX = i
; RAX = sum

    XOR RAX, RAX        ; sum = 0
    XOR RCX, RCX        ; i = 0
loop_start:
    CMP RCX, 10
    JGE loop_end
    
    ADD RAX, [RBX + RCX*4]  ; sum += arr[i]
                             ; Usa modo indexado!
    
    INC RCX             ; i++
    JMP loop_start
loop_end:
```

**Por que `RCX*4`?** Porque `int` tem 4 bytes, então `arr[i]` está no endereço `base + i*sizeof(int)`.

---

## Pipeline e Paralelismo

### ⚡ Pipeline de Instruções

**Conceito**: Dividir execução de instruções em **estágios**, permitindo **múltiplas instruções** em diferentes estágios **simultaneamente**.

**Analogia**: Linha de montagem de carros.
- Estação 1: Monta chassi
- Estação 2: Instala motor
- Estação 3: Pinta
- Estação 4: Acabamento

Sem pipeline: 1 carro a cada 4 horas
Com pipeline: 1 carro a cada 1 hora (após o primeiro)

**Pipeline Clássico de 5 Estágios (RISC)**:

```
Estágio 1: IF  (Instruction Fetch)    - Busca instrução
Estágio 2: ID  (Instruction Decode)   - Decodifica, lê registradores
Estágio 3: EX  (Execute)              - Executa na ALU
Estágio 4: MEM (Memory Access)        - Acessa memória (load/store)
Estágio 5: WB  (Write Back)           - Escreve resultado
```

**Exemplo de Execução**:

```
Ciclo: 1    2    3    4    5    6    7    8    9
I1:   IF   ID   EX  MEM   WB
I2:        IF   ID   EX  MEM   WB
I3:             IF   ID   EX  MEM   WB
I4:                  IF   ID   EX  MEM   WB
I5:                       IF   ID   EX  MEM   WB
```

**Sem pipeline**: 5 instruções × 5 ciclos = 25 ciclos
**Com pipeline**: 5 + 4 = 9 ciclos (speedup de ~2.78x)

**Speedup ideal**: N estágios → speedup de Nx (assintoticamente)

### 🚧 Hazards (Perigos) de Pipeline

#### 1. **Hazards Estruturais**
Dois estágios precisam do mesmo recurso ao mesmo tempo.

**Exemplo**: Se IF e MEM usam a mesma memória:
```
I1: IF   ID   EX  MEM   WB
I2:      IF  (conflito! I1 está acessando memória)
```

**Solução**: Cache separado para instruções e dados (arquitetura Harvard)

#### 2. **Hazards de Dados**
Instrução depende de resultado de instrução anterior ainda não concluída.

**Exemplo**:
```assembly
ADD R1, R2, R3    ; R1 = R2 + R3
SUB R4, R1, R5    ; R4 = R1 - R5  (precisa de R1!)
```

```
ADD: IF   ID   EX  MEM   WB
SUB:      IF   ID  (precisa de R1, mas ADD ainda não escreveu!)
```

**Soluções**:
- **Stalling**: Parar pipeline até resultado estar pronto (insere "bolhas")
- **Forwarding/Bypassing**: Passar resultado diretamente de EX para próxima instrução
- **Reordenação por compilador**: Reorganizar instruções independentes

**Compilador pode ajudar**:
```assembly
; Código original (hazard)
ADD R1, R2, R3
SUB R4, R1, R5

; Reordenado pelo compilador (sem hazard)
ADD R1, R2, R3
MUL R6, R7, R8    ; Instrução independente
SUB R4, R1, R5
```

#### 3. **Hazards de Controle (Branch Hazards)**
Não sabemos qual instrução buscar após um branch até ele ser resolvido.

```assembly
BEQ R1, R2, label
ADD R3, R4, R5       ; Buscar esta?
...
label: SUB R6, R7, R8 ; Ou esta?
```

**Soluções**:
- **Stalling**: Esperar resolução (ineficiente)
- **Branch Prediction**: Prever resultado do branch
  - **Estático**: Sempre prevê "não tomado" ou sempre "tomado"
  - **Dinâmico**: Usa histórico (tabelas BTB, PHT)
- **Speculative Execution**: Executa instruções especulativamente, descarta se errado

**Impacto no Compilador**:
- **Branch Delay Slot** (MIPS): Compilador deve preencher slot após branch com instrução útil
```assembly
BEQ R1, R2, label
ADD R3, R4, R5       ; Sempre executada (delay slot)
```

### 🔀 Paralelismo em Nível de Instrução (ILP)

**Superscalar**: Múltiplas instruções por ciclo (múltiplos pipelines).

**Exemplo**: Processador 4-way superscalar pode executar 4 instruções simultâneas:
```
Ciclo 1: I1, I2, I3, I4 (todos em IF)
Ciclo 2: I5, I6, I7, I8 (IF); I1, I2, I3, I4 (ID)
...
```

**VLIW (Very Long Instruction Word)**:
Compilador agrupa instruções paralelas:
```
[ADD R1,R2,R3 | MUL R4,R5,R6 | LOAD R7,0(R8) | STORE R9,0(R10)]
```

**SIMD (Single Instruction, Multiple Data)**:
Uma instrução opera em múltiplos dados:
```assembly
; SSE: Soma 4 floats de uma vez
ADDPS XMM0, XMM1   ; XMM0[0-3] += XMM1[0-3]
```

---

## Convenções de Chamada de Função

### 📞 Calling Conventions

Define **como** funções são chamadas:
- Como **argumentos** são passados?
- Como **valor de retorno** é passado?
- Quais **registradores** devem ser preservados?
- Quem **limpa a pilha**?

### 🎯 x86-64 System V ABI (Linux, macOS)

**Passagem de Argumentos** (inteiros/ponteiros):
1. RDI - primeiro argumento
2. RSI - segundo
3. RDX - terceiro
4. RCX - quarto
5. R8  - quinto
6. R9  - sexto
7+ - Pilha (direita para esquerda)

**Retorno**:
- RAX - valor de retorno

**Registradores Preservados (callee-saved)**:
```
RBX, RBP, R12-R15
Função deve salvar/restaurar se usar
```

**Registradores Não-Preservados (caller-saved)**:
```
RAX, RCX, RDX, RSI, RDI, R8-R11
Chamador deve salvar se precisar manter
```

**Exemplo**:
```c
int foo(int a, int b, int c) {
    return a + b + c;
}

int main() {
    int result = foo(1, 2, 3);
    return 0;
}
```

```assembly
foo:
    ; a em EDI, b em ESI, c em EDX
    MOV EAX, EDI        ; EAX = a
    ADD EAX, ESI        ; EAX += b
    ADD EAX, EDX        ; EAX += c
    RET                 ; retorna EAX

main:
    ; Preparar chamada
    MOV EDI, 1          ; primeiro arg
    MOV ESI, 2          ; segundo arg
    MOV EDX, 3          ; terceiro arg
    CALL foo
    ; Resultado em EAX
    RET
```

### 🏗️ Stack Frame

Cada chamada de função cria um **frame** na pilha:

```
┌─────────────────────┐ ← RSP (topo da pilha) 
│  Variáveis Locais   │
├─────────────────────┤
│  Registradores      │
│  Salvos             │
├─────────────────────┤ ← RBP (base do frame)
│  RBP Anterior       │  (saved frame pointer)
├─────────────────────┤
│  Endereço Retorno   │  (return address)
├─────────────────────┤
│  Argumentos 7+      │  (se houver)
├─────────────────────┤
│     ...             │
│  Frame da Função    │
│  Chamadora          │
└─────────────────────┘
```

**Prólogo da Função** (setup do frame):
```assembly
PUSH RBP            ; Salva frame pointer anterior
MOV RBP, RSP        ; RBP = novo frame base
SUB RSP, N          ; Aloca N bytes para variáveis locais
```

**Epílogo da Função** (teardown):
```assembly
MOV RSP, RBP        ; Restaura SP
POP RBP             ; Restaura frame pointer
RET                 ; Retorna
```

Ou de forma otimizada:
```assembly
LEAVE               ; Equivalente a MOV RSP,RBP; POP RBP
RET
```

### 🎓 Exemplo Completo

```c
int soma_e_multiplica(int x, int y) {
    int temp = x + y;
    return temp * 2;
}
```

```assembly
soma_e_multiplica:
    PUSH RBP                ; Salva frame pointer
    MOV RBP, RSP            ; Setup novo frame
    SUB RSP, 16             ; Aloca espaço (16-byte aligned)
    
    ; x em EDI, y em ESI
    MOV EAX, EDI            ; EAX = x
    ADD EAX, ESI            ; EAX = x + y
    MOV [RBP-4], EAX        ; temp = x + y (variável local)
    
    MOV EAX, [RBP-4]        ; EAX = temp
    ADD EAX, EAX            ; EAX = temp * 2
    
    LEAVE                   ; Cleanup frame
    RET                     ; Retorna EAX
```

---

## Compilação e Geração de Código

### 🔄 Do Código Fonte ao Assembly

**Pipeline de Compilação**:
```
Código Fonte (.c)
      ↓
Preprocessador (cpp)
      ↓
Código C Expandido
      ↓
Compilador (gcc -S)
      ↓
Assembly (.s)
      ↓
Assembler (as)
      ↓
Código Objeto (.o)
      ↓
Linker (ld)
      ↓
Executável
```

### 🎯 Exemplo: Expressões Aritméticas

```c
int x = (a + b) * (c - d);
```

**AST (Abstract Syntax Tree)**:
```
        =
       / \
      x   *
         / \
        +   -
       / \ / \
      a  b c  d
```

**Código de Três Endereços** (IR):
```
t1 = a + b
t2 = c - d
t3 = t1 * t2
x = t3
```

**Assembly (x86-64)**:
```assembly
; Assumindo: a em [RBP-4], b em [RBP-8], c em [RBP-12], d em [RBP-16]
MOV EAX, [RBP-4]        ; EAX = a
ADD EAX, [RBP-8]        ; EAX = a + b
MOV EBX, [RBP-12]       ; EBX = c
SUB EBX, [RBP-16]       ; EBX = c - d
IMUL EAX, EBX           ; EAX = (a+b) * (c-d)
MOV [RBP-20], EAX       ; x = resultado
```

**Com Otimização** (se a, b, c, d em registradores):
```assembly
; a=EDI, b=ESI, c=EDX, d=ECX
ADD EDI, ESI            ; EDI = a + b
SUB EDX, ECX            ; EDX = c - d
IMUL EDI, EDX           ; EDI = (a+b)*(c-d)
; Resultado em EDI
```

### 🎓 Exemplo: Estruturas de Controle

#### IF-ELSE

```c
if (x > 10) {
    y = 1;
} else {
    y = 2;
}
```

```assembly
    CMP DWORD PTR [x], 10   ; Compara x com 10
    JLE else_label          ; Se x <= 10, vai para else
then_label:
    MOV DWORD PTR [y], 1    ; y = 1
    JMP endif_label
else_label:
    MOV DWORD PTR [y], 2    ; y = 2
endif_label:
    ; Continua...
```

#### WHILE Loop

```c
int i = 0;
while (i < 10) {
    sum += i;
    i++;
}
```

```assembly
    MOV DWORD PTR [i], 0        ; i = 0
loop_start:
    CMP DWORD PTR [i], 10       ; i < 10?
    JGE loop_end                ; Se não, sai do loop
    
    MOV EAX, [i]
    ADD [sum], EAX              ; sum += i
    INC DWORD PTR [i]           ; i++
    
    JMP loop_start
loop_end:
```

#### FOR Loop

```c
for (int i = 0; i < 10; i++) {
    arr[i] = i * 2;
}
```

```assembly
    XOR ECX, ECX                ; i = 0 (ECX usado como contador)
for_start:
    CMP ECX, 10
    JGE for_end
    
    MOV EAX, ECX                ; EAX = i
    SHL EAX, 1                  ; EAX = i * 2
    LEA RDX, [arr]              ; RDX = endereço base de arr
    MOV [RDX + RCX*4], EAX      ; arr[i] = i * 2
    
    INC ECX
    JMP for_start
for_end:
```

---

## Arquiteturas RISC vs CISC

### 🏛️ CISC (Complex Instruction Set Computer)

**Filosofia**: Instruções complexas que fazem muito em uma instrução.

**Características**:
- **Instruções variáveis**: 1-15 bytes (x86)
- **Muitas instruções**: Centenas de instruções diferentes
- **Endereçamento complexo**: Múltiplos modos
- **Poucos registradores**: Historicamente (x86 tinha 8)
- **Microcódigo**: Instruções complexas decompostas internamente

**Exemplo x86**:
```assembly
; Uma instrução pode fazer muito
MOVS    ; Move string (copia bloco de memória)
LOOP    ; Decrementa contador e branch
REP MOVSB  ; Repete MOVSB até contador zerar
```

**Vantagens**:
- Código mais compacto
- Menos instruções para mesma tarefa
- Útil quando memória é cara/lenta

**Desvantagens**:
- Instruções lentas (múltiplos ciclos)
- Difícil pipeline
- Hardware complexo

### ⚡ RISC (Reduced Instruction Set Computer)

**Filosofia**: Instruções simples, rápidas, executáveis em poucos ciclos.

**Características**:
- **Instruções fixas**: 32 bits (MIPS, ARM)
- **Poucas instruções**: ~100-200 instruções
- **Endereçamento simples**: Poucos modos
- **Muitos registradores**: 32 registradores (MIPS)
- **Load/Store**: Só LOAD e STORE acessam memória

**Exemplo MIPS**:
```assembly
LW  $t0, 0($t1)     ; Load
ADD $t0, $t0, $t2   ; Compute
SW  $t0, 0($t1)     ; Store
```

**Vantagens**:
- Instruções rápidas (1 ciclo ideal)
- Pipeline eficiente
- Hardware simples

**Desvantagens**:
- Mais instruções para mesma tarefa
- Código maior

### 📊 Comparação

| Aspecto | CISC (x86) | RISC (MIPS, ARM) |
|---------|------------|------------------|
| **Tamanho Instrução** | Variável | Fixo |
| **Complexidade** | Alta | Baixa |
| **Registradores** | Poucos | Muitos |
| **Modos Endereçamento** | Muitos | Poucos |
| **Pipeline** | Difícil | Fácil |
| **Exemplo** | Intel x86, AMD64 | MIPS, ARM, RISC-V |

### 🔄 Convergência Moderna

Processadores modernos **convergem**:
- **x86 moderno**: Internamente RISC (micro-ops), externamente CISC
- **ARM moderno**: Adiciona instruções complexas (Thumb)

Intel/AMD decompõem instruções x86 complexas em **micro-operações** RISC-like que são executadas por pipeline RISC.

---

## Exemplos Práticos

### 💻 Exemplo 1: Soma Simples em Inline Assembly

Ver arquivo `exemploSomaSimplesAssmbly.c` neste diretório.

### 💻 Exemplo 2: Função Recursiva (Fatorial)

```c
int fatorial(int n) {
    if (n <= 1) return 1;
    return n * fatorial(n - 1);
}
```

**Assembly**:
```assembly
fatorial:
    PUSH RBP
    MOV RBP, RSP
    
    ; n em EDI
    CMP EDI, 1
    JG recursivo
    
    ; Caso base: n <= 1
    MOV EAX, 1
    POP RBP
    RET
    
recursivo:
    PUSH RDI                ; Salva n
    DEC EDI                 ; n - 1
    CALL fatorial           ; Chamada recursiva
    POP RDI                 ; Restaura n
    IMUL EAX, EDI           ; n * fatorial(n-1)
    
    POP RBP
    RET
```

### 💻 Exemplo 3: Acesso a Estrutura

```c
struct Point {
    int x;
    int y;
};

int soma_coordenadas(struct Point *p) {
    return p->x + p->y;
}
```

**Assembly**:
```assembly
soma_coordenadas:
    ; p em RDI (ponteiro para struct)
    MOV EAX, [RDI]          ; EAX = p->x (offset 0)
    ADD EAX, [RDI + 4]      ; EAX += p->y (offset 4)
    RET
```

**Explicação dos Offsets**:
```
struct Point em memória:
┌──────────┬──────────┐
│  x (4B)  │  y (4B)  │
├──────────┼──────────┤
offset: 0      4
```

---

## Exercícios Acadêmicos

### 📝 Exercício 1: Análise de Código Assembly

Dado o código assembly abaixo, determine o que ele faz:

```assembly
mystery:
    XOR EAX, EAX        ; EAX = 0
    XOR ECX, ECX        ; ECX = 0
loop_start:
    CMP ECX, EDI
    JGE loop_end
    ADD EAX, ECX
    INC ECX
    JMP loop_start
loop_end:
    RET
```

**Resposta**: Calcula a soma de 0 até n-1 (n em EDI). Equivalente a:
```c
int mystery(int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += i;
    }
    return sum;
}
```

### 📝 Exercício 2: Otimização

Compare as duas versões abaixo. Qual é mais eficiente e por quê?

**Versão A**:
```assembly
MOV EAX, [x]
IMUL EAX, 8
```

**Versão B**:
```assembly
MOV EAX, [x]
SHL EAX, 3
```

**Resposta**: Versão B é mais eficiente. `SHL` (shift left) é mais rápido que `IMUL` (multiplicação). Multiplicar por 8 = deslocar 3 bits à esquerda (8 = 2³). Compiladores fazem essa otimização automaticamente (**strength reduction**).

### 📝 Exercício 3: Geração de Código

Compile a seguinte expressão para assembly x86-64:

```c
int resultado = (a * b) + (c / d);
```

Assuma que a, b, c, d estão em EDI, ESI, EDX, ECX respectivamente.

**Resposta**:
```assembly
IMUL EDI, ESI       ; EDI = a * b
MOV EAX, EDX        ; EAX = c
CDQ                 ; Estende sinal de EAX para EDX:EAX
IDIV ECX            ; EAX = c / d
ADD EAX, EDI        ; EAX = (a*b) + (c/d)
; Resultado em EAX
```

### 📝 Exercício 4: Stack Frames

Desenhe o stack frame para a seguinte chamada:

```c
int foo(int a, int b) {
    int x = a + b;
    int y = x * 2;
    return y;
}

int main() {
    int z = foo(3, 5);
}
```

**Resposta**:
```
        ┌─────────────────┐ ← RSP ao entrar em foo
        │  (alinhamento)  │
        ├─────────────────┤
        │  y (4 bytes)    │ RBP - 8
        ├─────────────────┤
        │  x (4 bytes)    │ RBP - 4
        ├─────────────────┤ ← RBP
        │  RBP salvo      │
        ├─────────────────┤
        │ return address  │ (onde retornar em main)
        ├─────────────────┤
        │  ...            │
        │  Frame de main  │
        └─────────────────┘
```

### 📝 Exercício 5: Contagem de Ciclos

Dado um pipeline de 5 estágios, quantos ciclos são necessários para executar 10 instruções **sem hazards**?

**Resposta**: 
- Primeira instrução: 5 ciclos
- Próximas 9 instruções: 1 ciclo cada = 9 ciclos
- **Total: 14 ciclos**

Sem pipeline: 10 × 5 = 50 ciclos
Speedup: 50/14 ≈ 3.57x

---

## Perguntas para Pensar

### 🤔 Pergunta 1: Por que registradores são tão rápidos?

**Pergunta**: Por que acessar um registrador leva 1 ciclo, mas acessar RAM leva 100+ ciclos?

**Resposta**: 

Múltiplos fatores:

1. **Proximidade física**: Registradores estão **dentro da CPU**, a poucos micrômetros dos circuitos de execução. RAM está em chips separados, centímetros de distância.

2. **Tecnologia**: Registradores usam **flip-flops** (6-8 transistores por bit), extremamente rápidos mas caros. RAM usa **capacitores** (1 transistor + 1 capacitor por bit), mais lentos mas muito mais densos.

3. **Paralelismo**: Todos os bits de um registrador são acessados simultaneamente (64 bits em paralelo). Memória tem barramento limitado.

4. **Latência elétrica**: Sinais elétricos levam tempo para viajar. Mesmo à velocidade da luz no silício (~c/3), distância importa.

5. **Complexidade do controlador**: Acessar RAM requer:
   - Enviar endereço pelo barramento
   - Esperar DRAM refresh se necessário
   - Esperar CAS/RAS latency
   - Receber dados de volta

**Analogia**: Registrador = nota no seu bolso (acesso instantâneo). RAM = item em uma gaveta (precisa abrir, procurar, pegar).

### 🤔 Pergunta 2: Por que não fazer tudo em registradores?

**Pergunta**: Se registradores são tão rápidos, por que não ter milhares deles em vez de RAM?

**Resposta**:

1. **Custo**: Registradores são **extremamente caros**. Um chip com 1000 registradores de 64 bits custaria mais que todo o resto do processador combinado.

2. **Espaço físico**: Flip-flops ocupam muito espaço no die. Área é preciosa — cada mm² poderia ter mais cache ou mais núcleos.

3. **Energia**: Registradores consomem **muita energia** porque são sempre ativos. Milhares deles tornariam o processador forno.

4. **Complexidade de roteamento**: Mais registradores = mais fios interconectando = mais complexidade = maior latência nas conexões.

5. **Retornos decrescentes**: Para muitas aplicações, 16-32 registradores são suficientes com boa alocação.

**Princípio econômico**: Hierarquia de memória reflete **trade-offs ótimos** entre velocidade, custo, e capacidade.

### 🤔 Pergunta 3: Como branch prediction funciona?

**Pergunta**: Como o processador "adivinha" o resultado de um `if` antes de executá-lo?

**Resposta**:

**Branch Predictors** usam **histórico** para prever branches:

**1-bit Predictor**:
- Se último branch foi tomado → prevê tomado
- Se não tomado → prevê não tomado

**Problema**: Loops alternam previsão no final!
```c
for (i = 0; i < 100; i++) { ... }
// 99x tomado, 1x não tomado
// 1-bit erra 2x por loop (fim e recomeço)
```

**2-bit Predictor** (Saturating Counter):
```
Estados:
00: Fortemente Não-Tomado
01: Fracamente Não-Tomado
10: Fracamente Tomado
11: Fortemente Tomado

Transições:
Tomado: incrementa (satura em 11)
Não-Tomado: decrementa (satura em 00)
```

Agora loops funcionam bem: precisa 2 erros consecutivos para mudar previsão.

**Branch Target Buffer (BTB)**:
Cache que armazena:
- PC do branch
- Endereço alvo
- Histórico (2-bit counter)

**Two-Level Adaptive Predictor**:
Usa **padrões de histórico**:
```
00101 → Tomado
00110 → Não-Tomado
```

Aprende **correlações** entre branches.

**Modern Predictors**:
- **Perceptron-based**: Usa redes neurais simples
- **TAGE**: Tagged Geometric History Length
- **Accuracy**: >95% em código real!

**Custo de Missprediction**:
- Pipeline precisa ser **flushed**
- ~15-20 ciclos de penalidade
- Por isso alta taxa de acerto é crítica

### 🤔 Pergunta 4: Por que x86 domina desktops mas ARM domina mobile?

**Pergunta**: Por que arquiteturas diferentes dominam mercados diferentes?

**Resposta**:

**x86 em Desktops/Servers**:

**Vantagens**:
1. **Legacy**: Décadas de software compilado para x86
2. **Compatibilidade**: Rodar software antigo sem recompilação
3. **Performance absoluta**: CISC permite performance alta com muito poder/espaço
4. **Ecossistema**: Ferramentas, drivers, otimizações maduras

**Desvantagens**:
1. **Consumo de energia**: CISC complexo gasta mais energia
2. **Custo**: Chips mais complexos, mais caros
3. **Tamanho**: Dies maiores

**ARM em Mobile/Embedded**:

**Vantagens**:
1. **Eficiência energética**: RISC simples → menos energia por instrução
2. **Custo**: Menor área de silício → mais barato
3. **Calor**: Menos dissipação térmica (crucial em dispositivos passivamente resfriados)
4. **Licenciamento**: ARM licencia designs, permitindo customização

**Desvantagens**:
1. **Performance absoluta**: Historicamente menor (mudando com Apple M1/M2!)
2. **Ecossistema**: Menos software nativo (mas melhorando)

**Convergência**:
- ARM invade data centers (AWS Graviton)
- Apple M1/M2 provam que ARM pode competir em desktops
- x86 melhora eficiência energética
- Diferenças diminuindo com designs híbridos

**Resposta curta**: **Requisitos diferentes**. Desktop prioriza performance e compatibilidade. Mobile prioriza bateria e custo.

### 🤔 Pergunta 5: O que é o "Gargalo de von Neumann"?

**Pergunta**: Qual é a limitação fundamental da arquitetura de von Neumann?

**Resposta**:

**Gargalo de von Neumann**: O **barramento compartilhado** entre CPU e memória limita throughput.

**Problema**:
```
CPU (rápida, GHz) ←→ Barramento (lento) ←→ Memória (lenta)
```

- CPU pode executar bilhões de instruções por segundo
- Mas só pode buscar ~milhões de dados por segundo da RAM
- **CPU fica esperando memória** (memory-bound programs)

**Consequências**:
1. **Cache essencial**: Sem cache, CPUs modernas seriam ~100x mais lentas
2. **Prefetching**: Buscar dados antecipadamente
3. **Bandwidth vs. Latency**: Ambos limitados

**Soluções**:

**1. Hierarquia de Memória**:
- Múltiplos níveis de cache amenizam problema

**2. Arquitetura Harvard**:
- Barramentos separados para instruções e dados
- Usado em DSPs, microcontroladores

**3. Memória On-Die**:
- eDRAM, HBM (High Bandwidth Memory)
- Memória mais próxima da CPU

**4. Processamento In-Memory**:
- Executar computação na própria memória
- Elimina transferências

**5. Paralelismo**:
- Múltiplos cores acessam memória simultaneamente
- NUMA (Non-Uniform Memory Access)

**Impacto no Design de Algoritmos**:
Algoritmos devem ser **cache-friendly**:
- Localidade espacial e temporal
- Blocking/tiling de loops
- Estruturas de dados compactas

**Analogia**: Cozinheiro muito rápido (CPU) com geladeira pequena (cache) e supermercado longe (RAM). Maior parte do tempo é gasto indo e voltando do supermercado, não cozinhando!

### 🤔 Pergunta 6: Como compiladores otimizam loops?

**Pergunta**: Que transformações compiladores aplicam para acelerar loops?

**Resposta**:

**1. Loop Invariant Code Motion (LICM)**:
Move cálculos que não mudam para fora do loop.

```c
// Antes
for (i = 0; i < n; i++) {
    x = y + z;  // y e z não mudam!
    a[i] = x * i;
}

// Depois
x = y + z;
for (i = 0; i < n; i++) {
    a[i] = x * i;
}
```

**2. Loop Unrolling**:
Desenrola loop para reduzir overhead de controle e permitir mais ILP.

```c
// Antes
for (i = 0; i < 100; i++) {
    sum += a[i];
}

// Depois (unroll 4x)
for (i = 0; i < 100; i += 4) {
    sum += a[i];
    sum += a[i+1];
    sum += a[i+2];
    sum += a[i+3];
}
```

**Benefícios**:
- Menos branches
- Mais instruções independentes (melhor ILP)
- Melhor uso de pipeline

**3. Loop Fusion**:
Combina loops adjacentes que iteram sobre mesmos dados.

```c
// Antes
for (i = 0; i < n; i++) a[i] = b[i] + 1;
for (i = 0; i < n; i++) c[i] = a[i] * 2;

// Depois
for (i = 0; i < n; i++) {
    a[i] = b[i] + 1;
    c[i] = a[i] * 2;
}
```

**Benefício**: Melhor localidade de cache.

**4. Loop Tiling/Blocking**:
Divide loop para caber dados no cache.

```c
// Multiplicação de matrizes
// Antes: percorre matrizes grandes
for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
        for (k = 0; k < N; k++)
            C[i][j] += A[i][k] * B[k][j];

// Depois: processa blocos que cabem no cache
for (ii = 0; ii < N; ii += TILE)
    for (jj = 0; jj < N; jj += TILE)
        for (kk = 0; kk < N; kk += TILE)
            for (i = ii; i < min(ii+TILE,N); i++)
                for (j = jj; j < min(jj+TILE,N); j++)
                    for (k = kk; k < min(kk+TILE,N); k++)
                        C[i][j] += A[i][k] * B[k][j];
```

**5. Vetorização (SIMD)**:
Transforma loop em operações vetoriais.

```c
// Antes
for (i = 0; i < n; i++) {
    c[i] = a[i] + b[i];
}

// Assembly vetorizado (AVX)
for (i = 0; i < n; i += 8) {
    __m256 va = _mm256_load_ps(&a[i]);
    __m256 vb = _mm256_load_ps(&b[i]);
    __m256 vc = _mm256_add_ps(va, vb);
    _mm256_store_ps(&c[i], vc);
}
// 8 adições em uma instrução!
```

**6. Software Pipelining**:
Solapa iterações para manter pipeline cheio.

**7. Loop Strength Reduction**:
Substitui operações caras por baratas.

```c
// Antes
for (i = 0; i < n; i++) {
    a[i] = i * 5;  // Multiplicação
}

// Depois
for (i = 0, t = 0; i < n; i++, t += 5) {
    a[i] = t;  // Adição
}
```

---

## Referências e Leitura Adicional

### 📚 Livros Fundamentais

1. **Patterson, D. A., & Hennessy, J. L.** (2017). *Computer Organization and Design: The Hardware/Software Interface* (5th ed.). Morgan Kaufmann.
   - O livro definitivo sobre organização de computadores
   - Abordagem MIPS/RISC
   - Excelente para entender pipeline, cache, paralelismo

2. **Hennessy, J. L., & Patterson, D. A.** (2017). *Computer Architecture: A Quantitative Approach* (6th ed.). Morgan Kaufmann.
   - Mais avançado que o anterior
   - Foco em performance e métricas quantitativas
   - Essencial para otimizações de compiladores

3. **Tanenbaum, A. S., & Austin, T.** (2012). *Structured Computer Organization* (6th ed.). Pearson.
   - Abordagem em camadas: digital logic → microarchitecture → ISA → OS
   - Excelente para entender abstração

4. **Bryant, R. E., & O'Hallaron, D. R.** (2015). *Computer Systems: A Programmer's Perspective* (3rd ed.). Pearson.
   - Perspectiva do programador
   - Excelente cobertura de assembly x86-64, otimizações, memória

5. **Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D.** (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Pearson.
   - O "Livro do Dragão"
   - Conecta compiladores com arquitetura
   - Geração e otimização de código

### 🌐 Recursos Online

**Documentação Oficial**:
- **Intel® 64 and IA-32 Architectures Software Developer Manuals**: https://www.intel.com/sdm
- **ARM Architecture Reference Manual**: https://developer.arm.com/documentation
- **RISC-V Specifications**: https://riscv.org/technical/specifications/

**Cursos**:
- **Nand2Tetris**: https://www.nand2tetris.org/ (construa um computador do zero!)
- **MIT 6.004**: Computation Structures (https://6004.mit.edu/)
- **CS:APP (Bryant & O'Hallaron)**: http://csapp.cs.cmu.edu/

**Ferramentas Interativas**:
- **Compiler Explorer**: https://godbolt.org/ (veja assembly gerado por compiladores)
- **CPU Simulator**: https://github.com/mortbopet/Ripes (simula RISC-V)

### 🔧 Ferramentas Práticas

**Disassemblers/Debuggers**:
```bash
# Disassembly de binário
objdump -d executavel

# GDB para debug
gdb executavel

# Veja código assembly gerado
gcc -S -O2 programa.c

# Veja otimizações aplicadas
gcc -O2 -fopt-info-vec programa.c
```

**Performance Analysis**:
```bash
# Conta ciclos, cache misses, etc
perf stat ./programa

# Profiling
perf record ./programa
perf report

# Cache analysis
valgrind --tool=cachegrind ./programa
```

### 📊 Papers Clássicos

1. **Hennessy, J., & Patterson, D.** (2012). "Computer Architecture: A Quantitative Approach"
   - Define métricas de performance fundamentais

2. **Yeh, T. Y., & Patt, Y. N.** (1991). "Two-Level Adaptive Training Branch Prediction"
   - Branch prediction moderno

3. **Lam, M. S., Rothberg, E. E., & Wolf, M. E.** (1991). "The Cache Performance and Optimizations of Blocked Algorithms"
   - Loop tiling para otimização de cache

---

## 🎓 Conclusão

Organização de computadores é **fundamental** para qualquer engenheiro de compiladores. Entender como o hardware funciona permite:

1. **Gerar código eficiente**: Explorar registradores, cache, pipeline
2. **Fazer trade-offs informados**: Tamanho vs. velocidade, memória vs. computação
3. **Depurar efetivamente**: Entender assembly facilita debugging de baixo nível
4. **Otimizar algoritmos**: Algoritmos cache-aware são ordens de magnitude mais rápidos

**Próximos Passos**:
- Implemente os exercícios propostos
- Explore códigos gerados por compiladores reais (gcc, clang)
- Estude otimizações específicas para arquitetura alvo
- Experimente com inline assembly
- Leia especificações de ISAs reais

**Lembre-se**: A fronteira entre hardware e software é onde a mágica acontece. Compiladores são a ponte que torna computação abstrata em realidade concreta. 🚀

---

*Este documento foi criado como material educacional para o curso de Compiladores. Para dúvidas, sugestões ou correções, consulte o repositório principal.*
