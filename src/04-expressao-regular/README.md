# 04 — Expressões Regulares

## Índice
1. [Objetivos](#objetivos)
2. [Introdução às Expressões Regulares](#introdução-às-expressões-regulares)
3. [Fundamentos Teóricos](#fundamentos-teóricos)
4. [Como Funcionam os Algoritmos](#como-funcionam-os-algoritmos)
5. [Limitações e Imperfeições](#limitações-e-imperfeições)
6. [Vantagens e Desvantagens](#vantagens-e-desvantagens)
7. [Por Que o Processamento é Rápido (Mas Limitado)](#por-que-o-processamento-é-rápido-mas-limitado)
8. [Aplicações em Compiladores](#aplicações-em-compiladores)
9. [Exercícios](#exercícios)
10. [Implementações Incluídas](#implementações-incluídas)

---

## Objetivos
- Dominar a sintaxe e semântica de expressões regulares.
- Implementar conversão de ER para AFN (algoritmo de Thompson).
- Compreender as limitações teóricas e práticas das ERs.
- Aplicar ERs na análise léxica de linguagens.
- Analisar complexidade e performance de algoritmos de matching.

---

## Introdução às Expressões Regulares

**Expressões Regulares (ERs)** são padrões de texto que descrevem conjuntos de strings. Na ciência da computação, especialmente em compiladores, ERs são fundamentais para a **análise léxica**, a primeira fase da compilação que converte o código fonte em uma sequência de tokens.

### Definição Formal
Uma expressão regular é uma notação concisa para descrever **linguagens regulares**, que são linguagens que podem ser reconhecidas por **Autômatos Finitos** (AF). As ERs são construídas recursivamente usando:

1. **Literais**: Caracteres individuais como `a`, `b`, `0`, `1`
2. **Concatenação**: Sequência de símbolos `ab` (a seguido de b)
3. **União (alternação)**: `a|b` (a OU b)
4. **Estrela de Kleene**: `a*` (zero ou mais ocorrências de a)
5. **Parênteses**: `(ab)*` (agrupamento para controlar precedência)

### Sintaxe Básica
```
Símbolo         Significado                     Exemplo
--------------------------------------------------------------
.               Qualquer caractere              a.c casa com "abc", "axc"
*               Zero ou mais                    ab*c casa com "ac", "abc", "abbc"
+               Um ou mais                      ab+c casa com "abc", "abbc"
?               Zero ou um (opcional)           ab?c casa com "ac", "abc"
|               Alternação (OU)                 a|b casa com "a" ou "b"
[]              Classe de caracteres            [abc] casa com "a", "b" ou "c"
[^]             Classe negada                   [^abc] casa com qualquer exceto a,b,c
^               Início da linha                 ^abc casa "abc" só no início
$               Final da linha                  abc$ casa "abc" só no final
{n}             Exatamente n vezes              a{3} casa com "aaa"
{n,m}           Entre n e m vezes               a{2,4} casa "aa", "aaa", "aaaa"
\d              Dígito [0-9]                    \d{3} casa "123"
\w              Palavra [a-zA-Z0-9_]            \w+ casa palavras
\s              Espaço em branco                \s+ casa espaços
```

---

## Fundamentos Teóricos

### Hierarquia de Chomsky
As expressões regulares pertencem ao **Tipo 3** da Hierarquia de Chomsky — as **Linguagens Regulares**. Esta é a classe mais restrita de linguagens formais, mas também a mais eficiente para reconhecimento.

```
Tipo 0: Linguagens Recursivamente Enumeráveis (Máquina de Turing)
Tipo 1: Linguagens Sensíveis ao Contexto (Autômato Linearmente Limitado)
Tipo 2: Linguagens Livres de Contexto (Autômato de Pilha)
Tipo 3: Linguagens Regulares (Autômato Finito) ← EXPRESSÕES REGULARES
```

### Equivalência: ER ↔ AFN ↔ AFD

Um dos resultados fundamentais da teoria da computação é que três formalismos são **equivalentes em poder expressivo**:

1. **Expressões Regulares (ER)**: Notação concisa e declarativa
2. **Autômatos Finitos Não-Determinísticos (AFN)**: Máquinas com transições ε e múltiplos caminhos possíveis
3. **Autômatos Finitos Determinísticos (AFD)**: Máquinas com exatamente uma transição por símbolo

```
ER → AFN → AFD → ER
```

**Teorema de Kleene (1956)**: Uma linguagem pode ser descrita por uma expressão regular se e somente se pode ser reconhecida por um autômato finito.

### Por Que Essa Equivalência É Importante?
- **ERs** são convenientes para **humanos escreverem** padrões
- **AFNs** são fáceis de **construir algoritmicamente** a partir de ERs
- **AFDs** são **eficientes para executar** (reconhecimento em tempo O(n))

---

## Como Funcionam os Algoritmos

### 1. Algoritmo de Thompson (1968)
O **algoritmo de Thompson** converte uma expressão regular em um AFN com ε-transições de forma **composicional e recursiva**.

#### Estratégia
Para cada operação da ER, Thompson define um **fragmento de AFN** com:
- Um **estado inicial** único
- Um **estado final** único  
- **ε-transições** (transições vazias) para conectar fragmentos

#### Construções Básicas

**Literal (a)**:
```
(i) --a--> (f)
Estado inicial (i) → símbolo 'a' → estado final (f)
```

**Concatenação (AB)**:
```
    A           B
(i) → → (m) → → (f)
Conecta o final de A ao início de B com ε-transição
```

**União (A|B)**:
```
       ε    A    ε
(i) → → → → → → → (f)
   ↘        ↗
    ε  B  ε
    → → → →
Duas opções paralelas com ε-transições
```

**Estrela (A*)**:
```
     ε ←──────┐
     ↓        │
(i) → → [A] → ┘ → (f)
   ↘ ε ────────→ ↗
Loop opcional com bypass direto
```

#### Exemplo Completo: (a|b)*abb

1. Construir fragmentos para `a`, `b`, `abb`
2. Aplicar união `(a|b)`
3. Aplicar estrela `(a|b)*`
4. Concatenar com `abb`

O AFN resultante terá vários estados e ε-transições, mas reconhece exatamente as strings que terminam em "abb" precedidas por qualquer sequência de 'a's e 'b's.

#### Complexidade
- **Tempo**: O(m), onde m é o tamanho da ER
- **Espaço**: O(m) estados no AFN resultante
- **Propriedade**: O AFN tem no máximo **2m estados**

### 2. Construção de Subconjuntos (AFN → AFD)
O algoritmo de **construção de subconjuntos** (subset construction) converte um AFN em um AFD equivalente.

#### Ideia Central
Cada estado do AFD representa um **conjunto de estados do AFN** que podem ser alcançados simultaneamente.

#### Passos
1. **ε-closure**: Para cada conjunto de estados, calcular todos os estados alcançáveis via ε-transições
2. **Transições**: Para cada símbolo, calcular o próximo conjunto de estados
3. **Estados finais**: Conjuntos que contêm pelo menos um estado final do AFN

#### Exemplo
AFN com estados {0, 1, 2}:
```
Estado AFD    Conjunto AFN    Transições
----------------------------------------------
q0            {0}             a→q1, b→q2
q1            {1,2}           a→q1, b→q1
q2            {2}             a→q2, b→q0
```

#### Complexidade
- **Tempo**: O(2^n), onde n é o número de estados do AFN
- **Espaço**: No pior caso, **2^n estados** no AFD (explosão exponencial)
- Na prática, muitos estados são inalcançáveis e podem ser eliminados

### 3. Matching com AFD
Uma vez construído o AFD, o reconhecimento de uma string é **extremamente simples e eficiente**:

```c
bool match(AFD *afd, const char *texto) {
    int estado = afd->estado_inicial;
    
    for (int i = 0; texto[i] != '\0'; i++) {
        estado = afd->transicao[estado][texto[i]];
        
        if (estado == ESTADO_ERRO) {
            return false;  // Nenhuma transição válida
        }
    }
    
    return afd->estados_finais[estado];  // Verifica se é estado final
}
```

**Complexidade**: O(n), onde n é o tamanho do texto
- Cada caractere é processado exatamente uma vez
- Uma única operação de lookup na tabela de transições
- **Sem backtracking** — avança sempre para frente

### 4. Engines de Regex Modernas: NFA e DFA Híbridos

Implementações reais (como POSIX `regex.h`, PCRE, RE2) usam estratégias diferentes:

#### Abordagem NFA (Backtracking)
- **Usado por**: Perl, Python (re), JavaScript, Ruby
- **Estratégia**: Simula o AFN diretamente com backtracking
- **Vantagens**: Suporta features avançadas (backreferences, lookahead)
- **Desvantagens**: Pode ter complexidade **O(2^n)** no pior caso (catastrófico)

#### Abordagem DFA (Sem Backtracking)
- **Usado por**: RE2 (Google), Rust regex, Go regexp
- **Estratégia**: Constrói DFA ou simula AFN sem backtracking
- **Vantagens**: Garantia de **O(n)**, sem surpresas de performance
- **Desvantagens**: Não suporta backreferences ou construções recursivas

#### Abordagem Híbrida (Lazy DFA)
- **Usado por**: RE2
- **Estratégia**: Constrói estados do DFA **sob demanda** durante o matching
- **Vantagens**: Combina eficiência O(n) com menor uso de memória
- **Cache de estados**: Reutiliza estados já calculados

---

## Limitações e Imperfeições

### 1. Limitações Teóricas: O Que ERs NÃO Podem Fazer

#### Linguagens Não-Regulares
Expressões regulares **não conseguem** reconhecer linguagens que requerem **contagem ou balanceamento**:

**Exemplo 1: Parênteses Balanceados**
```
Linguagem: { (), (()), ((())), ... }
Padrão: n parênteses abertos seguidos de n parênteses fechados
```
❌ **Impossível com ER**: Requer contar abertos e casá-los com fechados (precisa de pilha)
✅ **Possível com**: Gramática Livre de Contexto (autômato de pilha)

**Exemplo 2: a^n b^n**
```
Linguagem: { ab, aabb, aaabbb, aaaabbbb, ... }
```
❌ **Impossível com ER**: Não pode contar os 'a's e garantir mesmo número de 'b's
✅ **Possível com**: Gramática Livre de Contexto

**Prova (Lema do Bombeamento)**:
O **Lema do Bombeamento** para linguagens regulares prova que certas linguagens não são regulares. Se uma linguagem L é regular, então existe um inteiro p (pumping length) tal que toda string s ∈ L com |s| ≥ p pode ser dividida em xyz onde:
1. |xy| ≤ p
2. |y| > 0
3. xy^i z ∈ L para todo i ≥ 0

Linguagens como a^n b^n violam essa propriedade, provando que não são regulares.

### 2. Backtracking Catastrófico

Engines de regex baseadas em backtracking (NFA) podem ter **performance exponencial** em casos patológicos.

#### Exemplo Clássico: (a+)+b
```c
Texto: "aaaaaaaaaaaaaaaaaaaaaaaaaaaa"  // 28 'a's, sem 'b' no final
Regex: (a+)+b

Tentativas:
- (aaaa...)(aaaa) b?  ← Falha
- (aaa...a)(aaa.a) b?  ← Falha  
- (aa....aa)(aa..a) b?  ← Falha
- ...

Número de combinações: 2^n onde n = número de 'a's
```

**Tempo de execução**:
```
10 'a's:   ~1 ms
15 'a's:   ~30 ms
20 'a's:   ~1 segundo
25 'a's:   ~30 segundos
30 'a's:   ~15 minutos
```

#### Por Que Acontece?
- O engine tenta **todas as maneiras possíveis** de agrupar os 'a's
- Cada `+` cria um ponto de decisão
- Aninhamento de quantificadores multiplica as possibilidades
- **Explosão combinatória** → O(2^n) ou O(n!)

#### Solução: Regex Engines Sem Backtracking
- **RE2** (Google): Garante O(n) sempre
- **Rust regex**: Usa algoritmo similar ao RE2
- Sacrificam features (backreferences) pela previsibilidade

### 3. Greedy vs. Lazy Matching

#### Greedy (Guloso) — Padrão
```
Texto: <div>Conteúdo</div><div>Mais</div>
Regex: <div>.*</div>
Match: <div>Conteúdo</div><div>Mais</div>  ← TUDO (máximo possível)
```

#### Lazy (Preguiçoso)
```
Regex: <div>.*?</div>
Match: <div>Conteúdo</div>  ← Para no primeiro </div>
```

**Problema**: Comportamento greedy pode ser não-intuitivo e causar bugs sutis.

### 4. Unicode e Caracteres Multibyte

Regexes clássicas operam em **bytes** ou **caracteres ASCII**. Com Unicode:
- **Emojis**: 👍 pode ser 4 bytes (U+1F44D)
- **Combinações**: é = e + ´ (caractere base + diacrítico)
- **`.` pode não casar um "caractere visual"** completo

Bibliotecas modernas (PCRE2, ICU) suportam Unicode, mas com overhead.

### 5. Legibilidade e Manutenção

Regexes complexas são **notoriamente difíceis de ler**:
```regex
^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$
```
Esta regex valida endereços IPv4, mas é praticamente ilegível.

**Problema**: Regexes são "write-only code" — fáceis de escrever, difíceis de entender depois.

---

## Vantagens e Desvantagens

### ✅ Vantagens

#### 1. **Concisão Extrema**
Uma única linha de regex pode substituir centenas de linhas de código imperativo:
```c
// Validar email com código C tradicional: ~50 linhas
// Validar email com regex: 1 linha
regex_match("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$", email)
```

#### 2. **Performance Determinística (AFD)**
- Reconhecimento em **O(n) garantido** (engines baseadas em AFD)
- Não há "casos piores" inesperados
- Ideal para parsing de grandes volumes de texto

#### 3. **Ubiquidade**
- Suportadas em **todas as linguagens modernas** (C, Python, Java, JavaScript, etc.)
- Ferramentas de linha de comando: `grep`, `sed`, `awk`
- Editores de texto, IDEs, sistemas de busca

#### 4. **Fundamentação Teórica Sólida**
- Base matemática rigorosa (teoria de autômatos)
- Propriedades formais bem compreendidas
- Algoritmos com garantias de correção

#### 5. **Essenciais para Compiladores**
- Análise léxica é **impossível** de fazer eficientemente sem ERs
- Geradores de lexers (Lex, Flex) usam ERs como entrada
- Toda linguagem de programação depende de ERs para tokenização

### ❌ Desvantagens

#### 1. **Poder Expressivo Limitado**
- Não reconhecem linguagens livres de contexto (parênteses balanceados)
- Não conseguem contar ou fazer matching recursivo
- Inadequadas para parsing de sintaxe completa (precisa de parser)

#### 2. **Backtracking Catastrófico**
- Engines NFA podem ter complexidade **exponencial**
- Ataques de negação de serviço (ReDoS - Regex Denial of Service)
- Exemplo real: GitHub teve vulnerabilidades ReDoS em 2012

#### 3. **Difíceis de Debugar**
- Comportamento não-intuitivo em casos complexos
- Ferramentas de debug ainda são limitadas
- Dificuldade de entender por que um match falhou

#### 4. **Fragmentação de Implementações**
- **POSIX BRE** (Basic Regular Expressions)
- **POSIX ERE** (Extended Regular Expressions)
- **PCRE** (Perl Compatible Regular Expressions)
- **ECMAScript** (JavaScript regex)
- Cada um com **sintaxe e features diferentes**

#### 5. **Overhead de Compilação**
- Regex deve ser **compilada** para AFN/AFD antes do uso
- Em loops, compilar a mesma regex repetidamente é **ineficiente**
- Sempre compile uma vez e reutilize:
```c
// ❌ RUIM: Compila regex a cada iteração
for (int i = 0; i < 1000000; i++) {
    regex_t reg;
    regcomp(&reg, pattern, REG_EXTENDED);
    regexec(&reg, texts[i], ...);
    regfree(&reg);  // Muito lento!
}

// ✅ BOM: Compila uma vez, usa muitas vezes
regex_t reg;
regcomp(&reg, pattern, REG_EXTENDED);
for (int i = 0; i < 1000000; i++) {
    regexec(&reg, texts[i], ...);
}
regfree(&reg);
```

---

## Por Que o Processamento é Rápido (Mas Limitado)

### Velocidade: O(n) vs. O(2^n)

#### Engines Baseadas em DFA (Rápidas)
```
Velocidade: O(n) — Linear no tamanho do texto
Memória: O(2^m) — Exponencial no tamanho da regex (pior caso)
```

**Por que são rápidas?**
1. **Uma operação por caractere**: Lê caractere → lookup tabela → próximo estado
2. **Sem backtracking**: Nunca "volta atrás" para tentar alternativas
3. **Previsível**: Performance não depende do texto de entrada
4. **Cache-friendly**: Acesso sequencial à memória

**Analogia**: Como seguir um mapa de metrô — você sempre sabe qual é a próxima estação, sem ambiguidade.

#### Engines Baseadas em NFA (Podem Ser Lentas)
```
Velocidade: O(n) no melhor caso, O(2^n) no pior caso
Memória: O(m) — Linear no tamanho da regex
```

**Por que podem ser lentas?**
1. **Backtracking**: Tenta um caminho, falha, volta e tenta outro
2. **Explosão combinatória**: Quantificadores aninhados criam árvore de possibilidades
3. **Dependente do input**: Performance varia drasticamente com o texto

**Analogia**: Como resolver um labirinto por tentativa e erro — pode precisar explorar todos os caminhos errados antes de encontrar o correto.

### Limitações de Velocidade

#### 1. Explosão de Estados (DFA)
Ao converter AFN → AFD, o número de estados pode explodir:
```
AFN com n estados → DFA com até 2^n estados
```

**Exemplo**:
- Regex: `.*a.*b.*c.*d.*e.*f.*g.*h.*i.*j.*k`  
- AFN: ~50 estados
- DFA: **Milhões ou bilhões** de estados
- **Inviável de construir completamente**

**Solução: Lazy DFA Construction**
- Construir estados **sob demanda** durante matching
- Cachear estados já visitados
- Limitar cache (descartar estados antigos se memória acabar)

#### 2. Overhead de Inicialização
```
Custo total = Compilação + Execução

Para textos pequenos:
- Compilação pode ser > 90% do tempo total
- DFA pode não compensar

Para textos grandes:
- Compilação amortizada → insignificante
- DFA sempre vence
```

#### 3. Features Avançadas Custam Caro
```
Feature                    Custo
-----------------------------------------------
Literal simples (abc)      O(n) — Muito rápido
Classes [a-z]              O(n) — Rápido
Alternação (a|b|c)         O(n) — Rápido (DFA) ou O(n×m) (NFA)
Quantificadores *+?        O(n) — Rápido (DFA) ou O(2^n) (NFA)
Backreferences \1          O(2^n) — Sempre lento
Lookahead (?=...)          O(2^n) — Sempre lento
```

### Comparação: Outras Abordagens de Busca de Texto

```
Algoritmo               Complexidade    Uso de Memória    Aplicação
-------------------------------------------------------------------------
Busca naive             O(n×m)          O(1)              Casos simples
Boyer-Moore             O(n/m) avg      O(m)              Strings fixas
Aho-Corasick            O(n+m+z)        O(m)              Múltiplos padrões
DFA Regex               O(n)            O(2^m)            Padrões complexos
NFA Regex               O(2^n)          O(m)              Features avançadas
```

Onde:
- n = tamanho do texto
- m = tamanho do padrão/regex
- z = número de matches

### Por Que C Usa POSIX Regex (Relativamente Lento)?

A biblioteca padrão C (`regex.h`) usa **NFA com backtracking** porque:
1. **Histórico**: POSIX definiu antes das otimizações modernas
2. **Compatibilidade**: Manter comportamento esperado
3. **Features**: Suportar backreferences e grupos de captura
4. **Simplicidade**: Implementação mais simples que DFA otimizado

Para **máxima performance em C**, use bibliotecas como:
- **RE2** (C++ com binding C)
- **Hyperscan** (Intel) — Vetorização SIMD, multi-pattern
- **Tre** — AFD com suporte a fuzzy matching

---

## Aplicações em Compiladores

### 1. Análise Léxica (Tokenização)

A **primeira fase** de todo compilador usa expressões regulares para definir **tokens**:

```
Token          Expressão Regular
-------------------------------------------------
IDENTIFICADOR  [a-zA-Z_][a-zA-Z0-9_]*
NUMERO         [0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?
OPERADOR       [+\-*/=<>!&|]+
PARENTESES     [(){}[\]]
WHITESPACE     [ \t\n\r]+
COMENTARIO     //.*|/\*([^*]|\*[^/])*\*/
STRING         "([^"\\]|\\.)*"
```

#### Fluxo de Trabalho
1. **Especificação**: Escrever ERs para cada tipo de token (arquivo `.l` no Lex/Flex)
2. **Geração**: Ferramenta gera código C com AFD otimizado
3. **Tokenização**: Código gerado lê fonte e produz stream de tokens
4. **Parser**: Recebe tokens e constrói árvore sintática

#### Exemplo com Lex/Flex
```lex
%%
[0-9]+              { return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*  { return IDENTIFIER; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
[ \t\n]             { /* ignorar */ }
.                   { printf("Erro léxico: %c\n", yytext[0]); }
%%
```

Flex compila isso para um **AFD otimizado** em C que tokeniza código fonte em **O(n)**.

### 2. Validação de Entrada

Compiladores validam literais:
```c
// Número inteiro hexadecimal: 0x[0-9a-fA-F]+
0x1A3F   ✅
0xGHIJ   ❌

// Número de ponto flutuante: [0-9]+\.[0-9]+([eE][+-]?[0-9]+)?
3.14     ✅
2.71e-5  ✅
..5      ❌
```

### 3. Otimização: Constant Folding
Identificar padrões em código para otimizar:
```c
// Antes: 2 + 3
// Regex encontra padrão: NUMERO OPERADOR NUMERO
// Depois: 5 (computado em tempo de compilação)
```

### 4. Análise Semântica
Verificar convenções de nomes:
```c
// Variáveis devem começar com letra minúscula
isValid   ✅
IsValid   ❌ (convenção violada)
```

### 5. Highlight de Sintaxe em IDEs
Editores usam regexes para colorir código:
```
KEYWORD     →  azul (if, for, while)
STRING      →  verde ("hello")
COMMENT     →  cinza (// comentário)
FUNCTION    →  amarelo (functionName(...))
```

### Por Que Regexes São Insuficientes para Parsing Completo?

```c
// Código C válido:
if (x > 0) {
    if (y > 0) {
        printf("positivo");
    }
}

// Regex NÃO consegue verificar:
- Parênteses balanceados: ( ( ) )
- Chaves balanceadas: { { } }
- Estruturas aninhadas arbitrariamente
```

**Solução**: Após tokenização com regexes, usar **parser** (análise sintática) com gramática livre de contexto.

---

## Exercícios

### Exercícios Teóricos

1. **Desenhe o AFN resultante** da aplicação do algoritmo de Thompson para a expressão `(a|b)*abb`. Mostre todos os estados e transições, incluindo ε-transições.

2. **Prove usando o Lema do Bombeamento** que a linguagem L = {a^n b^n | n ≥ 0} não é regular.

3. **Converta o AFN da questão 1 em um AFD** usando o algoritmo de construção de subconjuntos. Quantos estados o AFD possui?

4. **Análise de complexidade**: Explique por que a regex `(a+)+b` causa backtracking catastrófico. Quantas tentativas são feitas para a string "aaaaaaaaaa" (10 'a's sem 'b')?

### Exercícios Práticos

5. **Escreva uma ER para números inteiros** com sinal opcional (+/-) e separadores de milhar opcionais (vírgula ou ponto).
   ```
   Válidos: 42, +1000, -5, 1.234.567, 1,234,567
   Inválidos: 1.23.4, +-5, 5.
   ```

6. **Implemente um matcher simples** que aceite uma ER com apenas:
   - Literais (a-z)
   - Concatenação
   - Operador * (estrela)
   - Operador | (união)

7. **Crie ERs para validar**:
   - **Email**: `usuario@dominio.com`
   - **URL**: `http://` ou `https://` seguido de domínio
   - **Telefone brasileiro**: `(XX) XXXXX-XXXX` ou `(XX) XXXX-XXXX`
   - **CPF**: `XXX.XXX.XXX-XX`

8. **Análise de performance**: Meça o tempo de compilação vs. execução de uma regex complexa. Compare:
   ```c
   // Regex compilada uma vez, executada 1.000.000 vezes
   // vs.
   // Regex compilada e executada 1.000.000 vezes
   ```

9. **Implemente o algoritmo de Thompson em C**: Dado uma ER simples, construa o AFN correspondente. Use uma estrutura de dados adequada para representar estados e transições.

10. **ReDoS Attack**: Teste a vulnerabilidade de diferentes engines de regex:
    ```c
    Regex: (a+)+b
    Inputs: "a", "aa", "aaa", ..., "aaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    Meça o tempo de execução para cada input.
    ```

---

## Implementações Incluídas

Este diretório contém implementações em C que demonstram diferentes aspectos de expressões regulares:

### Arquivos de Exemplo

1. **0exemploRegex.c**: Implementação básica de regex simples do zero (sem bibliotecas)
   - Suporta: `.` (qualquer), `*` (estrela), `^` (início), `$` (fim)
   - Algoritmo recursivo com backtracking
   - Demonstra conceitos fundamentais

2. **1exemploSimples.c**: Uso da biblioteca POSIX `regex.h`
   - Compilação e execução de regex
   - Funções `regcomp()` e `regexec()`
   - Tratamento de erros

3. **2exemploProcurarCorrespondencia.c**: Busca de padrões em texto
   - Encontrar todas as ocorrências de um padrão
   - Uso de `REG_EXTENDED` para sintaxe avançada

4. **3exemploVerificaEmail.c**: Validação de endereços de email
   - Regex para formato de email
   - Demonstra uso prático em validação

5. **4exemploSubstituir.c**: Substituição de padrões
   - Encontrar e substituir texto baseado em regex
   - Manipulação de strings

6. **5exemploValidaNumero.c**: Validação de números
   - Inteiros, decimais, notação científica
   - Casos extremos e edge cases

7. **6exemploExtracaoDatas.c**: Extração de datas de texto
   - Captura de grupos
   - Parsing de formatos variados

8. **7exemploDivisaoStringDeli.c**: Divisão de strings com delimitadores
   - Tokenização usando regex como separador
   - Similar a `strtok()` mas mais poderoso

9. **8exemploValidacaoIp4.c**: Validação de endereços IPv4
   - Regex complexa para validar formato
   - Verificação de ranges (0-255)

### Como Compilar e Executar

```bash
# Compilar um exemplo específico
gcc -o exemplo 1exemploSimples.c

# Executar
./exemplo

# Compilar todos (se existir Makefile)
make all

# Limpar executáveis
make clean
```

### Estrutura de Dados Sugerida para Implementação de Thompson

```c
typedef struct State {
    int id;
    bool is_final;
    struct Transition *transitions;
    int num_transitions;
} State;

typedef struct Transition {
    char symbol;        // '\0' para ε-transição
    State *to_state;
} Transition;

typedef struct NFA {
    State *start;
    State *final;
    State **all_states;
    int num_states;
} NFA;

// Funções para construção
NFA* thompson_literal(char c);
NFA* thompson_concat(NFA *a, NFA *b);
NFA* thompson_union(NFA *a, NFA *b);
NFA* thompson_star(NFA *a);

// Simulação
bool nfa_match(NFA *nfa, const char *text);
```

---

## Referências e Leitura Adicional

### Livros Clássicos
- **Compilers: Principles, Techniques, and Tools** (Dragon Book) — Aho, Sethi, Ullman
- **Introduction to Automata Theory, Languages, and Computation** — Hopcroft, Motwani, Ullman
- **Mastering Regular Expressions** — Jeffrey Friedl (O'Reilly)

### Papers Fundamentais
- **Regular Expression Search Algorithm** (1968) — Ken Thompson
- **Regular Expression Matching Can Be Simple And Fast** (2007) — Russ Cox (Google)

### Ferramentas Online
- **regex101.com** — Teste e debug de regexes com explicações
- **regexr.com** — Visualização e referência
- **debuggex.com** — Visualização de AFN/AFD

### Implementações de Referência
- **RE2** (Google) — https://github.com/google/re2
- **Rust regex** — https://docs.rs/regex/
- **TRE** — https://laurikari.net/tre/

---

## Conclusão

Expressões regulares são uma ferramenta **fundamental e poderosa** em compiladores e processamento de texto, mas **não são perfeitas**:

✅ **Rápidas** (O(n)) quando implementadas como DFA  
✅ **Concisas** e expressivas para padrões comuns  
✅ **Ubíquas** em todas as linguagens e ferramentas  

❌ **Limitadas** a linguagens regulares (Tipo 3 de Chomsky)  
❌ **Perigosas** com backtracking (ReDoS)  
❌ **Complexas** de entender e debugar  

A chave é **entender as limitações e usar a ferramenta certa para cada problema**:
- **Regex com DFA**: Tokenização, validação simples, busca eficiente
- **Parser livre de contexto**: Sintaxe completa, estruturas aninhadas
- **Algoritmos especializados**: Problemas muito específicos (Boyer-Moore para busca exata)

Estudar expressões regulares profundamente não só ajuda a usá-las melhor, mas também revela princípios fundamentais de ciência da computação: a relação entre **notação**, **algoritmo** e **máquina** — a essência da teoria de compilação.