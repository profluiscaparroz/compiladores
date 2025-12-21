# 07 — Analisador Léxico

## Índice
1. [Introdução](#introdução)
2. [Conceitos Fundamentais](#conceitos-fundamentais)
3. [Fundamentos Teóricos](#fundamentos-teóricos)
4. [Fases da Análise Léxica](#fases-da-análise-léxica)
5. [Formalismo Matemático](#formalismo-matemático)
6. [Implementação em C](#implementação-em-c)
7. [Tratamento de Erros Léxicos](#tratamento-de-erros-léxicos)
8. [Ferramentas e Geradores](#ferramentas-e-geradores)
9. [Otimizações e Desempenho](#otimizações-e-desempenho)
10. [Como Compilar e Usar](#como-compilar-e-usar)
11. [Referências Acadêmicas](#referências-acadêmicas)

---

## Introdução

A **análise léxica** (ou *scanning*) é a **primeira fase** de um compilador. Seu papel fundamental é ler a sequência de caracteres do código-fonte e agrupá-los em unidades significativas chamadas **tokens**. Esta fase atua como uma interface entre o texto bruto do programa e as fases subsequentes do compilador, simplificando significativamente o trabalho do analisador sintático.

### Importância da Análise Léxica

Como descrito por **Aho, Sethi e Ullman** (1986) no clássico "Compiladores: Princípios, Técnicas e Ferramentas" (conhecido como "Livro do Dragão"):

> "A análise léxica é a fase do compilador responsável por particionar a cadeia de entrada em unidades significativas chamadas tokens."

A análise léxica simplifica o projeto do compilador ao:
- **Abstrair detalhes de baixo nível**: Remove espaços em branco, comentários e formatação
- **Agrupar caracteres**: Identifica padrões significativos na entrada
- **Facilitar a análise sintática**: Fornece uma sequência estruturada de tokens
- **Melhorar o desempenho**: Permite otimizações específicas para leitura de caracteres
- **Modularizar o compilador**: Separa responsabilidades claramente definidas

### Objetivos deste Módulo

- Implementar um analisador léxico completo em C
- Reconhecer tokens de uma linguagem didática
- Aplicar conceitos de autômatos finitos e expressões regulares
- Compreender a teoria e prática da análise léxica
- Desenvolver estratégias de tratamento de erros

---

## Conceitos Fundamentais

### O que são Tokens?

Um **token** representa uma **categoria de símbolo léxico**, ou seja, uma unidade básica da linguagem de programação. Exemplos incluem:

- **Palavras-chave**: `if`, `while`, `return`, `int`, `float`
- **Identificadores**: `x`, `total`, `soma_media`, `contador`
- **Operadores**: `+`, `-`, `*`, `/`, `==`, `!=`, `<=`, `>=`
- **Delimitadores**: `(`, `)`, `{`, `}`, `;`, `,`
- **Literais numéricos**: `123`, `3.14`, `0xFF`, `2.5e-3`
- **Literais de string**: `"texto"`, `"Hello, World!"`

### Diferença entre Símbolo, Lexema e Token

**Andrew W. Appel** (1997), em "Modern Compiler Implementation", enfatiza a distinção clara entre estes conceitos:

| Termo | Definição | Exemplo |
|-------|-----------|---------|
| **Símbolo** | Um caractere individual da entrada | `a`, `1`, `=`, `+` |
| **Lexema** | Uma sequência concreta de caracteres que forma um token | `int`, `x1`, `123`, `<=` |
| **Token** | A representação abstrata da categoria do lexema | `KEYWORD`, `IDENTIFIER`, `NUMBER`, `RELOP` |

#### Exemplo Prático

Considere o código C:
```c
int x = 10;
```

A análise léxica produz:

| Lexema | Token | Atributo |
|--------|-------|----------|
| `int`  | `KEYWORD_INT` | — |
| `x`    | `IDENTIFIER` | "x" |
| `=`    | `ASSIGN_OP` | — |
| `10`   | `NUMBER` | valor: 10 |
| `;`    | `SEMICOLON` | — |

### Estrutura de um Token

Um token geralmente contém:
- **Tipo**: A categoria do token (ex: `IDENTIFIER`, `NUMBER`, `KEYWORD`)
- **Valor léxico**: O texto original ou valor associado (lexema)
- **Posição**: Linha e coluna no código-fonte (para mensagens de erro)
- **Atributos adicionais**: Informações específicas (ex: valor numérico, tipo de operador)

---

## Fundamentos Teóricos

### Linguagens Regulares e Expressões Regulares

A análise léxica é fundamentada na teoria das linguagens regulares. **Hopcroft, Motwani e Ullman** (2006), em "Introduction to Automata Theory, Languages, and Computation", estabelecem que:

> "As linguagens regulares são exatamente aquelas que podem ser reconhecidas por autômatos finitos."

Cada tipo de token pode ser descrito por uma **expressão regular**:

| Token | Expressão Regular | Descrição |
|-------|-------------------|-----------|
| Identificador | `[a-zA-Z_][a-zA-Z0-9_]*` | Letra ou `_`, seguido de letras, dígitos ou `_` |
| Número inteiro | `[0-9]+` | Um ou mais dígitos |
| Número real | `[0-9]+\.[0-9]+` | Dígitos, ponto, dígitos |
| Espaços em branco | `[ \t\n\r]+` | Espaços, tabs, quebras de linha |
| Operador de atribuição | `=` | Símbolo de igual |
| Operador de igualdade | `==` | Dois símbolos de igual |
| Operador relacional | `<=|>=|<|>` | Operadores de comparação |

### Autômatos Finitos

Segundo **Kenneth C. Louden** (1997), em *Compiler Construction: Principles and Practice*, o scanner é responsável por identificar os padrões léxicos, ignorar caracteres irrelevantes e reconhecer erros primitivos no texto-fonte.
As expressões regulares são implementadas através de **autômatos finitos**:

#### Autômato Finito Determinístico (AFD)

Um AFD para reconhecer identificadores em C:

```
Estado inicial: q0
Estados finais: {q1}

Transições:
  q0 --[a-zA-Z_]--> q1 (aceitação)
  q1 --[a-zA-Z0-9_]--> q1 (continua)
  q1 --[outros]--> erro
```

Diagrama de estados:
```
        [a-zA-Z_]
    ┌─────────────┐
    │             ↓
  (q0)          ((q1))
                  ↑ │
                  └─┘
              [a-zA-Z0-9_]
```

#### Exemplo: Reconhecimento de Números

AFD para números inteiros e reais:

```
        [0-9]         [0-9]
    ┌─────────┐   ┌─────────┐
    │         ↓   │         ↓
  (q0) ──→ ((q1)) ──.──→ (q2) ──→ ((q3))
                          │         ↑
                          └─────────┘
                             [0-9]

Legenda:
  (q0)  - Estado inicial
  ((q1)) - Estado final (número inteiro)
  (q2)  - Estado intermediário (após o ponto)
  ((q3)) - Estado final (número real)
```

### Conversão de Expressões Regulares para Autômatos

O processo padrão envolve:

1. **Construção de Thompson**: Converte expressão regular → AFN (Autômato Finito Não-determinístico)
2. **Construção de subconjuntos**: Converte AFN → AFD
3. **Minimização de Hopcroft**: Reduz o número de estados do AFD

---

## Fases da Análise Léxica

### 1. Leitura de Caracteres

A entrada é lida caractere por caractere, frequentemente usando um **buffer duplo** para otimizar operações de I/O:

```c
// Buffer duplo para leitura eficiente
char buffer1[BUFFER_SIZE];
char buffer2[BUFFER_SIZE];
int current_buffer = 0;
```

O analisador mantém dois ponteiros:
- **lexemeBegin**: Início do lexema atual
- **forward**: Posição atual de leitura

### 2. Reconhecimento de Padrões

O analisador tenta casar a sequência lida com padrões definidos. Aplica-se a **regra do maior prefixo** (*longest match rule*):

> "Sempre escolha o maior lexema possível que forme um token válido."

Exemplo:
- Entrada: `while`
- Casamento possível: `w`, `wh`, `whi`, `whil`, `while`
- Escolhido: `while` (palavra-chave)

### 3. Geração de Token

Após reconhecer o padrão, cria-se um token com:
- Tipo do token
- Valor léxico (lexema)
- Posição no código (linha, coluna)

### 4. Ignorar Elementos Irrelevantes

- **Espaços em branco**: ` `, `\t`, `\n`, `\r`
- **Comentários**: `//` (linha) e `/* */` (bloco)
- **Diretivas de pré-processador** (em algumas implementações)

---

## Formalismo Matemático

### Linguagens Regulares

Do ponto de vista teórico, a análise léxica trabalha com **linguagens regulares**, que são aquelas que podem ser descritas por:

- **Expressões regulares** (regex)
- **Autômatos finitos determinísticos (AFD)**
- **Autômatos finitos não-determinísticos (AFN)**
- **Gramáticas regulares** (tipo 3 na hierarquia de Chomsky)

### Teorema da Equivalência

**Teorema (Kleene, 1951)**: As seguintes representações são equivalentes:
1. Linguagens regulares
2. Autômatos finitos
3. Expressões regulares
4. Gramáticas regulares

Esta equivalência permite que escolhamos a representação mais conveniente para cada tarefa:
- **Especificação**: Expressões regulares (mais intuitivas)
- **Implementação**: Autômatos finitos (mais eficientes)

### Relação entre Modelos

```
Expressão Regular
       │
       │ (Construção de Thompson)
       ↓
      AFN
       │
       │ (Construção de subconjuntos)
       ↓
      AFD
       │
       │ (Minimização de Hopcroft)
       ↓
  AFD Mínimo
```

---

## Implementação em C

### Estrutura de Dados

```c
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

// Lista de palavras-chave
const char* keywords[] = {
    "if", "else", "while", "for", 
    "int", "float", "return"
};
const int num_keywords = 7;

// Tipos de tokens
typedef enum {
    TOKEN_KEYWORD,      // Palavra-chave
    TOKEN_IDENTIFIER,   // Identificador
    TOKEN_NUMBER,       // Número (inteiro ou real)
    TOKEN_OPERATOR,     // Operador (+, -, *, /, ==, !=, etc.)
    TOKEN_DELIMITER,    // Delimitador ( ) { } ; ,
    TOKEN_STRING,       // String literal
    TOKEN_COMMENT,      // Comentário
    TOKEN_UNKNOWN,      // Token desconhecido
    TOKEN_EOF          // Fim do arquivo
} TokenType;

// Estrutura do token
typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
    int line;
    int column;
} Token;
```

### Função Principal: Obter Próximo Token

```c
// Verifica se uma string é uma palavra-chave
int is_keyword(const char* str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Obtém o próximo token do arquivo
Token get_next_token(FILE* fp, int* line, int* column) {
    char ch;
    Token token;
    int i = 0;

    // Pular espaços em branco
    while ((ch = fgetc(fp)) != EOF && isspace(ch)) {
        if (ch == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
    }

    if (ch == EOF) {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        token.line = *line;
        token.column = *column;
        return token;
    }

    token.line = *line;
    token.column = *column;

    // Verificar e ignorar comentários
    if (ch == '/') {
        char next = fgetc(fp);
        if (next == '/') {
            // Comentário de linha
            while ((ch = fgetc(fp)) != EOF && ch != '\n');
            (*line)++;
            *column = 1;
            return get_next_token(fp, line, column);
        } else if (next == '*') {
            // Comentário de bloco
            char prev = 0;
            while ((ch = fgetc(fp)) != EOF) {
                if (ch == '\n') {
                    (*line)++;
                    *column = 1;
                }
                if (prev == '*' && ch == '/') {
                    break;
                }
                prev = ch;
            }
            return get_next_token(fp, line, column);
        } else {
            ungetc(next, fp);
        }
    }

    // String literal
    if (ch == '"') {
        token.type = TOKEN_STRING;
        token.lexeme[i++] = ch;

        while ((ch = fgetc(fp)) != EOF) {
            (*column)++;

            if (ch == '\\') {
                // Caractere escapado: processa a sequência de escape
                char esc = fgetc(fp);
                if (esc == EOF) {
                    break;
                }
                (*column)++;

                switch (esc) {
                    case 'n':
                        token.lexeme[i++] = '\n';
                        break;
                    case 't':
                        token.lexeme[i++] = '\t';
                        break;
                    case 'r':
                        token.lexeme[i++] = '\r';
                        break;
                    case '0':
                        token.lexeme[i++] = '\0';
                        break;
                    case '"':
                        token.lexeme[i++] = '"';
                        break;
                    case '\\':
                        token.lexeme[i++] = '\\';
                        break;
                    default:
                        // Escape desconhecido: mantém a barra e o caractere original
                        token.lexeme[i++] = '\\';
                        token.lexeme[i++] = esc;
                        break;
                }
            } else {
                token.lexeme[i++] = ch;
                if (ch == '"') {
                    break;
                }
            }

            if (i >= MAX_TOKEN_LENGTH - 2) {
                fprintf(stderr, "Erro: String muito longa na linha %d\n", *line);
                token.type = TOKEN_UNKNOWN;

                /* Consumir até o fim da string ou EOF para tentar recuperar */
                while ((ch = fgetc(fp)) != EOF) {
                    (*column)++;
                    if (ch == '\\') {
                        /* pular caractere escapado */
                        int esc = fgetc(fp);
                        if (esc == EOF) {
                            break;
                        }
                        (*column)++;
                    } else if (ch == '"') {
                        break;
                    } else if (ch == '\n') {
                        (*line)++;
                        *column = 1;
                    }
                }

                /* Garante terminação da string armazenada, mesmo truncada */
                if (i >= MAX_TOKEN_LENGTH) {
                    i = MAX_TOKEN_LENGTH - 1;
                }
                token.lexeme[i] = '\0';
                return token;
            }
        }

        token.lexeme[i] = '\0';
        return token;
    }

    // Identificadores e palavras-chave
    if (isalpha(ch) || ch == '_') {
        token.lexeme[i++] = ch;
        (*column)++;

        while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
            token.lexeme[i++] = ch;
            (*column)++;
        }
        token.lexeme[i] = '\0';
        ungetc(ch, fp);

        if (is_keyword(token.lexeme)) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }

    // Números (inteiros e reais)
    if (isdigit(ch)) {
        token.lexeme[i++] = ch;
        (*column)++;

        while ((ch = fgetc(fp)) != EOF && isdigit(ch)) {
            token.lexeme[i++] = ch;
            (*column)++;
        }

        // Verificar se é número real
        if (ch == '.') {
            token.lexeme[i++] = ch;
            (*column)++;

            while ((ch = fgetc(fp)) != EOF && isdigit(ch)) {
                token.lexeme[i++] = ch;
                (*column)++;
            }
        }

        token.lexeme[i] = '\0';
        ungetc(ch, fp);
        token.type = TOKEN_NUMBER;
        return token;
    }

    // Operadores compostos e símbolos
    if (strchr("=<>!&|+-*/", ch)) {
        char next = fgetc(fp);
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        (*column)++;

        // Verificar operadores compostos
        if ((ch == '=' && next == '=') ||
            (ch == '!' && next == '=') ||
            (ch == '<' && next == '=') ||
            (ch == '>' && next == '=') ||
            (ch == '&' && next == '&') ||
            (ch == '|' && next == '|')) {
            token.lexeme[1] = next;
            token.lexeme[2] = '\0';
            (*column)++;
        } else {
            ungetc(next, fp);
        }

        token.type = TOKEN_OPERATOR;
        return token;
    }

    // Delimitadores
    if (strchr("(){};,", ch)) {
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        token.type = TOKEN_DELIMITER;
        (*column)++;
        return token;
    }

    // Token desconhecido
    token.lexeme[0] = ch;
    token.lexeme[1] = '\0';
    token.type = TOKEN_UNKNOWN;
    (*column)++;
    return token;
}
```

### Função de Conversão de Tipo para String

```c
const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_DELIMITER: return "DELIMITER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}
```

### Programa Principal

```c
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "Erro ao abrir arquivo: %s\n", argv[1]);
        return 1;
    }

    int line = 1, column = 1;
    Token token;

    printf("%-15s %-20s %s\n", "TIPO", "LEXEMA", "POSIÇÃO (L:C)");
    printf("%-15s %-20s %s\n", "----", "------", "--------------");

    do {
        token = get_next_token(fp, &line, &column);
        printf("%-15s %-20s %d:%d\n", 
               token_type_to_string(token.type), 
               token.lexeme,
               token.line,
               token.column);
    } while (token.type != TOKEN_EOF);

    fclose(fp);
    return 0;
}
```

### Exemplo de Entrada

Arquivo `entrada.txt`:
```c
int main() {
    int x = 42;
    if (x > 0) {
        // Este é um comentário
        return x + 1;
    }
    return 0;
}
```

### Saída Esperada

```
TIPO            LEXEMA               POSIÇÃO
----            ------               --------
KEYWORD         int                  1:1
IDENTIFIER      main                 1:5
DELIMITER       (                    1:9
DELIMITER       )                    1:10
DELIMITER       {                    1:12
KEYWORD         int                  2:5
IDENTIFIER      x                    2:9
OPERATOR        =                    2:11
NUMBER          42                   2:13
DELIMITER       ;                    2:15
KEYWORD         if                   3:5
DELIMITER       (                    3:8
IDENTIFIER      x                    3:9
OPERATOR        >                    3:11
NUMBER          0                    3:13
DELIMITER       )                    3:14
DELIMITER       {                    3:16
KEYWORD         return               5:9
IDENTIFIER      x                    5:16
OPERATOR        +                    5:18
NUMBER          1                    5:20
DELIMITER       ;                    5:21
DELIMITER       }                    6:5
KEYWORD         return               7:5
NUMBER          0                    7:12
DELIMITER       ;                    7:13
DELIMITER       }                    8:1
EOF             EOF                  8:2
```

### Diagrama de Fluxo do Analisador Léxico

```
        ┌─────────────────┐
        │  Código-fonte   │
        └────────┬────────┘
                 │
                 ↓
        ┌─────────────────┐
        │ Ler caractere   │◄─────┐
        └────────┬────────┘      │
                 │                │
                 ↓                │
        ┌─────────────────┐      │
        │ Espaço/coment?  │──Sim─┤
        └────────┬────────┘      │
                 │                │
                Não               │
                 ↓                │
        ┌─────────────────┐      │
        │ Identificar     │      │
        │ padrão do token │      │
        └────────┬────────┘      │
                 │                │
                 ↓                │
        ┌─────────────────┐      │
        │ Criar token     │      │
        └────────┬────────┘      │
                 │                │
                 ↓                │
        ┌─────────────────┐      │
        │ Retornar token  │      │
        └────────┬────────┘      │
                 │                │
                 ↓                │
        ┌─────────────────┐      │
        │ EOF alcançado?  │──Não─┘
        └────────┬────────┘
                 │
                Sim
                 ↓
        ┌─────────────────┐
        │   Finalizar     │
        └─────────────────┘
```

---

## Tratamento de Erros Léxicos

### Tipos de Erros Léxicos

1. **Caracteres inválidos**: Símbolos não reconhecidos (ex: `@`, `$` em C)
2. **Identificadores mal formados**: `1var` (começa com dígito)
3. **Strings não fechadas**: `"texto sem fechar`
4. **Números mal formados**: `3.14.159` (múltiplos pontos decimais)
5. **Comentários não fechados**: `/* comentário sem */`

### Estratégias de Recuperação

**Segundo Louden (1997),** podem ser adotadas várias estratégias de recuperação de erros léxicos:

#### 1. Modo Pânico (*Panic Mode*)
```c
// Ignorar caracteres até encontrar um delimitador seguro
if (token.type == TOKEN_UNKNOWN) {
    fprintf(stderr, "Erro léxico na linha %d: caractere inválido '%c'\n", 
            token.line, token.lexeme[0]);
    // Continuar análise
}
```

#### 2. Correção de Erros
```c
// Tentar corrigir pequenos erros automaticamente
if (ch == '=' && next != '=') {
    // Poderia ser '==' mal digitado como '='
    fprintf(stderr, "Aviso: você quis dizer '==' em vez de '='?\n");
}
```

#### 3. Relatório Detalhado
```c
typedef struct {
    char message[256];
    int line;
    int column;
} LexicalError;

void report_error(LexicalError err) {
    fprintf(stderr, "Erro léxico [%d:%d]: %s\n", 
            err.line, err.column, err.message);
}
```

### Exemplo de Tratamento de String Não Fechada

```c
if (ch == '"') {
    // ... leitura da string ...
    
    if (ch == EOF) {
        fprintf(stderr, 
                "Erro léxico na linha %d: string não fechada\n", 
                token.line);
        token.type = TOKEN_UNKNOWN;
        return token;
    }
}
```

---

## Ferramentas e Geradores

### Lex e Flex

**Lex** (Lesk, 1975) é uma ferramenta clássica UNIX para geração automática de analisadores léxicos. **Flex** (Fast Lex) é sua versão moderna e mais eficiente.

#### Exemplo de Especificação Lex/Flex

```lex
%{
#include <stdio.h>
%}

%%

"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
[a-zA-Z_][a-zA-Z0-9_]*  { return IDENTIFIER; }
[0-9]+          { return NUMBER; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"=="            { return EQUAL; }
[ \t\n]+        { /* ignorar espaços */ }
.               { return UNKNOWN; }

%%

int yywrap(void) {
    return 1;
}
```

Compilação e uso:
```bash
flex lexer.l
gcc lex.yy.c -o lexer -lfl
./lexer < entrada.txt
```

### Outras Ferramentas

- **PLY (Python Lex-Yacc)**: Biblioteca Python para análise léxica e sintática
- **ANTLR**: Gerador de parsers que inclui análise léxica
- **JFlex**: Gerador de scanners para Java
- **Ragel**: Gerador de máquinas de estado para várias linguagens

---

## Otimizações e Desempenho

### 1. Buffer Duplo

Para evitar leituras frequentes de I/O:

```c
#define BUFFER_SIZE 4096

typedef struct {
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    int current;
    int pos;
    FILE* fp;
} DoubleBuffer;

int next_char(DoubleBuffer* db) {
    if (db->pos >= BUFFER_SIZE) {
        // Trocar buffer e recarregar
        db->current = 1 - db->current;
        db->pos = 0;
        size_t bytes_read = fread(db->current ? db->buffer2 : db->buffer1,
                                  1, BUFFER_SIZE, db->fp);
        if (bytes_read == 0) {
            return EOF;
        }
    }
    return db->current ?
           (unsigned char)db->buffer2[db->pos++] :
           (unsigned char)db->buffer1[db->pos++];
}
```

### 2. Tabela de Hash para Palavras-Chave

```c
#include <string.h>

#define HASH_SIZE 101

typedef struct KeywordNode {
    char* keyword;
    TokenType type;
    struct KeywordNode* next;
} KeywordNode;

KeywordNode* keyword_table[HASH_SIZE];

unsigned int hash(const char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + hash + *str++;
    }
    return hash % HASH_SIZE;
}

void insert_keyword(const char* keyword, TokenType type) {
    unsigned int h = hash(keyword);
    KeywordNode* node = malloc(sizeof(KeywordNode));
    node->keyword = strdup(keyword);
    node->type = type;
    node->next = keyword_table[h];
    keyword_table[h] = node;
}

TokenType lookup_keyword(const char* str) {
    unsigned int h = hash(str);
    KeywordNode* node = keyword_table[h];
    while (node) {
        if (strcmp(node->keyword, str) == 0) {
            return node->type;
        }
        node = node->next;
    }
    return TOKEN_IDENTIFIER;
}
```

### 3. Tabela de Transição Pré-computada

Para autômatos finitos, usar tabelas de transição:

```c
#define NUM_STATES 10
#define NUM_CHARS 256

int transition_table[NUM_STATES][NUM_CHARS];

void init_transition_table() {
    // Inicializar todas as transições como erro (-1)
    for (int i = 0; i < NUM_STATES; i++) {
        for (int j = 0; j < NUM_CHARS; j++) {
            transition_table[i][j] = -1;
        }
    }
    
    // Definir transições válidas
    // Estado 0 (inicial) -> Estado 1 (letra)
    for (char c = 'a'; c <= 'z'; c++) {
        transition_table[0][c] = 1;
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        transition_table[0][c] = 1;
    }
    
    // Estado 1 -> Estado 1 (letra ou dígito)
    for (char c = 'a'; c <= 'z'; c++) {
        transition_table[1][c] = 1;
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        transition_table[1][c] = 1;
    }
    for (char c = '0'; c <= '9'; c++) {
        transition_table[1][c] = 1;
    }
}

int next_state(int current_state, char ch) {
    return transition_table[current_state][(unsigned char)ch];
}
```

### 4. Caching e Memoização

```c
typedef struct {
    char lexeme[MAX_TOKEN_LENGTH];
    Token token;
} CacheEntry;

#define CACHE_SIZE 256
CacheEntry token_cache[CACHE_SIZE];

Token* lookup_cache(const char* lexeme) {
    unsigned int h = hash(lexeme) % CACHE_SIZE;
    if (strcmp(token_cache[h].lexeme, lexeme) == 0) {
        return &token_cache[h].token;
    }
    return NULL;
}
```

---

## Como Compilar e Usar

### Estrutura do Projeto

```
src/07-analisador-lexico/
├── include/
│   ├── lexer.h           # Interface do analisador léxico
│   └── token.h           # Definições de tokens
├── src/
│   ├── lexer.c           # Implementação principal
│   ├── token.c           # Funções auxiliares de tokens
│   └── main.c            # Programa principal
├── tests/
│   ├── exemplo1.tl       # Teste 1
│   ├── exemplo2.tl       # Teste 2
│   └── exemplo3.tl       # Teste 3
├── Makefile              # Script de compilação
└── README.md             # Este arquivo
```

### Compilação

```bash
# Compilar o analisador léxico
make

# Compilar com debug
make debug

# Limpar arquivos compilados
make clean

# Executar todos os testes
make test
```

### Uso

```bash
# Analisar um arquivo específico
./build/lexer arquivo.txt

# Analisar com saída detalhada
./build/lexer -v arquivo.txt

# Analisar e salvar tokens em arquivo
./build/lexer arquivo.txt > tokens.out
```

### Linguagem Didática Suportada

#### Palavras-chave
```
if, else, while, for, int, float, return
```

#### Identificadores
- Padrão: `[a-zA-Z_][a-zA-Z0-9_]*`
- Exemplos: `x`, `total`, `soma_valores`, `_temp`

#### Números
- Inteiros: `[0-9]+` → `42`, `123`, `0`
- Reais: `[0-9]+\.[0-9]+` → `3.14`, `2.5`, `0.001`

#### Operadores
- Aritméticos: `+`, `-`, `*`, `/`
- Relacionais: `<`, `>`, `<=`, `>=`, `==`, `!=`
- Lógicos: `&&`, `||`, `!`
- Atribuição: `=`

#### Delimitadores
```
( ) { } ; ,
```

#### Comentários
- Linha: `// comentário até o fim da linha`
- Bloco: `/* comentário em múltiplas linhas */`

#### Strings
- Formato: `"texto entre aspas"`
- Suporta caracteres escapados: `\"`, `\\`, `\n`, `\t`

### Exemplo Completo

#### Arquivo de Entrada (`teste.c`)
```c
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    // Recursão
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int result = fibonacci(10);
    return 0;
}
```

#### Execução
```bash
./build/lexer teste.c
```

#### Saída
```
TIPO            LEXEMA               POSIÇÃO
----            ------               --------
KEYWORD         int                  1:1
IDENTIFIER      fibonacci            1:5
DELIMITER       (                    1:14
KEYWORD         int                  1:15
IDENTIFIER      n                    1:19
DELIMITER       )                    1:20
DELIMITER       {                    1:22
KEYWORD         if                   2:5
DELIMITER       (                    2:8
IDENTIFIER      n                    2:9
OPERATOR        <=                   2:11
NUMBER          1                    2:14
DELIMITER       )                    2:15
DELIMITER       {                    2:17
KEYWORD         return               3:9
IDENTIFIER      n                    3:16
DELIMITER       ;                    3:17
DELIMITER       }                    4:5
KEYWORD         return               6:5
IDENTIFIER      fibonacci            6:12
DELIMITER       (                    6:21
IDENTIFIER      n                    6:22
OPERATOR        -                    6:24
NUMBER          1                    6:26
DELIMITER       )                    6:27
OPERATOR        +                    6:29
IDENTIFIER      fibonacci            6:31
DELIMITER       (                    6:40
IDENTIFIER      n                    6:41
OPERATOR        -                    6:43
NUMBER          2                    6:45
DELIMITER       )                    6:46
DELIMITER       ;                    6:47
DELIMITER       }                    7:1
...
EOF             EOF                  12:2
```

---

## Referências Acadêmicas

### Livros-Texto Clássicos

1. **Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D.** (2006). *Compiladores: Princípios, Técnicas e Ferramentas*. 2ª Edição. Pearson Education.
   - Conhecido como "O Livro do Dragão"
   - Capítulo 3: Análise Léxica
   - Define formalmente tokens, lexemas e padrões
   - Apresenta autômatos finitos e expressões regulares
   - Algoritmos de construção de Thompson e subconjuntos

2. **Appel, A. W.** (2002). *Modern Compiler Implementation in C*. Cambridge University Press.
   - Abordagem prática de compiladores
   - Ênfase em implementação eficiente
   - Capítulo 2: Lexical Analysis
   - Exemplos completos em C

3. **Louden, K. C.** (1997). *Compiler Construction: Principles and Practice*. PWS Publishing Company.
   - Abordagem didática e clara
   - Implementações passo a passo
   - Tratamento detalhado de erros
   - Exemplos práticos em C

4. **Cooper, K. D., & Torczon, L.** (2011). *Engineering a Compiler*. 2ª Edição. Morgan Kaufmann.
   - Perspectiva de engenharia de compiladores
   - Ótimo para aspectos práticos e otimizações
   - Capítulo 2: Scanners

### Teoria da Computação

5. **Hopcroft, J. E., Motwani, R., & Ullman, J. D.** (2006). *Introduction to Automata Theory, Languages, and Computation*. 3ª Edição. Pearson.
   - Referência fundamental para autômatos
   - Teoremas de equivalência entre modelos
   - Linguagens regulares e expressões regulares
   - Algoritmos de conversão e minimização

6. **Sipser, M.** (2012). *Introduction to the Theory of Computation*. 3ª Edição. Cengage Learning.
   - Abordagem matemática rigorosa
   - Autômatos finitos determinísticos e não-determinísticos
   - Lema do bombeamento para linguagens regulares

### Artigos Seminais

7. **Lesk, M. E., & Schmidt, E.** (1975). "Lex - A Lexical Analyzer Generator". *UNIX Programmer's Manual*, Bell Laboratories.
   - Ferramenta clássica de geração de scanners
   - Base para Flex e outras ferramentas

8. **Thompson, K.** (1968). "Programming Techniques: Regular Expression Search Algorithm". *Communications of the ACM*, 11(6), 419-422.
   - Construção de Thompson para conversão regex → NFA
   - Algoritmo fundamental ainda usado hoje

9. **Hopcroft, J. E.** (1971). "An n log n Algorithm for Minimizing States in a Finite Automaton". *Theory of Machines and Computations*, 189-196.
   - Algoritmo eficiente de minimização de AFD
   - Complexidade O(n log n)

### Recursos Online

10. **The Lex & Yacc Page**: http://dinosaur.compilertools.net/
    - Documentação e tutoriais sobre Lex/Flex
    
11. **Flex Documentation**: https://github.com/westes/flex
    - Manual completo do Flex
    
12. **JFLAP**: http://www.jflap.org/
    - Software educacional para visualização de autômatos

---

## Resumo dos Papéis do Analisador Léxico

| Responsabilidade | Descrição |
|-----------------|-----------|
| **Filtrar entrada** | Remove espaços em branco, comentários e formatação |
| **Agrupar caracteres** | Identifica sequências que formam tokens |
| **Classificar tokens** | Atribui tipos e categorias aos lexemas |
| **Manter posição** | Rastreia linha e coluna para mensagens de erro |
| **Reportar erros** | Detecta e sinaliza entradas malformadas |
| **Otimizar leitura** | Usa buffers e caching para eficiência |
| **Interface com parser** | Fornece tokens para análise sintática |

---

## Exercícios Propostos

### Básicos

1. Modifique o lexer para reconhecer números hexadecimais (ex: `0x1A2F`)
2. Adicione suporte para caracteres literais (ex: `'a'`, `'\n'`)
3. Implemente contagem de tokens por tipo

### Intermediários

4. Adicione suporte para números em notação científica (ex: `2.5e-3`)
5. Implemente uma tabela de símbolos para identificadores
6. Crie relatórios de erro mais detalhados com contexto

### Avançados

7. Implemente um gerador de AFD a partir de expressões regulares
8. Adicione suporte para strings multi-linha
9. Desenvolva um modo de debug que mostre as transições de estado

---

*Este material faz parte da disciplina de Compiladores. Para questões ou sugestões, consulte o repositório do curso.*