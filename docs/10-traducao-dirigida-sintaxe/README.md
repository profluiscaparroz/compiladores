# Tradução Dirigida por Sintaxe em Compiladores

A **tradução dirigida por sintaxe** é uma técnica fundamental no projeto de compiladores, especialmente nas fases intermediárias entre a análise sintática e a geração de código. Ela consiste na associação de **ações semânticas** com as **regras gramaticais** de uma linguagem formal, de modo que a tradução de um programa-fonte seja orientada pela estrutura sintática que ele possui. Em outras palavras, a SDT utiliza a árvore sintática construída durante a análise sintática como guia para gerar uma tradução do código-fonte (como código intermediário, código objeto ou até mesmo uma árvore de semântica abstrata).

---

### 1. Fundamentos

A base teórica da tradução dirigida por sintaxe está na **gramática livre de contexto** (GLC). A ideia principal é estender essa gramática com **atributos** e **ações semânticas**:

* **Atributos**: Valores que podem ser associados aos símbolos não-terminais e terminais da gramática.
* **Ações semânticas**: Blocos de código (em geral, em alguma linguagem de programação como C ou Java) que calculam os atributos ou executam outras tarefas (como gerar código) com base nesses atributos.

A tradução dirigida por sintaxe é, portanto, um tipo de **gramática dirigida por atributos (attribute grammar)**.

---

### 2. Tipos de Atributos

Existem dois tipos principais de atributos:

* **Atributos sintetizados**: Calculados a partir dos atributos dos **filhos** de um nó na árvore sintática. São comuns em gramáticas do tipo **L-attributed**, típicas de analisadores de descida recursiva.
* **Atributos herdados**: Calculados a partir dos atributos dos **pais ou irmãos** (contexto externo) na árvore sintática. Usados para passar informações top-down.

Exemplo:
Considere uma regra da gramática para uma expressão aritmética:

```
E → E1 + T { E.val = E1.val + T.val }
```

Aqui, `E.val`, `E1.val` e `T.val` são atributos sintetizados que armazenam o valor da expressão.

---

### 3. Gramáticas Dirigidas por Sintaxe

Uma **SDT** pode ser embutida diretamente nas regras da gramática como ações semânticas, formando uma **gramática dirigida por sintaxe**. Essas ações podem ser:

* **In-line**: Escritas diretamente no meio da produção.
* **Pós-produção**: Escritas ao final da produção, mais comuns em ferramentas como Yacc ou Bison.

Exemplo com ação pós-produção:

```
S → id := E { print("Atribuição de valor: ", E.val) }
```

---

### 4. Modelos de Implementação

Existem dois modelos principais de tradução dirigida por sintaxe:

#### a) **Tradução durante a análise sintática (on-the-fly)**

É realizada enquanto a árvore sintática está sendo construída. Usada principalmente em **analisadores LL(1)** ou **LR(1)**.
Vantagem: evita construir a árvore completa.
Desvantagem: limitações quanto ao uso de atributos herdados.

#### b) **Tradução via árvore sintática anotada**

Primeiro constrói-se a árvore de derivação ou árvore sintática abstrata (AST), e depois ela é percorrida em uma **passagem posterior** para executar as ações.
Vantagem: mais flexível, permite múltiplas passagens.
Desvantagem: requer mais memória.

---

### 5. Aplicações da Tradução Dirigida por Sintaxe

A SDT é usada em várias fases do compilador:

* **Construção de árvores abstratas (ASTs)**: estrutura compacta e semanticamente significativa do código-fonte.
* **Geração de código intermediário**: por exemplo, tradução para código em 3 endereços.
* **Verificação de tipos**: assegura que os tipos de variáveis e expressões sejam compatíveis.
* **Verificação semântica**: por exemplo, verificar declaração prévia de variáveis.
* **Geração de código final**: transformar a AST ou representação intermediária em assembly.

---

### 6. Exemplo Prático

Para ilustrar, suponha a seguinte gramática e SDT:

```
E → E1 + T     { E.val = E1.val + T.val }
E → T          { E.val = T.val }
T → num        { T.val = num.lexval }
```

Dado o código de entrada `3 + 4`, a árvore sintática terá nós com atributos `val` que serão computados durante a análise ou em uma passagem posterior:

```
      E
     /|\
   E + T
   |    |
  T     num(4)
  |
num(3)
```

A avaliação será:

* `T.val = 3`
* `T.val = 4`
* `E1.val = 3`, `T.val = 4` → `E.val = 3 + 4 = 7`

---

### 7. Ferramentas que Utilizam SDT

Ferramentas conhecidas que suportam SDT incluem:

* **Yacc/Bison**: para análise sintática LR, com ações semânticas em C.
* **ANTLR**: combina análise sintática e semântica com ações em várias linguagens.
* **PLY (Python Lex-Yacc)**: implementa análise léxica e sintática com suporte a ações semânticas.

---

### 8. Vantagens e Desafios

**Vantagens:**

* Integração natural entre sintaxe e semântica.
* Modularidade: ações associadas diretamente a regras específicas.
* Possibilidade de reutilização da estrutura sintática para múltiplas finalidades.

**Desafios:**

* Atributos herdados podem complicar a implementação.
* Pode exigir múltiplas passagens em gramáticas mais complexas.
* A ordem de avaliação dos atributos precisa ser cuidadosamente controlada (especialmente em gramáticas com dependências cíclicas).

---

## Síntese: Fase de Tradução Dirigida por Sintaxe

A **tradução dirigida por sintaxe (Syntax-Directed Translation – SDT)** é uma fase intermediária do compilador que **relaciona a estrutura sintática do programa com seu significado semântico**, guiando a geração de código ou análise semântica a partir da **árvore sintática**.

### O que ela faz?

Ela **associa ações semânticas** às **regras da gramática** de uma linguagem de programação. Essas ações são responsáveis por:

* Calcular valores (ex: resultado de expressões)
* Verificar tipos
* Construir árvores abstratas (AST)
* Gerar código intermediário

### Como funciona?

Cada símbolo (como `E`, `T`, `F`) pode ter **atributos**, que são dados associados a ele, e cada produção da gramática tem **ações semânticas** que manipulam esses atributos. O processo pode ocorrer:

* **Durante a análise sintática** (ação embutida na derivação)
* **Após construir a árvore** (passagem posterior)

### Tipos de atributos:

* **Sintetizados**: fluem **de baixo para cima** (usados para montar resultados de expressões, por exemplo).
* **Herdados**: fluem **de cima para baixo** (usados para passar contexto, como tipo de variável).

### Exemplo simples:

Regra:

```
E → E + T  { E.val = E.val + T.val }
```

Se `E = 2 + 3`, o valor final `E.val` será `5`.

---

### Em resumo:

| Conceito                          | Função principal                                       |
| --------------------------------- | ------------------------------------------------------ |
| SDT (Syntax-Directed Translation) | Controlar a tradução com base na estrutura sintática   |
| Atributos                         | Guardar valores e contextos (sintetizados ou herdados) |
| Ações semânticas                  | Código que calcula ou verifica informações             |
| Resultado                         | Código intermediário, AST, verificação semântica etc.  |

---

### Por que é importante?

É nessa fase que o compilador **interpreta o que o programa realmente faz**, com base na sua forma gramatical. Sem ela, o compilador saberia **como o código é escrito**, mas não **o que ele significa**.

---

### Conclusão

A tradução dirigida por sintaxe representa uma ponte essencial entre a estrutura gramatical de uma linguagem de programação e a sua interpretação semântica. Ao associar atributos e ações às produções da gramática, os compiladores podem efetuar análises semânticas, otimizações e geração de código de forma organizada e eficiente. Seu uso é indispensável em compiladores modernos e continua sendo objeto de estudo tanto na academia quanto na indústria.
