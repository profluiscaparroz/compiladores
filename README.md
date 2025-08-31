# Compiladores — Estrutura de Aprendizagem

Este repositório organiza materiais e implementações didáticas de um compilador, fase a fase, com exemplos em C. A ideia é que você possa:
1) estudar os conceitos em ordem,
2) compilar e executar exemplos mínimos,
3) evoluir para um compilador simples.

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