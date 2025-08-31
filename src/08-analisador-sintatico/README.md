# 08 — Analisador Sintático

Objetivos:
- Implementar parsers para reconhecer estruturas sintáticas.
- Dominar técnicas de análise descendente (descida recursiva).
- Construir árvores de sintaxe abstrata (AST).

Conteúdo:
- Gramáticas livres de contexto e derivações.
- Parser por descida recursiva: um procedimento por não-terminal.
- Tratamento de precedência e associatividade de operadores.
- Construção de AST durante o parsing.

Exercícios:
1) Escreva uma gramática para expressões aritméticas com precedência.
2) Implemente um parser recursivo para a gramática acima.
3) Adicione suporte para declarações de variáveis e atribuições.
4) Construa e imprima a AST de programas simples.

Implementações incluídas:
- Parser de expressões aritméticas.
- Parser de blocos e declarações.
- Construtor de AST com pretty-printing.