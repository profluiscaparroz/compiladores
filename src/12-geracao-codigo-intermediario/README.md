# 12 — Geração de Código Intermediário

## Índice
1. [Introdução](#introdução)
2. [Conceitos Fundamentais](#conceitos-fundamentais)
3. [Fundamentos Teóricos](#fundamentos-teóricos)
4. [Representações Intermediárias](#representações-intermediárias)
5. [Código de Três Endereços](#código-de-três-endereços)
6. [Tradução Dirigida pela Sintaxe](#tradução-dirigida-pela-sintaxe)
7. [Geração de Código para Expressões](#geração-de-código-para-expressões)
8. [Geração de Código para Comandos de Controle](#geração-de-código-para-comandos-de-controle)
9. [Implementação em C](#implementação-em-c)
10. [Otimizações no Código Intermediário](#otimizações-no-código-intermediário)
11. [Exemplos Práticos](#exemplos-práticos)
12. [Como Compilar e Usar](#como-compilar-e-usar)
13. [Referências Acadêmicas](#referências-acadêmicas)

---

## Introdução

A **geração de código intermediário** é a **quarta fase** de um compilador, situada após as análises léxica, sintática e semântica. Esta fase transforma a Árvore Sintática Abstrata (AST) em uma representação intermediária que facilita a geração de código final e otimizações.

### Importância da Geração de Código Intermediário

Segundo **Aho, Lam, Sethi e Ullman** (2006), no clássico "Compiladores: Princípios, Técnicas e Ferramentas":

> "A geração de código intermediário é uma fase crucial que separa a análise do código fonte da síntese do código objeto, permitindo que um compilador seja facilmente retargetável para diferentes arquiteturas."

A geração de código intermediário é fundamental porque:
- **Separação de responsabilidades**: Desacopla análise do código fonte da geração de código de máquina
- **Portabilidade**: Facilita portabilidade para diferentes arquiteturas (retargetability)
- **Otimizações**: Oferece ponto de aplicação para otimizações independentes de máquina
- **Simplificação**: Código intermediário é mais simples que código de máquina
- **Reusabilidade**: Frontend pode ser reusado para diferentes backends

### Analogia com Tradução de Idiomas

Considere a tradução de um livro do português para várias línguas (inglês, francês, alemão, japonês):

**Abordagem Direta** (sem intermediário):
```
Português → Inglês
Português → Francês
Português → Alemão
Português → Japonês
```
Resultado: 4 tradutores especializados necessários

**Abordagem com Idioma Intermediário** (Esperanto):
```
Português → Esperanto → Inglês
                      → Francês
                      → Alemão
                      → Japonês
```
Resultado: 1 tradutor para Esperanto + 4 tradutores de Esperanto = reutilização!

Da mesma forma, código intermediário age como uma "língua franca" entre frontend e backends.

### Objetivos deste Módulo

- Compreender diferentes representações intermediárias
- Dominar código de três endereços (Three-Address Code)
- Implementar tradutores dirigidos pela sintaxe
- Gerar código intermediário para expressões e comandos
- Aplicar otimizações básicas no código intermediário
- Desenvolver geradores de código completos em C

---

## Conceitos Fundamentais

### O que é Código Intermediário?

Código intermediário é uma representação do programa fonte que:
- É **mais abstrata** que código de máquina
- É **mais concreta** que AST
- É **independente** da arquitetura alvo
- É **fácil** de otimizar e transformar

**Entrada**: Árvore Sintática Abstrata (AST)
```
    (+)
   /   \
  x    (*)
       /  \
      y    z
```

**Saída**: Código Intermediário
```
t1 = y * z
t2 = x + t1
```

### Características Desejáveis

Um bom código intermediário deve ser:

1. **Fácil de produzir**: Tradução simples e direta da AST
2. **Fácil de traduzir**: Para código de máquina de qualquer arquitetura
3. **Compacto**: Representação eficiente em memória
4. **Expressivo**: Capaz de representar todas as construções da linguagem
5. **Adequado para otimizações**: Facilita análise e transformação

### Níveis de Representação

O código intermediário situa-se entre código fonte e código objeto:

```
Código Fonte (Alto Nível)
    ↓
Árvore Sintática Abstrata (AST)
    ↓
Código Intermediário ← ESTAMOS AQUI
    ↓
Código Assembly
    ↓
Código de Máquina (Baixo Nível)
```

### Vantagens da Abordagem Multi-Fase

**Arquitetura de Compilador com Código Intermediário**:

```
┌─────────────┐
│   Frontend  │  ← Análise (léxica, sintática, semântica)
└──────┬──────┘
       │ AST
       ↓
┌─────────────┐
│  Gerador IR │  ← ESTA FASE
└──────┬──────┘
       │ IR (Intermediate Representation)
       ↓
┌─────────────┐
│ Otimizador  │  ← Otimizações independentes de máquina
└──────┬──────┘
       │ IR otimizado
       ↓
┌─────────────┐
│   Backend   │  ← Geração de código de máquina
└─────────────┘
```

**Benefícios**:
- **N + M em vez de N × M**: Para N linguagens e M arquiteturas, precisa-se de N frontends + M backends = N + M componentes
- **Sem intermediário**: Precisaria de N × M compiladores completos
- **Exemplo**: LLVM tem 1 IR, dezenas de frontends (C, C++, Rust, Swift) e backends (x86, ARM, RISC-V)

---

## Fundamentos Teóricos

### Tradução Dirigida pela Sintaxe

**Donald Knuth** (1968) formalizou a **tradução dirigida pela sintaxe** através de gramáticas de atributos.

Uma **Definição Dirigida pela Sintaxe** (Syntax-Directed Definition - SDD) é uma gramática livre de contexto com:
- **Atributos** associados a símbolos gramaticais
- **Regras semânticas** que computam valores de atributos
- **Ações** que geram código

#### Esquema de Tradução

Um **Esquema de Tradução** (Translation Scheme) é uma gramática com **ações** embutidas entre os símbolos:

```
E → T { E.code = T.code }
  | E₁ + T { temp = new_temp();
              emit(temp '=' E₁.place '+' T.place);
              E.place = temp;
              E.code = E₁.code || T.code || emit_code }
```

### Atributos Sintetizados e Herdados

**Atributos Sintetizados**:
- Fluem de baixo para cima na árvore (bottom-up)
- Calculados a partir dos filhos
- Exemplo: `tipo`, `valor`, `código gerado`

**Atributos Herdados**:
- Fluem de cima para baixo (top-down)
- Calculados a partir do pai ou irmãos
- Exemplo: `tipo esperado`, `rótulo de saída`

**Teorema**: Toda gramática S-atribuída (apenas atributos sintetizados) pode ser avaliada em uma passagem bottom-up.

---

## Representações Intermediárias

Existem diversas formas de representar código intermediário. Cada uma tem vantagens específicas.

### 1. Código de Três Endereços (Three-Address Code - TAC)

**Representação mais usada** na prática! Cada instrução tem no máximo:
- 1 operador
- 2 operandos (endereços)
- 1 resultado

**Forma geral**:
```
resultado = operando1 operador operando2
```

**Expressão**: `a = b * c + d * e`

**Código de Três Endereços**:
```
t1 = b * c
t2 = d * e
t3 = t1 + t2
a = t3
```

**Características**:
- ✅ Fácil de entender e implementar
- ✅ Próximo de assembly
- ✅ Ideal para otimizações
- ✅ Facilita geração de código de máquina

### 2. Notação Pós-Fixa (Reverse Polish Notation - RPN)

**Expressão**: `(a + b) * (c - d)`
**Notação Pós-Fixa**: `a b + c d - *`

Muito compacta, mas difícil de otimizar.

### 3. Representação em Grafo (DAG)

DAG (Grafo Acíclico Direcionado) elimina redundâncias, representando subexpressões comuns uma única vez.

---

## Código de Três Endereços

### Formato Geral

Código de três endereços (TAC) consiste em uma sequência de instruções da forma:

```
x = y op z
```

Onde:
- `x`, `y`, `z` são **endereços** (variáveis, constantes ou temporários)
- `op` é um **operador** aritmético ou lógico

### Tipos de Instruções

#### 1. Atribuição de Expressão Binária
```
x = y op z
```
Exemplos:
```
t1 = a + b
t2 = x * y
t3 = i - 1
```

#### 2. Atribuição de Cópia
```
x = y
```

#### 3. Saltos Incondicionais
```
goto L
```

#### 4. Saltos Condicionais
```
if x relop y goto L
```
Exemplos:
```
if a < b goto L1
if x == 0 goto L2
```

#### 5. Chamadas de Função
```
param x
call p, n
x = call p, n
```

### Representação em C

```c
typedef enum {
    TAC_ADD, TAC_SUB, TAC_MUL, TAC_DIV,
    TAC_COPY, TAC_LABEL, TAC_GOTO,
    TAC_IF_LT, TAC_IF_GT, TAC_IF_EQ,
    TAC_PARAM, TAC_CALL, TAC_RETURN
} TACOp;

typedef struct TACInstr {
    TACOp op;
    char* result;
    char* arg1;
    char* arg2;
    struct TACInstr* next;
} TACInstr;
```

### Temporários e Rótulos

**Temporários** (`t1`, `t2`, `t3`...):
- Armazenam resultados intermediários
- Criados pelo compilador
- Podem ser eliminados via alocação de registradores

**Gerador de temporários**:
```c
int temp_count = 0;

char* new_temp() {
    char* temp = malloc(10);
    sprintf(temp, "t%d", temp_count++);
    return temp;
}
```

**Rótulos** (`L1`, `L2`, `L3`...):
- Marcam posições para saltos
- Implementam controle de fluxo

**Gerador de rótulos**:
```c
int label_count = 0;

char* new_label() {
    char* label = malloc(10);
    sprintf(label, "L%d", label_count++);
    return label;
}
```

---

## Geração de Código para Expressões

### Tradução de Expressões Aritméticas

Para traduzir `a + b * c`:

**AST**:
```
      (+)
     /   \
    a    (*)
         /  \
        b    c
```

**TAC gerado**:
```
t1 = b * c
t2 = a + t1
```

### Implementação em C

```c
typedef struct {
    char* place;
} ExprResult;

ExprResult* translate_expression(ASTNode* node) {
    ExprResult* result = malloc(sizeof(ExprResult));
    
    switch (node->type) {
        case NODE_NUMBER:
            result->place = strdup(node->value);
            break;
            
        case NODE_IDENTIFIER:
            result->place = strdup(node->value);
            break;
            
        case NODE_BINARY_OP: {
            ExprResult* left = translate_expression(node->left);
            ExprResult* right = translate_expression(node->right);
            
            char* temp = new_temp();
            TACOp op = get_tac_op(node->value);
            emit(op, temp, left->place, right->place);
            
            result->place = temp;
            break;
        }
    }
    
    return result;
}
```

---

## Geração de Código para Comandos de Controle

### Comando if-then

**Código fonte**:
```c
if (a < b) {
    x = 1;
}
```

**TAC gerado**:
```
    if a < b goto L1
    goto L2
L1: x = 1
L2:
```

### Comando while

**Código fonte**:
```c
while (a < b) {
    a = a + 1;
}
```

**TAC gerado**:
```
L1: if a < b goto L2
    goto L3
L2: t1 = a + 1
    a = t1
    goto L1
L3:
```

### Implementação em C

```c
void translate_while_stmt(ASTNode* cond, ASTNode* body) {
    char* L1 = new_label();  // início
    char* L2 = new_label();  // corpo
    char* L3 = new_label();  // saída
    
    emit(TAC_LABEL, L1, NULL, NULL);
    
    ExprResult* cond_result = translate_expression(cond);
    emit(TAC_IF_EQ, L2, cond_result->place, "1");
    emit(TAC_GOTO, L3, NULL, NULL);
    
    emit(TAC_LABEL, L2, NULL, NULL);
    translate_statement(body);
    emit(TAC_GOTO, L1, NULL, NULL);
    
    emit(TAC_LABEL, L3, NULL, NULL);
}
```

---

## Otimizações no Código Intermediário

### 1. Eliminação de Subexpressões Comuns (CSE)

**Antes**:
```
t1 = b * c
t2 = t1 + d
a = t2
t3 = b * c    // redundante!
t4 = t3 + f
e = t4
```

**Depois**:
```
t1 = b * c
t2 = t1 + d
a = t2
t4 = t1 + f   // reutiliza t1
e = t4
```

### 2. Propagação de Constantes

**Antes**:
```
x = 5
t1 = x + 3
y = t1
```

**Depois**:
```
x = 5
y = 8
```

### 3. Dobramento de Constantes (Constant Folding)

**Antes**:
```
t1 = 3 * 4
t2 = 2 + t1
x = t2
```

**Depois**:
```
x = 14
```

---

## Exemplos Práticos

### Exemplo 1: Expressão Simples

**Código**: `resultado = a + b * c - d`

**TAC**:
```
t1 = b * c
t2 = a + t1
t3 = t2 - d
resultado = t3
```

### Exemplo 2: Condicional

**Código**:
```c
if (x > 0) {
    y = x * 2;
} else {
    y = x * -1;
}
```

**TAC**:
```
    if x > 0 goto L1
    goto L2
L1: t1 = x * 2
    y = t1
    goto L3
L2: t2 = 0 - x
    y = t2
L3:
```

### Exemplo 3: Loop

**Código**:
```c
soma = 0;
i = 1;
while (i <= n) {
    soma = soma + i;
    i = i + 1;
}
```

**TAC**:
```
    soma = 0
    i = 1
L1: if i <= n goto L2
    goto L3
L2: t1 = soma + i
    soma = t1
    t2 = i + 1
    i = t2
    goto L1
L3:
```

---

## Como Compilar e Usar

### Estrutura do Projeto

```
src/12-geracao-codigo-intermediario/
├── exemploSimples.c        # Gerador básico de TAC
├── exemploCompleto.c       # Gerador completo de TAC
└── README.md               # Este arquivo
```

### Compilação

```bash
# Compilar o exemplo simples
gcc exemploSimples.c -o exemploSimples -std=c99

# Executar
./exemploSimples
```

### Saída Esperada

```
=== EXEMPLO 1: (a + b) * c ===

=== CÓDIGO DE TRÊS ENDEREÇOS ===
  t0 = a + b
  t1 = t0 * c
  resultado = t1

Temporários usados: 2
```

---

## Referências Acadêmicas

### Livros-Texto Fundamentais

1. **Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D.** (2006). *Compiladores: Princípios, Técnicas e Ferramentas*. 2ª Edição. Pearson Education.
   - **Capítulo 6**: Geração de Código Intermediário
   - **Capítulo 8**: Geração de Código
   - Tradução dirigida pela sintaxe
   - Código de três endereços
   - **Essencial**: Referência definitiva

2. **Appel, A. W.** (2002). *Modern Compiler Implementation in C*. Cambridge University Press.
   - **Capítulo 7**: Intermediate Representation
   - Tradução para árvores IR
   - Implementação prática em C

3. **Cooper, K. D., & Torczon, L.** (2011). *Engineering a Compiler*. 2ª Edição. Morgan Kaufmann.
   - **Capítulo 5**: Intermediate Representations
   - Comparação entre diferentes IRs
   - SSA (Static Single Assignment)

4. **Muchnick, S. S.** (1997). *Advanced Compiler Design and Implementation*. Morgan Kaufmann.
   - **Capítulo 4**: Intermediate Representations
   - Control Flow Graphs (CFG)
   - Data Flow Analysis

### Artigos Seminais

5. **Cytron, R., et al.** (1991). "Efficiently Computing Static Single Assignment Form". *ACM TOPLAS*, 13(4), 451-490.
   - Introdução de SSA
   - Algoritmo de construção

6. **Lattner, C., & Adve, V.** (2004). "LLVM: A Compilation Framework". *CGO*.
   - LLVM IR moderna
   - Arquitetura retargetável

7. **Knuth, D. E.** (1968). "Semantics of Context-Free Languages". *Mathematical Systems Theory*, 2(2), 127-145.
   - Gramáticas de atributos
   - Base teórica

### Recursos Online

8. **LLVM Language Reference**: https://llvm.org/docs/LangRef.html
   - Documentação de LLVM IR
   - Exemplos práticos

9. **GCC Internals - RTL**: https://gcc.gnu.org/onlinedocs/gccint/RTL.html
   - Register Transfer Language
   - IR de baixo nível

10. **Stanford CS143**: http://web.stanford.edu/class/cs143/
    - Materiais de curso
    - Slides e exercícios

---

## Exercícios Propostos

### Básicos

1. **Gerador simples**: Implemente gerador de TAC para expressões aritméticas
2. **Contador**: Adicione estatísticas sobre temporários usados
3. **Operadores unários**: Adicione suporte para -, !

### Intermediários

4. **Comandos if/while**: Implemente geração para estruturas de controle
5. **Arrays**: Adicione suporte para indexação
6. **Funções**: Implemente chamadas de função
7. **Otimizações**: Implemente constant folding

### Avançados

8. **SSA**: Converta TAC para forma SSA
9. **CFG**: Construa Control Flow Graph
10. **Liveness**: Implemente análise de variáveis vivas
11. **Dead code**: Elimine código morto

### Desafios

12. **Short-circuit**: Implemente avaliação por curto-circuito
13. **Loop optimization**: Implemente loop invariant code motion
14. **Inline**: Substitua chamadas de funções pequenas
15. **Assembly**: Gere código assembly a partir do TAC

---

## Aplicações Reais

### 1. Compiladores Modernos

- **GCC**: Usa RTL (Register Transfer Language)
- **LLVM**: LLVM IR em SSA
- **JVM**: Bytecode Java

### 2. Máquinas Virtuais

- **Python**: CPython bytecode
- **.NET**: CIL (Common Intermediate Language)
- **JavaScript**: V8, SpiderMonkey

### 3. Análise de Segurança

- Verificação formal via IR
- Análise estática (Coverity)
- Symbolic execution

### 4. Compiladores JIT

- JavaScript engines (V8)
- LuaJIT
- Java HotSpot

---

## Resumo dos Conceitos-Chave

| Conceito | Descrição | Importância |
|----------|-----------|-------------|
| **TAC** | Instruções com ≤2 operandos | Formato padrão |
| **Temporários** | Variáveis auxiliares | Resultados intermediários |
| **Rótulos** | Marcadores para saltos | Controle de fluxo |
| **SDD** | Tradução dirigida pela sintaxe | Método sistemático |
| **IR** | Representação intermediária | Portabilidade |
| **Otimizações** | Transformações de código | Eficiência |

---

## Conclusão

A geração de código intermediário é uma fase **essencial** no processo de compilação moderna. Ela fornece uma ponte entre a análise de alto nível do código fonte e a geração de código de máquina.

**Principais takeaways**:

1. ✅ **Separação**: Frontend e backend independentes
2. ✅ **Portabilidade**: Um frontend para N backends
3. ✅ **Otimizações**: Ponto natural para transformações
4. ✅ **Simplicidade**: Mais fácil que assembly
5. ✅ **Flexibilidade**: Múltiplos níveis possíveis

Com os conceitos e implementações deste módulo, você está preparado para:
- Implementar geradores de código intermediário completos
- Entender arquiteturas de compiladores modernos
- Criar linguagens com backends reutilizáveis
- Aplicar otimizações em nível intermediário
- Desenvolver compiladores retargetáveis

**Próximos passos**: Otimizações avançadas e geração de código de máquina!

---

*Este material faz parte da disciplina de Compiladores. Para questões ou sugestões, consulte o repositório do curso.*
