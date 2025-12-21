# 08 — Analisador Sintático

## Índice
1. [Introdução](#introdução)
2. [Conceitos Fundamentais](#conceitos-fundamentais)
3. [Fundamentos Teóricos](#fundamentos-teóricos)
4. [Gramáticas Livres de Contexto](#gramáticas-livres-de-contexto)
5. [Análise Descendente Recursiva](#análise-descendente-recursiva)
6. [Precedência e Associatividade](#precedência-e-associatividade)
7. [Árvore Sintática Abstrata (AST)](#árvore-sintática-abstrata-ast)
8. [Implementação em C](#implementação-em-c)
9. [Tratamento de Erros Sintáticos](#tratamento-de-erros-sintáticos)
10. [Exemplos Práticos](#exemplos-práticos)
11. [Como Compilar e Usar](#como-compilar-e-usar)
12. [Referências Acadêmicas](#referências-acadêmicas)

---

## Introdução

A **análise sintática** (ou *parsing*) é a **segunda fase** de um compilador, situada logo após a análise léxica. Enquanto o analisador léxico verifica se os caracteres formam tokens válidos, o analisador sintático verifica se a **sequência de tokens** forma estruturas gramaticalmente corretas, de acordo com as regras da linguagem de programação.

### Importância da Análise Sintática

Segundo **Aho, Sethi e Ullman** (2006), no clássico "Compiladores: Princípios, Técnicas e Ferramentas":

> "O analisador sintático obtém uma cadeia de tokens do analisador léxico e verifica se a cadeia pode ser gerada pela gramática para a linguagem fonte."

A análise sintática é fundamental porque:
- **Valida a estrutura do programa**: Garante que o código segue as regras gramaticais
- **Constrói representações intermediárias**: Cria árvores sintáticas para análise posterior
- **Detecta erros estruturais**: Identifica problemas como parênteses não balanceados, instruções incompletas
- **Facilita a análise semântica**: Organiza o código em uma estrutura hierárquica
- **Permite otimizações**: A estrutura da árvore revela oportunidades de otimização

### Analogia com Linguagem Natural

Considere a frase em português: **"O compilador analisa o código."**

1. **Análise Léxica** (tokenização):
   - `O` → artigo
   - `compilador` → substantivo
   - `analisa` → verbo
   - `o` → artigo
   - `código` → substantivo
   - `.` → pontuação

2. **Análise Sintática** (estrutura):
   ```
   [Sentença]
      ├── [Sujeito: "O compilador"]
      │   ├── [Artigo: "O"]
      │   └── [Substantivo: "compilador"]
      ├── [Predicado: "analisa o código"]
      │   ├── [Verbo: "analisa"]
      │   └── [Objeto: "o código"]
      │       ├── [Artigo: "o"]
      │       └── [Substantivo: "código"]
      └── [Pontuação: "."]
   ```

Da mesma forma, em uma linguagem de programação, o parser verifica se a sequência de tokens forma uma estrutura válida.

### Objetivos deste Módulo

- Implementar parsers para reconhecer estruturas sintáticas
- Dominar técnicas de análise descendente (descida recursiva)
- Construir árvores de sintaxe abstrata (AST)
- Compreender gramáticas livres de contexto
- Tratar precedência e associatividade de operadores
- Desenvolver estratégias de recuperação de erros

---

## Conceitos Fundamentais

### O que é Análise Sintática?

A análise sintática é o processo de determinar se uma sequência de tokens pode ser gerada por uma **gramática formal** que descreve a linguagem de programação.

**Entrada**: Sequência de tokens do analisador léxico
```
[TOKEN_INT, TOKEN_IDENTIFIER(x), TOKEN_ASSIGN, TOKEN_NUMBER(10), TOKEN_SEMICOLON]
```

**Saída**: Árvore sintática ou mensagem de erro
```
[Declaração de Variável]
  ├── Tipo: int
  ├── Nome: x
  └── Valor: 10
```

### Parser vs Scanner

| Aspecto | Analisador Léxico (Scanner) | Analisador Sintático (Parser) |
|---------|----------------------------|-------------------------------|
| **Entrada** | Caracteres | Tokens |
| **Saída** | Tokens | Árvore sintática |
| **Teoria** | Linguagens regulares | Linguagens livres de contexto |
| **Reconhece** | Padrões simples (identificadores, números) | Estruturas hierárquicas (expressões, comandos) |
| **Implementação** | Autômatos finitos | Pushdown automata / Recursão |
| **Exemplo** | Reconhece `123` como NUMBER | Reconhece `x + 123` como expressão |

### Tipos de Parsers

**Andrew W. Appel** (2002), em "Modern Compiler Implementation", classifica os parsers em:

#### 1. Parsers Descendentes (Top-Down)
- Começam do símbolo inicial da gramática
- Tentam derivar a entrada
- **Exemplos**: Recursive Descent, LL(k), Predictive Parser

#### 2. Parsers Ascendentes (Bottom-Up)
- Começam dos tokens de entrada
- Reduzem até chegar ao símbolo inicial
- **Exemplos**: LR(k), LALR, SLR

Neste módulo, focaremos em **parsers descendentes recursivos**, que são:
- Fáceis de implementar manualmente
- Intuitivos e didáticos
- Amplamente usados em compiladores reais (GCC, Clang)

### Exemplo Cotidiano: Validação de Expressões Matemáticas

Imagine uma calculadora que precisa validar se uma expressão está correta:

✅ **Válidas**:
- `3 + 4`
- `(2 + 3) * 5`
- `10 / (5 - 3)`

❌ **Inválidas**:
- `3 + + 4` (dois operadores consecutivos)
- `(2 + 3 * 5` (parêntese não fechado)
- `* 5` (operador sem operandos)

O parser realiza exatamente essa verificação para código de programação!

---

## Fundamentos Teóricos

### Hierarquia de Chomsky

A análise sintática trabalha com **linguagens livres de contexto** (Tipo 2 na hierarquia de Chomsky).

**Noam Chomsky** (1956) estabeleceu uma hierarquia de linguagens formais:

```
Tipo 0: Linguagens Recursivamente Enumeráveis
   ↓  (mais restritivas)
Tipo 1: Linguagens Sensíveis ao Contexto
   ↓
Tipo 2: Linguagens Livres de Contexto  ← ANÁLISE SINTÁTICA
   ↓
Tipo 3: Linguagens Regulares  ← ANÁLISE LÉXICA
```

### Por que Linguagens Regulares não Bastam?

Linguagens regulares **não conseguem** contar elementos aninhados arbitrariamente:

❌ **Impossível com regex**: Validar parênteses balanceados `((()))`, `(()())`
✅ **Possível com gramáticas livres de contexto**: Usar recursão

**Exemplo prático em C**:
```c
if (a > 0) {
    if (b > 0) {
        if (c > 0) {
            // Estrutura aninhada arbitrariamente
        }
    }
}
```

Essa estrutura requer contagem de blocos aninhados, impossível com autômatos finitos.

### Teorema Fundamental

**Teorema (Teoria de Linguagens Formais)**: 
- Toda linguagem livre de contexto pode ser reconhecida por um **autômato de pilha** (pushdown automaton)
- Linguagens de programação práticas são **quase** livres de contexto (com algumas exceções tratadas na análise semântica)

---

## Gramáticas Livres de Contexto

### Definição Formal

Uma **gramática livre de contexto** (GLC) é uma quádrupla G = (V, Σ, R, S), onde:
- **V**: Conjunto de símbolos não-terminais (variáveis)
- **Σ**: Conjunto de símbolos terminais (tokens)
- **R**: Conjunto de regras de produção
- **S**: Símbolo inicial

### Notação BNF (Backus-Naur Form)

Desenvolvida por **John Backus** e **Peter Naur** (1960) para descrever a linguagem Algol 60.

**Formato**: `<não-terminal> ::= <sequência de símbolos>`

**Exemplo**: Gramática para expressões aritméticas simples
```bnf
<expression> ::= <expression> + <term>
               | <expression> - <term>
               | <term>

<term>       ::= <term> * <factor>
               | <term> / <factor>
               | <factor>

<factor>     ::= ( <expression> )
               | NUMBER
               | IDENTIFIER
```

### Notação EBNF (Extended BNF)

Extensão mais conveniente da BNF:

| Símbolo | Significado | Exemplo |
|---------|-------------|---------|
| `*` | Zero ou mais ocorrências | `digit*` |
| `+` | Uma ou mais ocorrências | `digit+` |
| `?` | Opcional (zero ou uma) | `sign?` |
| `|` | Alternativa | `'+'|'-'` |
| `( )` | Agrupamento | `('+' | '-')` |

**Exemplo em EBNF**:
```ebnf
expression = term (('+' | '-') term)*
term       = factor (('*' | '/') factor)*
factor     = '(' expression ')' | NUMBER | IDENTIFIER
```

### Derivações

Uma **derivação** é uma sequência de aplicações de regras de produção.

**Exemplo**: Derivar `3 + 4 * 5`

```
<expression>
⇒ <term> (('+' | '-') <term>)*
⇒ <factor> (('*' | '/') <factor>)* + <term>
⇒ NUMBER + <term>
⇒ 3 + <factor> (('*' | '/') <factor>)*
⇒ 3 + NUMBER * <factor>
⇒ 3 + 4 * NUMBER
⇒ 3 + 4 * 5
```

### Árvore de Derivação (Parse Tree)

Representa graficamente uma derivação:

```
        <expression>
            /    \
        <term>    +    <term>
           |          /   |   \
        <factor>  <factor> * <factor>
           |         |         |
           3         4         5
```

### Ambiguidade em Gramáticas

Uma gramática é **ambígua** se existe uma sentença com **duas ou mais** árvores de derivação diferentes.

**Exemplo de gramática ambígua**:
```bnf
E ::= E + E | E * E | NUMBER
```

Para a entrada `2 + 3 * 4`, há duas interpretações:
```
    (E)              (E)
   / | \            / | \
  E  + E           E  * E
  |   / | \       / | \ |
  2  E * E       E + E  4
     |   |       |   |
     3   4       2   3
```

Primeira: `2 + (3 * 4) = 14`
Segunda: `(2 + 3) * 4 = 20` ❌ Diferente!

**Solução**: Eliminar ambiguidade através de níveis de precedência.

---

## Análise Descendente Recursiva

### Conceito

O **parser descendente recursivo** é uma técnica de implementação de parsers top-down onde:
- Cada **não-terminal** da gramática corresponde a uma **função**
- As produções guiam a implementação da função
- A recursão imita a estrutura recursiva da gramática

**Kenneth C. Louden** (1997) descreve:
> "Parsers recursivos descendentes são aqueles em que cada não-terminal é implementado como uma função, e as produções ditam a estrutura dessa função."

### Estrutura Geral

Para cada produção `A ::= α₁ | α₂ | ... | αₙ`:

```c
void parse_A() {
    // Escolhe alternativa baseada no próximo token (lookahead)
    if (token matches α₁) {
        // implementa α₁
    } else if (token matches α₂) {
        // implementa α₂
    } else {
        // erro sintático
    }
}
```

### Exemplo: Parser para Expressões Simples

**Gramática**:
```bnf
expression ::= term (('+' | '-') term)*
term       ::= factor (('*' | '/') factor)*
factor     ::= NUMBER | '(' expression ')'
```

**Implementação em C**:

```c
// Função para reconhecer <factor>
void parse_factor() {
    if (current_token.type == TOKEN_NUMBER) {
        advance(); // consome NUMBER
    } else if (current_token.type == TOKEN_LPAREN) {
        advance(); // consome '('
        parse_expression(); // recursão
        expect(TOKEN_RPAREN); // espera ')'
    } else {
        error("Esperado NUMBER ou '('");
    }
}

// Função para reconhecer <term>
void parse_term() {
    parse_factor();
    
    while (current_token.type == TOKEN_MULTIPLY || 
           current_token.type == TOKEN_DIVIDE) {
        advance();
        parse_factor();
    }
}

// Função para reconhecer <expression>
void parse_expression() {
    parse_term();
    
    while (current_token.type == TOKEN_PLUS || 
           current_token.type == TOKEN_MINUS) {
        advance();
        parse_term();
    }
}
```

### Correspondência com a Gramática

Note como **cada função corresponde a um não-terminal**:
- `parse_expression()` ↔ `<expression>`
- `parse_term()` ↔ `<term>`
- `parse_factor()` ↔ `<factor>`

A estrutura do código **reflete exatamente** a estrutura da gramática!

### Recursão à Esquerda

Um problema comum em parsers descendentes: **recursão à esquerda**.

**Gramática com recursão à esquerda** (❌ problemática):
```bnf
E ::= E + T | T
```

Implementação ingênua entraria em **loop infinito**:
```c
void parse_E() {
    parse_E(); // Chamada recursiva imediata!
    // ...
}
```

**Solução**: Eliminar recursão à esquerda.

**Transformação**:
```bnf
E  ::= T E'
E' ::= + T E' | ε
```

Ou usar notação EBNF:
```bnf
E ::= T (+ T)*
```

---

## Precedência e Associatividade

### Conceitos

**Precedência** determina quais operadores são avaliados primeiro:
- `*` e `/` têm **maior precedência** que `+` e `-`
- `3 + 4 * 5` é interpretado como `3 + (4 * 5)`, não `(3 + 4) * 5`

**Associatividade** determina a ordem para operadores de mesma precedência:
- **Esquerda**: `10 - 5 - 2` = `(10 - 5) - 2` = `3`
- **Direita**: `a = b = c` = `a = (b = c)` (atribuição em C)

### Tabela de Precedência em C

| Nível | Operadores | Associatividade | Exemplo |
|-------|-----------|-----------------|---------|
| 1 (maior) | `()`, `[]`, `->` | Esquerda | `arr[i]` |
| 2 | `!`, `++`, `--`, unário `+`, `-` | Direita | `!flag` |
| 3 | `*`, `/`, `%` | Esquerda | `a * b / c` |
| 4 | `+`, `-` | Esquerda | `a + b - c` |
| 5 | `<`, `<=`, `>`, `>=` | Esquerda | `a < b` |
| 6 | `==`, `!=` | Esquerda | `a == b` |
| 7 | `&&` | Esquerda | `a && b` |
| 8 | `||` | Esquerda | `a || b` |
| 9 (menor) | `=`, `+=`, `-=` | Direita | `a = b = c` |

### Implementação de Precedência na Gramática

A precedência é codificada através de **níveis hierárquicos** de não-terminais:

```bnf
expression        ::= assignment
assignment        ::= logical_or ('=' assignment)?
logical_or        ::= logical_and ('||' logical_and)*
logical_and       ::= equality ('&&' equality)*
equality          ::= relational (('==' | '!=') relational)*
relational        ::= additive (('<' | '>' | '<=' | '>=') additive)*
additive          ::= multiplicative (('+' | '-') multiplicative)*
multiplicative    ::= unary (('*' | '/' | '%') unary)*
unary             ::= ('!' | '+' | '-')? primary
primary           ::= NUMBER | IDENTIFIER | '(' expression ')'
```

**Princípio**: Operadores de **menor precedência** aparecem em **níveis superiores** da gramática.

### Exemplo Prático

**Entrada**: `5 + 3 * 2`

**Análise**:
1. `parse_expression()` chama `parse_additive()`
2. `parse_additive()` chama `parse_multiplicative()` → reconhece `5`
3. Vê `+`, consome, chama `parse_multiplicative()` novamente
4. `parse_multiplicative()` reconhece `3`
5. Vê `*`, consome, reconhece `2`
6. Retorna `3 * 2` como um termo
7. `parse_additive()` combina: `5 + (3 * 2)`

**AST resultante**:
```
    (+)
   /   \
  5    (*)
       /  \
      3    2
```

A estrutura da árvore **reflete corretamente** a precedência!

---

## Árvore Sintática Abstrata (AST)

### Conceito

Uma **Árvore Sintática Abstrata** (AST - Abstract Syntax Tree) é uma representação estruturada do código-fonte que:
- Remove detalhes desnecessários (parênteses, pontos-e-vírgulas)
- Mantém apenas informações semanticamente relevantes
- Facilita análises posteriores e geração de código

### AST vs Parse Tree

**Parse Tree** (árvore de derivação):
- Contém **todos os símbolos** da gramática
- Segue exatamente as produções
- Verbosa e com muitos nós intermediários

**AST** (árvore sintática abstrata):
- Contém apenas **informações essenciais**
- Estrutura simplificada
- Foco no significado, não na sintaxe

**Exemplo**: Para a expressão `3 + 4 * 5`

**Parse Tree**:
```
        <expression>
            /    \
        <term>    +    <term>
           |          /   |   \
        <factor>  <factor> * <factor>
           |         |         |
         NUMBER    NUMBER    NUMBER
           |         |         |
           3         4         5
```

**AST** (simplificada):
```
     (+)
    /   \
   3    (*)
        /  \
       4    5
```

### Estrutura de Dados para AST em C

```c
typedef enum {
    NODE_NUMBER,
    NODE_IDENTIFIER,
    NODE_BINARY_OP,    // +, -, *, /, etc.
    NODE_UNARY_OP,     // -, !, etc.
    NODE_IF_STMT,
    NODE_WHILE_STMT,
    NODE_ASSIGN,
    NODE_FUNC_CALL,
    NODE_VAR_DECL,
    // ... outros tipos
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* value;              // Para identificadores, operadores, etc.
    struct ASTNode* left;     // Filho esquerdo
    struct ASTNode* right;    // Filho direito
    struct ASTNode** children; // Para nós com múltiplos filhos
    int child_count;
    int line;                 // Número da linha (para erros)
} ASTNode;
```

### Construção de AST durante o Parsing

Durante o parsing, cada função retorna um nó da AST:

```c
ASTNode* parse_factor() {
    if (current_token.type == TOKEN_NUMBER) {
        ASTNode* node = create_node(NODE_NUMBER, current_token.lexeme);
        advance();
        return node;
    } else if (current_token.type == TOKEN_LPAREN) {
        advance(); // consome '('
        ASTNode* expr = parse_expression();
        expect(TOKEN_RPAREN);
        return expr; // retorna a expressão dentro dos parênteses
    }
    // ...
}

ASTNode* parse_term() {
    ASTNode* left = parse_factor();
    
    while (current_token.type == TOKEN_MULTIPLY || 
           current_token.type == TOKEN_DIVIDE) {
        Token op = current_token;
        advance();
        
        ASTNode* node = create_node(NODE_BINARY_OP, op.lexeme);
        node->left = left;
        node->right = parse_factor();
        left = node;
    }
    
    return left;
}
```

### Percursos na AST

Após construir a AST, podemos percorrê-la de várias formas:

#### 1. Pré-ordem (Pre-order)
Visita: raiz → esquerda → direita
```
+ 3 * 4 5
```

#### 2. Em-ordem (In-order)
Visita: esquerda → raiz → direita
```
3 + 4 * 5
```

#### 3. Pós-ordem (Post-order)
Visita: esquerda → direita → raiz
```
3 4 5 * +
```

**Notação polonesa reversa** (RPN) é obtida por percurso pós-ordem!

### Exemplo de Impressão da AST

```c
void print_ast(ASTNode* node, int depth) {
    if (!node) return;
    
    // Indentação proporcional à profundidade
    for (int i = 0; i < depth; i++) printf("  ");
    
    // Imprimir nó atual
    switch (node->type) {
        case NODE_NUMBER:
            printf("NUM: %s\n", node->value);
            break;
        case NODE_BINARY_OP:
            printf("OP: %s\n", node->value);
            break;
        case NODE_IF_STMT:
            printf("IF\n");
            break;
        // ... outros casos
    }
    
    // Recursão para filhos
    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}
```

---

## Implementação em C

### Estrutura Completa do Parser

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ========== TIPOS DE TOKENS ==========
typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE,
    TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_SEMICOLON,
    TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE,
    TOKEN_ASSIGN, TOKEN_EQ, TOKEN_NE, TOKEN_LT, TOKEN_GT,
    TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[100];
    int line, column;
} Token;

// ========== AST ==========
typedef enum {
    NODE_NUMBER, NODE_IDENTIFIER,
    NODE_BINARY_OP, NODE_UNARY_OP,
    NODE_IF_STMT, NODE_WHILE_STMT,
    NODE_ASSIGN, NODE_COMPOUND
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* value;
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode** children;
    int child_count;
} ASTNode;

// ========== ESTADO DO PARSER ==========
Token* tokens;
int token_count;
int current_pos = 0;

// ========== FUNÇÕES AUXILIARES ==========
Token current_token() {
    if (current_pos < token_count)
        return tokens[current_pos];
    Token eof = {TOKEN_EOF, "EOF", 0, 0};
    return eof;
}

void advance() {
    if (current_pos < token_count)
        current_pos++;
}

void expect(TokenType type) {
    if (current_token().type == type) {
        advance();
    } else {
        printf("Erro: esperado token tipo %d, obtido %d\n", 
               type, current_token().type);
        exit(1);
    }
}

ASTNode* create_node(NodeType type, char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = NULL;
    node->right = NULL;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

// ========== FUNÇÕES DE PARSING ==========

// Declarações forward
ASTNode* parse_expression();

// <primary> ::= NUMBER | IDENTIFIER | '(' <expression> ')'
ASTNode* parse_primary() {
    Token tok = current_token();
    
    if (tok.type == TOKEN_NUMBER) {
        advance();
        return create_node(NODE_NUMBER, tok.lexeme);
    }
    
    if (tok.type == TOKEN_IDENTIFIER) {
        advance();
        return create_node(NODE_IDENTIFIER, tok.lexeme);
    }
    
    if (tok.type == TOKEN_LPAREN) {
        advance();
        ASTNode* expr = parse_expression();
        expect(TOKEN_RPAREN);
        return expr;
    }
    
    printf("Erro sintático: token inesperado '%s'\n", tok.lexeme);
    exit(1);
}

// <unary> ::= ('-' | '+')? <primary>
ASTNode* parse_unary() {
    Token tok = current_token();
    
    if (tok.type == TOKEN_PLUS || tok.type == TOKEN_MINUS) {
        advance();
        ASTNode* node = create_node(NODE_UNARY_OP, tok.lexeme);
        node->left = parse_unary();
        return node;
    }
    
    return parse_primary();
}

// <multiplicative> ::= <unary> (('*' | '/') <unary>)*
ASTNode* parse_multiplicative() {
    ASTNode* left = parse_unary();
    
    while (current_token().type == TOKEN_MULTIPLY || 
           current_token().type == TOKEN_DIVIDE) {
        Token op = current_token();
        advance();
        
        ASTNode* node = create_node(NODE_BINARY_OP, op.lexeme);
        node->left = left;
        node->right = parse_unary();
        left = node;
    }
    
    return left;
}

// <additive> ::= <multiplicative> (('+' | '-') <multiplicative>)*
ASTNode* parse_additive() {
    ASTNode* left = parse_multiplicative();
    
    while (current_token().type == TOKEN_PLUS || 
           current_token().type == TOKEN_MINUS) {
        Token op = current_token();
        advance();
        
        ASTNode* node = create_node(NODE_BINARY_OP, op.lexeme);
        node->left = left;
        node->right = parse_multiplicative();
        left = node;
    }
    
    return left;
}

// <expression> ::= <additive>
ASTNode* parse_expression() {
    return parse_additive();
}

// <statement> ::= <expression> ';'
ASTNode* parse_statement() {
    ASTNode* expr = parse_expression();
    expect(TOKEN_SEMICOLON);
    return expr;
}

// <program> ::= <statement>*
ASTNode* parse_program() {
    ASTNode* program = create_node(NODE_COMPOUND, "program");
    
    while (current_token().type != TOKEN_EOF) {
        ASTNode* stmt = parse_statement();
        // Adicionar stmt aos filhos de program
        // (simplificado para este exemplo)
    }
    
    return program;
}

// ========== IMPRESSÃO DA AST ==========
void print_ast(ASTNode* node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; i++) printf("  ");
    
    switch (node->type) {
        case NODE_NUMBER:
            printf("NUM: %s\n", node->value);
            break;
        case NODE_IDENTIFIER:
            printf("ID: %s\n", node->value);
            break;
        case NODE_BINARY_OP:
            printf("OP: %s\n", node->value);
            break;
        default:
            printf("NODE_%d\n", node->type);
    }
    
    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}

// ========== MAIN ==========
int main() {
    // Exemplo: tokens para "3 + 4 * 5;"
    Token example_tokens[] = {
        {TOKEN_NUMBER, "3", 1, 1},
        {TOKEN_PLUS, "+", 1, 3},
        {TOKEN_NUMBER, "4", 1, 5},
        {TOKEN_MULTIPLY, "*", 1, 7},
        {TOKEN_NUMBER, "5", 1, 9},
        {TOKEN_SEMICOLON, ";", 1, 10},
        {TOKEN_EOF, "EOF", 1, 11}
    };
    
    tokens = example_tokens;
    token_count = 7;
    
    printf("=== ANÁLISE SINTÁTICA ===\n");
    ASTNode* ast = parse_statement();
    
    printf("\n=== ÁRVORE SINTÁTICA ABSTRATA ===\n");
    print_ast(ast, 0);
    
    return 0;
}
```

### Saída do Programa

```
=== ANÁLISE SINTÁTICA ===

=== ÁRVORE SINTÁTICA ABSTRATA ===
OP: +
  NUM: 3
  OP: *
    NUM: 4
    NUM: 5
```

---

## Tratamento de Erros Sintáticos

### Tipos de Erros Sintáticos

1. **Token inesperado**
   ```c
   int x = ;  // Esperava expressão, encontrou ';'
   ```

2. **Parênteses/chaves não balanceados**
   ```c
   if (x > 0 { // Falta ')'
   ```

3. **Estrutura incompleta**
   ```c
   if (x > 0) // Falta corpo do if
   ```

4. **Operador sem operando**
   ```c
   x = + * 5; // '*' sem operando esquerdo
   ```

### Estratégias de Recuperação

**Segundo Aho et al. (2006)**, existem quatro estratégias principais:

#### 1. Modo Pânico (Panic Mode)
Descarta tokens até encontrar um **sincronizador** (ex: `;`, `}`, `end`).

```c
void parse_statement() {
    if (/* erro */) {
        fprintf(stderr, "Erro na linha %d\n", current_token().line);
        
        // Sincronizar: descartar até ';'
        while (current_token().type != TOKEN_SEMICOLON &&
               current_token().type != TOKEN_EOF) {
            advance();
        }
        
        if (current_token().type == TOKEN_SEMICOLON)
            advance();
    }
}
```

#### 2. Nível de Frase (Phrase-Level)
Realiza correções locais, como:
- Inserir `;` faltante
- Remover token extra
- Substituir operador incorreto

```c
if (current_token().type != TOKEN_SEMICOLON) {
    fprintf(stderr, "Aviso: ';' faltando na linha %d, inserindo\n",
            current_token().line);
    // Continua como se tivesse encontrado ';'
} else {
    advance();
}
```

#### 3. Produções de Erro
Adicionam produções especiais à gramática para tratar erros comuns.

```bnf
statement ::= if '(' expression ')' statement
            | if '(' expression statement  /* erro: ')' faltando */
```

#### 4. Correção Global
Encontra a menor sequência de mudanças para tornar o programa válido.
- **Vantagem**: Correção ótima
- **Desvantagem**: Muito complexo e lento
- **Uso**: Raramente implementado na prática

### Mensagens de Erro Úteis

✅ **Boa mensagem**:
```
Erro sintático na linha 15, coluna 8:
    if (x > 0 {
              ^
Esperado ')', encontrado '{'
```

❌ **Mensagem ruim**:
```
Syntax error
```

### Implementação de Relatório de Erros

```c
typedef struct {
    char message[256];
    int line;
    int column;
    char* hint;  // Sugestão de correção
} SyntaxError;

void report_error(SyntaxError err) {
    fprintf(stderr, "\nErro sintático na linha %d, coluna %d:\n",
            err.line, err.column);
    fprintf(stderr, "  %s\n", err.message);
    
    if (err.hint) {
        fprintf(stderr, "Dica: %s\n", err.hint);
    }
}

void expect_with_hint(TokenType expected, const char* hint) {
    if (current_token().type == expected) {
        advance();
    } else {
        SyntaxError err;
        snprintf(err.message, sizeof(err.message),
                "Esperado '%s', encontrado '%s'",
                token_name(expected),
                current_token().lexeme);
        err.line = current_token().line;
        err.column = current_token().column;
        err.hint = (char*)hint;
        
        report_error(err);
        // Tentar recuperar...
    }
}
```

---

## Exemplos Práticos

### Exemplo 1: Calculadora Simples

**Código-fonte**:
```c
resultado = (10 + 20) * 3;
```

**Tokens**:
```
IDENTIFIER(resultado), ASSIGN, LPAREN, NUMBER(10), PLUS, 
NUMBER(20), RPAREN, MULTIPLY, NUMBER(3), SEMICOLON
```

**AST**:
```
      ASSIGN
     /      \
resultado   MULTIPLY
           /        \
        PLUS         3
       /    \
     10     20
```

**Interpretação**: `resultado = ((10 + 20) * 3) = 90`

### Exemplo 2: Estrutura Condicional

**Código-fonte**:
```c
if (nota >= 7) {
    resultado = aprovado;
} else {
    resultado = reprovado;
}
```

**AST simplificada**:
```
        IF
       / | \
      /  |  \
    GE  THEN ELSE
   /  \   |    |
nota  7  =    =
        / \   / \
   resultado  resultado
   aprovado   reprovado
```

### Exemplo 3: Precedência de Operadores

**Expressão**: `a + b * c - d / e`

**Interpretação correta**: `a + (b * c) - (d / e)`

**AST**:
```
        (-)
       /   \
     (+)   (/)
    /  \   / \
   a   (*)d  e
       / \
      b   c
```

A estrutura da árvore **garante** a precedência correta!

### Exemplo 4: Aplicação do Dia a Dia - Validação de Fórmulas

Imagine uma planilha eletrônica (como Excel) que precisa validar fórmulas:

✅ **Válidas**:
- `=A1 + B2`
- `=SUM(A1:A10) / COUNT(B1:B10)`
- `=IF(C1>100, "Alto", "Baixo")`

❌ **Inválidas**:
- `=A1 + + B2` (operador duplicado)
- `=SUM(A1:A10` (parêntese não fechado)
- `=IF C1>100, "Alto")` (falta '(' após IF)

O parser sintático é responsável por essa validação!

### Exemplo 5: Compilador de Expressões Regulares

Expressões regulares também têm sintaxe:

**Regex**: `(a|b)*c+`

**AST do regex**:
```
    CONCAT
    /     \
  STAR    PLUS
   |       |
   OR      'c'
  /  \
'a'  'b'
```

O parser garante que operadores (`*`, `+`, `|`) sejam aplicados corretamente!

---

## Como Compilar e Usar

### Estrutura do Projeto

```
src/08-analisador-sintatico/
├── exemploSimples.c      # Parser básico de expressões
├── exemploCompleto.c     # Parser completo com AST
├── exemploSimplificado.c # Versão simplificada didática
├── entrada.txt           # Arquivo de teste
└── README.md             # Este arquivo
```

### Compilação

```bash
# Compilar o exemplo simples
gcc exemploSimples.c -o exemploSimples

# Compilar o exemplo completo
gcc exemploCompleto.c -o exemploCompleto

# Compilar o exemplo simplificado
gcc exemploSimplificado.c -o exemploSimplificado
```

### Execução

```bash
# Executar o exemplo simples
./exemploSimples

# Executar o exemplo completo
./exemploCompleto

# Com arquivo de entrada personalizado
./exemploSimples < meu_codigo.txt
```

### Exemplos de Entrada

#### Arquivo `entrada.txt`:
```c
if (a + 5) b = 10;
x + 3 * (y - 2);
```

#### Saída Esperada:
```
=== ANÁLISE SINTÁTICA ===
Analisando: if (a + 5) b = 10;
✓ Análise bem-sucedida

Analisando: x + 3 * (y - 2);
✓ Análise bem-sucedida

=== ÁRVORE SINTÁTICA ABSTRATA ===
IF
  OP: +
    ID: a
    NUM: 5
  ASSIGN
    ID: b
    NUM: 10

EXPRESSION
  OP: +
    ID: x
    OP: *
      NUM: 3
      OP: -
        ID: y
        NUM: 2
```

### Testando Erros

Crie um arquivo `erro.txt`:
```c
x + + 5;
(a + b;
```

Execute:
```bash
./exemploSimples < erro.txt
```

Saída esperada:
```
Erro sintático na linha 1:
  Esperado NUMBER ou IDENTIFIER, encontrado '+'

Erro sintático na linha 2:
  Esperado ')', encontrado ';'
  Dica: Verifique se todos os parênteses estão balanceados
```

---

## Referências Acadêmicas

### Livros-Texto Fundamentais

1. **Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D.** (2006). *Compiladores: Princípios, Técnicas e Ferramentas*. 2ª Edição. Pearson Education.
   - **Capítulo 4**: Análise Sintática
   - Cobre gramáticas livres de contexto, parsers LL e LR
   - Algoritmos de construção de tabelas de parsing
   - Tratamento de ambiguidade e precedência
   - **Essencial**: Referência definitiva para análise sintática

2. **Appel, A. W.** (2002). *Modern Compiler Implementation in C*. Cambridge University Press.
   - **Capítulo 3**: Parsing
   - Implementação prática de parsers em C
   - Recursive descent e LL parsing
   - Construção de AST
   - Integração com análise léxica

3. **Louden, K. C.** (1997). *Compiler Construction: Principles and Practice*. PWS Publishing Company.
   - Abordagem muito didática
   - Exemplos passo a passo
   - Ênfase em parsers descendentes recursivos
   - Tratamento detalhado de erros

4. **Cooper, K. D., & Torczon, L.** (2011). *Engineering a Compiler*. 2ª Edição. Morgan Kaufmann.
   - **Capítulo 3**: Parsers
   - Perspectiva de engenharia
   - Top-down e bottom-up parsing
   - Ferramentas automatizadas (YACC, Bison)

### Teoria das Linguagens Formais

5. **Hopcroft, J. E., Motwani, R., & Ullman, J. D.** (2006). *Introduction to Automata Theory, Languages, and Computation*. 3ª Edição. Pearson.
   - **Capítulos 5 e 7**: Gramáticas livres de contexto e autômatos de pilha
   - Teoremas de equivalência
   - Formas normais (CNF, GNF)
   - Lema do bombeamento para CFL

6. **Sipser, M.** (2012). *Introduction to the Theory of Computation*. 3ª Edição. Cengage Learning.
   - **Capítulo 2**: Linguagens livres de contexto
   - Tratamento matemático rigoroso
   - Pushdown automata
   - Propriedades de fechamento

### Gramáticas e Notações

7. **Backus, J. W., et al.** (1960). "Report on the Algorithmic Language ALGOL 60". *Communications of the ACM*, 3(5), 299-314.
   - Introdução da notação BNF
   - Primeira descrição formal de linguagem de programação
   - Base para todas as notações modernas

8. **Knuth, D. E.** (1965). "On the Translation of Languages from Left to Right". *Information and Control*, 8(6), 607-639.
   - Introdução do parsing LR
   - Fundamentos teóricos do parsing bottom-up
   - Algoritmo de construção de tabelas LR

### Parsers Práticos

9. **Parr, T.** (2013). *The Definitive ANTLR 4 Reference*. Pragmatic Bookshelf.
   - ANTLR: gerador de parsers moderno
   - Técnicas LL(*)
   - Adaptive parsing
   - Construção automática de AST

10. **Levine, J. R.** (2009). *Flex & Bison*. O'Reilly Media.
    - Uso prático de Flex (lexer) e Bison (parser)
    - Integração de scanner e parser
    - Exemplos completos de compiladores

### Artigos Seminais

11. **Dijkstra, E. W.** (1961). "ALGOL 60 Translation". *Supplement to the ALGOL 60 Report*.
    - Primeiros algoritmos de parsing
    - Recursive descent parsing

12. **DeRemer, F. L.** (1971). "Simple LR(k) Grammars". *Communications of the ACM*, 14(7), 453-460.
    - Parsers SLR
    - Simplificação de parsing LR

### Recursos Online

13. **Dragon Book Resources**: https://suif.stanford.edu/dragonbook/
    - Materiais suplementares do livro do dragão
    - Exercícios e soluções

14. **YACC/Bison Manual**: https://www.gnu.org/software/bison/manual/
    - Documentação oficial do GNU Bison
    - Tutoriais e exemplos

15. **Compiler Explorer**: https://godbolt.org/
    - Visualize ASTs de compiladores reais (GCC, Clang)
    - Compare diferentes implementações

---

## Exercícios Propostos

### Básicos

1. **Estender a gramática**: Adicione suporte para operador de módulo (`%`)
2. **Operadores de comparação**: Implemente `<`, `>`, `<=`, `>=`, `==`, `!=`
3. **Contadores**: Adicione contadores para rastrear número de operações de cada tipo na AST

### Intermediários

4. **Declarações de variáveis**: Adicione suporte para `int x = 10;`
5. **Estrutura while**: Implemente loops `while (condition) { body }`
6. **Funções simples**: Permita definição e chamada de funções sem parâmetros
7. **Otimização de constantes**: Detecte e simplifique operações entre constantes na AST

### Avançados

8. **Análise de ambiguidade**: Implemente um detector de gramáticas ambíguas
9. **Gerador de código**: A partir da AST, gere código assembly ou de máquina virtual
10. **Melhor recuperação de erros**: Implemente panic mode com múltiplos sincronizadores
11. **Pretty printer**: Crie um formatador de código a partir da AST

### Desafios

12. **Parser LL(1)**: Implemente um parser tabular LL(1) com tabela de parsing pré-computada
13. **Parser LR**: Estude e implemente um parser LR(0) ou SLR(1) simples
14. **Otimizações na AST**: Implemente transformações como:
    - Eliminação de código morto
    - Propagação de constantes
    - Redução de força (ex: `x * 2` → `x + x`)

---

## Aplicações Reais do Parsing

### 1. Compiladores e Interpretadores
- **GCC, Clang**: Compiladores C/C++ usam parsing descendente recursivo
- **Python, Ruby**: Interpretadores constroem AST antes da execução
- **Java**: `javac` constrói AST para bytecode generation

### 2. IDEs e Editores
- **IntelliJ, VS Code**: Parsing em tempo real para:
  - Syntax highlighting
  - Auto-completação
  - Refatoração automática
  - Detecção de erros

### 3. Ferramentas de Análise
- **ESLint, Pylint**: Analisam AST para encontrar problemas
- **Prettier**: Formata código a partir da AST
- **Code coverage tools**: Instrumentam código através da AST

### 4. Linguagens de Domínio Específico (DSLs)
- **SQL**: Queries são parsed antes da execução
- **Regex**: Expressões regulares têm parsers dedicados
- **Configurações**: YAML, JSON, TOML precisam de parsing

### 5. Documentação e Análise
- **Doxygen, Javadoc**: Extraem documentação através de parsing
- **Code clone detection**: Compara ASTs para encontrar código duplicado
- **Métricas de software**: Calculam complexidade ciclomática via AST

---

## Resumo dos Conceitos-Chave

| Conceito | Descrição | Importância |
|----------|-----------|-------------|
| **Gramática Livre de Contexto** | Conjunto de regras que define a sintaxe | Base teórica do parsing |
| **Parser Descendente** | Constrói árvore da raiz para folhas | Fácil de implementar manualmente |
| **Recursão** | Funções que chamam a si mesmas | Imita estrutura recursiva da gramática |
| **Precedência** | Ordem de avaliação de operadores | Garante interpretação correta |
| **AST** | Representação estruturada do código | Facilita análises posteriores |
| **Recuperação de Erros** | Continuar após encontrar erro | Melhor experiência do usuário |

---

## Conclusão

A análise sintática é uma fase **crucial** no processo de compilação. Ela transforma uma sequência linear de tokens em uma **estrutura hierárquica** (AST) que representa o significado do programa.

**Principais takeaways**:

1. ✅ **Parsers validam estrutura**: Garantem que código segue regras gramaticais
2. ✅ **AST é fundamental**: Base para todas as análises posteriores
3. ✅ **Precedência importa**: Determina interpretação correta de expressões
4. ✅ **Recursão é natural**: Imita perfeitamente estruturas aninhadas
5. ✅ **Erros devem ser tratados**: Mensagens claras ajudam programadores

Com os conceitos e implementações deste módulo, você está preparado para:
- Implementar parsers para linguagens reais
- Entender como compiladores processam código
- Criar suas próprias linguagens de domínio específico
- Analisar e transformar código programaticamente

**Próximos passos**: Análise semântica, onde verificamos tipos, escopos e outras propriedades que não podem ser expressas por gramáticas livres de contexto!

---

*Este material faz parte da disciplina de Compiladores. Para questões ou sugestões, consulte o repositório do curso.*