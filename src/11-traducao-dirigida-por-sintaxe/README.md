# 11 — Tradução Dirigida por Sintaxe

## Índice
1. [Introdução](#introdução)
2. [Conceitos Fundamentais](#conceitos-fundamentais)
3. [Fundamentos Teóricos](#fundamentos-teóricos)
4. [Gramáticas de Atributos](#gramáticas-de-atributos)
5. [Atributos Sintetizados e Herdados](#atributos-sintetizados-e-herdados)
6. [Definições S-Atribuídas e L-Atribuídas](#definições-s-atribuídas-e-l-atribuídas)
7. [Avaliação de Atributos](#avaliação-de-atributos)
8. [Implementação em C](#implementação-em-c)
9. [Aplicações Práticas](#aplicações-práticas)
10. [Exemplos Práticos](#exemplos-práticos)
11. [Como Compilar e Usar](#como-compilar-e-usar)
12. [Referências Acadêmicas](#referências-acadêmicas)

---

## Introdução

A **Tradução Dirigida por Sintaxe** (Syntax-Directed Translation - SDT) é uma técnica fundamental em compiladores que associa **ações semânticas** à estrutura sintática de uma linguagem. Enquanto a análise sintática valida a estrutura gramatical do código-fonte, a tradução dirigida por sintaxe permite que **ações computacionais** sejam executadas durante ou após o parsing, produzindo uma representação traduzida do programa.

### Importância da Tradução Dirigida por Sintaxe

Segundo **Aho, Lam, Sethi e Ullman** (2006), no clássico "Compiladores: Princípios, Técnicas e Ferramentas":

> "A tradução dirigida por sintaxe é feita associando regras ou fragmentos de programa com as produções de uma gramática. Uma gramática com regras semânticas associadas é chamada de definição dirigida por sintaxe."

A tradução dirigida por sintaxe é essencial porque:
- **Conecta sintaxe e semântica**: Liga a estrutura gramatical às ações necessárias
- **Permite geração de código**: Produz código intermediário ou de máquina durante o parsing
- **Facilita verificações**: Realiza cálculos de tipos, verificação de compatibilidade
- **Suporta otimizações**: Permite transformações durante a construção da árvore
- **Generaliza o processo**: Fornece framework para diversas traduções

### Analogia com Linguagem Natural

Considere a tradução de uma frase em português para inglês:

**Português**: "O compilador analisa o código"
**Inglês**: "The compiler analyzes the code"

A tradução não é apenas uma substituição palavra por palavra:
- **Análise sintática**: Identifica sujeito, verbo, objeto
- **Ações semânticas**: Para cada componente identificado, gera equivalente em inglês
- **Composição**: Monta frase traduzida respeitando gramática do inglês

Da mesma forma, tradução dirigida por sintaxe em compiladores:
```
Expressão em C:     x = a + b * 3;
                       ↓
Análise sintática:  [Atribuição: x ← [+: a, [*: b, 3]]]
                       ↓
Ações semânticas:   Gerar código intermediário
                       ↓
Código traduzido:   t1 = b * 3
                    t2 = a + t1
                    x = t2
```

### Objetivos deste Módulo

- Compreender gramáticas de atributos e suas aplicações
- Dominar atributos sintetizados e herdados
- Implementar esquemas de tradução em C
- Construir tradutores para expressões aritméticas
- Gerar código intermediário durante parsing
- Aplicar tradução dirigida por sintaxe em problemas reais

---

## Conceitos Fundamentais

### O que é Tradução Dirigida por Sintaxe?

Tradução dirigida por sintaxe é o processo de computar valores ou executar ações **associados aos nós de uma árvore de análise sintática**, seguindo a estrutura da gramática.

**Componentes principais**:

1. **Gramática livre de contexto**: Define a sintaxe da linguagem fonte
2. **Atributos**: Valores associados a símbolos gramaticais
3. **Regras semânticas**: Especificam como calcular atributos
4. **Esquema de tradução**: Especifica ordem das ações

### Diferença entre Análise e Tradução

| Aspecto | Análise Sintática | Tradução Dirigida por Sintaxe |
|---------|------------------|------------------------------|
| **Objetivo** | Verificar estrutura gramatical | Produzir saída traduzida |
| **Entrada** | Tokens | Tokens |
| **Saída** | Árvore sintática / Sucesso ou erro | Código traduzido / Valores calculados |
| **Ações** | Reconhecimento de padrões | Computação de valores, geração de código |
| **Exemplo** | Reconhece `3 + 4 * 5` | Gera código para calcular `3 + 4 * 5` |

### Fases de Compilação e SDT

A tradução dirigida por sintaxe aparece em várias fases do compilador:

```
┌─────────────────────────┐
│  Código Fonte           │
└───────────┬─────────────┘
            │
            v
┌─────────────────────────┐
│  Análise Léxica         │
└───────────┬─────────────┘
            │ tokens
            v
┌─────────────────────────┐
│  Análise Sintática      │←─── SDT AQUI: Construção de AST
│  + SDT                  │     Anotação de tipos
└───────────┬─────────────┘     Verificações básicas
            │ AST anotada
            v
┌─────────────────────────┐
│  Análise Semântica      │←─── SDT AQUI: Verificação de tipos
│  + SDT                  │     Análise de escopo
└───────────┬─────────────┘     Cálculo de endereços
            │ AST verificada
            v
┌─────────────────────────┐
│  Geração de Código      │←─── SDT AQUI: Geração de código
│  Intermediário + SDT    │     intermediário
└───────────┬─────────────┘
            │ código intermediário
            v
┌─────────────────────────┐
│  Otimização             │
└───────────┬─────────────┘
            │
            v
┌─────────────────────────┐
│  Geração de Código      │
│  de Máquina             │
└─────────────────────────┘
```

### Exemplo Introdutório

**Problema**: Calcular o valor de uma expressão aritmética

**Gramática**:
```
E → E + T    { E.val = E.val + T.val }
E → T        { E.val = T.val }
T → T * F    { T.val = T.val * F.val }
T → F        { T.val = F.val }
F → (E)      { F.val = E.val }
F → num      { F.val = num.lexval }
```

**Para a entrada** `3 + 4 * 5`:

```
              E.val = 23
             / | \
       E.val=3 + T.val=20
          |      / | \
       T.val=3  T  * F.val=5
          |     |     |
       F.val=3  F.val=4  num
          |     |         5
        num   num
         3     4
```

Cada nó tem um **atributo** `.val` calculado a partir dos filhos!

---

## Fundamentos Teóricos

### Definição Formal

Uma **Definição Dirigida por Sintaxe** (Syntax-Directed Definition - SDD) é uma generalização de uma gramática livre de contexto onde:

**Formalmente**, uma SDD é uma tripla (G, A, R) onde:
- **G = (N, T, P, S)**: Gramática livre de contexto
- **A**: Conjunto de atributos associados aos símbolos gramaticais
- **R**: Conjunto de regras semânticas, uma para cada produção

### Atributos

Um **atributo** é um valor associado a um símbolo gramatical (terminal ou não-terminal). Pode representar:

- **Tipos**: `int`, `float`, `bool`
- **Valores**: `42`, `3.14`, `true`
- **Código**: Sequência de instruções
- **Informações contextuais**: Escopo, tabela de símbolos
- **Propriedades sintáticas**: Número de declarações, profundidade

**Notação**: `X.a` denota o atributo `a` do símbolo `X`

### Regras Semânticas

Uma **regra semântica** define como calcular o valor de um atributo. Associada a uma produção, especifica:
- Quais atributos depende
- Como computar o novo valor
- Efeitos colaterais (geração de código, impressão)

**Exemplo**:
```
Produção:        L → E n
Regra semântica: print(E.val)
```

### Grafos de Dependência

Um **grafo de dependência** mostra as relações entre atributos em uma árvore de análise:
- **Nós**: Atributos dos símbolos na árvore
- **Arestas**: Dependências (A → B significa "B depende de A")

**Ordem de avaliação**: Ordenação topológica do grafo de dependência.

**Exemplo** para `E → E₁ + T`:
```
E₁.val ──→ E.val ←── T.val
```

O atributo `E.val` **depende** de `E₁.val` e `T.val`.

---

## Gramáticas de Atributos

### Definição

Uma **gramática de atributos** é uma gramática livre de contexto aumentada com:
1. **Conjunto de atributos** para cada símbolo
2. **Regras semânticas** para cada produção

**Donald Knuth** (1968) formalizou o conceito em seu artigo seminal "Semantics of Context-Free Languages".

### Estrutura de uma Gramática de Atributos

**Gramática** para expressões aritméticas com avaliação:

```
Produção              Regras Semânticas
────────────────────  ──────────────────────────────
E → E₁ + T            E.val = E₁.val + T.val
E → E₁ - T            E.val = E₁.val - T.val
E → T                 E.val = T.val

T → T₁ * F            T.val = T₁.val * F.val
T → T₁ / F            T.val = T₁.val / F.val
T → F                 T.val = F.val

F → (E)               F.val = E.val
F → num               F.val = num.lexval
```

### Exemplo Completo: Avaliação de `2 + 3 * 4`

**Árvore de análise com atributos**:

```
                    E.val = 14
                   / |  \
           E.val=2 +   T.val=12
              |         / |  \
           T.val=2  T.val=3 * F.val=4
              |        |        |
           F.val=2  F.val=3   num
              |        |        4
            num      num
             2        3
```

**Cálculo dos atributos** (ordem bottom-up):
1. `num (2) → F.val = 2`
2. `F → T: T.val = 2`
3. `T → E: E₁.val = 2`
4. `num (3) → F.val = 3`
5. `F → T₁: T₁.val = 3`
6. `num (4) → F.val = 4`
7. `T₁ * F: T.val = 3 * 4 = 12`
8. `E₁ + T: E.val = 2 + 12 = 14`

### Tipos de Gramáticas de Atributos

1. **S-atribuídas**: Apenas atributos sintetizados
2. **L-atribuídas**: Atributos sintetizados + herdados com restrições
3. **Gerais**: Sem restrições (podem ter circularidades)

---

## Atributos Sintetizados e Herdados

### Atributos Sintetizados

Um atributo é **sintetizado** se seu valor é calculado **a partir dos atributos dos filhos** na árvore de análise.

**Direção**: Bottom-up (das folhas para a raiz)

**Exemplo**: Cálculo de expressões
```
E → E₁ + T    { E.val = E₁.val + T.val }
              ↑         ↑          ↑
           calculado  usa filhos  usa filhos
```

**Características**:
- Fáceis de implementar em parsers bottom-up
- Podem ser calculados durante o parsing
- Não dependem do contexto "acima" na árvore
- Representam informações que "sobem" na árvore

### Atributos Herdados

Um atributo é **herdado** se seu valor é calculado **a partir dos atributos do pai ou irmãos** na árvore de análise.

**Direção**: Top-down (da raiz para as folhas)

**Exemplo**: Propagação de tipos
```
D → T L       { L.tipo = T.tipo }
              ↓
          passa tipo para baixo
```

**Características**:
- Naturais em parsers descendentes recursivos
- Representam informações contextuais
- Propagam informações "para baixo" na árvore
- Úteis para verificação de tipos, escopo

### Comparação

| Aspecto | Sintetizado | Herdado |
|---------|------------|---------|
| **Direção** | ↑ Bottom-up | ↓ Top-down |
| **Depende de** | Filhos | Pai ou irmãos |
| **Calculado em** | Redução (bottom-up) | Derivação (top-down) |
| **Usado para** | Valores, código gerado | Contexto, tipos esperados |
| **Exemplo típico** | Valor de expressão | Tipo de declaração |

### Exemplo Combinando Ambos

**Problema**: Declarações com tipo propagado

```
Produção              Regras Semânticas
────────────────────  ──────────────────────────────
D → T L               L.tipo = T.tipo         (herdado)
T → int               T.tipo = "int"          (sintetizado)
T → float             T.tipo = "float"        (sintetizado)
L → L₁, id            L₁.tipo = L.tipo        (herdado)
                      addtipo(id, L.tipo)
L → id                addtipo(id, L.tipo)
```

**Para** `int a, b, c`:

```
              D
             / \
         T      L.tipo="int"
         |      / \
       "int"   L₁  , id(c)
               |   tipo="int"
              / \
           L₂  , id(b)
            |  tipo="int"
          id(a)
        tipo="int"
```

O tipo `"int"` é **sintetizado** de `T` e depois **herdado** por `L`, `L₁`, `L₂`, propagando para todas as variáveis.

---

## Definições S-Atribuídas e L-Atribuídas

### Definições S-Atribuídas

Uma SDD é **S-atribuída** se todos os atributos são **sintetizados**.

**Propriedades**:
- Avaliação simples: ordem pós-ordem na árvore
- Compatível com parsers LR e bottom-up
- Não requer pilha de atributos separada
- Eficiente para implementar

**Teorema**: Toda definição S-atribuída pode ser implementada durante parsing bottom-up sem construir árvore explícita.

**Exemplo**: Calculadora de expressões (vista anteriormente)

### Definições L-Atribuídas

Uma SDD é **L-atribuída** se para cada produção `A → X₁ X₂ ... Xₙ`, cada atributo herdado de `Xⱼ` depende apenas de:
1. Atributos de `X₁, X₂, ..., Xⱼ₋₁` (símbolos à esquerda)
2. Atributos herdados de `A`

**L** vem de **Left-to-right** (esquerda para direita).

**Propriedades**:
- Avaliação em profundidade, esquerda para direita
- Compatível com parsers LL e descendentes recursivos
- Pode ser implementada com uma passagem pela árvore
- Mais expressiva que S-atribuída

**Restrições**:
❌ **Proibido**: `A → X Y { X.h = Y.s }` (X depende de Y à direita)
✅ **Permitido**: `A → X Y { Y.h = X.s }` (Y depende de X à esquerda)

**Exemplo**: Declarações de tipo (vista anteriormente)

### Comparação

```
S-atribuída ⊂ L-atribuída ⊂ Gramáticas de Atributos Gerais
  (mais restrita)           (mais expressiva)
```

| Característica | S-Atribuída | L-Atribuída |
|---------------|------------|-------------|
| **Atributos** | Apenas sintetizados | Sintetizados + herdados (restritos) |
| **Avaliação** | Pós-ordem | Profundidade (L-R) |
| **Parsers** | LR, Bottom-up | LL, Descendente recursivo |
| **Complexidade** | Simples | Moderada |
| **Expressividade** | Limitada | Maior |

---

## Avaliação de Atributos

### Ordem de Avaliação

Para avaliar atributos corretamente, é necessário respeitar as **dependências** entre eles.

**Algoritmo geral**:
1. Construir **grafo de dependência** dos atributos
2. Verificar se é **acíclico** (sem circularidades)
3. Fazer **ordenação topológica**
4. Avaliar atributos nessa ordem

### Métodos de Avaliação

#### 1. Avaliação em Árvore de Análise

Constrói árvore completa, depois avalia atributos.

**Vantagens**:
- Flexível, suporta qualquer ordem
- Fácil de entender e depurar

**Desvantagens**:
- Consome muita memória
- Requer duas passagens (parse + avaliação)

#### 2. Avaliação Durante o Parsing (On-the-fly)

**Para S-atribuídas**: Usar pilha semântica em parser bottom-up

```c
// Pilha de valores
int value_stack[MAX];
int val_top = 0;

void reduce_E_plus_T() {
    // E → E + T
    int T_val = value_stack[val_top--];
    int E1_val = value_stack[val_top--];
    int E_val = E1_val + T_val;
    value_stack[++val_top] = E_val;
}
```

**Para L-atribuídas**: Passar atributos como parâmetros em parser descendente

```c
void parse_L(char* tipo_herdado) {
    // L → id
    Token id = expect(TOKEN_ID);
    addtipo(id.lexeme, tipo_herdado);
}

void parse_D() {
    // D → T L
    char* tipo = parse_T();  // retorna tipo sintetizado
    parse_L(tipo);           // passa tipo como herdado
}
```

#### 3. Avaliação em Múltiplas Passagens

Algumas gramáticas requerem múltiplas passagens:
- Primeira: construir estrutura básica
- Subsequentes: refinar informações

**Exemplo**: Verificação de tipos com forward references

### Circularidades

Uma gramática tem **circularidade** se o grafo de dependência contém **ciclos**.

**Exemplo problemático**:
```
A → B    { A.val = B.val }
B → A    { B.val = A.val }
```

**Circularidade**: `A.val → B.val → A.val` (ciclo!)

**Solução**: Reformular gramática ou usar valor padrão.

---

## Implementação em C

### Estrutura Básica

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ========== TOKENS ==========
typedef enum {
    TOKEN_NUM, TOKEN_PLUS, TOKEN_MINUS,
    TOKEN_MULT, TOKEN_DIV,
    TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    int value;  // Para números
} Token;

// ========== LEXER ==========
char* input;
int pos = 0;

Token next_token() {
    while (input[pos] == ' ') pos++;
    
    Token tok;
    if (input[pos] == '\0') {
        tok.type = TOKEN_EOF;
        return tok;
    }
    
    if (isdigit(input[pos])) {
        tok.type = TOKEN_NUM;
        tok.value = 0;
        while (isdigit(input[pos])) {
            tok.value = tok.value * 10 + (input[pos] - '0');
            pos++;
        }
        return tok;
    }
    
    switch (input[pos++]) {
        case '+': tok.type = TOKEN_PLUS; break;
        case '-': tok.type = TOKEN_MINUS; break;
        case '*': tok.type = TOKEN_MULT; break;
        case '/': tok.type = TOKEN_DIV; break;
        case '(': tok.type = TOKEN_LPAREN; break;
        case ')': tok.type = TOKEN_RPAREN; break;
    }
    
    return tok;
}

Token current_token;

void advance() {
    current_token = next_token();
}

// ========== PARSER COM SDT (S-ATRIBUÍDO) ==========

// Declarações forward
int parse_expr();
int parse_term();
int parse_factor();

// E → T E'
// E' → + T E' | - T E' | ε
int parse_expr() {
    int val = parse_term();
    
    while (current_token.type == TOKEN_PLUS || 
           current_token.type == TOKEN_MINUS) {
        TokenType op = current_token.type;
        advance();
        int right = parse_term();
        
        // Ação semântica: calcular valor
        if (op == TOKEN_PLUS) {
            val = val + right;
        } else {
            val = val - right;
        }
    }
    
    return val;  // Atributo sintetizado
}

// T → F T'
// T' → * F T' | / F T' | ε
int parse_term() {
    int val = parse_factor();
    
    while (current_token.type == TOKEN_MULT || 
           current_token.type == TOKEN_DIV) {
        TokenType op = current_token.type;
        advance();
        int right = parse_factor();
        
        // Ação semântica: calcular valor
        if (op == TOKEN_MULT) {
            val = val * right;
        } else {
            if (right == 0) {
                fprintf(stderr, "Erro: divisão por zero\n");
                exit(1);
            }
            val = val / right;
        }
    }
    
    return val;  // Atributo sintetizado
}

// F → num | (E)
int parse_factor() {
    if (current_token.type == TOKEN_NUM) {
        int val = current_token.value;
        advance();
        return val;  // Atributo sintetizado do terminal
    }
    
    if (current_token.type == TOKEN_LPAREN) {
        advance();  // consome '('
        int val = parse_expr();
        if (current_token.type != TOKEN_RPAREN) {
            fprintf(stderr, "Erro: esperado ')'\n");
            exit(1);
        }
        advance();  // consome ')'
        return val;
    }
    
    fprintf(stderr, "Erro sintático\n");
    exit(1);
}

// ========== MAIN ==========
int main() {
    printf("=== CALCULADORA COM SDT ===\n");
    printf("Digite expressões aritméticas.\n");
    printf("Exemplo: 3 + 4 * 5\n\n");
    
    char line[256];
    while (1) {
        printf(">>> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        
        // Remover \n
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;
        
        input = line;
        pos = 0;
        advance();  // Carregar primeiro token
        
        int resultado = parse_expr();
        
        if (current_token.type != TOKEN_EOF) {
            printf("Aviso: caracteres extras após expressão\n");
        }
        
        printf("Resultado: %d\n\n", resultado);
    }
    
    return 0;
}
```

### Exemplo com Geração de Código

Agora vamos estender para **gerar código intermediário de três endereços**:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ========== CÓDIGO DE TRÊS ENDEREÇOS ==========
typedef struct {
    char op;        // '+', '-', '*', '/'
    char* arg1;     // Primeiro argumento
    char* arg2;     // Segundo argumento
    char* result;   // Resultado (temporário)
} Instruction;

#define MAX_INSTRUCTIONS 100
Instruction code[MAX_INSTRUCTIONS];
int code_count = 0;

int temp_count = 0;

char* new_temp() {
    char* temp = malloc(10);
    sprintf(temp, "t%d", temp_count++);
    return temp;
}

void emit(char op, char* arg1, char* arg2, char* result) {
    code[code_count].op = op;
    code[code_count].arg1 = strdup(arg1);
    code[code_count].arg2 = strdup(arg2);
    code[code_count].result = strdup(result);
    code_count++;
}

void print_code() {
    printf("\n=== CÓDIGO INTERMEDIÁRIO GERADO ===\n");
    for (int i = 0; i < code_count; i++) {
        printf("%s = %s %c %s\n", 
               code[i].result,
               code[i].arg1,
               code[i].op,
               code[i].arg2);
    }
}

// ========== TOKENS (mesmo do exemplo anterior) ==========
typedef enum {
    TOKEN_NUM, TOKEN_ID, TOKEN_PLUS, TOKEN_MINUS,
    TOKEN_MULT, TOKEN_DIV, TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_ASSIGN, TOKEN_SEMI, TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[32];
    int value;
} Token;

// Variáveis globais para lexer
char* input;
int pos = 0;
Token current_token;

// ========== LEXER ==========
Token next_token() {
    while (input[pos] == ' ' || input[pos] == '\t') pos++;
    
    Token tok;
    tok.lexeme[0] = '\0';
    
    if (input[pos] == '\0') {
        tok.type = TOKEN_EOF;
        return tok;
    }
    
    if (isdigit(input[pos])) {
        tok.type = TOKEN_NUM;
        tok.value = 0;
        int i = 0;
        while (isdigit(input[pos])) {
            tok.lexeme[i++] = input[pos];
            tok.value = tok.value * 10 + (input[pos] - '0');
            pos++;
        }
        tok.lexeme[i] = '\0';
        return tok;
    }
    
    if (isalpha(input[pos])) {
        tok.type = TOKEN_ID;
        int i = 0;
        while (isalnum(input[pos])) {
            tok.lexeme[i++] = input[pos++];
        }
        tok.lexeme[i] = '\0';
        return tok;
    }
    
    switch (input[pos++]) {
        case '+': tok.type = TOKEN_PLUS; strcpy(tok.lexeme, "+"); break;
        case '-': tok.type = TOKEN_MINUS; strcpy(tok.lexeme, "-"); break;
        case '*': tok.type = TOKEN_MULT; strcpy(tok.lexeme, "*"); break;
        case '/': tok.type = TOKEN_DIV; strcpy(tok.lexeme, "/"); break;
        case '(': tok.type = TOKEN_LPAREN; strcpy(tok.lexeme, "("); break;
        case ')': tok.type = TOKEN_RPAREN; strcpy(tok.lexeme, ")"); break;
        case '=': tok.type = TOKEN_ASSIGN; strcpy(tok.lexeme, "="); break;
        case ';': tok.type = TOKEN_SEMI; strcpy(tok.lexeme, ";"); break;
    }
    
    return tok;
}

void advance() {
    current_token = next_token();
}

// ========== PARSER COM GERAÇÃO DE CÓDIGO ==========

char* parse_expr();
char* parse_term();
char* parse_factor();

char* parse_expr() {
    char* addr = parse_term();
    
    while (current_token.type == TOKEN_PLUS || 
           current_token.type == TOKEN_MINUS) {
        char op = (current_token.type == TOKEN_PLUS) ? '+' : '-';
        advance();
        char* addr2 = parse_term();
        
        // Ação semântica: gerar código
        char* temp = new_temp();
        emit(op, addr, addr2, temp);
        addr = temp;
    }
    
    return addr;  // Retorna endereço do resultado
}

char* parse_term() {
    char* addr = parse_factor();
    
    while (current_token.type == TOKEN_MULT || 
           current_token.type == TOKEN_DIV) {
        char op = (current_token.type == TOKEN_MULT) ? '*' : '/';
        advance();
        char* addr2 = parse_factor();
        
        // Ação semântica: gerar código
        char* temp = new_temp();
        emit(op, addr, addr2, temp);
        addr = temp;
    }
    
    return addr;
}

char* parse_factor() {
    if (current_token.type == TOKEN_NUM) {
        char* addr = strdup(current_token.lexeme);
        advance();
        return addr;
    }
    
    if (current_token.type == TOKEN_ID) {
        char* addr = strdup(current_token.lexeme);
        advance();
        return addr;
    }
    
    if (current_token.type == TOKEN_LPAREN) {
        advance();
        char* addr = parse_expr();
        if (current_token.type != TOKEN_RPAREN) {
            fprintf(stderr, "Erro: esperado ')'\n");
            exit(1);
        }
        advance();
        return addr;
    }
    
    fprintf(stderr, "Erro sintático\n");
    exit(1);
}

void parse_assignment() {
    // id = expr ;
    if (current_token.type != TOKEN_ID) {
        fprintf(stderr, "Erro: esperado identificador\n");
        exit(1);
    }
    
    char* id = strdup(current_token.lexeme);
    advance();
    
    if (current_token.type != TOKEN_ASSIGN) {
        fprintf(stderr, "Erro: esperado '='\n");
        exit(1);
    }
    advance();
    
    char* addr = parse_expr();
    
    // Gerar código de atribuição
    Instruction instr;
    instr.op = '=';
    instr.arg1 = addr;
    instr.arg2 = "";
    instr.result = id;
    if (code_count >= MAX_INSTRUCTIONS) {
        fprintf(stderr, "Erro: limite de instruções excedido\n");
        exit(1);
    }
    code[code_count++] = instr;
    
    if (current_token.type != TOKEN_SEMI) {
        fprintf(stderr, "Erro: esperado ';'\n");
        exit(1);
    }
    advance();
}

int main() {
    printf("=== GERADOR DE CÓDIGO COM SDT ===\n");
    printf("Digite atribuições como: x = a + b * 3;\n\n");
    
    char line[256];
    while (1) {
        printf(">>> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;
        
        // Reset
        code_count = 0;
        temp_count = 0;
        input = line;
        pos = 0;
        advance();
        
        parse_assignment();
        
        print_code();
        printf("\n");
    }
    
    return 0;
}
```

**Exemplo de execução**:
```
>>> x = a + b * 3;

=== CÓDIGO INTERMEDIÁRIO GERADO ===
t0 = b * 3
t1 = a + t0
x = t1
```

---

## Aplicações Práticas

### 1. Avaliação de Expressões

**Problema**: Calculadora que avalia expressões aritméticas.

**Gramática S-atribuída**:
```
E → E + T    { E.val = E₁.val + T.val }
E → T        { E.val = T.val }
T → T * F    { T.val = T₁.val * F.val }
T → F        { T.val = F.val }
F → (E)      { F.val = E.val }
F → num      { F.val = num.valor }
```

**Aplicação**: Calculadoras, planilhas eletrônicas, intérpretes.

### 2. Verificação de Tipos

**Problema**: Verificar compatibilidade de tipos em expressões.

**Gramática com atributos de tipo**:
```
E → E₁ + E₂   { if (E₁.tipo == int && E₂.tipo == int)
                   E.tipo = int
                else
                   erro("tipos incompatíveis") }

E → num       { E.tipo = int }
E → real      { E.tipo = float }
```

**Aplicação**: Compiladores, IDEs com análise em tempo real.

### 3. Geração de Código Intermediário

**Problema**: Traduzir expressões para código de três endereços.

**Esquema de tradução**:
```
E → E₁ + E₂   { E.addr = new_temp()
                emit(E.addr "=" E₁.addr "+" E₂.addr) }

E → id        { E.addr = id.lexeme }
```

**Aplicação**: Compiladores otimizadores, JITs.

### 4. Contagem de Construções

**Problema**: Contar número de nós de cada tipo em uma árvore.

```
E → E + T     { E.count = E₁.count + T.count + 1 }
E → T         { E.count = T.count }
T → num       { T.count = 1 }
```

**Aplicação**: Métricas de código, análise de complexidade.

### 5. Pretty Printing

**Problema**: Formatar código-fonte com indentação.

**Gramática L-atribuída**:
```
S → if E S₁   { S₁.indent = S.indent + 1
                print("if", E)
                print_indented(S₁, S₁.indent) }
```

**Aplicação**: Formatadores de código (Prettier, clang-format).

### 6. Conversão de Notações

**Problema**: Converter infixa para pós-fixa (notação polonesa reversa).

**Gramática**:
```
E → E₁ + T    { E.posfixa = E₁.posfixa || T.posfixa || "+" }
E → T         { E.posfixa = T.posfixa }
T → num       { T.posfixa = num.lexeme }
```

**Exemplo**: `3 + 4 * 5` → `3 4 5 * +`

**Aplicação**: Interpretadores baseados em pilha, calculadoras HP.

---

## Exemplos Práticos

### Exemplo 1: Calculadora Simples

**Entrada**: `3 + 4 * 5`

**Árvore de análise com atributos**:
```
              E.val = 23
             / |  \
    E.val=3  +   T.val=20
       |         / |  \
    T.val=3  T.val=4 * F.val=5
       |        |        |
    F.val=3  F.val=4   num(5)
       |        |
     num(3)   num(4)
```

**Sequência de avaliação**:
1. `num(3) → F.val = 3 → T.val = 3 → E₁.val = 3`
2. `num(4) → F.val = 4 → T₁.val = 4`
3. `num(5) → F.val = 5`
4. `T₁.val * F.val = 4 * 5 = 20 → T.val = 20`
5. `E₁.val + T.val = 3 + 20 = 23 → E.val = 23`

### Exemplo 2: Geração de Código para Atribuição

**Entrada**: `x = a + b * c - d`

**Código gerado**:
```
t1 = b * c
t2 = a + t1
t3 = t2 - d
x = t3
```

**Árvore com atributos** `.addr`:
```
                 =
               /   \
            x      -.addr=t3
                   / \
             +.addr=t2  d
             / \
          a   *.addr=t1
              / \
             b   c
```

### Exemplo 3: Declarações com Tipos

**Entrada**: `int a, b, c;`

**Gramática L-atribuída**:
```
D → T L           { L.tipo = T.tipo }
T → int           { T.tipo = "int" }
L → L₁ , id       { L₁.tipo = L.tipo
                    addtipo(id, L.tipo) }
L → id            { addtipo(id, L.tipo) }
```

**Árvore com propagação de tipos**:
```
           D
          / \
      T      L.tipo="int"
      |      / \
    "int"   L₁  , id(c)
            |   tipo="int" → adiciona c:int
           / \
        L₂  , id(b)
         |  tipo="int" → adiciona b:int
       id(a)
      tipo="int" → adiciona a:int
```

**Resultado**: Tabela de símbolos com `a`, `b`, `c` do tipo `int`.

### Exemplo 4: Conversão Infixa → Pós-fixa

**Entrada**: `(a + b) * c`

**Gramática**:
```
E → E + T    { E.posfixa = E₁.posfixa || T.posfixa || "+" }
E → T        { E.posfixa = T.posfixa }
T → T * F    { T.posfixa = T₁.posfixa || F.posfixa || "*" }
T → F        { T.posfixa = F.posfixa }
F → (E)      { F.posfixa = E.posfixa }
F → id       { F.posfixa = id.lexeme }
```

**Árvore**:
```
                T.posfixa = "ab+c*"
                / |  \
      T.posfixa="ab+"  *  F.posfixa="c"
           |              |
      F.posfixa="ab+"     id(c)
           |
        (E.posfixa="ab+")
           |
      E.posfixa="ab+"
         / | \
    E.posfixa="a" + T.posfixa="b"
        |            |
    T.posfixa="a"  F.posfixa="b"
        |            |
    F.posfixa="a"  id(b)
        |
      id(a)
```

**Saída**: `a b + c *`

### Exemplo 5: Verificação de Tipos em Expressões

**Entrada**: `x + 3.14` onde `x` é `int`

**Gramática**:
```
E → E₁ + E₂   { if (E₁.tipo == int && E₂.tipo == int)
                   E.tipo = int
                else if (E₁.tipo == float || E₂.tipo == float)
                   E.tipo = float
                else
                   erro("tipos incompatíveis") }
```

**Avaliação**:
1. `x → E₁.tipo = int` (busca na tabela de símbolos)
2. `3.14 → E₂.tipo = float`
3. `int + float → E.tipo = float` (promoção de tipo)

---

## Como Compilar e Usar

### Estrutura do Projeto

```
src/11-traducao-dirigida-por-sintaxe/
├── calculadora.c          # Calculadora com avaliação
├── gerador_codigo.c       # Gerador de código intermediário
├── conversor_notacao.c    # Infixa para pós-fixa
├── entrada.txt            # Exemplos de entrada
└── README.md              # Este arquivo
```

### Compilação

```bash
# Compilar a calculadora
gcc calculadora.c -o calculadora

# Compilar o gerador de código
gcc gerador_codigo.c -o gerador_codigo

# Compilar o conversor de notação
gcc conversor_notacao.c -o conversor_notacao

# Compilar todos com warnings
gcc calculadora.c -o calculadora -Wall -Wextra -std=c99
gcc gerador_codigo.c -o gerador_codigo -Wall -Wextra -std=c99
gcc conversor_notacao.c -o conversor_notacao -Wall -Wextra -std=c99
```

### Execução

```bash
# Executar calculadora interativa
./calculadora

# Exemplo de uso
>>> 3 + 4 * 5
Resultado: 23

>>> (10 + 20) / 3
Resultado: 10

# Executar gerador de código
./gerador_codigo

>>> x = a + b * 3;

=== CÓDIGO INTERMEDIÁRIO GERADO ===
t0 = b * 3
t1 = a + t0
x = t1
```

### Exemplos de Entrada

**Arquivo** `entrada.txt`:
```
3 + 4 * 5
(10 - 2) * 3
100 / (5 + 5)
```

**Execução**:
```bash
./calculadora < entrada.txt
```

**Saída esperada**:
```
Resultado: 23
Resultado: 24
Resultado: 10
```

### Testes

```bash
# Testar expressões válidas
echo "3 + 4 * 5" | ./calculadora

# Testar erros
echo "3 + + 4" | ./calculadora
# Deve exibir: Erro sintático

echo "10 / 0" | ./calculadora
# Deve exibir: Erro: divisão por zero
```

---

## Referências Acadêmicas

### Livros-Texto Fundamentais

1. **Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D.** (2006). *Compiladores: Princípios, Técnicas e Ferramentas*. 2ª Edição. Pearson Education.
   - **Capítulo 5**: Tradução Dirigida por Sintaxe
   - Definições S-atribuídas e L-atribuídas
   - Construção de árvores sintáticas
   - Geração de código intermediário
   - **Essencial**: Referência definitiva para SDT

2. **Appel, A. W.** (2002). *Modern Compiler Implementation in C*. Cambridge University Press.
   - **Capítulo 4**: Abstract Syntax
   - Implementação prática de SDT
   - Construção de ASTs
   - Geração de código intermediário

3. **Louden, K. C.** (1997). *Compiler Construction: Principles and Practice*. PWS Publishing Company.
   - Abordagem didática de SDT
   - Exemplos passo a passo
   - Implementação em C
   - Exercícios práticos

4. **Cooper, K. D., & Torczon, L.** (2011). *Engineering a Compiler*. 2ª Edição. Morgan Kaufmann.
   - **Capítulo 4**: Intermediate Representations
   - Tradução para código intermediário
   - Otimizações durante tradução

### Teoria de Gramáticas de Atributos

5. **Knuth, D. E.** (1968). "Semantics of Context-Free Languages". *Mathematical Systems Theory*, 2(2), 127-145.
   - **Artigo seminal**: Introdução de gramáticas de atributos
   - Definição formal de atributos sintetizados e herdados
   - Base teórica de toda SDT moderna
   - Grafos de dependência

6. **Knuth, D. E.** (1971). "Examples of Formal Semantics". *Symposium on Semantics of Algorithmic Languages*.
   - Exemplos práticos de uso de gramáticas de atributos
   - Aplicações em análise semântica

7. **Lewis, P. M., Rosenkrantz, D. J., & Stearns, R. E.** (1974). "Attributed Translations". *Journal of Computer and System Sciences*, 9(3), 279-307.
   - Formalização matemática de traduções atribuídas
   - Propriedades de gramáticas L-atribuídas

### Implementação e Ferramentas

8. **Bochmann, G. V.** (1976). "Semantic Evaluation from Left to Right". *Communications of the ACM*, 19(2), 55-62.
   - Algoritmos para avaliar atributos em ordem L-R
   - Implementação eficiente de gramáticas L-atribuídas

9. **Kennedy, K., & Warren, S. K.** (1976). "Automatic Generation of Efficient Evaluators for Attribute Grammars". *Conference Record of the Third ACM Symposium on Principles of Programming Languages*.
   - Geração automática de avaliadores
   - Otimizações em tempo de compilação

10. **Kastens, U.** (1980). "Ordered Attribute Grammars". *Acta Informatica*, 13(3), 229-256.
    - Gramáticas de atributos ordenadas
    - Garantias de avaliabilidade
    - Algoritmos de verificação

### Geradores de Compiladores

11. **Johnson, S. C.** (1975). "Yacc: Yet Another Compiler-Compiler". *Computing Science Technical Report*, Bell Laboratories.
    - Integração de SDT em Yacc
    - Pilha semântica
    - Ações embutidas

12. **Levine, J. R.** (2009). *Flex & Bison*. O'Reilly Media.
    - Uso prático de SDT em Bison
    - Exemplos de calculadoras e compiladores
    - Integração de lexer e parser com atributos

### Geração de Código Intermediário

13. **Muchnick, S. S.** (1997). *Advanced Compiler Design and Implementation*. Morgan Kaufmann.
    - Representações intermediárias
    - Tradução para código de três endereços
    - Otimizações durante geração

14. **Wilhelm, R., & Maurer, D.** (1995). *Compiler Design*. Addison-Wesley.
    - Técnicas de geração de código
    - Tradução de construções de alto nível
    - Alocação de temporários

### Aplicações Específicas

15. **Paakki, J.** (1995). "Attribute Grammar Paradigms—A High-Level Methodology in Language Implementation". *ACM Computing Surveys*, 27(2), 196-255.
    - Survey abrangente sobre gramáticas de atributos
    - Paradigmas de implementação
    - Aplicações em compiladores reais

16. **Deransart, P., Jourdan, M., & Lorho, B.** (1988). *Attribute Grammars: Definitions, Systems and Bibliography*. Springer-Verlag.
    - Catálogo completo de sistemas baseados em AG
    - Bibliografia extensiva
    - Comparação de abordagens

### Recursos Online

17. **Stanford Compiler Course**: http://web.stanford.edu/class/cs143/
    - Materiais sobre SDT e geração de código
    - Slides e exercícios

18. **ANTLR Documentation**: https://www.antlr.org/
    - Framework moderno com suporte a atributos
    - Exemplos de gramáticas atribuídas

19. **Compiler Explorer**: https://godbolt.org/
    - Visualize tradução de código em compiladores reais
    - Compare diferentes estratégias de geração

---

## Exercícios Propostos

### Básicos

1. **Calculadora estendida**: Adicione operadores `%` (módulo) e `^` (potência)
2. **Variáveis**: Permita atribuições e uso de variáveis na calculadora
3. **Atributos múltiplos**: Calcule simultaneamente valor e altura da árvore
4. **Verificação simples**: Detecte divisão por zero em tempo de compilação

### Intermediários

5. **Conversor de bases**: Tradução de números decimais para binário/hexadecimal
6. **Otimizador de constantes**: Calcule operações entre constantes em compile-time
7. **Verificador de tipos**: Implemente verificação de tipos com promoção automática
8. **Gerador para pilha**: Traduza expressões para instruções de máquina de pilha

### Avançados

9. **Arrays**: Adicione suporte para arrays com cálculo de endereços
10. **Funções**: Implemente tradução de declarações e chamadas de funções
11. **Estruturas de controle**: Traduza `if`, `while`, `for` para código intermediário
12. **Tabela de símbolos**: Integre SDT com gerenciamento de escopo

### Desafios

13. **Linguagem completa**: Crie um compilador completo para mini-linguagem
14. **Otimizações avançadas**: Implemente eliminação de subexpressões comuns durante tradução
15. **Gramática L-atribuída**: Implemente avaliação one-pass para gramática L-atribuída complexa
16. **Gerador de bytecode**: Traduza para bytecode de máquina virtual (tipo JVM)

---

## Resumo dos Conceitos-Chave

| Conceito | Descrição | Importância |
|----------|-----------|-------------|
| **SDT** | Associar ações à estrutura sintática | Base para todas as traduções |
| **Atributos** | Valores associados a símbolos | Propagam informações na árvore |
| **Sintetizados** | Calculados dos filhos ↑ | Valores, código gerado |
| **Herdados** | Calculados do pai ↓ | Contexto, tipos esperados |
| **S-atribuída** | Apenas sintetizados | Simples, bottom-up |
| **L-atribuída** | Sint. + herdados (L-R) | Mais expressiva, top-down |
| **Código intermediário** | Representação traduzida | Facilita otimização e geração |

---

## Conclusão

A **Tradução Dirigida por Sintaxe** é uma técnica fundamental que conecta a análise sintática à geração de código e outras ações semânticas. Ela fornece um framework elegante e formal para especificar como transformar estruturas sintáticas em representações intermediárias ou finais.

**Principais takeaways**:

1. ✅ **SDT generaliza parsing**: Vai além de reconhecimento para produzir saídas
2. ✅ **Atributos propagam informações**: Sintetizados sobem, herdados descem
3. ✅ **S-atribuída é simples**: Apenas atributos sintetizados, avaliação bottom-up
4. ✅ **L-atribuída é expressiva**: Adiciona contexto com herdados, avaliação L-R
5. ✅ **Aplicações diversas**: Avaliação, geração de código, verificação de tipos
6. ✅ **Implementação natural**: Integra-se perfeitamente com parsers recursivos

Com os conceitos e implementações deste módulo, você está preparado para:
- Implementar tradutores completos para linguagens
- Gerar código intermediário durante parsing
- Realizar verificações semânticas com contexto
- Criar DSLs (Domain-Specific Languages)
- Entender como compiladores reais funcionam

**Próximos passos**: Geração de código de máquina, otimizações mais avançadas, análise de fluxo de dados!

---

*Este material faz parte da disciplina de Compiladores. Para questões ou sugestões, consulte o repositório do curso.*
