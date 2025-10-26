# 03 — Autômatos Finitos

## Índice
1. [Introdução](#introdução)
2. [Definições Formais](#definições-formais)
3. [Autômatos Finitos Determinísticos (AFD)](#autômatos-finitos-determinísticos-afd)
4. [Autômatos Finitos Não-Determinísticos (AFN)](#autômatos-finitos-não-determinísticos-afn)
5. [Conversão AFN→AFD](#conversão-afnafd)
6. [Minimização de Autômatos](#minimização-de-autômatos)
7. [Aplicações Práticas](#aplicações-práticas)
8. [Exercícios](#exercícios)
9. [Implementações Incluídas](#implementações-incluídas)

---

## Introdução

Autômatos finitos são modelos matemáticos fundamentais na teoria da computação e constituem a base para o desenvolvimento de compiladores, sistemas de reconhecimento de padrões e diversas outras aplicações computacionais. Desenvolvidos na década de 1950 por matemáticos como Stephen Kleene, Michael Rabin e Dana Scott, os autômatos finitos representam máquinas abstratas capazes de reconhecer linguagens regulares.

Um autômato finito pode ser imaginado como uma máquina que lê uma sequência de símbolos (string) da esquerda para a direita, um símbolo por vez, e decide se essa sequência pertence ou não a uma linguagem específica. Durante esse processo, o autômato transita entre diferentes estados baseado nos símbolos lidos, iniciando em um estado inicial e terminando em um estado que pode ser de aceitação ou rejeição.

A importância dos autômatos finitos na ciência da computação é difícil de superestimar. Eles são utilizados em compiladores para análise léxica, em editores de texto para busca de padrões, em protocolos de rede para validação de mensagens, e em sistemas embarcados para controle de comportamento. Além disso, servem como introdução aos conceitos mais avançados da teoria da computação, como máquinas de Turing e decidibilidade.

Na academia, os autômatos finitos são estudados dentro da teoria das linguagens formais e autômatos, um campo que conecta matemática, lógica e ciência da computação. Esta teoria estabelece os limites do que pode ser computado por diferentes tipos de máquinas abstratas e fornece ferramentas matemáticas rigorosas para analisar algoritmos e linguagens de programação.

---

## Definições Formais

### Autômato Finito Determinístico (AFD)

Um **Autômato Finito Determinístico** é formalmente definido como uma quíntupla:

**M = (Q, Σ, δ, q₀, F)**

onde:

- **Q**: Conjunto finito de estados do autômato. Exemplo: Q = {q₀, q₁, q₂}
- **Σ**: Alfabeto de entrada, conjunto finito de símbolos que o autômato pode ler. Exemplo: Σ = {0, 1}
- **δ**: Função de transição δ: Q × Σ → Q, que mapeia um par (estado atual, símbolo lido) para um novo estado
- **q₀**: Estado inicial, onde q₀ ∈ Q
- **F**: Conjunto de estados finais ou de aceitação, onde F ⊆ Q

**Propriedades importantes de um AFD:**

1. **Determinismo**: Para cada par (estado, símbolo), existe exatamente uma transição possível
2. **Completude**: A função de transição está definida para todos os pares (q, a) onde q ∈ Q e a ∈ Σ
3. **Finitude**: O autômato possui um número finito de estados

### Linguagem Aceita por um AFD

Uma string w = a₁a₂...aₙ é **aceita** por um AFD M se existe uma sequência de estados r₀, r₁, ..., rₙ onde:

1. r₀ = q₀ (começa no estado inicial)
2. δ(rᵢ, aᵢ₊₁) = rᵢ₊₁ para i = 0, 1, ..., n-1 (segue as transições)
3. rₙ ∈ F (termina em um estado de aceitação)

A **linguagem** reconhecida por M, denotada L(M), é o conjunto de todas as strings aceitas por M:

**L(M) = {w ∈ Σ* | M aceita w}**

---

## Autômatos Finitos Determinísticos (AFD)

### Conceitos Básicos

Um AFD opera de maneira simples e previsível. Imagine um AFD como uma máquina de estados com as seguintes características:

- **Leitura sequencial**: O autômato lê a entrada da esquerda para a direita, um símbolo por vez
- **Estado atual**: A qualquer momento, o autômato está em exatamente um estado
- **Transições determinísticas**: Para cada símbolo lido, há exatamente uma transição possível
- **Decisão final**: Após ler toda a entrada, o autômato aceita se estiver em um estado final, ou rejeita caso contrário

### Exemplo Visual Simples

Considere um AFD que aceita strings binárias com número par de zeros:

```
Estados: Q = {q₀, q₁}
Alfabeto: Σ = {0, 1}
Estado inicial: q₀
Estados finais: F = {q₀}

Transições:
- δ(q₀, 0) = q₁  (de par para ímpar)
- δ(q₀, 1) = q₀  (continua par)
- δ(q₁, 0) = q₀  (de ímpar para par)
- δ(q₁, 1) = q₁  (continua ímpar)
```

Diagrama de transição:
```
    1          1
  ┌───┐      ┌───┐
  │   ↓      ↓   │
 (q₀) ←──0──→ q₁
  ^              
  │──────────────│
       início
```

**Exemplo de execução:**
- String "110": q₀ →(1) q₀ →(1) q₀ →(0) q₁ → **Rejeita** (não está em F)
- String "1100": q₀ →(1) q₀ →(1) q₀ →(0) q₁ →(0) q₀ → **Aceita** (está em F)

### Implementação em C

A implementação de um AFD em C geralmente envolve:

1. **Representação de estados**: Usar inteiros (0, 1, 2, ...) ou enumerações
2. **Tabela de transição**: Array bidimensional `transicao[estado][simbolo]`
3. **Conjunto de estados finais**: Array booleano ou conjunto de inteiros
4. **Simulação**: Loop que processa cada símbolo da entrada

### Aplicações de AFDs

Os AFDs são amplamente utilizados em:

- **Análise Léxica**: Reconhecimento de tokens em compiladores (identificadores, números, operadores)
- **Validação de Entrada**: Verificação de formatos (CPF, email, telefone, URLs)
- **Expressões Regulares**: Implementação de padrões de busca em editores de texto
- **Protocolos de Comunicação**: Validação de mensagens e sequências de comandos
- **Circuitos Digitais**: Controle de sequência em hardware

---

## Autômatos Finitos Não-Determinísticos (AFN)

### Definição Formal

Um **Autômato Finito Não-Determinístico** é definido como:

**M = (Q, Σ, δ, q₀, F)**

A diferença principal está na função de transição:

- **δ**: Q × (Σ ∪ {ε}) → P(Q)

onde P(Q) é o conjunto potência de Q (conjunto de todos os subconjuntos de Q).

### Características do Não-Determinismo

Um AFN difere de um AFD em três aspectos fundamentais:

1. **Múltiplas transições**: Para um par (estado, símbolo), pode haver zero, uma ou múltiplas transições possíveis
2. **ε-transições**: O autômato pode mudar de estado sem consumir nenhum símbolo de entrada
3. **Escolha de caminho**: O autômato pode "escolher" qual transição seguir quando há múltiplas opções

**Importante**: Uma string é aceita por um AFN se existe **pelo menos um** caminho de aceitação. Não é necessário que todos os caminhos aceitem.

### Teorema da Equivalência

**Teorema (Rabin-Scott, 1959)**: Para todo AFN, existe um AFD equivalente que reconhece a mesma linguagem.

Este teorema é fundamental porque:
- Permite usar AFNs para modelar linguagens de forma mais intuitiva
- Garante que podemos convertê-los para AFDs para implementação eficiente
- Estabelece que AFDs e AFNs têm o mesmo poder computacional

### Quando Usar AFN vs AFD

**Use AFN quando:**
- Modelar o problema de forma mais natural e compacta
- Número de estados do AFD equivalente seria muito grande
- Fase de design e análise conceitual

**Use AFD quando:**
- Implementação para produção (mais eficiente)
- Simulação em tempo real
- Minimização de recursos computacionais

---

## Conversão AFN→AFD

### Algoritmo de Construção de Subconjuntos

O algoritmo mais conhecido para converter AFN em AFD é o **algoritmo de construção de subconjuntos** (subset construction):

**Ideia central**: Cada estado do AFD resultante representa um subconjunto de estados do AFN original.

**Passos do algoritmo:**

1. **ε-closure(q)**: Calcular o fecho-ε de cada estado (conjunto de estados alcançáveis via ε-transições)

2. **Estado inicial do AFD**: q₀' = ε-closure(q₀)

3. **Para cada estado não processado S do AFD:**
   - Para cada símbolo a ∈ Σ:
     - T = ε-closure(δ(s, a)) para todo s ∈ S
     - Criar transição δ'(S, a) = T
     - Se T é novo, adicionar à lista de estados a processar

4. **Estados finais do AFD**: Qualquer estado S que contenha pelo menos um estado final do AFN

**Complexidade**: O AFD resultante pode ter até 2^|Q| estados, onde |Q| é o número de estados do AFN.

### Exemplo Prático

AFN que aceita strings que contêm "ab" ou "ba":

```
AFN:
- Estados: {q₀, q₁, q₂, q₃, q₄}
- Estado inicial: q₀
- Estados finais: {q₂, q₄}
- Transições:
  ε: q₀ → q₁, q₀ → q₃
  a: q₁ → q₂
  b: q₂ → (nenhum)
  b: q₃ → q₄
  a: q₄ → (nenhum)
```

Após a conversão, o AFD terá estados compostos como {q₀, q₁, q₃}, {q₂}, {q₄}, etc.

---

## Minimização de Autômatos

### Algoritmo de Particionamento

A **minimização** de um AFD busca encontrar um AFD equivalente com o menor número possível de estados.

**Algoritmo de Hopcroft (1971):**

1. **Partição inicial**: Dividir estados em dois grupos: finais (F) e não-finais (Q - F)

2. **Refinamento iterativo**: Para cada partição P e cada símbolo a:
   - Dividir P em subgrupos onde estados no mesmo subgrupo vão para a mesma partição ao ler a
   - Repetir até não haver mais refinamentos

3. **Construção do AFD mínimo**: Cada partição final se torna um estado do novo AFD

**Propriedade**: Dois estados são equivalentes se, para toda string w, ambos levam a estados finais ou ambos levam a estados não-finais.

### Teorema da Minimalidade

**Teorema (Myhill-Nerode)**: Para qualquer linguagem regular L, existe um único AFD mínimo (a menos de renomeação de estados) que reconhece L.

Este teorema garante que:
- A minimização produz um resultado único
- Não existe AFD menor que reconheça a mesma linguagem
- A minimização é um processo bem definido matematicamente

---

## Aplicações Práticas

### 1. Compiladores - Análise Léxica

Na fase de análise léxica, os AFDs são usados para reconhecer tokens:

```c
// AFD para reconhecer identificadores: [a-zA-Z_][a-zA-Z0-9_]*
Estado inicial: q0
  q0 --[a-zA-Z_]--> q1 (final)
  q1 --[a-zA-Z0-9_]--> q1
```

### 2. Validação de Dados

Validação de CPF (formato: XXX.XXX.XXX-XX):
- 11 estados para capturar os dígitos e pontuação
- Estados de transição para pontos e hífen
- Estado final após o 11º dígito

### 3. Protocolos de Rede

AFDs são usados para validar sequências de comandos em protocolos como HTTP, FTP, SMTP:
- Cada estado representa uma etapa do protocolo
- Transições validam comandos e respostas
- Estados finais indicam conclusão bem-sucedida

### 4. Expressões Regulares

Todo padrão de expressão regular pode ser convertido em um AFN, depois em um AFD:
- Engines de regex usam AFDs para busca eficiente
- Thompson's construction converte regex → AFN
- Subset construction converte AFN → AFD

---

## Exercícios

### Exercícios Básicos

1) **Construa um AFD** que aceite strings binárias com número par de 0's.
   - Dica: Use dois estados (par/ímpar)

2) **Desenhe o diagrama** de um AFD que aceite strings que começam e terminam com 'a'.

3) **Implemente em C** um AFD que valide números de telefone no formato (XX) XXXX-XXXX.

### Exercícios Intermediários

4) **Converta o seguinte AFN em AFD** usando construção de subconjuntos:
   ```
   Estados: {q0, q1, q2}
   δ(q0, a) = {q0, q1}
   δ(q1, b) = {q2}
   Estados finais: {q2}
   ```

5) **Minimize o AFD** com estados {q0, q1, q2, q3, q4} onde:
   - Estados finais: {q2, q4}
   - δ(q0,a)=q1, δ(q0,b)=q2
   - δ(q1,a)=q2, δ(q1,b)=q3
   - δ(q2,a)=q2, δ(q2,b)=q4
   - δ(q3,a)=q4, δ(q3,b)=q3
   - δ(q4,a)=q4, δ(q4,b)=q4

### Exercícios Avançados

6) **Implemente um simulador genérico** que aceite a definição de um AFD via arquivo de configuração (formato JSON ou texto).

7) **Prove formalmente** que a linguagem L = {0ⁿ1ⁿ | n ≥ 0} não é regular usando o Lema do Bombeamento.

8) **Desenvolva um conversor AFN→AFD** completo com ε-transições.

---

## Implementações Incluídas

Este diretório contém várias implementações práticas em C:

### Exemplos Básicos

- **1exemplo.c**: AFD para reconhecer números (apenas dígitos)
  - Demonstra conceitos básicos de estados e transições
  - Exemplo didático para iniciantes

- **2exemplo.c**: AFD para validar identificadores em C
  - Mostra uso de funções da biblioteca (isalpha, isalnum)
  - Aplica regras reais de uma linguagem de programação

- **3exemplo.c**: AFD para validar endereços de e-mail
  - Exemplo mais complexo com múltiplos estados
  - Demonstra validação de formato estruturado

### Exemplos Avançados

- **exemploAFD.c**: Implementação completa e genérica de AFD
  - Estrutura de dados para representar AFDs
  - Múltiplos exemplos (divisibilidade por 3, comentários C++, identificadores)
  - Funções para construção e simulação de AFDs
  - Inclui visualização de tabelas de transição

### Como Compilar e Executar

```bash
# Compilar um exemplo
gcc -o programa 1exemplo.c

# Executar
./programa

# Compilar com flags de otimização e warnings
gcc -Wall -O2 -o programa 1exemplo.c
```

### Estrutura Recomendada para Projetos

Para projetos maiores, considere organizar o código assim:

```
projeto/
├── include/
│   └── automaton.h      # Definições e protótipos
├── src/
│   ├── automaton.c      # Implementação do AFD
│   └── main.c           # Programa principal
└── Makefile             # Script de compilação
```

---

## Referências Acadêmicas

1. **Hopcroft, J. E., Motwani, R., & Ullman, J. D.** (2006). *Introduction to Automata Theory, Languages, and Computation*. 3rd Edition. Pearson.

2. **Sipser, M.** (2012). *Introduction to the Theory of Computation*. 3rd Edition. Cengage Learning.

3. **Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D.** (2006). *Compilers: Principles, Techniques, and Tools*. 2nd Edition. Pearson.

4. **Kleene, S. C.** (1956). "Representation of Events in Nerve Nets and Finite Automata". *Automata Studies*, Princeton University Press.

5. **Rabin, M. O., & Scott, D.** (1959). "Finite Automata and Their Decision Problems". *IBM Journal of Research and Development*, 3(2), 114-125.

---

## Recursos Adicionais

- **JFLAP**: Software educacional para visualizar e simular autômatos (www.jflap.org)
- **Automaton Simulator**: Ferramenta online interativa
- **Regex101**: Para experimentar com expressões regulares e ver os AFDs gerados

---

*Este material faz parte da disciplina de Compiladores e Teoria da Computação.*