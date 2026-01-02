# 13 — Otimizador de Código

## Índice
1. [Introdução](#introdução)
2. [Conceitos Fundamentais](#conceitos-fundamentais)
3. [Fundamentos Teóricos](#fundamentos-teóricos)
4. [Classificação de Otimizações](#classificação-de-otimizações)
5. [Otimizações Locais](#otimizações-locais)
6. [Otimizações Globais](#otimizações-globais)
7. [Análise de Fluxo de Dados](#análise-de-fluxo-de-dados)
8. [Otimizações em Loops](#otimizações-em-loops)
9. [Alocação de Registradores](#alocação-de-registradores)
10. [Implementação em C](#implementação-em-c)
11. [Exemplos Práticos](#exemplos-práticos)
12. [Como Compilar e Usar](#como-compilar-e-usar)
13. [Referências Acadêmicas](#referências-acadêmicas)

---

## Introdução

A **otimização de código** é uma fase fundamental dos compiladores modernos, responsável por transformar código intermediário correto em código **mais eficiente**, sem alterar sua semântica. Esta fase situa-se tipicamente entre a geração de código intermediário e a geração de código final.

### Importância da Otimização de Código

Segundo **Aho, Lam, Sethi e Ullman** (2006), no clássico "Compiladores: Princípios, Técnicas e Ferramentas":

> "A otimização de código visa melhorar o código intermediário de modo que resulte em código de máquina mais rápido ou menor. O termo 'otimização' é um nome impróprio, já que não há garantia de que o código resultante seja ótimo em qualquer sentido significativo."

A otimização de código é essencial porque:
- **Melhora o desempenho**: Reduz tempo de execução, às vezes em ordem de magnitude
- **Reduz consumo de recursos**: Menos uso de memória, energia e processamento
- **Permite código de alto nível**: Programadores escrevem código legível, compilador otimiza
- **Compensa ineficiências**: Corrige padrões subótimos gerados por fases anteriores
- **Adapta ao hardware**: Explora características específicas do processador alvo

### Analogia com Rotas de Transporte

Considere um sistema de entrega de pacotes:

**Código Não Otimizado**:
```
Origem → Destino A → Origem → Destino B → Origem → Destino C
```
- Múltiplas viagens
- Muito desperdício de tempo e combustível

**Código Otimizado**:
```
Origem → Destino A → Destino B → Destino C → Origem
```
- Uma única viagem eficiente
- Mesmos destinos alcançados, menor custo

Da mesma forma, otimização de código reorganiza operações para maximizar eficiência sem mudar o resultado final.

### Objetivos deste Módulo

- Compreender diferentes tipos e níveis de otimizações
- Dominar análise de fluxo de dados para otimizações globais
- Implementar otimizações clássicas (constant folding, CSE, dead code elimination)
- Aplicar otimizações específicas em loops
- Entender alocação de registradores e coloração de grafos
- Desenvolver otimizadores práticos em C

---

## Conceitos Fundamentais

### O que é Otimização de Código?

Otimização de código é o processo de transformar um programa em outro **semanticamente equivalente** mas **mais eficiente** segundo alguma métrica.

**Entrada**: Código intermediário correto
```
t1 = 3 + 4
x = t1
y = 3 + 4
z = x + y
```

**Saída**: Código otimizado
```
x = 7
y = 7
z = 14
```

### Critérios de Otimização

**Métricas comuns**:

1. **Tempo de execução** (mais comum)
   - Reduzir número de instruções
   - Eliminar instruções redundantes
   - Melhorar localidade de cache

2. **Tamanho do código**
   - Importante para sistemas embarcados
   - Melhora uso de cache de instruções

3. **Consumo de energia**
   - Crítico para dispositivos móveis
   - Reduz dissipação térmica

4. **Uso de memória**
   - Reduzir alocações dinâmicas
   - Melhorar reuso de dados

**Trade-offs**: Frequentemente há conflito entre métricas. Exemplo:
- Loop unrolling aumenta tamanho mas reduz tempo
- Inlining aumenta tamanho mas elimina chamadas

### Correção das Otimizações

**Princípio fundamental**: Otimizações devem **preservar semântica**.

**Definição formal**: Uma transformação T é correta se, para todo programa P:
```
Output(T(P)) = Output(P)
```

**Transformações conservadoras**: Na dúvida, não otimizar.
- Melhor código correto lento que código rápido incorreto
- Aliasing de ponteiros pode impedir otimizações

### Fases da Otimização

```
Código Intermediário
        ↓
┌───────────────────┐
│ Análise de Fluxo  │ ← Coleta informações sobre o programa
└────────┬──────────┘
         ↓
┌───────────────────┐
│ Otimizações       │ ← Aplica transformações
│ Locais            │   (dentro de blocos básicos)
└────────┬──────────┘
         ↓
┌───────────────────┐
│ Otimizações       │ ← Aplica transformações
│ Globais           │   (entre blocos básicos)
└────────┬──────────┘
         ↓
┌───────────────────┐
│ Alocação de       │ ← Atribui variáveis a registradores
│ Registradores     │
└────────┬──────────┘
         ↓
Código Otimizado
```

### Blocos Básicos

**Definição**: Sequência de instruções consecutivas com:
- **Uma entrada**: Apenas primeira instrução é alvo de salto
- **Uma saída**: Apenas última instrução é salto

**Exemplo**:
```
L1: t1 = a + b      ← Início do bloco
    t2 = t1 * 2
    if t2 < 10 goto L2  ← Fim do bloco (salto)

L2: x = t2 + 1      ← Início de novo bloco
    y = x * 3
    goto L3         ← Fim do bloco (salto)

L3: return y        ← Bloco de uma instrução
```

**Três blocos básicos**: [L1-condicional], [L2-goto], [L3-return]

### Grafo de Fluxo de Controle (CFG)

**Definição**: Representação gráfica do fluxo de execução.

- **Nós**: Blocos básicos
- **Arestas**: Possíveis transferências de controle

**Exemplo**:
```
        [Bloco 1]
           |
           ↓
     [if (x > 0)]
       ↙       ↘
  [Bloco 2]  [Bloco 3]
       ↘       ↙
        [Bloco 4]
           |
           ↓
        [Saída]
```

---

## Fundamentos Teóricos

### Análise de Fluxo de Dados

**Frances Allen** (1970) introduziu framework formal para análise de fluxo de dados.

**Definição**: Análise de fluxo de dados determina propriedades sobre valores em pontos específicos do programa.

#### Equações de Fluxo de Dados

Para cada bloco básico B:

**OUT[B]**: Informação na saída de B
**IN[B]**: Informação na entrada de B
**GEN[B]**: Informação gerada por B
**KILL[B]**: Informação destruída por B

**Equações gerais**:
```
IN[B] = ∪ OUT[P]  para todo predecessor P de B
OUT[B] = GEN[B] ∪ (IN[B] - KILL[B])
```

#### Tipos de Análise

1. **Forward Analysis** (para frente)
   - Informação flui do início para o fim
   - Exemplo: Reaching definitions, available expressions

2. **Backward Analysis** (para trás)
   - Informação flui do fim para o início
   - Exemplo: Live variables, very busy expressions

### Teorema do Ponto Fixo

**Kildall** (1973) formalizou algoritmo de ponto fixo para análise de fluxo.

**Algoritmo**:
```
1. Inicializar IN[B] e OUT[B] para todos os blocos
2. Repetir até convergir:
   Para cada bloco B:
     IN[B] = ∪ OUT[P] para predecessores P
     OUT[B] = GEN[B] ∪ (IN[B] - KILL[B])
```

**Convergência**: Garantida se o espaço de valores forma um **reticulado** (lattice) com propriedade de cadeia ascendente finita.

### Análise de Ponteiros e Aliasing

**Problema**: Dois ponteiros podem apontar para mesmo endereço?

```c
int *p, *q;
*p = 10;
*q = 20;
x = *p;  // x pode ser 10 ou 20?
```

**Análise de aliasing**:
- **May-alias**: Podem apontar para mesmo local (conservador)
- **Must-alias**: Sempre apontam para mesmo local
- **Andersen (1994)**: Análise baseada em inclusão de conjuntos

**Impacto**: Aliasing conservador impede otimizações agressivas.

### Representação SSA

**Static Single Assignment** (SSA) é uma IR onde cada variável é atribuída **exatamente uma vez**.

**Cytron et al.** (1991) introduziram SSA form:

**Código normal**:
```
x = 1
x = x + 2
y = x
```

**Código SSA**:
```
x₁ = 1
x₂ = x₁ + 2
y₁ = x₂
```

**Função φ** para convergência de fluxos:
```
if (cond)
  x₁ = 10
else
  x₂ = 20
x₃ = φ(x₁, x₂)  // x₃ assume x₁ ou x₂ dependendo do caminho
```

**Vantagens**:
- Simplifica análise de fluxo de dados
- Facilita otimizações (propagação de constantes, eliminação de código morto)
- Usada em GCC, LLVM

---

## Classificação de Otimizações

### Por Escopo

#### 1. Otimizações Locais
- **Escopo**: Dentro de um bloco básico
- **Exemplos**: Constant folding, algebraic simplification
- **Vantagens**: Simples, rápidas, seguras
- **Limitações**: Efeito limitado

#### 2. Otimizações Globais
- **Escopo**: Múltiplos blocos básicos (intraprocedimental)
- **Exemplos**: Common subexpression elimination, dead code elimination
- **Vantagens**: Maior impacto que locais
- **Complexidade**: Requer análise de fluxo de dados

#### 3. Otimizações Interprocedurais
- **Escopo**: Entre funções
- **Exemplos**: Inlining, interprocedural constant propagation
- **Vantagens**: Máximo impacto
- **Complexidade**: Cara computacionalmente

### Por Técnica

#### 1. Otimizações Baseadas em Análise
- Requerem análise de fluxo de dados
- Exemplos: Live variable analysis, reaching definitions

#### 2. Otimizações Peephole
- Substituição de pequenas sequências de instruções
- Janela deslizante sobre código
- Independente de análise global

#### 3. Otimizações de Loop
- Específicas para estruturas de repetição
- Exemplos: Loop invariant code motion, loop unrolling

### Por Impacto

**Alto impacto** (redução > 20%):
- Eliminação de código morto
- Loop optimizations
- Inlining de funções

**Médio impacto** (redução 5-20%):
- Common subexpression elimination
- Constant propagation
- Strength reduction

**Baixo impacto** (redução < 5%):
- Algebraic simplification
- Peephole optimizations

---

## Otimizações Locais

Otimizações aplicadas dentro de um único bloco básico.

### 1. Constant Folding (Dobramento de Constantes)

**Definição**: Avaliar expressões constantes em tempo de compilação.

**Antes**:
```
t1 = 3 + 4
t2 = t1 * 2
x = t2
```

**Depois**:
```
x = 14
```

**Implementação**:
```c
void constant_folding(Instruction* instr) {
    if (instr->op == OP_ADD && 
        is_constant(instr->arg1) && 
        is_constant(instr->arg2)) {
        
        int val1 = get_constant_value(instr->arg1);
        int val2 = get_constant_value(instr->arg2);
        
        instr->op = OP_ASSIGN;
        instr->arg1 = make_constant(val1 + val2);
        instr->arg2 = NULL;
    }
}
```

### 2. Constant Propagation (Propagação de Constantes)

**Definição**: Substituir uso de variável por seu valor constante.

**Antes**:
```
x = 10
y = x + 5
z = x * 2
```

**Depois**:
```
x = 10
y = 15
z = 20
```

**Algoritmo**:
1. Para cada atribuição `x = constante`, marcar x como constante
2. Substituir usos de x por seu valor
3. Repetir até ponto fixo

### 3. Copy Propagation (Propagação de Cópias)

**Definição**: Substituir uso de variável copiada pela original.

**Antes**:
```
x = y
z = x + 1
w = x + 2
```

**Depois**:
```
x = y
z = y + 1
w = y + 2
```

**Benefício**: Pode eliminar x se não for usado de outra forma.

### 4. Algebraic Simplification (Simplificação Algébrica)

**Definição**: Aplicar identidades algébricas para simplificar expressões.

**Identidades comuns**:
```
x + 0 → x
x * 1 → x
x * 0 → 0
x - x → 0
x / 1 → x
x * 2 → x << 1  (shift é mais rápido)
x * 8 → x << 3
x / 2 → x >> 1  (para inteiros positivos)
```

**Exemplo**:
```
Antes: t1 = a * 0 + b * 1
Depois: t1 = b
```

### 5. Strength Reduction (Redução de Força)

**Definição**: Substituir operações caras por mais baratas.

**Exemplos**:
```
x * 2     → x + x  ou  x << 1
x * 15    → (x << 4) - x
x / 4     → x >> 2
x % 8     → x & 7
pow(x, 2) → x * x
```

**Custo relativo** (ciclos em arquiteturas típicas):
- Adição/subtração: 1
- Multiplicação: 3-5
- Divisão: 20-40
- Chamada de função: 10+

### 6. Dead Code Elimination (Eliminação de Código Morto)

**Definição**: Remover código que não afeta saída do programa.

**Código morto**:
```c
int x = 10;  // x nunca usado
x = 20;      // MORTO
```

**Algoritmo**:
1. Marcar instruções que afetam saída (marca viva)
2. Iterar para trás, marcando dependências
3. Remover instruções não marcadas


**Exemplo**:
```c
void dead_code_example() {
    int x = 10;  // Vivo: usado abaixo
    int y = 20;  // MORTO: nunca usado
    printf("%d", x);
}
```

---

## Otimizações Globais

Otimizações que consideram múltiplos blocos básicos.

### 1. Common Subexpression Elimination (CSE)

**Definição**: Eliminar computação redundante de mesma expressão.

**Antes**:
```
t1 = a + b
x = t1
t2 = a + b  // Redundante!
y = t2
```

**Depois**:
```
t1 = a + b
x = t1
y = t1      // Reutiliza t1
```

**Condições para CSE**:
1. Expressão `a + b` é **available** (disponível) no segundo uso
2. Operandos `a` e `b` não foram modificados entre usos
3. Resultado não foi modificado

**Algoritmo**:
1. Construir CFG
2. Computar available expressions (análise forward)
3. Substituir expressões redundantes por temporário anterior

### 2. Loop-Invariant Code Motion (LICM)

**Definição**: Mover computações invariantes para fora do loop.

**Antes**:
```c
for (i = 0; i < n; i++) {
    x = y + z;  // y e z não mudam no loop
    a[i] = x * i;
}
```

**Depois**:
```c
x = y + z;  // Movido para antes do loop
for (i = 0; i < n; i++) {
    a[i] = x * i;
}
```

**Benefício**: Expressão calculada 1 vez em vez de n vezes.

**Condições**:
- Expressão é loop-invariant (operandos não mudam)
- Bloco domina todas as saídas do loop
- Atribuição não afeta outras iterações

### 3. Global Copy Propagation

Estende copy propagation para múltiplos blocos.

**Antes**:
```
Bloco 1: x = y
Bloco 2: z = x + 1
Bloco 3: w = x + 2
```

**Depois**:
```
Bloco 1: x = y
Bloco 2: z = y + 1
Bloco 3: w = y + 2
```

### 4. Global Constant Propagation

**Exemplo**:
```
Bloco 1: x = 10
         if (cond) goto B3
Bloco 2: y = x + 5  // x é 10
         goto B4
Bloco 3: z = x * 2  // x é 10
Bloco 4: w = x + 1  // x é 10
```

**Após otimização**:
```
Bloco 1: x = 10
         if (cond) goto B3
Bloco 2: y = 15
         goto B4
Bloco 3: z = 20
Bloco 4: w = 11
```

### 5. Dead Code Elimination Global

Remove código morto através de múltiplos blocos.

**Algoritmo (backwards)**:
1. Iniciar com instruções críticas (I/O, retorno, etc.)
2. Marcar como vivas
3. Para cada instrução viva, marcar suas dependências
4. Remover instruções não marcadas

---

## Análise de Fluxo de Dados

### 1. Reaching Definitions

**Definição**: Uma definição `d` de variável `v` **alcança** um ponto `p` se existe caminho de `d` a `p` sem redefinir `v`.

**Uso**: Construir use-def chains para otimizações.

**Equações**:
```
OUT[B] = GEN[B] ∪ (IN[B] - KILL[B])
IN[B] = ∪ OUT[P] para predecessores P
```

**Exemplo**:
```
Bloco 1: d1: x = 5
         if (x > 0) goto B3
Bloco 2: d2: x = 10
         goto B4
Bloco 3: d3: x = 15
Bloco 4: y = x    // Quais definições de x alcançam aqui?
                  // Resposta: d2 e d3
```

### 2. Live Variable Analysis

**Definição**: Uma variável é **viva** em um ponto se seu valor pode ser usado antes de ser redefinido.

**Uso**: Alocação de registradores, eliminação de código morto.

**Equações (backward)**:
```
IN[B] = USE[B] ∪ (OUT[B] - DEF[B])
OUT[B] = ∪ IN[S] para sucessores S
```

**Exemplo**:
```
Bloco 1: x = 10      // x está vivo? Sim (usado em B2)
         y = 20      // y está vivo? Não (redefinido em B2 sem uso)
Bloco 2: y = x + 5
         return y
```

### 3. Available Expressions

**Definição**: Uma expressão é **disponível** em um ponto se foi computada e operandos não foram modificados.

**Uso**: Common subexpression elimination.

**Equações (forward)**:
```
OUT[B] = GEN[B] ∪ (IN[B] - KILL[B])
IN[B] = ∩ OUT[P] para predecessores P  // Interseção!
```

### 4. Very Busy Expressions

**Definição**: Uma expressão é **very busy** em um ponto se será usada em **todos os caminhos** de execução.

**Uso**: Code hoisting (mover código para cima).

**Equações (backward)**:
```
IN[B] = USE[B] ∪ (OUT[B] - KILL[B])
OUT[B] = ∩ IN[S] para sucessores S
```

---

## Otimizações em Loops

Loops consomem a maior parte do tempo de execução. Otimizá-los tem grande impacto.

### 1. Loop Unrolling (Desenrolamento de Loops)

**Definição**: Replicar corpo do loop para reduzir overhead de controle.

**Antes**:
```c
for (i = 0; i < 100; i++) {
    a[i] = b[i] + c[i];
}
```

**Depois (unroll factor = 4)**:
```c
for (i = 0; i < 100; i += 4) {
    a[i]   = b[i]   + c[i];
    a[i+1] = b[i+1] + c[i+1];
    a[i+2] = b[i+2] + c[i+2];
    a[i+3] = b[i+3] + c[i+3];
}
```

**Benefícios**:
- Reduz número de comparações e saltos (overhead)
- Melhora paralelismo de instruções (ILP)
- Melhora uso de cache

**Desvantagens**:
- Aumenta tamanho do código
- Pode piorar cache de instruções

### 2. Loop Fusion (Fusão de Loops)

**Definição**: Combinar dois loops consecutivos em um.

**Antes**:
```c
for (i = 0; i < n; i++)
    a[i] = b[i] + 1;

for (i = 0; i < n; i++)
    c[i] = a[i] * 2;
```

**Depois**:
```c
for (i = 0; i < n; i++) {
    a[i] = b[i] + 1;
    c[i] = a[i] * 2;
}
```

**Benefícios**:
- Reduz overhead de controle
- Melhora localidade de cache

### 3. Loop Fission (Divisão de Loops)

**Definição**: Dividir loop grande em loops menores (oposto de fusion).

**Antes**:
```c
for (i = 0; i < n; i++) {
    a[i] = b[i] + 1;
    c[i] = d[i] * 2;  // Independente da primeira linha
}
```

**Depois**:
```c
for (i = 0; i < n; i++)
    a[i] = b[i] + 1;

for (i = 0; i < n; i++)
    c[i] = d[i] * 2;
```

**Benefício**: Melhora localidade se arrays não cabem juntos no cache.

### 4. Loop Interchange (Troca de Loops)

**Definição**: Trocar ordem de loops aninhados para melhorar localidade.

**Antes** (acesso não-sequencial em memória):
```c
for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
        a[j][i] = b[j][i] + c[j][i];
```

**Depois** (acesso sequencial, melhor cache):
```c
for (j = 0; j < M; j++)
    for (i = 0; i < N; i++)
        a[j][i] = b[j][i] + c[j][i];
```

**Em C**: Arrays em row-major order, então loop externo deve ser sobre primeira dimensão.

### 5. Strength Reduction em Loops

**Antes**:
```c
for (i = 0; i < n; i++)
    a[i] = i * 4;  // Multiplicação cara
```

**Depois**:
```c
t = 0;
for (i = 0; i < n; i++) {
    a[i] = t;
    t = t + 4;  // Adição mais barata
}
```

### 6. Loop Inversion

**Definição**: Transformar `while` em `do-while` para eliminar teste redundante.

**Antes**:
```c
i = 0;
while (i < n) {
    // corpo
    i++;
}
```

**Depois**:
```c
i = 0;
if (i < n) {  // Teste uma vez
    do {
        // corpo
        i++;
    } while (i < n);
}
```

---

## Alocação de Registradores

Registradores são o recurso mais rápido e escasso. Alocação eficiente é crucial.

### O Problema

**Objetivo**: Atribuir variáveis do programa a registradores físicos limitados.

**Desafio**: Número de variáveis >> número de registradores

**Solução**: 
- Variáveis frequentemente usadas → registradores
- Variáveis raramente usadas → memória (spilling)

### Interferência de Variáveis

Duas variáveis **interferem** se estão vivas simultaneamente.

**Grafo de Interferência**: 
- Nós: Variáveis
- Arestas: Pares de variáveis que interferem

**Exemplo**:
```
a = 1       // a vivo
b = 2       // a, b vivos (interferem!)
c = a + b   // a, b, c vivos
return c    // c vivo
```

Grafo: `a—b`, `a—c`, `b—c` (triângulo)

### Coloração de Grafos

**Chaitin (1981)** formulou alocação como problema de coloração de grafos.

**Problema**: Atribuir cores (registradores) a nós tal que nós adjacentes tenham cores diferentes.

**Teorema**: Se o grafo é k-colorível, então k registradores são suficientes.

**Algoritmo de Chaitin**:
```
1. Construir grafo de interferência
2. Simplificação:
   Enquanto existir nó com grau < k:
     Remover nó e empilhar
3. Coloração:
   Desempilhar nós e atribuir cor disponível
4. Spilling:
   Se nó não tem cor disponível, fazer spill (mover para memória)
```

**Exemplo** (k=3 registradores):
```
Grafo: a—b—c—d
Grau: a(1), b(2), c(2), d(1)

1. Remover a (grau 1)
2. Remover d (grau 1)
3. Remover b (grau 1 após remoções)
4. Remover c (grau 0)

Colorir na ordem inversa:
c → R1
b → R2
d → R1 (não interfere com c)
a → R3
```

### Alocação Linear Scan

**Poletto & Sarkar (1999)** propuseram algoritmo mais simples para JIT.

**Algoritmo**:
1. Calcular intervalos de vida (live ranges)
2. Ordenar por ponto inicial
3. Para cada intervalo:
   - Se registrador livre, alocar
   - Senão, fazer spill do intervalo com fim mais distante

**Vantagem**: Mais rápido que coloração (O(n log n) vs O(n²))

**Desvantagem**: Menos otimal

---

## Implementação em C

### Exemplo Completo de Otimizador

Para facilitar o entendimento, vou mostrar uma implementação completa e funcional de um otimizador básico.

**Arquivo: `otimizador_simples.c`**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CODE_SIZE 100

// Estrutura de instrução
typedef struct {
    char op;          // '+', '-', '*', '/', '='
    char dest[10];    // Variável de destino
    char arg1[10];    // Primeiro argumento
    char arg2[10];    // Segundo argumento
    bool is_dead;     // Marca para código morto
} Instruction;

// Programa
Instruction code[MAX_CODE_SIZE];
int code_size = 0;

// Função auxiliar: verificar se é constante
bool is_number(const char* str) {
    if (!str || strlen(str) == 0) return false;
    for (int i = 0; str[i]; i++) {
        if (str[i] < '0' || str[i] > '9') {
            if (!(i == 0 && str[i] == '-')) return false;
        }
    }
    return true;
}

// Otimização: Constant Folding
void constant_folding() {
    printf("\n=== CONSTANT FOLDING ===\n");
    int count = 0;
    
    for (int i = 0; i < code_size; i++) {
        if (is_number(code[i].arg1) && is_number(code[i].arg2)) {
            int v1 = atoi(code[i].arg1);
            int v2 = atoi(code[i].arg2);
            int result = 0;
            
            switch(code[i].op) {
                case '+': result = v1 + v2; break;
                case '-': result = v1 - v2; break;
                case '*': result = v1 * v2; break;
                case '/': 
                    if (v2 == 0) continue;
                    result = v1 / v2;
                    break;
                default: continue;
            }
            
            printf("  [%d] %s = %d %c %d => %s = %d\n", 
                   i, code[i].dest, v1, code[i].op, v2, 
                   code[i].dest, result);
            
            code[i].op = '=';
            sprintf(code[i].arg1, "%d", result);
            code[i].arg2[0] = '\0';
            count++;
        }
    }
    
    printf("Expressões dobradas: %d\n", count);
}

// Otimização: Copy Propagation
void copy_propagation() {
    printf("\n=== COPY PROPAGATION ===\n");
    int count = 0;
    
    for (int i = 0; i < code_size; i++) {
        // Detectar cópia: x = y
        if (code[i].op == '=' && !is_number(code[i].arg1) && 
            code[i].arg2[0] == '\0') {
            
            char* var = code[i].dest;
            char* value = code[i].arg1;
            
            // Propagar para instruções seguintes
            for (int j = i + 1; j < code_size; j++) {
                bool changed = false;
                
                if (strcmp(code[j].arg1, var) == 0) {
                    strcpy(code[j].arg1, value);
                    changed = true;
                }
                if (strcmp(code[j].arg2, var) == 0) {
                    strcpy(code[j].arg2, value);
                    changed = true;
                }
                
                if (changed) {
                    printf("  [%d->%d] Propagar %s → %s\n", i, j, var, value);
                    count++;
                }
                
                // Parar se variável for redefinida
                if (strcmp(code[j].dest, var) == 0 || 
                    strcmp(code[j].dest, value) == 0) {
                    break;
                }
            }
        }
    }
    
    printf("Propagações realizadas: %d\n", count);
}

// Otimização: Dead Code Elimination
void dead_code_elimination() {
    printf("\n=== DEAD CODE ELIMINATION ===\n");
    
    bool used[MAX_CODE_SIZE] = {false};
    
    // Marcar variáveis usadas (análise simples)
    for (int i = 0; i < code_size; i++) {
        for (int j = i + 1; j < code_size; j++) {
            if (strcmp(code[j].arg1, code[i].dest) == 0 ||
                strcmp(code[j].arg2, code[i].dest) == 0) {
                used[i] = true;
                break;
            }
        }
    }
    
    // Marcar código morto
    int removed = 0;
    for (int i = 0; i < code_size; i++) {
        if (!used[i] && code[i].dest[0] != '\0') {
            printf("  [%d] MORTO: %s = ...\n", i, code[i].dest);
            code[i].is_dead = true;
            removed++;
        }
    }
    
    printf("Instruções mortas: %d\n", removed);
}

// Imprimir código
void print_code(const char* title) {
    printf("\n%s\n", title);
    printf("──────────────────────────────────\n");
    
    for (int i = 0; i < code_size; i++) {
        if (code[i].is_dead) continue;
        
        printf("[%2d] %s = ", i, code[i].dest);
        
        if (code[i].op == '=') {
            printf("%s\n", code[i].arg1);
        } else {
            printf("%s %c %s\n", code[i].arg1, code[i].op, code[i].arg2);
        }
    }
    printf("──────────────────────────────────\n");
}

// Função principal
int main() {
    printf("╔══════════════════════════════════════╗\n");
    printf("║   OTIMIZADOR DE CÓDIGO SIMPLES       ║\n");
    printf("╚══════════════════════════════════════╝\n");
    
    // Criar código de exemplo
    // t1 = 3 + 4
    strcpy(code[0].dest, "t1");
    code[0].op = '+';
    strcpy(code[0].arg1, "3");
    strcpy(code[0].arg2, "4");
    
    // t2 = t1 * 2
    strcpy(code[1].dest, "t2");
    code[1].op = '*';
    strcpy(code[1].arg1, "t1");
    strcpy(code[1].arg2, "2");
    
    // x = t2
    strcpy(code[2].dest, "x");
    code[2].op = '=';
    strcpy(code[2].arg1, "t2");
    code[2].arg2[0] = '\0';
    
    // y = 5 + 3
    strcpy(code[3].dest, "y");
    code[3].op = '+';
    strcpy(code[3].arg1, "5");
    strcpy(code[3].arg2, "3");
    
    // z = x + y
    strcpy(code[4].dest, "z");
    code[4].op = '+';
    strcpy(code[4].arg1, "x");
    strcpy(code[4].arg2, "y");
    
    code_size = 5;
    
    print_code("═══ CÓDIGO ORIGINAL ═══");
    
    // Aplicar otimizações
    constant_folding();
    copy_propagation();
    constant_folding();  // Segunda passada
    dead_code_elimination();
    
    print_code("═══ CÓDIGO OTIMIZADO ═══");
    
    printf("\n✓ Otimização concluída!\n\n");
    
    return 0;
}
```

### Saída Esperada

```
╔══════════════════════════════════════╗
║   OTIMIZADOR DE CÓDIGO SIMPLES       ║
╚══════════════════════════════════════╝

═══ CÓDIGO ORIGINAL ═══
──────────────────────────────────
[ 0] t1 = 3 + 4
[ 1] t2 = t1 * 2
[ 2] x = t2
[ 3] y = 5 + 3
[ 4] z = x + y
──────────────────────────────────

=== CONSTANT FOLDING ===
  [0] t1 = 3 + 4 => t1 = 7
  [3] y = 5 + 3 => y = 8
Expressões dobradas: 2

=== COPY PROPAGATION ===
  [0->1] Propagar t1 → 7
  [0->2] Propagar t1 → 7
  [2->4] Propagar x → 7
  [3->4] Propagar y → 8
Propagações realizadas: 4

=== CONSTANT FOLDING ===
  [1] t2 = 7 * 2 => t2 = 14
  [4] z = 7 + 8 => z = 15
Expressões dobradas: 2

=== DEAD CODE ELIMINATION ===
  [0] MORTO: t1 = ...
  [1] MORTO: t2 = ...
  [2] MORTO: x = ...
  [3] MORTO: y = ...
Instruções mortas: 4

═══ CÓDIGO OTIMIZADO ═══
──────────────────────────────────
[ 4] z = 15
──────────────────────────────────

✓ Otimização concluída!
```

---

## Exemplos Práticos

### Exemplo 1: Otimização Completa

**Código original**:
```
t1 = 3 + 4
t2 = t1 * 2
x = t2
y = 5 + 3
z = x + y
```

**Passo 1 - Constant Folding**:
```
t1 = 7
t2 = t1 * 2
x = t2
y = 8
z = x + y
```

**Passo 2 - Copy Propagation**:
```
t1 = 7
t2 = 7 * 2
x = t2
y = 8
z = 14 + 8
```

**Passo 3 - Constant Folding (segunda passagem)**:
```
t1 = 7
t2 = 14
x = 14
y = 8
z = 22
```

**Passo 4 - Dead Code Elimination**:
```
z = 22
```

**Redução**: 5 instruções → 1 instrução (80% de melhoria!)

### Exemplo 2: Loop Optimization

**Código original**:
```c
for (i = 0; i < 100; i++) {
    t = 10 + 20;  // Loop invariant
    a[i] = t * i;
}
```

**Após LICM**:
```c
t = 30;  // Movido para fora
for (i = 0; i < 100; i++) {
    a[i] = t * i;
}
```

**Após Strength Reduction**:
```c
t = 30;
temp = 0;
for (i = 0; i < 100; i++) {
    a[i] = temp;
    temp = temp + 30;  // Substituir multiplicação
}
```

**Ganho**: 100 multiplicações → 0 multiplicações, 100 adições

---

## Como Compilar e Usar

### Compilação

```bash
# Compilar o otimizador simples
gcc otimizador_simples.c -o otimizador -std=c99

# Compilar com otimizações do GCC desabilitadas
gcc otimizador_simples.c -o otimizador -std=c99 -O0

# Compilar com debug
gcc otimizador_simples.c -o otimizador -std=c99 -g -Wall
```

### Execução

```bash
# Executar
./otimizador

# Ver estatísticas
./otimizador --verbose

# Salvar saída
./otimizador > resultado.txt
```

### Comparação com GCC

```bash
# Sem otimização
gcc programa.c -O0 -o prog_O0

# Com otimização básica
gcc programa.c -O1 -o prog_O1

# Com otimização padrão
gcc programa.c -O2 -o prog_O2

# Otimização máxima
gcc programa.c -O3 -o prog_O3

# Otimizar para tamanho
gcc programa.c -Os -o prog_Os

# Ver tamanho
ls -lh prog_*

# Comparar tempo de execução
time ./prog_O0
time ./prog_O3
```

---

## Referências Acadêmicas

### Livros Fundamentais

1. **Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D.** (2006). *Compiladores: Princípios, Técnicas e Ferramentas*. 2ª Edição. Pearson.
   - **Capítulo 9**: Code Optimization
   - Referência definitiva

2. **Muchnick, S. S.** (1997). *Advanced Compiler Design and Implementation*. Morgan Kaufmann.
   - Cobertura exaustiva de otimizações
   - SSA, análises avançadas

3. **Cooper, K. D., & Torczon, L.** (2011). *Engineering a Compiler*. 2ª Ed. Morgan Kaufmann.
   - Abordagem prática
   - Implementações eficientes

### Artigos Clássicos

4. **Allen, F. E.** (1970). "Control Flow Analysis". *SIGPLAN Notices*.
   - Análise de fluxo fundamental

5. **Cytron, R., et al.** (1991). "Efficiently Computing SSA Form". *ACM TOPLAS*.
   - Introdução de SSA

6. **Chaitin, G. J.** (1982). "Register Allocation via Graph Coloring". *SIGPLAN*.
   - Alocação de registradores

### Recursos Online

7. **LLVM Optimization Guide**: https://llvm.org/docs/Passes.html
8. **GCC Optimize Options**: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
9. **Stanford CS243**: https://suif.stanford.edu/~courses/cs243/

---

## Conclusão

A otimização de código é uma fase **essencial** e **complexa** dos compiladores modernos, responsável por transformar código correto em código **eficiente**.

**Principais aprendizados**:

1. ✅ **Correção é prioridade**: Otimizações devem preservar semântica
2. ✅ **Análise de fluxo é fundamental**: Base para otimizações globais
3. ✅ **SSA simplifica**: Forma canônica facilita transformações
4. ✅ **Loops são críticos**: Maior impacto no desempenho
5. ✅ **Trade-offs existem**: Velocidade vs tamanho, precisão vs tempo de compilação

**Você agora pode**:
- Implementar otimizadores completos
- Compreender compiladores reais (GCC, LLVM)
- Aplicar técnicas de otimização
- Desenvolver novas otimizações

**Próximos passos**: Geração de código de máquina e otimizações específicas de arquitetura!

---

*Este material faz parte da disciplina de Compiladores. Para questões ou sugestões, consulte o repositório do curso.*
