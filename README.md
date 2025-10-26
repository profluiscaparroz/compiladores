# Compiladores — Estrutura de Aprendizagem

## Introdução aos Compiladores

### O que é um Compilador?

Um **compilador** é um programa especializado que traduz código escrito em uma **linguagem de programação de alto nível** (como C, Java, Python) para uma **linguagem de nível mais baixo** (como código de máquina ou assembly). Esta tradução é essencial porque os processadores modernos só conseguem executar instruções em linguagem de máquina — um conjunto limitado de operações binárias específicas da arquitetura do hardware.

A **importância dos compiladores** na computação moderna é fundamental: eles são a ponte que permite que programadores humanos escrevam código em notações próximas à linguagem natural (com estruturas como `if`, `while`, `for`) e que esse código seja transformado em instruções compreensíveis pela máquina. Sem compiladores, seria necessário programar diretamente em linguagem de máquina, o que é extremamente trabalhoso, propenso a erros e não portável entre diferentes arquiteturas.

### Contexto Histórico e Evolução

Os primeiros computadores, na década de 1940, eram programados diretamente em **linguagem de máquina** através de painéis de chaves ou cartões perfurados. Com a criação da **linguagem Assembly** nos anos 1950, surgiu o primeiro passo na abstração: símbolos mnemônicos (como `MOV`, `ADD`) substituíram códigos binários, e **montadores** (assemblers) faziam a tradução.

O marco histórico dos compiladores modernos foi o desenvolvimento do **FORTRAN** (FORmula TRANslation) em 1957 pela IBM, considerado o **primeiro compilador de linguagem de alto nível**. O projeto inicial levou 18 pessoas e três anos, mas provou que era possível traduzir automaticamente expressões matemáticas complexas em código de máquina eficiente.

Desde então, a **teoria de compilação** evoluiu significativamente:
- **Anos 1960**: Desenvolvimento das linguagens ALGOL e COBOL; formalização de gramáticas (Backus-Naur Form).
- **Anos 1970**: Criação de C e Pascal; otimização de código; surgimento de geradores de analisadores léxicos (lex) e sintáticos (yacc).
- **Anos 1980-1990**: Compiladores para linguagens orientadas a objetos (C++, Java); Just-In-Time compilation (JIT).
- **Anos 2000-presente**: Compiladores modernos como GCC, LLVM; compilação para múltiplas plataformas; análise estática avançada.

### Fases da Compilação

O processo de compilação é tradicionalmente dividido em **fases**, cada uma transformando a representação do programa fonte progressivamente até chegar ao código objeto. As principais fases são:

#### 1. Análise Léxica (Scanning)
O **analisador léxico** (ou scanner) lê o código fonte como uma sequência de caracteres e o agrupa em **tokens** — unidades significativas da linguagem como palavras-chave (`if`, `while`), identificadores (`x`, `total`), números (`42`, `3.14`), operadores (`+`, `==`) e delimitadores (`{`, `;`).

**Exemplo:** A expressão `int soma = a + 10;` é tokenizada em:
```
[PALAVRA_CHAVE: int] [IDENTIFICADOR: soma] [ATRIBUICAO: =] 
[IDENTIFICADOR: a] [OPERADOR: +] [NUMERO: 10] [DELIMITADOR: ;]
```

A análise léxica é fundamentada na **teoria de autômatos finitos** e **expressões regulares**, que fornecem a base matemática para reconhecer padrões em cadeias de caracteres.

#### 2. Análise Sintática (Parsing)
O **analisador sintático** (ou parser) organiza os tokens em uma **árvore sintática** (parse tree ou AST — Abstract Syntax Tree) que representa a estrutura hierárquica do programa de acordo com a **gramática** da linguagem.

**Exemplo:** A expressão `a + b * c` é estruturada como:
```
       +
      / \
     a   *
        / \
       b   c
```

A análise sintática baseia-se em **gramáticas livres de contexto** (Type 2 na Hierarquia de Chomsky) e utiliza técnicas como **análise descendente** (recursive descent, LL) ou **ascendente** (shift-reduce, LR, LALR).

#### 3. Análise Semântica
O **analisador semântico** verifica a **consistência lógica** do programa, garantindo que as operações façam sentido dentro das regras da linguagem. Isso inclui:
- **Verificação de tipos**: `int x = "texto";` é semanticamente incorreto.
- **Verificação de escopos**: Variáveis devem ser declaradas antes do uso.
- **Verificação de compatibilidade**: Operações devem ser aplicadas a tipos compatíveis.

Nesta fase, constrói-se a **tabela de símbolos** que mapeia identificadores (variáveis, funções) aos seus atributos (tipo, escopo, localização na memória).

#### 4. Otimização de Código Intermediário
O compilador pode gerar uma **representação intermediária** (como código de três endereços ou bytecode) e aplicar **otimizações** para melhorar eficiência:
- **Eliminação de código morto** (dead code elimination)
- **Propagação de constantes** (constant propagation)
- **Eliminação de subexpressões comuns** (common subexpression elimination)
- **Otimização de loops** (loop unrolling, loop invariant code motion)

#### 5. Geração de Código
A fase final traduz a representação intermediária otimizada em **código de máquina** ou **assembly** específico da arquitetura alvo. Envolve:
- **Seleção de instruções**: Mapear operações de alto nível em instruções da CPU.
- **Alocação de registradores**: Atribuir variáveis aos registradores físicos do processador.
- **Escalonamento de instruções**: Reordenar instruções para aproveitar paralelismo do hardware.

### Relação com Linguagens Formais e Teoria da Computação

A **teoria de compiladores** está profundamente conectada com conceitos fundamentais de **Ciência da Computação**:

- **Hierarquia de Chomsky**: Classifica linguagens formais em tipos (regulares, livres de contexto, sensíveis ao contexto, recursivamente enumeráveis), determinando quais ferramentas matemáticas podem reconhecê-las.
- **Autômatos Finitos (AFD/AFN)**: Modelam o reconhecimento de tokens na análise léxica.
- **Autômatos de Pilha (PDA)**: Reconhecem linguagens livres de contexto na análise sintática.
- **Máquinas de Turing**: Fundamentam a noção de computabilidade e decidibilidade.

Esses modelos teóricos não são apenas abstrações acadêmicas: eles são a base de ferramentas práticas como **geradores de analisadores léxicos** (lex/flex) e **sintáticos** (yacc/bison), amplamente utilizados na indústria.

### Aplicações e Arquitetura Moderna

Compiladores modernos vão além da simples tradução de código:

- **Compiladores otimizadores** (GCC, LLVM/Clang): Aplicam dezenas de passes de otimização para gerar código altamente eficiente.
- **Compiladores JIT** (Just-In-Time): Utilizados em Java (JVM), JavaScript (V8), C# (.NET) para compilar código em tempo de execução.
- **Transpiladores**: Convertem código de uma linguagem de alto nível para outra (ex.: TypeScript → JavaScript).
- **Ferramentas de análise estática**: Detectam bugs, vulnerabilidades e violações de estilo sem executar o código.
- **Compiladores para domínios específicos** (DSL): CUDA para GPUs, SQL para bancos de dados, VHDL para hardware.

A **arquitetura de compiladores modernos** frequentemente separa o processo em:
- **Front-end**: Análises léxica, sintática e semântica (dependente da linguagem fonte).
- **Middle-end**: Otimizações independentes de arquitetura (trabalha na representação intermediária).
- **Back-end**: Geração de código e otimizações específicas da arquitetura alvo.

Essa separação facilita a criação de **compiladores multiplataforma**: um único front-end pode ser combinado com múltiplos back-ends para gerar código para diferentes processadores (x86, ARM, RISC-V).

### Objetivo Deste Repositório

Este repositório organiza materiais e implementações didáticas de um compilador, fase a fase, com exemplos em C. A ideia é que você possa:
1) Estudar os conceitos teóricos de forma progressiva, desde linguagens formais até geração de código.
2) Compilar e executar exemplos mínimos que demonstram cada fase da compilação.
3) Evoluir gradualmente para a construção de um compilador simples, mas funcional.

O material está estruturado para que alunos possam acompanhar o processo completo de desenvolvimento de um compilador, integrando teoria e prática de forma coesa.

## Estrutura (pasta `src/`)

- 02-linguagens-formais
  - Conceitos de linguagens e gramáticas.
  - Exercícios de classificação e reconhecimento simples.
- 03-automatos
  - Autômatos finitos (AFD/AFN), conversão AFN→AFD, minimização.
  - Simuladores em C e geração opcional de gráficos.
- 04-expressao-regular
  - Expressões regulares, notação posfixa e construção de Thompson (ER→AFN).
- 05-organizacao-computadores
  - Ponte entre compilação e arquitetura (registradores, memória, instruções).
- 06-representacao-grafica
  - Visualização de autômatos/AST via Graphviz (DOT).
- 07-analisador-lexico
  - Tokenização de uma linguagem didática (C incluído).
- 08-analisador-sintatico
  - Parsing (ex.: descida recursiva para expressões e blocos).
- 09-analisador-semantico
  - Tabela de símbolos, escopos e checagens de tipos.

Cada módulo contém:
- `README.md` com objetivos, teoria curta, instruções e exercícios.
- `include/` (arquivos `.h`), `src/` (arquivos `.c`), `tests/` (entradas).
- `Makefile` para compilar e rodar exemplos (quando aplicável).

## Pré-requisitos

- Compilador C (gcc ou clang)
- make
- (Opcional) Graphviz para geração de gráficos (dot)

No Linux/macOS:
- `gcc --version` ou `clang --version` deve funcionar
- `make --version` deve funcionar

No Windows:
- Recomenda-se WSL, MSYS2, Cygwin ou MinGW para usar `make` e `gcc`.

## Como começar (exemplo: Analisador Léxico)

Neste repositório, o módulo `src/07-analisador-lexico` inclui uma implementação em C de um analisador léxico para uma linguagem mínima (palavras-chave, identificadores, números, operadores, delimitadores e comentários).

Para compilar e rodar o léxico:
1. Na raiz do repositório, execute:
   ```
   make lexer
   ```
2. Rode com um arquivo de teste:
   ```
   ./src/07-analisador-lexico/build/lexer ./src/07-analisador-lexico/tests/exemplo1.tl
   ```

A saída listará tokens (tipo, lexema, posição linha:coluna).

## Roteiro sugerido de estudo

1) 02 → 03 → 04: dos modelos teóricos ao reconhecimento efetivo.
2) 07: implemente e experimente o léxico (base para as próximas fases).
3) 08: adicione um analisador sintático (ex.: expressões e blocos).
4) 09: inclua tabela de símbolos e verificação semântica.
5) (Opcional) Geração de código ou interpretação.

## Exercícios gerais

- Adapte o léxico para novas palavras-chave e operadores.
- Gere DOT/PNG de AFDs construídos a partir de expressões regulares.
- Escreva uma gramática para a linguagem didática e faça um parser por descida recursiva.
- Adicione escopos e tipos simples (int, bool) com verificações semânticas.

Bom estudo!