# 03 — Autômatos

Objetivos:
- Compreender autômatos finitos determinísticos (AFD) e não-determinísticos (AFN).
- Implementar simuladores de AFD e AFN em C.
- Realizar conversões AFN→AFD e minimização de autômatos.

Conteúdo:
- Definição formal de AFD: M = (Q, Σ, δ, q0, F).
- Autômatos não-determinísticos (AFN) e ε-transições.
- Algoritmo de conversão AFN→AFD (construção de subconjuntos).
- Minimização de AFDs via algoritmo de particionamento.

Exercícios:
1) Construa um AFD que aceite palavras com número par de a's.
2) Converta o AFN dado em AFD equivalente usando construção de subconjuntos.
3) Minimize o AFD resultante do exercício anterior.
4) Implemente um simulador que aceite a definição de um AFD via arquivo.

Implementações incluídas:
- Simulador básico de AFD.
- Conversor AFN→AFD.
- Algoritmo de minimização.