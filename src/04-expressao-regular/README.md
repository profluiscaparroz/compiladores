# 04 — Expressões Regulares

Objetivos:
- Dominar a sintaxe e semântica de expressões regulares.
- Implementar conversão de ER para AFN (algoritmo de Thompson).
- Aplicar ERs na análise léxica de linguagens.

Conteúdo:
- Sintaxe: literais, concatenação, união (|), estrela (*), parênteses.
- Equivalência entre ER, AFN e AFD.
- Algoritmo de Thompson: ER → AFN com ε-transições.
- Aplicações práticas: validação, extração, substituição.

Exercícios:
1) Escreva uma ER para números inteiros com sinal opcional.
2) Converta a ER (a|b)*abb para AFN usando Thompson.
3) Implemente um matcher simples que aceite uma ER e teste palavras.
4) Crie ERs para: emails, URLs, números de telefone.

Implementações incluídas:
- Parser de expressões regulares.
- Construção de Thompson (ER→AFN).
- Matcher básico para testing.