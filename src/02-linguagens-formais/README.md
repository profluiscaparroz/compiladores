# 02 — Linguagens Formais

## Introdução

As **linguagens formais** constituem um dos pilares fundamentais da Ciência da Computação, fornecendo uma base matemática rigorosa para o estudo de linguagens de programação, compiladores, e teoria da computação. Segundo Hopcroft e Ullman (2001), uma linguagem formal é um conjunto de cadeias de símbolos que podem ser descritas por regras precisas e não ambíguas, diferentemente das linguagens naturais que são inerentemente ambíguas e dependentes de contexto cultural.

O estudo de linguagens formais teve início com os trabalhos pioneiros de Noam Chomsky na década de 1950, originalmente aplicados à linguística teórica, mas que rapidamente encontraram aplicações fundamentais na computação. A hierarquia proposta por Chomsky classifica as linguagens em quatro tipos distintos, cada um com características computacionais específicas e diferentes capacidades expressivas (Chomsky, 1956, 1959).

## Conceitos Fundamentais

### Alfabeto e Palavras

Um **alfabeto** (geralmente denotado por Σ) é um conjunto finito não-vazio de símbolos. Por exemplo, Σ = {0, 1} representa o alfabeto binário, enquanto Σ = {a, b, c, ..., z} representa o alfabeto latino minúsculo. Uma **palavra** ou **cadeia** é uma sequência finita de símbolos do alfabeto. A palavra vazia, denotada por ε (epsilon), representa a cadeia que não contém símbolos (Sipser, 2013).

A **concatenação** de palavras é a operação que une duas palavras para formar uma nova palavra. Se w₁ = "abc" e w₂ = "def", então a concatenação w₁w₂ = "abcdef". Esta operação é associativa mas não comutativa, isto é, (w₁w₂)w₃ = w₁(w₂w₃), mas em geral w₁w₂ ≠ w₂w₁.

### Operações sobre Linguagens

A **estrela de Kleene** (Σ*) representa o conjunto de todas as palavras possíveis que podem ser formadas com símbolos do alfabeto Σ, incluindo a palavra vazia. Por exemplo, se Σ = {a, b}, então Σ* = {ε, a, b, aa, ab, ba, bb, aaa, aab, ...}. O operador + (Σ⁺) é similar, mas exclui a palavra vazia (Kleene, 1956).

Uma **linguagem formal** L sobre um alfabeto Σ é simplesmente um subconjunto de Σ*, ou seja, L ⊆ Σ*. Podemos definir linguagens através de enumeração explícita (para linguagens finitas), por propriedades que suas palavras devem satisfazer, ou através de gramáticas formais.

### Gramáticas Formais

Uma **gramática formal** G é uma quádrupla G = (V, Σ, R, S), onde:
- **V** é um conjunto finito de símbolos não-terminais (variáveis)
- **Σ** é um conjunto finito de símbolos terminais (o alfabeto)
- **R** é um conjunto finito de regras de produção
- **S** ∈ V é o símbolo inicial

As **regras de produção** (ou simplesmente produções) têm a forma α → β, onde α e β são cadeias de símbolos terminais e não-terminais. Uma **derivação** é uma sequência de aplicações de regras de produção que transforma o símbolo inicial em uma cadeia de terminais (Lewis & Papadimitriou, 1998).

## Hierarquia de Chomsky

A hierarquia de Chomsky organiza as linguagens formais em quatro tipos, ordenados por poder expressivo crescente. Esta classificação é fundamental porque cada tipo está associado a um modelo específico de autômato que pode reconhecer suas linguagens (Chomsky, 1956, 1959).

### Tipo-3: Linguagens Regulares

As **linguagens regulares** são o tipo mais restritivo e mais simples da hierarquia. Suas gramáticas possuem produções da forma A → aB ou A → a, onde A e B são não-terminais e a é um terminal. Estas linguagens podem ser reconhecidas por **autômatos finitos** (determinísticos ou não-determinísticos) e descritas por **expressões regulares** (Hopcroft et al., 2001).

Segundo Sipser (2013), as linguagens regulares possuem propriedades de fechamento importantes: são fechadas sob união, concatenação, estrela de Kleene, complemento e interseção. No entanto, têm limitações fundamentais: não podem "contar" ou manter informações sobre quantidades ilimitadas. O clássico exemplo é a linguagem L = {aⁿbⁿ | n ≥ 0}, que não é regular, conforme demonstrado pelo Lema do Bombeamento para linguagens regulares.

**Aplicações práticas**: análise léxica em compiladores, validação de expressões regulares, processamento de texto, protocolos de rede.

### Tipo-2: Linguagens Livres de Contexto

As **linguagens livres de contexto** (LLC) são definidas por gramáticas cujas produções têm a forma A → α, onde A é um não-terminal e α é uma cadeia qualquer de terminais e não-terminais. Estas linguagens são reconhecidas por **autômatos de pilha** (pushdown automata) (Hopcroft & Ullman, 2001).

As LLC são fundamentais na construção de compiladores, pois a maioria das estruturas sintáticas de linguagens de programação pode ser descrita por gramáticas livres de contexto. Por exemplo, expressões aritméticas, estruturas de controle (if-then-else, while), e declarações de variáveis são tipicamente especificadas usando LLC (Aho et al., 2006).

Um exemplo clássico de LLC é a linguagem L = {aⁿbⁿ | n ≥ 0}, que pode ser gerada pela gramática S → aSb | ε. Esta linguagem requer uma pilha para "contar" os a's e garantir que haja o mesmo número de b's.

**Aplicações práticas**: análise sintática em compiladores, parsing de XML/JSON, verificação de parênteses balanceados, processamento de linguagens naturais.

### Tipo-1: Linguagens Sensíveis ao Contexto

As **linguagens sensíveis ao contexto** (LSC) possuem gramáticas com produções da forma αAβ → αγβ, onde A é não-terminal e α, β, γ são cadeias de símbolos (com γ não-vazio). A característica distintiva é que a substituição de A depende do contexto (α e β) ao seu redor (Chomsky, 1959).

Estas linguagens são reconhecidas por **autômatos linearmente limitados** (linear bounded automata), um tipo restrito de máquina de Turing cuja fita é limitada ao tamanho da entrada. Um exemplo clássico é L = {aⁿbⁿcⁿ | n ≥ 0}, que exige controle simultâneo sobre três contagens (Hopcroft et al., 2001).

**Aplicações práticas**: certas construções de linguagens de programação (como verificação de tipos dependentes de contexto), análise de linguagem natural, processamento de estruturas biológicas.

### Tipo-0: Linguagens Recursivamente Enumeráveis

As **linguagens recursivamente enumeráveis** (LRE) são o tipo mais geral, sem restrições nas regras de produção (α → β, onde α e β são cadeias quaisquer). Estas linguagens correspondem exatamente aos problemas que podem ser resolvidos por uma **máquina de Turing** (Turing, 1936; Sipser, 2013).

É importante notar que nem todas as LRE são decidíveis: algumas podem ter problemas para os quais uma máquina de Turing pode nunca parar. O famoso Problema da Parada demonstra que existem questões computacionalmente insolúveis mesmo dentro desta classe mais geral (Turing, 1936).

## Propriedades e Teoremas Importantes

O **Lema do Bombeamento** (Pumping Lemma) é uma ferramenta essencial para provar que certas linguagens não pertencem a uma classe específica. Existem versões do lema para linguagens regulares e para linguagens livres de contexto (Hopcroft et al., 2001).

A **Tese de Church-Turing** estabelece que qualquer função computável pode ser calculada por uma máquina de Turing, conectando o conceito matemático de computabilidade com implementações práticas (Church, 1936; Turing, 1936).

## Relação com Compiladores

No desenvolvimento de compiladores, diferentes níveis da hierarquia são utilizados em diferentes fases (Aho et al., 2006):

1. **Análise Léxica**: utiliza linguagens regulares e autômatos finitos para reconhecer tokens
2. **Análise Sintática**: utiliza linguagens livres de contexto e autômatos de pilha para verificar a estrutura do programa
3. **Análise Semântica**: pode requerer linguagens sensíveis ao contexto para verificações de tipo e escopo

## Exercícios

1) Classifique as linguagens abaixo segundo Chomsky e justifique:
   - L1 = { aⁿbⁿ | n ≥ 0 } - Tipo-2 (livre de contexto, requer pilha)
   - L2 = { aⁿbᵐ | n,m ≥ 0 } - Tipo-3 (regular, não requer contagem sincronizada)
   - L3 = { w | w tem número par de 1's, Σ={0,1} } - Tipo-3 (regular, estados finitos suficientes)

2) Dê uma gramática regular para L = { (ab)ⁿ | n ≥ 0 }.
   Solução: S → abS | ε

3) Explique por que { aⁿbⁿ | n ≥ 0 } não é regular usando o Lema do Bombeamento.
   Demonstração: Suponha que L é regular com constante de bombeamento p. Considere w = aᵖbᵖ. Pelo lema, w = xyz com |xy| ≤ p e |y| > 0. Logo, y consiste apenas de a's. Bombeando para k=0, obtemos xz = aᵖ⁻|y|bᵖ, que não pertence a L, contradição.

## Leituras Sugeridas

### Livros Fundamentais
- **Sipser, M.** (2013). *Introduction to the Theory of Computation*, 3rd Edition. Cengage Learning. - Excelente introdução moderna, muito didática.
- **Hopcroft, J. E., Motwani, R., & Ullman, J. D.** (2001). *Introduction to Automata Theory, Languages, and Computation*, 3rd Edition. Pearson. - Texto clássico e abrangente.
- **Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D.** (2006). *Compilers: Principles, Techniques, and Tools*, 2nd Edition. Pearson. - O "Dragon Book", essencial para aplicações em compiladores.
- **Lewis, H. R., & Papadimitriou, C. H.** (1998). *Elements of the Theory of Computation*, 2nd Edition. Prentice Hall. - Abordagem mais concisa e matemática.

### Artigos Seminais
- **Chomsky, N.** (1956). "Three models for the description of language". *IRE Transactions on Information Theory*, 2(3), 113-124.
- **Chomsky, N.** (1959). "On certain formal properties of grammars". *Information and Control*, 2(2), 137-167.
- **Turing, A. M.** (1936). "On computable numbers, with an application to the Entscheidungsproblem". *Proceedings of the London Mathematical Society*, 2(1), 230-265.
- **Kleene, S. C.** (1956). "Representation of events in nerve nets and finite automata". *Automata Studies*, 34, 3-41.

## Implementações em C

Os arquivos incluídos neste diretório demonstram implementações práticas de reconhecedores para cada tipo da hierarquia:
- **tipo3.c**: Reconhecedor para linguagens regulares (aⁿbⁿ com máquina de estados)
- **tipo2.c**: Reconhecedor para linguagens livres de contexto (aⁿbⁿ com pilha)
- **tipo1.c**: Reconhecedor para linguagens sensíveis ao contexto (aⁿbᵐcᵐdⁿ)
- **tipo0.c**: Reconhecedor para linguagens recursivamente enumeráveis (wwᴿ - palíndromos)