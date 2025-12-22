# 09 — Analisador Semântico

## Índice
1. [Introdução](#introdução)
2. [Conceitos Fundamentais](#conceitos-fundamentais)
3. [Fundamentos Teóricos](#fundamentos-teóricos)
4. [Tabela de Símbolos](#tabela-de-símbolos)
5. [Sistema de Tipos](#sistema-de-tipos)
6. [Análise de Escopo](#análise-de-escopo)
7. [Verificações Semânticas](#verificações-semânticas)
8. [Implementação em C](#implementação-em-c)
9. [Tratamento de Erros Semânticos](#tratamento-de-erros-semânticos)
10. [Exemplos Práticos](#exemplos-práticos)
11. [Como Compilar e Usar](#como-compilar-e-usar)
12. [Referências Acadêmicas](#referências-acadêmicas)

---

## Introdução

A **análise semântica** é a **terceira fase** de um compilador, situada imediatamente após as análises léxica e sintática. Enquanto o analisador léxico verifica a estrutura dos tokens e o analisador sintático valida a estrutura gramatical do programa, o analisador semântico verifica se o programa **faz sentido** do ponto de vista de significado, tipos e contexto.

### Importância da Análise Semântica

Segundo **Aho, Lam, Sethi e Ullman** (2006), no clássico "Compiladores: Princípios, Técnicas e Ferramentas":

> "O analisador semântico usa a árvore sintática e as informações na tabela de símbolos para verificar a consistência semântica do programa fonte com a definição da linguagem."

A análise semântica é fundamental porque:
- **Detecta erros que a sintaxe não captura**: Uso de variáveis não declaradas, incompatibilidade de tipos
- **Garante correção do programa**: Verifica se operações fazem sentido no contexto
- **Prepara para geração de código**: Coleta informações necessárias para as fases posteriores
- **Melhora a qualidade do código**: Detecta avisos como variáveis não utilizadas
- **Implementa regras da linguagem**: Aplica semântica estática definida pela especificação

### Analogia com Linguagem Natural

Considere as frases em português:

1. **Sintaticamente correta, semanticamente incorreta**:
   - "As ideias verdes incolores dormem furiosamente."
   - Gramaticalmente perfeita, mas sem sentido lógico

2. **Sintaticamente e semanticamente correta**:
   - "O compilador detecta erros no código."
   - Correta em ambos os aspectos

Da mesma forma, em programação:

```c
// Sintaticamente correto, semanticamente incorreto
int x;
x = "texto";  // Erro: tentando atribuir string a inteiro

// Correto em ambos os aspectos
int x;
x = 42;  // OK: tipo compatível
```

### Objetivos deste Módulo

- Implementar tabela de símbolos com suporte a escopos aninhados
- Realizar verificação de tipos estática
- Analisar escopo e visibilidade de identificadores
- Detectar erros semânticos em tempo de compilação
- Compreender sistemas de tipos e suas propriedades
- Desenvolver estratégias de recuperação de erros semânticos

---

## Conceitos Fundamentais

### O que é Análise Semântica?

A análise semântica é o processo de verificar se um programa sintaticamente correto também é **semanticamente válido**, ou seja, se respeita as regras de significado da linguagem.

**Entrada**: Árvore sintática abstrata (AST) do analisador sintático
```
[Atribuição]
  ├── Variável: x (tipo: int)
  └── Valor: "texto" (tipo: string)
```

**Saída**: AST anotada com tipos ou mensagens de erro
```
ERRO SEMÂNTICO: Incompatibilidade de tipos
  Esperado: int
  Obtido: string
  Linha: 5, Coluna: 8
```

### Semântica Estática vs Dinâmica

**Louden** (1997) distingue claramente dois tipos de semântica:

| Aspecto | Semântica Estática | Semântica Dinâmica |
|---------|-------------------|-------------------|
| **Quando** | Tempo de compilação | Tempo de execução |
| **Verificações** | Tipos, declarações, escopos | Valores, divisão por zero, limites |
| **Exemplos** | `int x = "texto";` (erro de tipo) | `int y = x / 0;` (divisão por zero) |
| **Responsável** | Analisador semântico | Sistema de execução |
| **Detectável** | Antes da execução | Durante a execução |

Este módulo foca em **semântica estática**, que pode ser verificada sem executar o programa.

### Propriedades Semânticas Verificadas

1. **Declaração de Identificadores**
   - Toda variável usada deve ter sido declarada
   - Não pode haver declarações duplicadas no mesmo escopo

2. **Compatibilidade de Tipos**
   - Operações devem ser aplicadas a tipos apropriados
   - Atribuições devem respeitar tipos compatíveis

3. **Regras de Escopo**
   - Identificadores devem estar visíveis no ponto de uso
   - Escopos aninhados seguem regras de sombreamento

4. **Verificações de Funções**
   - Número e tipos de argumentos devem coincidir
   - Funções devem retornar valores do tipo declarado

5. **Verificações de Controle de Fluxo**
   - `break` e `continue` apenas dentro de loops
   - `return` com valor correto para tipo de retorno

### Diferenças entre Fases

| Fase | Reconhece | Exemplo de Erro |
|------|-----------|-----------------|
| **Léxica** | Tokens válidos | `123abc` (identificador começando com dígito) |
| **Sintática** | Estrutura gramatical | `if x > 0 {` (falta `)`) |
| **Semântica** | Significado e contexto | `int x = "abc";` (tipo incompatível) |

---

## Fundamentos Teóricos

### Gramáticas de Atributos

**Donald Knuth** (1968) introduziu o conceito de **gramáticas de atributos** para especificar formalmente a semântica.

Uma **gramática de atributos** é uma gramática livre de contexto aumentada com:
- **Atributos**: Informações associadas a símbolos da gramática
- **Regras semânticas**: Especificam como calcular valores de atributos

#### Tipos de Atributos

1. **Atributos Sintetizados** (bottom-up)
   - Calculados a partir dos filhos na AST
   - Fluem das folhas para a raiz
   - Exemplo: tipo de uma expressão

2. **Atributos Herdados** (top-down)
   - Calculados a partir do pai ou irmãos
   - Fluem da raiz para as folhas
   - Exemplo: escopo atual, tipo esperado

#### Exemplo: Verificação de Tipos

Gramática com atributos:
```
E → E₁ + E₂     { E.type = if (E₁.type == int && E₂.type == int)
                           then int else error }
E → num         { E.type = int }
E → id          { E.type = symbol_table.lookup(id.lexeme).type }
```

### S-Atribuídas vs L-Atribuídas

**Teorema (Knuth, 1968)**:
- **S-atribuídas**: Apenas atributos sintetizados → avaliação em pós-ordem
- **L-atribuídas**: Atributos sintetizados e herdados restritos → avaliação em pré-ordem ou percurso em profundidade

Compiladores práticos geralmente usam gramáticas L-atribuídas por serem mais expressivas que S-atribuídas, mas ainda eficientes.

### Sistemas de Tipos

Um **sistema de tipos** é um método formal para classificar valores e expressões em categorias chamadas **tipos**.

**Benjamin Pierce** (2002), em "Types and Programming Languages", define:

> "Um sistema de tipos é um método tratável e sintático para provar a ausência de certos comportamentos de programa, classificando frases de acordo com os tipos de valores que computam."

#### Propriedades de Sistemas de Tipos

1. **Segurança de Tipos** (*Type Safety*)
   - Programas bem tipados não causam erros de tipo em tempo de execução
   - "Programas bem tipados não podem dar errado" (Milner, 1978)

2. **Soundness** (Correção)
   - Se o verificador aceita, não há erros de tipo
   - Pode rejeitar programas válidos (conservador)

3. **Completeness** (Completude)
   - Se não há erros de tipo, o verificador aceita
   - Difícil de alcançar na prática

#### Tipagem Forte vs Fraca

| Aspecto | Tipagem Forte | Tipagem Fraca |
|---------|--------------|---------------|
| **Conversões** | Explícitas (casting) | Implícitas (coerção) |
| **Segurança** | Alta | Baixa |
| **Erros** | Detectados em compilação | Podem ocorrer em execução |
| **Exemplos** | Java, Python, Haskell | C, JavaScript |

---

## Tabela de Símbolos

A **tabela de símbolos** é uma estrutura de dados fundamental que armazena informações sobre identificadores no programa.

### Definição e Propósito

**Aho et al. (2006)** define:

> "A tabela de símbolos é uma estrutura de dados que mantém um registro para cada nome de variável, com campos para os atributos do nome."

**Propósitos principais**:
- Armazenar declarações de identificadores
- Verificar se identificadores foram declarados antes do uso
- Verificar redeclarações no mesmo escopo
- Recuperar informações (tipo, endereço) sobre identificadores
- Gerenciar escopos aninhados

### Informações Armazenadas

Para cada identificador, a tabela de símbolos mantém:

```c
typedef struct Symbol {
    char* name;           // Nome do identificador
    SymbolType type;      // Tipo (int, float, function, etc.)
    int scope_level;      // Nível de escopo
    int offset;           // Deslocamento na memória
    int line_declared;    // Linha onde foi declarado
    int is_initialized;   // Foi inicializado?
    int is_used;          // Foi usado?
    
    // Para funções
    int num_params;       // Número de parâmetros
    SymbolType* param_types;  // Tipos dos parâmetros
    SymbolType return_type;   // Tipo de retorno
} Symbol;
```

### Estruturas de Dados

#### 1. Lista Simples

```c
typedef struct SymbolNode {
    Symbol* symbol;
    struct SymbolNode* next;
} SymbolNode;

SymbolNode* symbol_list = NULL;
```

**Vantagens**: Simples de implementar
**Desvantagens**: Busca O(n)

#### 2. Tabela Hash

```c
#define TABLE_SIZE 211

typedef struct HashEntry {
    Symbol* symbol;
    struct HashEntry* next;
} HashEntry;

HashEntry* hash_table[TABLE_SIZE];

unsigned int hash(const char* name) {
    unsigned int h = 0;
    while (*name) {
        h = (h << 5) + h + *name++;
    }
    return h % TABLE_SIZE;
}
```

**Vantagens**: Busca O(1) média
**Desvantagens**: Colisões, memória extra

#### 3. Pilha de Escopos

Para gerenciar escopos aninhados:

```c
typedef struct Scope {
    HashEntry* symbols[TABLE_SIZE];
    struct Scope* parent;
    int level;
} Scope;

Scope* current_scope = NULL;

void enter_scope() {
    Scope* new_scope = malloc(sizeof(Scope));
    memset(new_scope->symbols, 0, sizeof(new_scope->symbols));
    new_scope->parent = current_scope;
    new_scope->level = current_scope ? current_scope->level + 1 : 0;
    current_scope = new_scope;
}

void exit_scope() {
    if (current_scope) {
        Scope* old_scope = current_scope;
        current_scope = current_scope->parent;
        // Liberar memória do escopo antigo
        free(old_scope);
    }
}
```

### Operações Fundamentais

#### 1. Inserir Símbolo

```c
void insert_symbol(const char* name, SymbolType type, int line) {
    // Verificar redeclaração no escopo atual
    Symbol* existing = lookup_current_scope(name);
    if (existing) {
        semantic_error("Variável '%s' já declarada no escopo atual (linha %d)",
                      name, existing->line_declared);
        return;
    }
    
    // Criar novo símbolo
    Symbol* sym = malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type;
    sym->scope_level = current_scope->level;
    sym->line_declared = line;
    sym->is_initialized = 0;
    sym->is_used = 0;
    
    // Inserir na tabela hash do escopo atual
    unsigned int h = hash(name);
    HashEntry* entry = malloc(sizeof(HashEntry));
    entry->symbol = sym;
    entry->next = current_scope->symbols[h];
    current_scope->symbols[h] = entry;
}
```

#### 2. Buscar Símbolo

```c
Symbol* lookup_symbol(const char* name) {
    Scope* scope = current_scope;
    
    // Buscar em todos os escopos, do atual para os externos
    while (scope) {
        unsigned int h = hash(name);
        HashEntry* entry = scope->symbols[h];
        
        // Buscar na lista encadeada do bucket
        while (entry) {
            if (strcmp(entry->symbol->name, name) == 0) {
                entry->symbol->is_used = 1;  // Marcar como usado
                return entry->symbol;
            }
            entry = entry->next;
        }
        
        scope = scope->parent;  // Subir para escopo pai
    }
    
    return NULL;  // Não encontrado
}

Symbol* lookup_current_scope(const char* name) {
    if (!current_scope) return NULL;
    
    unsigned int h = hash(name);
    HashEntry* entry = current_scope->symbols[h];
    
    while (entry) {
        if (strcmp(entry->symbol->name, name) == 0) {
            return entry->symbol;
        }
        entry = entry->next;
    }
    
    return NULL;
}
```

#### 3. Imprimir Tabela de Símbolos

```c
void print_symbol_table() {
    printf("\n=== TABELA DE SÍMBOLOS ===\n");
    printf("%-20s %-10s %-8s %-8s %-10s\n", 
           "Nome", "Tipo", "Escopo", "Linha", "Usado?");
    printf("%-20s %-10s %-8s %-8s %-10s\n", 
           "----", "----", "------", "-----", "------");
    
    Scope* scope = current_scope;
    while (scope) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashEntry* entry = scope->symbols[i];
            while (entry) {
                Symbol* sym = entry->symbol;
                printf("%-20s %-10s %-8d %-8d %-10s\n",
                       sym->name,
                       type_to_string(sym->type),
                       sym->scope_level,
                       sym->line_declared,
                       sym->is_used ? "Sim" : "Não");
                entry = entry->next;
            }
        }
        scope = scope->parent;
    }
}
```

### Escopos Aninhados

**Regras de escopo**:

1. **Escopo Léxico** (usado pela maioria das linguagens)
   - Determinado pela estrutura do código
   - Blocos aninhados têm acesso a variáveis externas

2. **Sombreamento** (*Shadowing*)
   - Variável interna "esconde" variável externa de mesmo nome
   - Busca sempre encontra a declaração mais interna

**Exemplo**:
```c
int x = 10;        // Escopo global

void func() {
    int x = 20;    // Sombreia x global
    
    if (x > 15) {
        int x = 30;  // Sombreia x da função
        // Aqui, x vale 30
    }
    
    // Aqui, x vale 20
}

// Aqui, x vale 10
```

---

## Sistema de Tipos

### Tipos Primitivos

Tipos básicos fornecidos pela linguagem:

```c
typedef enum {
    TYPE_VOID,      // void (sem tipo)
    TYPE_INT,       // int
    TYPE_FLOAT,     // float
    TYPE_CHAR,      // char
    TYPE_BOOL,      // bool
    TYPE_STRING,    // string
    TYPE_ERROR      // tipo de erro
} PrimitiveType;
```

### Tipos Compostos

1. **Arrays**: `int arr[10]`
2. **Estruturas**: `struct Point { int x, y; }`
3. **Ponteiros**: `int* ptr`
4. **Funções**: `int (*func)(int, int)`

### Verificação de Tipos

#### Regras de Compatibilidade

**Operações Aritméticas** (`+`, `-`, `*`, `/`):
```
int    OP  int    → int
float  OP  float  → float
int    OP  float  → float  (promoção de int para float)
float  OP  int    → float  (promoção de int para float)
outros              → ERRO
```

**Operações Relacionais** (`<`, `>`, `<=`, `>=`, `==`, `!=`):
```
int    CMP  int    → bool
float  CMP  float  → bool
int    CMP  float  → bool  (promoção)
float  CMP  int    → bool  (promoção)
outros              → ERRO
```

**Operações Lógicas** (`&&`, `||`, `!`):
```
bool  OP  bool  → bool
outros          → ERRO
```

#### Implementação de Verificação

```c
SymbolType check_binary_operation(char* op, SymbolType left, SymbolType right) {
    // Operadores aritméticos
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
        strcmp(op, "*") == 0 || strcmp(op, "/") == 0) {
        
        if (left == TYPE_INT && right == TYPE_INT) {
            return TYPE_INT;
        }
        if ((left == TYPE_FLOAT || left == TYPE_INT) &&
            (right == TYPE_FLOAT || right == TYPE_INT)) {
            return TYPE_FLOAT;
        }
        
        semantic_error("Operação aritmética com tipos incompatíveis");
        return TYPE_ERROR;
    }
    
    // Operadores relacionais
    if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 ||
        strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0 ||
        strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {
        
        if ((left == TYPE_INT || left == TYPE_FLOAT) &&
            (right == TYPE_INT || right == TYPE_FLOAT)) {
            return TYPE_BOOL;
        }
        
        semantic_error("Comparação entre tipos incompatíveis");
        return TYPE_ERROR;
    }
    
    // Operadores lógicos
    if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
        if (left == TYPE_BOOL && right == TYPE_BOOL) {
            return TYPE_BOOL;
        }
        
        semantic_error("Operador lógico requer operandos booleanos");
        return TYPE_ERROR;
    }
    
    return TYPE_ERROR;
}
```

### Coerção de Tipos

**Coerção implícita** (conversão automática):
```c
int x = 10;
float y = x;  // OK: int → float (alargamento)
```

**Coerção explícita** (casting):
```c
float x = 3.14;
int y = (int)x;  // OK: float → int (estreitamento, com cast)
```

#### Hierarquia de Conversões

```
char → int → float → double
```

**Regra**: Conversões "para cima" (alargamento) são seguras e automáticas. Conversões "para baixo" (estreitamento) requerem cast explícito.

### Inferência de Tipos

Algumas linguagens modernas suportam **inferência de tipos**:

```c
// Em linguagens com inferência (ex: Rust, Haskell)
var x = 10;        // Tipo inferido: int
var y = 3.14;      // Tipo inferido: float
var z = x + y;     // Tipo inferido: float
```

**Algoritmo de Hindley-Milner** (1969): Sistema de inferência de tipos para cálculo lambda tipado, usado em ML, Haskell.

---

## Análise de Escopo

### Regras de Escopo

1. **Declaração antes do uso**
   ```c
   x = 10;   // ERRO: x não declarado
   int x;
   ```

2. **Visibilidade em blocos aninhados**
   ```c
   int x = 1;
   {
       int y = 2;
       // x e y visíveis aqui
   }
   // apenas x visível aqui
   ```

3. **Redeclaração no mesmo escopo**
   ```c
   int x = 1;
   int x = 2;  // ERRO: redeclaração
   ```

### Verificação de Escopo na AST

```c
void check_statement(ASTNode* node) {
    switch (node->type) {
        case NODE_COMPOUND:
            enter_scope();  // Novo bloco = novo escopo
            for (int i = 0; i < node->child_count; i++) {
                check_statement(node->children[i]);
            }
            exit_scope();
            break;
            
        case NODE_VAR_DECL:
            // Verificar redeclaração
            if (lookup_current_scope(node->value)) {
                semantic_error("Variável '%s' já declarada neste escopo", 
                             node->value);
            } else {
                insert_symbol(node->value, node->var_type, node->line);
            }
            break;
            
        case NODE_IDENTIFIER:
            // Verificar se foi declarado
            Symbol* sym = lookup_symbol(node->value);
            if (!sym) {
                semantic_error("Variável '%s' não declarada", node->value);
            }
            node->expr_type = sym->type;  // Anotar tipo na AST
            break;
    }
}
```

### Variáveis Não Utilizadas

Detectar variáveis declaradas mas nunca usadas:

```c
void check_unused_variables() {
    Scope* scope = current_scope;
    
    while (scope) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashEntry* entry = scope->symbols[i];
            while (entry) {
                Symbol* sym = entry->symbol;
                if (!sym->is_used) {
                    warning("Variável '%s' declarada mas não utilizada (linha %d)",
                           sym->name, sym->line_declared);
                }
                entry = entry->next;
            }
        }
        scope = scope->parent;
    }
}
```

---

## Verificações Semânticas

### 1. Verificação de Atribuições

```c
void check_assignment(ASTNode* node) {
    // node->left: variável de destino
    // node->right: expressão de valor
    
    Symbol* var = lookup_symbol(node->left->value);
    if (!var) {
        semantic_error("Variável '%s' não declarada", node->left->value);
        return;
    }
    
    SymbolType expr_type = check_expression(node->right);
    
    if (!types_compatible(var->type, expr_type)) {
        semantic_error("Incompatibilidade de tipos na atribuição:\n"
                      "  Esperado: %s\n"
                      "  Obtido: %s",
                      type_to_string(var->type),
                      type_to_string(expr_type));
    }
    
    var->is_initialized = 1;  // Marcar como inicializado
}
```

### 2. Verificação de Expressões

```c
SymbolType check_expression(ASTNode* node) {
    switch (node->type) {
        case NODE_NUMBER:
            return TYPE_INT;
            
        case NODE_FLOAT_LITERAL:
            return TYPE_FLOAT;
            
        case NODE_IDENTIFIER: {
            Symbol* sym = lookup_symbol(node->value);
            if (!sym) {
                semantic_error("Variável '%s' não declarada", node->value);
                return TYPE_ERROR;
            }
            
            if (!sym->is_initialized) {
                warning("Variável '%s' usada antes de ser inicializada",
                       node->value);
            }
            
            return sym->type;
        }
        
        case NODE_BINARY_OP: {
            SymbolType left_type = check_expression(node->left);
            SymbolType right_type = check_expression(node->right);
            return check_binary_operation(node->value, left_type, right_type);
        }
        
        case NODE_UNARY_OP: {
            SymbolType operand_type = check_expression(node->left);
            
            if (strcmp(node->value, "-") == 0 || strcmp(node->value, "+") == 0) {
                if (operand_type != TYPE_INT && operand_type != TYPE_FLOAT) {
                    semantic_error("Operador unário aritmético requer tipo numérico");
                    return TYPE_ERROR;
                }
                return operand_type;
            }
            
            if (strcmp(node->value, "!") == 0) {
                if (operand_type != TYPE_BOOL) {
                    semantic_error("Operador lógico '!' requer tipo booleano");
                    return TYPE_ERROR;
                }
                return TYPE_BOOL;
            }
            
            return TYPE_ERROR;
        }
        
        default:
            return TYPE_ERROR;
    }
}
```

### 3. Verificação de Chamadas de Função

```c
void check_function_call(ASTNode* node) {
    // node->value: nome da função
    // node->children: argumentos
    
    Symbol* func = lookup_symbol(node->value);
    if (!func) {
        semantic_error("Função '%s' não declarada", node->value);
        return;
    }
    
    if (func->type != TYPE_FUNCTION) {
        semantic_error("'%s' não é uma função", node->value);
        return;
    }
    
    // Verificar número de argumentos
    if (node->child_count != func->num_params) {
        semantic_error("Função '%s' espera %d argumentos, mas recebeu %d",
                      node->value, func->num_params, node->child_count);
        return;
    }
    
    // Verificar tipos dos argumentos
    for (int i = 0; i < node->child_count; i++) {
        SymbolType arg_type = check_expression(node->children[i]);
        SymbolType expected_type = func->param_types[i];
        
        if (!types_compatible(expected_type, arg_type)) {
            semantic_error("Argumento %d da função '%s': "
                          "esperado %s, obtido %s",
                          i + 1, node->value,
                          type_to_string(expected_type),
                          type_to_string(arg_type));
        }
    }
    
    // Anotar tipo de retorno na AST
    node->expr_type = func->return_type;
}
```

### 4. Verificação de Comandos de Controle

```c
// Contexto global para verificar break/continue
int loop_depth = 0;

void check_while_statement(ASTNode* node) {
    // node->left: condição
    // node->right: corpo
    
    SymbolType cond_type = check_expression(node->left);
    if (cond_type != TYPE_BOOL) {
        semantic_error("Condição do 'while' deve ser booleana");
    }
    
    loop_depth++;  // Entrar em loop
    check_statement(node->right);
    loop_depth--;  // Sair do loop
}

void check_break_statement(ASTNode* node) {
    if (loop_depth == 0) {
        semantic_error("'break' fora de loop");
    }
}

void check_return_statement(ASTNode* node, SymbolType expected_return) {
    if (node->left) {  // return com valor
        SymbolType return_type = check_expression(node->left);
        
        if (!types_compatible(expected_return, return_type)) {
            semantic_error("Tipo de retorno incompatível:\n"
                          "  Esperado: %s\n"
                          "  Obtido: %s",
                          type_to_string(expected_return),
                          type_to_string(return_type));
        }
    } else {  // return sem valor
        if (expected_return != TYPE_VOID) {
            semantic_error("Função deve retornar valor do tipo %s",
                          type_to_string(expected_return));
        }
    }
}
```

---

## Implementação em C

### Estrutura Completa do Analisador Semântico

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define TABLE_SIZE 211
#define MAX_ERRORS 100

// ========== TIPOS ==========
typedef enum {
    TYPE_VOID, TYPE_INT, TYPE_FLOAT, TYPE_BOOL, 
    TYPE_STRING, TYPE_FUNCTION, TYPE_ERROR
} SymbolType;

typedef struct Symbol {
    char* name;
    SymbolType type;
    int scope_level;
    int line_declared;
    int is_initialized;
    int is_used;
    
    // Para funções
    int num_params;
    SymbolType* param_types;
    SymbolType return_type;
} Symbol;

typedef struct HashEntry {
    Symbol* symbol;
    struct HashEntry* next;
} HashEntry;

typedef struct Scope {
    HashEntry* symbols[TABLE_SIZE];
    struct Scope* parent;
    int level;
} Scope;

// ========== VARIÁVEIS GLOBAIS ==========
Scope* current_scope = NULL;
int error_count = 0;
int warning_count = 0;

// ========== FUNÇÕES AUXILIARES ==========

const char* type_to_string(SymbolType type) {
    switch (type) {
        case TYPE_VOID: return "void";
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_BOOL: return "bool";
        case TYPE_STRING: return "string";
        case TYPE_FUNCTION: return "function";
        case TYPE_ERROR: return "error";
        default: return "unknown";
    }
}

void semantic_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "ERRO SEMÂNTICO: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
    error_count++;
}

void warning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "AVISO: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
    warning_count++;
}

unsigned int hash(const char* name) {
    unsigned int h = 0;
    while (*name) {
        h = (h << 5) + h + *name++;
    }
    return h % TABLE_SIZE;
}

int types_compatible(SymbolType expected, SymbolType actual) {
    if (expected == actual) return 1;
    
    // Promoção de int para float
    if (expected == TYPE_FLOAT && actual == TYPE_INT) return 1;
    
    return 0;
}

// ========== GERENCIAMENTO DE ESCOPO ==========

void init_semantic_analyzer() {
    current_scope = malloc(sizeof(Scope));
    memset(current_scope->symbols, 0, sizeof(current_scope->symbols));
    current_scope->parent = NULL;
    current_scope->level = 0;
}

void enter_scope() {
    Scope* new_scope = malloc(sizeof(Scope));
    memset(new_scope->symbols, 0, sizeof(new_scope->symbols));
    new_scope->parent = current_scope;
    new_scope->level = current_scope ? current_scope->level + 1 : 0;
    current_scope = new_scope;
}

void exit_scope() {
    if (current_scope && current_scope->parent) {
        Scope* old_scope = current_scope;
        current_scope = current_scope->parent;
        // Liberar símbolos do escopo antigo
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashEntry* entry = old_scope->symbols[i];
            while (entry) {
                HashEntry* next = entry->next;
                free(entry->symbol->name);
                if (entry->symbol->param_types) {
                    free(entry->symbol->param_types);
                }
                free(entry->symbol);
                free(entry);
                entry = next;
            }
        }
        free(old_scope);
    }
}

// ========== TABELA DE SÍMBOLOS ==========

void insert_symbol(const char* name, SymbolType type, int line) {
    // Verificar redeclaração no escopo atual
    unsigned int h = hash(name);
    HashEntry* entry = current_scope->symbols[h];
    
    while (entry) {
        if (strcmp(entry->symbol->name, name) == 0) {
            semantic_error("Variável '%s' já declarada no escopo atual (linha %d)",
                          name, entry->symbol->line_declared);
            return;
        }
        entry = entry->next;
    }
    
    // Criar novo símbolo
    Symbol* sym = malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type;
    sym->scope_level = current_scope->level;
    sym->line_declared = line;
    sym->is_initialized = 0;
    sym->is_used = 0;
    sym->num_params = 0;
    sym->param_types = NULL;
    
    // Inserir na tabela
    HashEntry* new_entry = malloc(sizeof(HashEntry));
    new_entry->symbol = sym;
    new_entry->next = current_scope->symbols[h];
    current_scope->symbols[h] = new_entry;
}

Symbol* lookup_symbol(const char* name) {
    Scope* scope = current_scope;
    
    while (scope) {
        unsigned int h = hash(name);
        HashEntry* entry = scope->symbols[h];
        
        while (entry) {
            if (strcmp(entry->symbol->name, name) == 0) {
                entry->symbol->is_used = 1;
                return entry->symbol;
            }
            entry = entry->next;
        }
        
        scope = scope->parent;
    }
    
    return NULL;
}

void print_symbol_table() {
    printf("\n=== TABELA DE SÍMBOLOS ===\n");
    printf("%-20s %-10s %-8s %-8s %-10s\n", 
           "Nome", "Tipo", "Escopo", "Linha", "Usado?");
    printf("%-20s %-10s %-8s %-8s %-10s\n", 
           "----", "----", "------", "-----", "------");
    
    Scope* scope = current_scope;
    while (scope) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashEntry* entry = scope->symbols[i];
            while (entry) {
                Symbol* sym = entry->symbol;
                printf("%-20s %-10s %-8d %-8d %-10s\n",
                       sym->name,
                       type_to_string(sym->type),
                       sym->scope_level,
                       sym->line_declared,
                       sym->is_used ? "Sim" : "Não");
                entry = entry->next;
            }
        }
        scope = scope->parent;
    }
}

// ========== VERIFICAÇÃO SEMÂNTICA ==========

SymbolType check_expression(ASTNode* node) {
    // Implementação como mostrado anteriormente
    // ...
    return TYPE_INT;  // Placeholder
}

void check_statement(ASTNode* node) {
    // Implementação como mostrado anteriormente
    // ...
}

void semantic_analysis(ASTNode* ast) {
    printf("\n=== ANÁLISE SEMÂNTICA ===\n");
    
    init_semantic_analyzer();
    check_statement(ast);
    
    printf("\nErros: %d\n", error_count);
    printf("Avisos: %d\n", warning_count);
    
    if (error_count == 0) {
        printf("\n✓ Análise semântica concluída com sucesso!\n");
    } else {
        printf("\n✗ Análise semântica falhou.\n");
    }
    
    print_symbol_table();
}
```

---

## Tratamento de Erros Semânticos

### Categorias de Erros

1. **Erros Fatais** (impedem compilação)
   - Variável não declarada
   - Incompatibilidade de tipos
   - Redeclaração de identificador

2. **Avisos** (não impedem compilação)
   - Variável não utilizada
   - Variável não inicializada
   - Conversão implícita

### Relatório de Erros Detalhado

```c
typedef struct SemanticError {
    char message[256];
    int line;
    int column;
    char* hint;
} SemanticError;

SemanticError errors[MAX_ERRORS];
int error_index = 0;

void report_error(const char* msg, int line, int col, const char* hint) {
    if (error_index < MAX_ERRORS) {
        snprintf(errors[error_index].message, 256, "%s", msg);
        errors[error_index].line = line;
        errors[error_index].column = col;
        errors[error_index].hint = hint ? strdup(hint) : NULL;
        error_index++;
    }
}

void print_error_report() {
    printf("\n=== RELATÓRIO DE ERROS ===\n");
    for (int i = 0; i < error_index; i++) {
        printf("\nErro #%d [%d:%d]:\n", 
               i + 1, errors[i].line, errors[i].column);
        printf("  %s\n", errors[i].message);
        if (errors[i].hint) {
            printf("  Dica: %s\n", errors[i].hint);
        }
    }
}
```

### Recuperação de Erros

Estratégias para continuar análise após erro:

1. **Modo Pânico**: Pular até próximo delimitador seguro (`;`, `}`)
2. **Tipo Erro**: Atribuir `TYPE_ERROR` para prevenir erros cascata
3. **Símbolo Fictício**: Inserir símbolo temporário para continuar

```c
SymbolType check_expression_safe(ASTNode* node) {
    if (!node) return TYPE_ERROR;
    
    SymbolType type = check_expression(node);
    
    // Se houver erro, retornar TYPE_ERROR para evitar erros cascata
    if (type == TYPE_ERROR) {
        return TYPE_ERROR;
    }
    
    return type;
}
```

---

## Exemplos Práticos

### Exemplo 1: Programa Simples Correto

**Código-fonte**:
```c
int main() {
    int x = 10;
    int y = 20;
    int soma = x + y;
    return 0;
}
```

**Análise Semântica**:
```
✓ Declarações: todas as variáveis declaradas
✓ Tipos: todas as operações com tipos compatíveis
✓ Escopo: todas as variáveis visíveis nos pontos de uso
✓ Retorno: tipo de retorno compatível (int)

Tabela de Símbolos:
  main:   function, retorno int
  x:      int, escopo 1
  y:      int, escopo 1
  soma:   int, escopo 1
```

### Exemplo 2: Erro de Tipo

**Código-fonte**:
```c
int main() {
    int x = "texto";  // ERRO: atribuição incompatível
    return 0;
}
```

**Saída do Analisador**:
```
ERRO SEMÂNTICO (linha 2): Incompatibilidade de tipos na atribuição
  Esperado: int
  Obtido: string
  Dica: Use conversão explícita ou corrija o tipo da variável
```

### Exemplo 3: Variável Não Declarada

**Código-fonte**:
```c
int main() {
    x = 10;  // ERRO: x não declarado
    return 0;
}
```

**Saída do Analisador**:
```
ERRO SEMÂNTICO (linha 2): Variável 'x' não declarada
  Dica: Declare a variável antes de usá-la (ex: int x;)
```

### Exemplo 4: Escopo Aninhado

**Código-fonte**:
```c
int x = 1;  // Escopo global

int main() {
    int y = 2;  // Escopo função
    
    if (y > 0) {
        int z = 3;  // Escopo bloco
        x = x + z;  // OK: x global, z local
    }
    
    // z = 4;  // ERRO: z não visível aqui
    
    return 0;
}
```

**Tabela de Símbolos**:
```
Nome     Tipo    Escopo  Linha   Usado?
----     ----    ------  -----   ------
x        int     0       1       Sim
main     func    0       3       Sim
y        int     1       4       Sim
z        int     2       7       Sim
```

### Exemplo 5: Verificação de Função

**Código-fonte**:
```c
int soma(int a, int b) {
    return a + b;
}

int main() {
    int x = soma(10, 20);      // OK
    int y = soma(10);          // ERRO: argumentos insuficientes
    int z = soma(10, "abc");   // ERRO: tipo incompatível
    return 0;
}
```

**Saída do Analisador**:
```
ERRO SEMÂNTICO (linha 7): Função 'soma' espera 2 argumentos, mas recebeu 1

ERRO SEMÂNTICO (linha 8): Argumento 2 da função 'soma':
  Esperado: int
  Obtido: string
```

---

## Como Compilar e Usar

### Estrutura do Projeto

```
src/09-analisador-semantico/
├── exemploSimples.c        # Exemplo básico
├── exemploCompleto.c       # Exemplo completo
├── exemploSimplificado.c   # Versão didática
├── entrada.txt             # Arquivo de teste válido
├── entradaInvalida.txt     # Arquivo de teste com erros
└── README.md               # Este arquivo
```

### Compilação

```bash
# Compilar o exemplo simples
gcc exemploSimples.c -o exemploSimples

# Compilar o exemplo completo
gcc exemploCompleto.c -o exemploCompleto -std=c99

# Compilar com warnings extras
gcc exemploCompleto.c -o exemploCompleto -Wall -Wextra
```

### Execução

```bash
# Executar análise semântica
./exemploCompleto

# Com arquivo de entrada
./exemploCompleto < entrada.txt

# Redirecionar saída
./exemploCompleto > analise.txt
```

### Exemplo de Entrada Válida

Arquivo `entrada.txt`:
```c
int main() {
    int x = 10;
    int y = 20;
    int z = x + y;
    
    if (z > 25) {
        return 1;
    }
    
    return 0;
}
```

### Saída Esperada

```
=== ANÁLISE SEMÂNTICA ===
Analisando programa...

✓ Declaração de função 'main'
✓ Declaração de variável 'x' (tipo: int)
✓ Declaração de variável 'y' (tipo: int)
✓ Declaração de variável 'z' (tipo: int)
✓ Expressão 'x + y' (tipo: int)
✓ Condição 'z > 25' (tipo: bool)
✓ Retorno compatível com tipo da função

Erros: 0
Avisos: 0

✓ Análise semântica concluída com sucesso!

=== TABELA DE SÍMBOLOS ===
Nome                 Tipo       Escopo   Linha    Usado?
----                 ----       ------   -----    ------
main                 function   0        1        Sim
x                    int        1        2        Sim
y                    int        1        3        Sim
z                    int        1        4        Sim
```

---

## Referências Acadêmicas

### Livros-Texto Fundamentais

1. **Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D.** (2006). *Compiladores: Princípios, Técnicas e Ferramentas*. 2ª Edição. Pearson Education.
   - **Capítulo 5 e 6**: Análise Semântica e Ambientes de Execução
   - Gramáticas de atributos e verificação de tipos
   - Tabelas de símbolos e gerenciamento de memória
   - **Essencial**: Referência definitiva para análise semântica

2. **Appel, A. W.** (2002). *Modern Compiler Implementation in C*. Cambridge University Press.
   - **Capítulo 5**: Semantic Analysis
   - Implementação prática em C
   - Sistemas de tipos e verificação
   - Tabela de símbolos eficiente

3. **Louden, K. C.** (1997). *Compiler Construction: Principles and Practice*. PWS Publishing Company.
   - Abordagem didática clara
   - Exemplos passo a passo
   - Tratamento detalhado de escopos
   - Verificação de tipos prática

4. **Cooper, K. D., & Torczon, L.** (2011). *Engineering a Compiler*. 2ª Edição. Morgan Kaufmann.
   - **Capítulo 4 e 5**: Context-Sensitive Analysis
   - Análise de fluxo de dados
   - Otimizações semânticas
   - Aspectos de engenharia

### Teoria de Tipos

5. **Pierce, B. C.** (2002). *Types and Programming Languages*. MIT Press.
   - Fundamentos matemáticos de sistemas de tipos
   - Cálculo lambda tipado
   - Polimorfismo e subtipagem
   - Inferência de tipos (Hindley-Milner)
   - **Referência**: Teoria formal de tipos

6. **Cardelli, L.** (1996). "Type Systems". *The Computer Science and Engineering Handbook*.
   - Taxonomia de sistemas de tipos
   - Segurança de tipos
   - Tipagem forte vs fraca
   - Verificação estática vs dinâmica

### Semântica Formal

7. **Winskel, G.** (1993). *The Formal Semantics of Programming Languages*. MIT Press.
   - Semântica operacional
   - Semântica denotacional
   - Semântica axiomática
   - Fundamentos matemáticos

8. **Nielson, H. R., & Nielson, F.** (1992). *Semantics with Applications*. Wiley.
   - Semântica estrutural
   - Análise estática
   - Verificação de programas
   - Aplicações práticas

### Gramáticas de Atributos

9. **Knuth, D. E.** (1968). "Semantics of Context-Free Languages". *Mathematical Systems Theory*, 2(2), 127-145.
   - **Artigo seminal**: Introdução de gramáticas de atributos
   - Atributos sintetizados e herdados
   - Base teórica da análise semântica

10. **Waite, W. M., & Goos, G.** (1984). *Compiler Construction*. Springer-Verlag.
    - Implementação de gramáticas de atributos
    - Estratégias de avaliação
    - Geração automática de analisadores

### Sistemas de Tipos Específicos

11. **Milner, R.** (1978). "A Theory of Type Polymorphism in Programming". *Journal of Computer and System Sciences*, 17, 348-375.
    - Polimorfismo paramétrico
    - Algoritmo de unificação
    - Base para ML e Haskell

12. **Hindley, R.** (1969). "The Principal Type-Scheme of an Object in Combinatory Logic". *Transactions of the American Mathematical Society*, 146, 29-60.
    - Inferência de tipos
    - Tipos principais
    - Fundamento teórico

### Implementações Práticas

13. **Muchnick, S. S.** (1997). *Advanced Compiler Design and Implementation*. Morgan Kaufmann.
    - Técnicas avançadas de análise
    - Otimizações interprocedurais
    - Análise de ponteiros

14. **Wilhelm, R., & Maurer, D.** (1995). *Compiler Design*. Addison-Wesley.
    - Análise de fluxo de dados
    - Análise de dependências
    - Verificação de propriedades

### Ferramentas e Geradores

15. **Johnson, S. C.** (1975). "Yacc: Yet Another Compiler-Compiler". *Computing Science Technical Report*, Bell Laboratories.
    - Gerador de parsers com ações semânticas
    - Integração de análise sintática e semântica

16. **Levine, J. R.** (2009). *Flex & Bison*. O'Reilly Media.
    - Uso prático de geradores
    - Ações semânticas em Yacc/Bison
    - Construção de tabelas de símbolos

### Recursos Online

17. **Stanford Compiler Course**: http://web.stanford.edu/class/cs143/
    - Materiais de curso de compiladores
    - Slides sobre análise semântica
    - Exercícios e projetos

18. **LLVM Tutorial**: https://llvm.org/docs/tutorial/
    - Implementação de análise semântica
    - Geração de código intermediário
    - Exemplos práticos

19. **GCC Internals**: https://gcc.gnu.org/onlinedocs/gccint/
    - Documentação interna do GCC
    - Análise semântica em compilador real
    - Representações intermediárias

---

## Exercícios Propostos

### Básicos

1. **Tabela de símbolos simples**: Implemente uma tabela de símbolos usando lista encadeada
2. **Verificação de declaração**: Detecte uso de variáveis não declaradas
3. **Tipos primitivos**: Implemente verificação de tipos para int e float
4. **Redeclaração**: Detecte variáveis declaradas mais de uma vez no mesmo escopo

### Intermediários

5. **Escopos aninhados**: Implemente pilha de escopos com enter_scope() e exit_scope()
6. **Verificação de expressões**: Verifique tipos em expressões aritméticas (+, -, *, /)
7. **Operadores relacionais**: Implemente verificação para <, >, <=, >=, ==, !=
8. **Avisos**: Detecte variáveis declaradas mas nunca utilizadas
9. **Inicialização**: Avise sobre uso de variáveis não inicializadas

### Avançados

10. **Chamadas de função**: Verifique número e tipos de argumentos
11. **Arrays**: Implemente verificação de índices e tipos de arrays
12. **Estruturas**: Suporte para structs com acesso a campos
13. **Conversões implícitas**: Implemente coerção automática de int para float
14. **Análise de fluxo**: Detecte código inalcançável após return

### Desafios

15. **Inferência de tipos**: Implemente inferência básica de tipos (estilo auto em C++)
16. **Análise interprocedimental**: Verifique compatibilidade entre módulos
17. **Otimização semântica**: Implemente constant folding e propagation
18. **Verificação de ponteiros**: Detecte dereferenciamento de ponteiros NULL
19. **Sistema de tipos complexo**: Implemente polimorfismo paramétrico básico

---

## Aplicações Reais da Análise Semântica

### 1. Compiladores Modernos

- **GCC/Clang**: Análise semântica completa para C/C++
  - Verificação de tipos complexos (templates, sobrecarga)
  - Análise de ponteiros e memória
  - Avisos sobre código suspeito

- **Java Compiler (javac)**: 
  - Verificação de tipos estrita
  - Análise de exceções (checked exceptions)
  - Verificação de acesso a membros

- **Rust Compiler**: 
  - Sistema de ownership e borrowing
  - Análise de tempo de vida (lifetimes)
  - Verificação de segurança de memória em compile-time

### 2. IDEs e Editores

- **IntelliJ IDEA**: 
  - Análise semântica em tempo real
  - Detecção de erros antes da compilação
  - Sugestões de refatoração

- **Visual Studio Code**:
  - Language servers (LSP) com análise semântica
  - Code completion inteligente
  - Navegação por símbolos

### 3. Ferramentas de Análise Estática

- **Coverity**: Detecta bugs complexos via análise semântica
- **SonarQube**: Análise de qualidade de código
- **Clang Static Analyzer**: Encontra bugs sem executar código

### 4. Linguagens Tipadas Gradualmente

- **TypeScript**: Adiciona tipos a JavaScript
- **Python Type Hints**: Análise opcional de tipos
- **PHP com Psalm/PHPStan**: Verificação estática

### 5. Verificação Formal

- **SPARK Ada**: Prova formal de correção
- **Dafny**: Linguagem com verificação automática
- **F***: Verificação de segurança e correção

---

## Resumo dos Conceitos-Chave

| Conceito | Descrição | Importância |
|----------|-----------|-------------|
| **Tabela de Símbolos** | Estrutura que armazena informações sobre identificadores | Fundamental para todas as verificações |
| **Sistema de Tipos** | Classificação de valores e operações | Previne erros de tipo |
| **Análise de Escopo** | Verificação de visibilidade de identificadores | Garante uso correto de variáveis |
| **Gramáticas de Atributos** | Formalismo para especificar semântica | Base teórica |
| **Verificação Estática** | Detecção de erros sem executar | Aumenta confiabilidade |
| **Coerção de Tipos** | Conversão automática entre tipos | Flexibilidade vs segurança |

---

## Conclusão

A análise semântica é uma fase **essencial** no processo de compilação. Ela garante que programas sintaticamente corretos também sejam **semanticamente válidos**, detectando erros que não podem ser capturados pela análise léxica ou sintática.

**Principais takeaways**:

1. ✅ **Tabela de símbolos é fundamental**: Armazena informações sobre todos os identificadores
2. ✅ **Tipos previnem erros**: Verificação de tipos detecta incompatibilidades
3. ✅ **Escopos organizam contexto**: Gerenciamento correto de visibilidade
4. ✅ **Detecção precoce de erros**: Problemas encontrados antes da execução
5. ✅ **Preparação para fases posteriores**: Coleta informações para geração de código

Com os conceitos e implementações deste módulo, você está preparado para:
- Implementar analisadores semânticos completos
- Entender sistemas de tipos de linguagens reais
- Criar linguagens de programação com verificação estática
- Desenvolver ferramentas de análise de código

**Próximos passos**: Geração de código intermediário e otimizações, onde utilizamos as informações coletadas na análise semântica para produzir código eficiente!

---

*Este material faz parte da disciplina de Compiladores. Para questões ou sugestões, consulte o repositório do curso.*