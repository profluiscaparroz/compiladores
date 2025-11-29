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

## Fundamentos Teóricos Avançados

### Propriedades de Fechamento

As propriedades de fechamento são fundamentais para a compreensão teórica e prática das linguagens formais. Segundo Hopcroft et al. (2001) e Sipser (2013), uma classe de linguagens é **fechada** sob uma operação se, ao aplicar essa operação a linguagens da classe, o resultado permanece na mesma classe.

**Linguagens Regulares** são fechadas sob:
- União, Concatenação, Estrela de Kleene (por definição de expressões regulares)
- Complemento (se L é regular, então Σ* - L também é regular)
- Interseção (via De Morgan: L₁ ∩ L₂ = (L₁ᶜ ∪ L₂ᶜ)ᶜ)
- Diferença, Reverso, Homomorfismo e Homomorfismo Inverso

**Linguagens Livres de Contexto** são fechadas sob:
- União, Concatenação, Estrela de Kleene
- Homomorfismo e Homomorfismo Inverso
- NÃO são fechadas sob interseção e complemento (Bar-Hillel et al., 1961)

### Teorema de Myhill-Nerode

O **Teorema de Myhill-Nerode** (Myhill, 1957; Nerode, 1958) fornece uma caracterização alternativa das linguagens regulares usando relações de equivalência:

Uma linguagem L ⊆ Σ* é regular se e somente se a relação de equivalência ≡_L, definida por:
> x ≡_L y ⟺ (∀z ∈ Σ*: xz ∈ L ⟺ yz ∈ L)

tem um número finito de classes de equivalência. Além disso, o número de classes é igual ao número mínimo de estados de qualquer AFD que reconhece L.

Este teorema é particularmente útil para provar que linguagens NÃO são regulares, fornecendo uma alternativa ao Lema do Bombeamento. Por exemplo, para L = {aⁿbⁿ | n ≥ 0}, as palavras a, aa, aaa, ... pertencem a classes de equivalência distintas (infinitas classes), logo L não é regular.

### Lema de Ogden

O **Lema de Ogden** (Ogden, 1968) é uma generalização do Lema do Bombeamento para linguagens livres de contexto. Enquanto o Lema do Bombeamento padrão para LLC pode falhar em demonstrar que certas linguagens não são livres de contexto, o Lema de Ogden introduz a noção de "marcar" posições especiais na palavra, permitindo provas mais sofisticadas.

**Enunciado**: Para toda LLC L, existe uma constante n tal que, para toda palavra z ∈ L com pelo menos n posições marcadas, z pode ser decomposta como z = uvwxy, satisfazendo:
1. vwx contém no máximo n posições marcadas
2. vx contém pelo menos uma posição marcada
3. Para todo i ≥ 0, uvⁱwxⁱy ∈ L

### Formas Normais de Gramáticas

A **Forma Normal de Chomsky (FNC)** e a **Forma Normal de Greibach (FNG)** são representações canônicas de gramáticas livres de contexto com propriedades úteis:

**Forma Normal de Chomsky** (Chomsky, 1959):
- Todas as produções são da forma A → BC ou A → a
- Facilita o algoritmo CYK (Cocke-Younger-Kasami) para parsing em O(n³)
- Demonstra que toda LLC pode ser reconhecida em tempo polinomial

**Forma Normal de Greibach** (Greibach, 1965):
- Todas as produções são da forma A → aα, onde a ∈ Σ e α ∈ V*
- Elimina recursão à esquerda
- Cada derivação consome exatamente um terminal por passo
- Útil para parsers LL

### Problema da Ambiguidade

Uma gramática é **ambígua** se existe alguma palavra que possui mais de uma árvore de derivação (ou mais de uma derivação mais à esquerda). O problema de determinar se uma gramática é ambígua é **indecidível** (Cantor, 1962; Floyd, 1962).

Segundo Aho et al. (2006), a ambiguidade é particularmente problemática em linguagens de programação:
- A famosa ambiguidade do **dangling else**: `if (a) if (b) S1; else S2;` — o else pertence a qual if?
- Expressões como `a - b - c`: associatividade à esquerda vs. à direita

Linguagens como C e Java resolvem ambiguidades através de regras de precedência e associatividade especificadas na documentação da linguagem.

### Linguagens Inerentemente Ambíguas

Uma linguagem é **inerentemente ambígua** se toda gramática que a gera é ambígua. Parikh (1966) demonstrou que a linguagem:

> L = {aⁿbⁿcᵐdᵐ | n, m ≥ 1} ∪ {aⁿbᵐcᵐdⁿ | n, m ≥ 1}

é inerentemente ambígua. Isso tem implicações profundas: não importa quão cuidadosamente projetemos uma gramática para L, sempre haverá palavras com múltiplas derivações.

## Exercícios Básicos

1) Classifique as linguagens abaixo segundo Chomsky e justifique:
   - L1 = { aⁿbⁿ | n ≥ 0 } - Tipo-2 (livre de contexto, requer pilha)
   - L2 = { aⁿbᵐ | n,m ≥ 0 } - Tipo-3 (regular, não requer contagem sincronizada)
   - L3 = { w | w tem número par de 1's, Σ={0,1} } - Tipo-3 (regular, estados finitos suficientes)

2) Dê uma gramática regular para L = { (ab)ⁿ | n ≥ 0 }.
   Solução: S → abS | ε

3) Explique por que { aⁿbⁿ | n ≥ 0 } não é regular usando o Lema do Bombeamento.
   Demonstração: Suponha que L é regular com constante de bombeamento p. Considere w = aᵖbᵖ. Pelo lema, w = xyz com |xy| ≤ p e |y| > 0. Logo, y consiste apenas de a's. Bombeando para k=0, obtemos xz = aᵖ⁻|y|bᵖ, que não pertence a L, contradição.

## Exercícios Avançados

4) **Teorema de Myhill-Nerode**: Use o Teorema de Myhill-Nerode para provar que L = {ww | w ∈ {a,b}*} não é regular.

   **Solução**: Considere as palavras aⁱ para i = 0, 1, 2, ... Precisamos mostrar que aⁱ e aʲ (com i ≠ j) pertencem a classes de equivalência distintas sob ≡_L.
   
   Para aⁱ, o sufixo z = aⁱ resulta em aⁱaⁱ = a²ⁱ ∈ L (pois a²ⁱ = aⁱaⁱ).
   Para aʲ com j ≠ i, o mesmo sufixo z = aⁱ resulta em aʲaⁱ. Se j ≠ i, então aʲaⁱ ∉ L (não pode ser escrito como ww para nenhum w).
   
   Logo, existem infinitas classes de equivalência, e L não é regular.

5) **Lema de Ogden**: Prove que L = {aⁿbⁿcⁿ | n ≥ 1} não é livre de contexto usando o Lema do Bombeamento para LLC.

   **Solução**: Seja p a constante do bombeamento. Considere z = aᵖbᵖcᵖ ∈ L. Pelo lema, z = uvwxy com |vwx| ≤ p e |vx| > 0.
   
   Como |vwx| ≤ p, a substring vwx não pode conter simultaneamente a's e c's (a distância entre o último 'a' e o primeiro 'c' é p). Logo, vx contém no máximo dois tipos de símbolos.
   
   Bombeando para i = 0 (uwy) ou i = 2 (uv²wx²y), obtemos uma palavra onde as contagens de a's, b's e c's não são iguais, contradizendo a pertinência a L.

6) **Forma Normal de Chomsky**: Converta a gramática G = ({S, A}, {a, b}, P, S) para FNC, onde P:
   - S → aAb | ab
   - A → aAb | ab
   
   **Solução**: 
   Passo 1: Substituir terminais em produções longas
   - Introduzir T_a → a, T_b → b
   
   Passo 2: Eliminar produções com mais de 2 não-terminais
   - S → T_a X₁ | T_a T_b, onde X₁ → A T_b
   - A → T_a X₁ | T_a T_b
   
   Gramática em FNC:
   - S → T_a X₁ | T_a T_b
   - A → T_a X₁ | T_a T_b
   - X₁ → A T_b
   - T_a → a
   - T_b → b

7) **Ambiguidade**: Mostre que a gramática E → E + E | E * E | (E) | id é ambígua e proponha uma gramática não-ambígua equivalente.

   **Solução**: A gramática é ambígua porque a expressão "id + id * id" tem duas árvores de derivação distintas:
   
   Árvore 1: E → E + E → id + E → id + E * E → id + id * id (+ primeiro)
   Árvore 2: E → E * E → E + E * E → id + id * id (* primeiro)
   
   Gramática não-ambígua (com precedência e associatividade à esquerda):
   - E → E + T | T
   - T → T * F | F
   - F → (E) | id
   
   Esta gramática força * a ter maior precedência que +, e ambos são associativos à esquerda.

8) **Pumping Lemma avançado**: Prove que L = {aⁱbʲcᵏ | i = j ou j = k, mas não ambos} não é livre de contexto.

   **Solução**: Esta é uma linguagem complexa que requer técnicas avançadas.
   
   **Estratégia usando Lema de Ogden**:
   Seja n a constante do Lema de Ogden. Considere a palavra z = aⁿbⁿc²ⁿ ∈ L (pois i = j = n, mas j ≠ k = 2n).
   
   Marque todas as posições dos b's (n posições marcadas). Pelo Lema de Ogden, z = uvwxy onde:
   - vwx contém no máximo n posições marcadas (então está contida em uma região limitada)
   - vx contém pelo menos uma posição marcada (ao menos um b em vx)
   
   **Caso 1**: Se vx contém apenas b's, bombear para i=2 aumenta o número de b's, quebrando i=j.
   **Caso 2**: Se vx contém a's e b's, bombear altera desigualmente, também quebrando a condição.
   **Caso 3**: Se vx contém b's e c's, análise similar leva a contradição.
   
   Em todos os casos, a palavra bombeada viola a condição "i = j ou j = k, mas não ambos".
   
   Para uma prova completa e formal, consulte Hopcroft et al. (2001), Capítulo 7, ou Sipser (2013), seção sobre propriedades de fechamento de LLCs.

## Problemas Práticos do Dia-a-Dia (Estilo StackOverflow)

Esta seção apresenta problemas reais que desenvolvedores enfrentam e que podem ser resolvidos com conhecimento de linguagens formais e teoria de compiladores.

### Problema 1: Validação de Expressões com Parênteses Balanceados

**Contexto (StackOverflow #3426427 e similares)**: "Preciso validar se uma expressão matemática tem parênteses, colchetes e chaves corretamente balanceados. Regex não funciona. Por quê?"

**Por que Regex não funciona**: 
Expressões regulares correspondem a linguagens regulares (Tipo 3). O balanceamento de parênteses requer "lembrar" quantos parênteses abertos existem — informação ilimitada que autômatos finitos não podem armazenar. A linguagem L = {(ⁿ)ⁿ | n ≥ 0} é livre de contexto (Tipo 2), não regular.

**Solução correta**: Usar um autômato de pilha (implementação com pilha explícita):
```c
bool balanceado(const char* expr) {
    Stack* pilha = stack_create();
    for (int i = 0; expr[i]; i++) {
        char c = expr[i];
        if (c == '(' || c == '[' || c == '{') {
            stack_push(pilha, c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (stack_empty(pilha)) {
                stack_destroy(pilha);
                return false;
            }
            char topo = stack_pop(pilha);
            if ((c == ')' && topo != '(') ||
                (c == ']' && topo != '[') ||
                (c == '}' && topo != '{')) {
                stack_destroy(pilha);
                return false;
            }
        }
    }
    bool resultado = stack_empty(pilha);
    stack_destroy(pilha);
    return resultado;
}
```

**Lição**: Conhecer a hierarquia de Chomsky evita horas tentando fazer regex funcionar para problemas impossíveis.

### Problema 2: Parser de Configuração com Aninhamento

**Contexto (StackOverflow #tagged/parsing)**: "Meu arquivo de configuração permite blocos aninhados como `group { server { host: ... } }`. Minha solução com string.split() não funciona para aninhamento profundo."

**Análise teórica**: 
Estruturas aninhadas são inerentemente livres de contexto. Abordagens baseadas apenas em expressões regulares (split, regex) falham porque não podem rastrear níveis de aninhamento arbitrários.

**Solução**: Implementar um parser descendente recursivo:
```c
// Gramática: Config → Block* ; Block → ID "{" Config "}" | ID ":" Value
Node* parse_config(Lexer* lex) {
    Node* root = node_create(CONFIG);
    while (!lex_eof(lex) && !lex_peek(lex, "}")) {
        node_add_child(root, parse_block(lex));
    }
    return root;
}

Node* parse_block(Lexer* lex) {
    Token* id = lex_expect(lex, TOKEN_ID);
    if (lex_match(lex, "{")) {
        Node* block = node_create_with_name(BLOCK, id->lexeme);
        Node* nested_config = parse_config(lex);
        node_add_child(block, nested_config);  // Adiciona config aninhada como filho
        lex_expect(lex, "}");
        return block;
    } else {
        lex_expect(lex, ":");
        Token* val = lex_expect(lex, TOKEN_VALUE);
        return node_create_keyval(id->lexeme, val->lexeme);
    }
}
```

### Problema 3: Extrair Strings de Código Fonte

**Contexto (StackOverflow #tagged/regex+parsing)**: "Preciso extrair todas as strings literais de um código JavaScript, mas minha regex falha com strings contendo aspas escapadas como `"ele disse \"olá\""` ou strings dentro de comentários."

**Por que é complexo**:
1. Strings podem conter aspas escapadas: `"abc\"def"`
2. Strings dentro de comentários não devem ser capturadas
3. JavaScript tem template literals com interpolação: `` `${expr}` ``

**Análise**: 
Um lexer adequado precisa de estados (normal, dentro_string, dentro_comentario) — essencialmente um autômato finito com semântica mais rica. A regex simples `"[^"]*"` falha miseravelmente.

**Solução**: Implementar análise léxica correta:
```c
Token* scan_string(Lexer* lex) {
    char quote = lex_advance(lex); // " ou '
    StringBuilder* sb = sb_create();
    while (!lex_eof(lex)) {
        char c = lex_advance(lex);
        if (c == '\\') {
            sb_append(sb, lex_advance(lex)); // caractere escapado
        } else if (c == quote) {
            return token_create(TOKEN_STRING, sb_to_string(sb));
        } else {
            sb_append(sb, c);
        }
    }
    return token_error("String não terminada");
}
```

### Problema 4: Validar Identificadores de Variáveis

**Contexto (StackOverflow #13875205 e similares)**: "Qual regex valida identificadores válidos em C? Preciso aceitar `_var1` mas rejeitar `1var`."

**Solução com teoria formal**:
Identificadores em C são uma linguagem regular:
- Primeiro caractere: letra ou underscore [a-zA-Z_]
- Caracteres seguintes: letra, dígito ou underscore [a-zA-Z0-9_]*

**Expressão regular**: `[a-zA-Z_][a-zA-Z0-9_]*`

**AFD correspondente**:
```
Estado inicial: q0
q0 --[a-zA-Z_]--> q1 (aceitação)
q1 --[a-zA-Z0-9_]--> q1
q0 --[0-9]--> q_erro (rejeição: não pode começar com dígito)
```

Este é um exemplo clássico onde a teoria (expressões regulares = autômatos finitos) se aplica diretamente.

### Problema 5: Avaliação de Expressões Matemáticas

**Contexto (StackOverflow #tagged/expression-evaluation)**: "Preciso avaliar expressões como `3 + 4 * 2 / (1 - 5)^2` respeitando precedência de operadores."

**Por que é um problema de compiladores**:
1. Tokenização: separar números, operadores, parênteses
2. Parsing: construir árvore respeitando precedência (^ > */ > +-)
3. Avaliação: percorrer árvore em pós-ordem

**Gramática para expressões (não-ambígua)**:
```
Expr   → Term (('+' | '-') Term)*
Term   → Factor (('*' | '/') Factor)*
Factor → Base ('^' Factor)?        // ^ é associativo à direita
Base   → NUMBER | '(' Expr ')'
```

**Algoritmo Shunting-Yard** (Dijkstra, 1961):
Converte notação infixa para pós-fixa usando pilha de operadores, respeitando precedência e associatividade. É uma aplicação direta de autômatos de pilha.

### Problema 6: Detectar Código Morto

**Contexto (StackOverflow #tagged/dead-code)**: "Como detectar automaticamente código que nunca será executado no meu programa?"

**Conexão com teoria**:
- Análise de alcançabilidade em grafos de fluxo de controle (CFG)
- Construir CFG é análise sintática + semântica
- Encontrar nós não alcançáveis a partir do nó inicial é busca em grafo

**Limitação teórica**:
O problema geral é **indecidível** (redutível ao Problema da Parada). Se pudéssemos detectar todo código morto, poderíamos resolver o Problema da Parada:
```c
void f() {
    if (programa_termina(P, X)) {
        codigo_aqui();  // morto sse P não termina em X
    }
}
```

Compiladores usam aproximações conservadoras: detectam código obviamente morto (após `return`, condições constantes `if (false)`), mas podem não detectar casos sutis.

### Problema 7: Syntax Highlighting em Editores

**Contexto (StackOverflow #tagged/syntax-highlighting)**: "Por que o syntax highlighting do meu editor às vezes erra, especialmente em strings multi-linha ou comentários aninhados?"

**Explicação teórica**:
A maioria dos editores usa expressões regulares (linguagens Tipo 3) para highlighting por performance. Porém:
- Strings multi-linha requerem rastrear estado entre linhas
- Comentários aninhados `/* /* */ */` requerem contagem (Tipo 2)
- Template literals com interpolação `${...}` requerem pilha

**Solução moderna**: 
Editores como VSCode usam gramáticas TextMate (basicamente regex com estados) ou parsers incrementais (Tree-sitter) que mantêm AST parcial atualizada.

### Problema 8: Injeção de SQL e Sanitização

**Contexto (StackOverflow #tagged/sql-injection)**: "Por que concatenar strings para SQL é perigoso? `query = 'SELECT * FROM users WHERE id = ' + user_input`"

**Análise sob ótica de linguagens formais**:
O problema é que `user_input` pode conter tokens que alteram a estrutura sintática da query. Se `user_input = "1; DROP TABLE users; --"`, a query resultante tem estrutura completamente diferente da pretendida.

**A gramática de SQL** é livre de contexto. Quando concatenamos strings, estamos essencialmente tentando compor gramáticas de forma ad-hoc, sem garantias.

**Solução correta — Prepared Statements**:
```sql
PREPARE stmt FROM 'SELECT * FROM users WHERE id = ?';
EXECUTE stmt USING @user_input;
```

O parser processa a query estruturalmente primeiro, depois substitui parâmetros como dados (não código). A separação entre análise sintática e dados é fundamental.

### Problema 9: Migração de Código entre Linguagens

**Contexto**: "Preciso converter 100.000 linhas de código de Python 2 para Python 3. Substituição textual não funciona."

**Por que é um problema de compiladores**:
- A transformação precisa entender a estrutura do código (AST)
- Algumas mudanças são contextuais: `print x` → `print(x)` mas `print(x, y)` permanece
- Tipos e semântica importam: `unicode` → `str`, divisão inteira vs. real

**Solução**: Ferramentas como **2to3** (Python) ou **jscodeshift** (JavaScript) operam no nível de AST:
1. Parse do código fonte → AST
2. Transformações na AST (padrões → substituições)
3. Geração de código a partir da AST modificada

Este é compilação source-to-source (transpilação), aplicando diretamente conceitos de análise sintática e geração de código.

### Problema 10: Performance de Regex

**Contexto (StackOverflow #tagged/regex-performance)**: "Minha regex `(a+)+b` causa timeout em strings como `aaaaaaaaaaaaaac`. Por quê?"

**Análise com teoria de autômatos**:
A regex `(a+)+b` é patológica. O motor de regex (que usa backtracking, não AFD puro) tenta exponencialmente muitas formas de particionar os a's entre os grupos `(a+)` antes de falhar.

Para string "a" × n seguida de "c":
- Número de tentativas ≈ 2ⁿ (catastrófico!)

**Solução**:
1. Simplificar regex: `(a+)+b` = `a+b` (equivalente, linear)
2. Usar motores baseados em AFD (RE2, rust/regex) que garantem O(n)
3. Entender que regex com grupos aninhados + quantificadores pode ter complexidade exponencial

**Lição**: Conhecer a teoria (AFN → AFD, backtracking vs. Thompson NFA) ajuda a escrever regex eficientes e evitar ReDoS (Regular Expression Denial of Service).

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
- **Myhill, J.** (1957). "Finite automata and the representation of events". *WADD Technical Report*, 57-624.
- **Nerode, A.** (1958). "Linear automaton transformations". *Proceedings of the American Mathematical Society*, 9(4), 541-544.
- **Ogden, W.** (1968). "A helpful result for proving inherent ambiguity". *Mathematical Systems Theory*, 2(3), 191-194.
- **Greibach, S. A.** (1965). "A new normal-form theorem for context-free phrase structure grammars". *Journal of the ACM*, 12(1), 42-52.
- **Bar-Hillel, Y., Perles, M., & Shamir, E.** (1961). "On formal properties of simple phrase structure grammars". *Zeitschrift für Phonetik, Sprachwissenschaft und Kommunikationsforschung*, 14, 143-172.
- **Parikh, R. J.** (1966). "On context-free languages". *Journal of the ACM*, 13(4), 570-581.
- **Dijkstra, E. W.** (1961). "Algol 60 translation: An algol 60 translator for the x1 and making a translator for algol 60". *Report MR 35/61*, Mathematisch Centrum, Amsterdam.

## Implementações em C

Os arquivos incluídos neste diretório demonstram implementações práticas de reconhecedores para cada tipo da hierarquia:
- **tipo3.c**: Reconhecedor para linguagens regulares (aⁿbⁿ com máquina de estados)
- **tipo2.c**: Reconhecedor para linguagens livres de contexto (aⁿbⁿ com pilha)
- **tipo1.c**: Reconhecedor para linguagens sensíveis ao contexto (aⁿbᵐcᵐdⁿ)
- **tipo0.c**: Reconhecedor para linguagens recursivamente enumeráveis (wwᴿ - palíndromos)