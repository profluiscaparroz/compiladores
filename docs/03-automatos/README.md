# Fundamentos Teóricos dos Autômatos Finitos em Computação

# Introdução à Teoria da Computação

A **Teoria da Computação** é um ramo da matemática e da ciência da computação que estuda os modelos formais de cálculo e os problemas que podem ser resolvidos computacionalmente. Ela tem três pilares principais: **teoria dos autômatos**, **teoria da computabilidade** e **complexidade computacional** (Sipser, 2012).

Os autômatos finitos são um dos modelos mais simples de computação formal e têm aplicações em linguagens formais, reconhecimento de padrões e análise lexical (Hopcroft, Motwani & Ullman, 2007). Esses modelos fornecem a base para a compreensão de sistemas mais complexos, como máquinas de Turing e autômatos com pilha.

A **Teoria da Computação** estuda os limites do que pode ser resolvido por um computador e como podemos modelar esses problemas de maneira formal. Ela ajuda a responder perguntas como:  
- Quais problemas podem ser resolvidos por um computador?  
- Quais problemas são impossíveis de resolver?  
- Qual é a maneira mais eficiente de resolver um problema?  

Ela se divide em três áreas principais:  

1. **Teoria dos Autômatos** – Modela computação com máquinas abstratas, como autômatos finitos e máquinas de Turing.  
2. **Teoria da Computabilidade** – Estuda quais problemas podem ou não ser resolvidos computacionalmente.  
3. **Complexidade Computacional** – Analisa a eficiência de algoritmos e a dificuldade de problemas.  

---

## **1. Teoria dos Autômatos - Computadores Modelados Como Máquinas**  

A **Teoria dos Autômatos** é um ramo da **Teoria da Computação** que estuda modelos matemáticos para descrever o funcionamento de sistemas computacionais. Esses modelos, chamados de **autômatos**, representam computadores simplificados que processam entradas e tomam decisões com base em regras pré-definidas.

Os autômatos são usados para entender **quais problemas podem ser resolvidos computacionalmente**, **como os computadores interpretam linguagens formais** e **como projetar sistemas eficientes**. Eles são fundamentais na construção de **compiladores, linguagens de programação, circuitos digitais e inteligência artificial**.

---

### **1.1. O Que São Autômatos?**
Autômatos são **máquinas abstratas** que recebem uma sequência de símbolos como entrada e passam por estados internos até chegar a um estado final. Dependendo do tipo de autômato, eles podem ser usados para reconhecer padrões, analisar estruturas de frases ou até mesmo simular computadores reais.

#### **1.1.1. Componentes de um Autômato**
Todo autômato pode ser descrito por:  
- **Estados (Q):** Representam as configurações internas da máquina.  
- **Alfabeto (Σ):** Conjunto finito de símbolos que o autômato pode processar.  
- **Função de Transição (δ):** Define como a máquina muda de estado com base nos símbolos de entrada.  
- **Estado Inicial (q₀):** Onde o autômato começa sua execução.  
- **Estados de Aceitação (F):** Indicam se a entrada é aceita ou rejeitada.

---

### **1.2. Tipos de Autômatos**
A teoria dos autômatos classifica essas máquinas em diferentes tipos, de acordo com sua complexidade e capacidade computacional.

#### **1.2.1 Autômatos Finitos Determinísticos (AFD)**
Os **Autômatos Finitos Determinísticos** são os mais simples e possuem as seguintes características:
- Para cada estado e símbolo de entrada, há **apenas uma transição possível**.
- Eles **não possuem memória além do estado atual**.
- São usados principalmente para **reconhecimento de padrões e análise lexical**.

### **Exemplo de AFD: Verificando se uma sequência termina em '01'**
Imagine um autômato que verifica se um número binário termina em '01'.

#### **Definição formal do AFD**
- **Estados:** {q₀, q₁, q₂}
- **Alfabeto:** {0,1}
- **Transições:**
  - (q₀, 0) → q₁
  - (q₀, 1) → q₀
  - (q₁, 0) → q₁
  - (q₁, 1) → q₂
  - (q₂, 0) → q₁
  - (q₂, 1) → q₀
- **Estado inicial:** q₀
- **Estado de aceitação:** q₂ (porque significa que a entrada terminou em '01').

Esse autômato aceita sequências como **“1001”**, **“00001”**, mas rejeita **“110”**.

---

#### **1.2.2 Autômatos Finitos Não Determinísticos (AFND)**
Nos **Autômatos Finitos Não Determinísticos (AFND)**:
- Para um mesmo estado e símbolo de entrada, **podem existir múltiplas transições possíveis**.
- Eles podem ter **transições vazias (ε-transições)**, ou seja, mudanças de estado sem consumir um símbolo.
- Apesar de parecerem mais poderosos, **qualquer AFND pode ser convertido para um AFD equivalente**.

### **Exemplo de AFND: Palavras que terminam em ‘ab’ ou ‘ba’**
Podemos definir um autômato que aceita palavras terminando em ‘ab’ ou ‘ba’ com estados sobrepostos, algo difícil de modelar em um AFD sem aumentar muito a quantidade de estados.

---

#### **1.2.3 Autômatos com Pilha (AP)**
Os **Autômatos com Pilha (AP)** são uma extensão dos autômatos finitos que incluem uma **memória na forma de uma pilha**. Eles são usados para reconhecer **linguagens livres de contexto**, como expressões matemáticas e a estrutura sintática de linguagens de programação.

- Além dos estados e transições, os APs podem **empilhar e desempilhar símbolos**, permitindo que eles "lembrem" eventos passados.
- São usados em **analisadores sintáticos de compiladores**.

### **Exemplo de AP: Linguagem de parênteses balanceados**
Este autômato aceita sequências do tipo **“(()())”** mas rejeita **“(()”**:
1. Se encontra ‘(‘, empilha um símbolo.
2. Se encontra ‘)’, desempilha um símbolo.
3. Se a pilha estiver vazia no final, a entrada é aceita.

---

#### **1.2.4 Máquinas de Turing**
As **Máquinas de Turing** são o modelo computacional mais poderoso. Elas **têm uma fita infinita como memória e podem ler/escrever nela**. Isso as torna capazes de simular qualquer algoritmo computável, formando a base da teoria da computabilidade.

##### **Exemplo de Máquina de Turing: Somador de números binários**
Uma máquina de Turing pode receber **“110 + 101”** e transformar em **“1011”**, seguindo um conjunto de regras de transição.

---

### **1.3. Aplicações dos Autômatos**
A teoria dos autômatos é amplamente aplicada em várias áreas da computação, incluindo:

1. **Compiladores e Analisadores Léxicos:**  
   - Linguagens de programação são definidas por gramáticas formais.  
   - Autômatos finitos são usados para identificar palavras-chave e tokens.  

2. **Expressões Regulares:**  
   - Usadas para pesquisa de texto em editores e sistemas de busca.  
   - Exemplo: `grep`, `sed`, `regex` em Python, JavaScript e outras linguagens.  

3. **Circuitos Digitais e Autômatos em Hardware:**  
   - Máquinas de estados são usadas para projetar **processadores e protocolos de comunicação**.  

4. **Inteligência Artificial e Aprendizado de Máquina:**  
   - Sistemas que tomam decisões baseadas em regras podem ser modelados como FSMs.  

---

A **Teoria dos Autômatos** é essencial para entender os limites da computação e projetar sistemas eficientes. Desde **compiladores** até **inteligência artificial**, os autômatos modelam computação de maneira simplificada, mas poderosa. A partir deles, podemos construir máquinas mais complexas, como **Máquinas de Turing**, que servem como base para a ciência da computação moderna.  

---


## **2. Teoria da Computabilidade - O Que é Possível Resolver?**  

A **Teoria da Computabilidade** estuda quais problemas podem ser resolvidos por algoritmos e quais não podem. Ela busca compreender os limites da computação e classificar problemas conforme sua resolubilidade. Esta teoria foi formalizada principalmente por **Alan Turing** e **Alonzo Church**, que demonstraram que existem problemas **computacionalmente insolúveis** – ou seja, problemas para os quais não existe algoritmo capaz de resolvê-los em todas as situações possíveis.

---

### **Fundamentos da Computabilidade**  

### **2.1 O que é um Algoritmo?**  
Um **algoritmo** é uma sequência finita de passos bem definidos para resolver um problema. Um computador, seja físico ou teórico, pode ser visto como um executor de algoritmos.  

### **2.2 Modelos Matemáticos de Computação**  
Para estudar computabilidade, cientistas da computação criaram modelos matemáticos de computação, os principais são:  

1. **Máquinas de Turing** (Alan Turing, 1936)   
   - Modelo abstrato de computação que pode simular qualquer algoritmo computável.  
   - Possui uma fita infinita onde lê/escreve símbolos seguindo regras pré-definidas.  
   - Se um problema não pode ser resolvido por uma Máquina de Turing, então ele é considerado **não computável**.  

2. **Lambda Cálculo** (Alonzo Church, 1936)   
   - Modelo baseado em funções matemáticas e substituição de expressões.  
   - Prova que certas funções podem ser computadas apenas manipulando expressões simbólicas.  

3. **Funções Recursivas**  
   - Baseadas na teoria dos números, usadas para definir funções computáveis.  

Os três modelos acima foram provados como equivalentes (**Tese de Church-Turing**), o que significa que qualquer problema computável pode ser resolvido por qualquer um deles.

---

### **2.3. Classificação dos Problemas Computacionais**  

#### **2.3.1 Problemas Decidíveis (ou Computáveis)**  
Um problema é **decidível** se existe um algoritmo que pode fornecer uma resposta **correta** para qualquer entrada **em tempo finito**. Ou seja, sempre sabemos se a resposta é "Sim" ou "Não".  

#### **Exemplo: O Problema da Multiplicação**  
Dado dois números inteiros $a$ e $b$, podemos sempre calcular $a \times b$ com um algoritmo simples. Isso é um **problema decidível**, pois sempre conseguimos encontrar a resposta.  

#### **Exemplo: Verificar um Número Primo**  
Dado um número $n$, podemos verificar se ele é primo testando se é divisível apenas por 1 e por ele mesmo. Isso pode ser feito com um algoritmo eficiente, tornando o problema **decidível**.  

---

#### **2.3.2 Problemas Semidecidíveis** 
Um problema é **semidecidível** se existe um algoritmo que pode dizer "Sim" quando a resposta for afirmativa, mas pode rodar indefinidamente sem resposta se a resposta for "Não".  

#### **Exemplo: O Problema do Teorema Matemático**  
Dado um teorema, existe uma prova matemática que o demonstra como verdadeiro?  

- Se a prova existir, um programa pode verificá-la e dizer "Sim".  
- Mas se a prova não existir, o programa pode nunca parar, pois não há como provar que nunca encontrará a prova.  

Isso ocorre porque há infinitas possibilidades a serem testadas.  

---

#### **2.3.3 Problemas Indecidíveis (ou Não Computáveis)**  
Um problema é **indecidível** se não existe nenhum algoritmo que possa resolvê-lo corretamente para todas as entradas possíveis. Isso significa que nenhum computador, mesmo idealizado, pode encontrar sempre a resposta correta.  

##### **2.3.3.1 O Problema da Parada (Halting Problem)**  
Alan Turing demonstrou que o **problema da parada** é indecidível. Ele consiste na seguinte pergunta:  

 **Dado um programa e uma entrada, podemos determinar se o programa eventualmente termina ou roda para sempre?**  

- Se um programa fosse capaz de resolver esse problema, então poderíamos prever o comportamento de qualquer código antes mesmo de executá-lo.  
- No entanto, Turing provou que tal programa **não pode existir**, pois levaria a contradições lógicas.  

 **Conclusão:** Não há um algoritmo universal que possa prever se qualquer outro programa irá parar ou rodar indefinidamente.  

##### **2.3.3.2 O Problema da Equivalência de Programas**   
**Dado dois programas diferentes, eles sempre produzem o mesmo resultado para qualquer entrada?**  

- Não há como garantir essa verificação para programas arbitrários.  
- Esse é outro problema indecidível, pois exigiria verificar um número infinito de execuções.  

---

## **3. Consequências da Computabilidade**  

1. **Limites da Computação:** Há problemas que simplesmente não podem ser resolvidos, não importa o quão poderoso seja o computador.  
2. **Criptografia:** A computabilidade ajuda a garantir que certos problemas (como a fatoração de números primos) sejam difíceis o suficiente para serem usados na segurança digital.  
3. **Verificação de Software:** Embora possamos testar software para encontrar erros, não podemos provar matematicamente que um programa está livre de todos os bugs possíveis.  

---

## **Conclusão**  

A **Teoria da Computabilidade** estabelece os fundamentos sobre o que é possível calcular e o que não é. Ela nos mostra que:  

- Alguns problemas são **decidíveis**, ou seja, sempre podem ser resolvidos com um algoritmo.  
⚠️ Outros são **semidecidíveis**, onde às vezes conseguimos uma resposta, mas não sempre.  
- E alguns problemas são **indecidíveis**, ou seja, não podem ser resolvidos por nenhum algoritmo, como o **problema da parada**.  

Essa teoria tem aplicações diretas em **inteligência artificial, linguagens de programação, verificação de software e criptografia**.  

---

## **3. Complexidade Computacional - Quão Difícil é Resolver um Problema?**  

A **Complexidade Computacional** é um campo da **Teoria da Computação** que estuda **o tempo e os recursos necessários para resolver um problema computacional**. O objetivo é classificar os problemas com base em **quão rápido** ou **quão eficiente** um algoritmo pode resolvê-los.  

---

### **3.1. Medindo a Complexidade: Tempo e Espaço**
A complexidade de um problema é geralmente medida em **função do tamanho da entrada (n)**. Por exemplo, se um problema envolve processar uma lista de números, o tamanho da entrada é **quantos números há na lista**.  

Os principais recursos analisados são:  

- **Tempo**: Quantos passos o algoritmo leva para ser executado?  
- **Espaço**: Quanta memória é usada durante a execução?  

A notação matemática usada para medir essa complexidade é a **notação Big-O (O grande)**, que expressa o crescimento do tempo de execução à medida que o tamanho da entrada aumenta.  

### **Exemplos de diferentes crescimentos de tempo**  
Se tivermos um algoritmo que precisa processar um conjunto de números, a complexidade pode variar assim:  

| Complexidade | Nome | Exemplo |
|-------------|------|---------|
| **O(1)** | Tempo constante | Acessar um elemento em um array |
| **O(log n)** | Tempo logarítmico | Pesquisa binária |
| **O(n)** | Tempo linear | Percorrer uma lista |
| **O(n log n)** | Tempo quase linear | Algoritmos eficientes de ordenação, como Merge Sort |
| **O(n²)** | Tempo quadrático | Algoritmo de ordenação ineficiente, como Bubble Sort |
| **O(2ⁿ)** | Tempo exponencial | Resolver o problema da mochila por força bruta |
| **O(n!)** | Tempo fatorial | Resolver um problema de permutações, como o Caixeiro Viajante |  

---

### **3.2. Classes de Complexidade: P, NP, NP-Completos e NP-Difíceis**
Os cientistas da computação classificam os problemas em diferentes **classes de complexidade**. As principais são:

#### **3.2.1 Classe P: Problemas "Fáceis"**
A classe **P** contém problemas que podem ser resolvidos por um algoritmo eficiente, ou seja, em **tempo polinomial**.  

 **Exemplo:** Ordenar uma lista de números  
- O algoritmo **Merge Sort** roda em **O(n log n)**, o que significa que ele é **rápido e eficiente**.  
- Como é resolvido rapidamente, ele pertence à classe **P**.  

#### **3.2.2 Classe NP: Problemas Difíceis de Resolver, mas Fáceis de Verificar**
A classe **NP (nondeterministic polynomial time)** contém problemas em que:  
- A **solução pode ser verificada rapidamente**, mas  
- **Encontrar a solução pode ser muito difícil**.  

 **Exemplo:** Sudoku  
- Se alguém te der uma solução para um Sudoku, você pode verificá-la **rapidamente** (basta conferir se os números seguem as regras).  
- Mas **encontrar essa solução do zero pode levar muito tempo**, pois há muitas possibilidades.  

Se um problema estiver em **NP**, ele **pode ser difícil de resolver, mas fácil de verificar**.  

#### **3.2.3 NP-Completos: Os Problemas Mais Difíceis de NP**  
Os **problemas NP-Completos** são os mais difíceis dentro da classe **NP**. Se alguém encontrar um jeito eficiente de resolver **um** problema NP-Completo, isso significaria que **todos os problemas NP poderiam ser resolvidos rapidamente** (o que ainda não foi provado).  

 **Exemplo:** Problema do Caixeiro Viajante (Travelling Salesman Problem - TSP)  
- Um vendedor precisa visitar várias cidades e encontrar o caminho mais curto.  
- Parece simples, mas, conforme o número de cidades aumenta, o número de possibilidades cresce **exponencialmente**.  
- **Não há um algoritmo eficiente conhecido** para resolver isso rapidamente.  

#### **3.2.4 NP-Difícil: Mais Difícil que NP**  
Um problema é chamado **NP-Difícil** se ele **é pelo menos tão difícil quanto os problemas NP-Completos**, mas **não precisa estar em NP** (ou seja, nem sempre sua solução pode ser verificada rapidamente).  

 **Exemplo:** Problema da Parada  
- Ele pergunta: **Dado um programa e uma entrada, esse programa vai rodar para sempre ou vai parar em algum momento?**  
- Alan Turing provou que **não há algoritmo que possa resolver esse problema para todos os casos possíveis**.  
- Isso significa que o **Problema da Parada é NP-Difícil**, mas **não está em NP**, pois nem sequer conseguimos verificar uma solução eficientemente.  

---

### **3.3. O Grande Mistério: P vs NP**
Uma das maiores questões em ciência da computação é se **P = NP**. Isso significa perguntar:  

 **"Se conseguimos verificar rapidamente a solução de um problema, também conseguimos encontrar essa solução rapidamente?"**  

Atualmente, **ninguém sabe a resposta**. Se fosse provado que **P = NP**, então todos os problemas difíceis, como o Caixeiro Viajante, poderiam ser resolvidos rapidamente! Isso revolucionaria áreas como criptografia, inteligência artificial e logística.  

 **Curiosidade:** O Clay Mathematics Institute oferece um **prêmio de US$ 1 milhão** para quem resolver essa questão!  

---

### **3.4. Aplicações do Estudo da Complexidade Computacional**
Compreender a complexidade computacional tem impactos diretos no mundo real. Alguns exemplos incluem:  

 **Criptografia** 🔒  
- Algoritmos de criptografia (como RSA) se baseiam na **dificuldade de fatorar números grandes**.  
- Se alguém provar que **P = NP**, a maioria dos sistemas de segurança se tornaria obsoleta.  

 **Logística e Otimização** 🚚  
- Empresas como Amazon e Uber usam algoritmos de otimização para encontrar as **melhores rotas e distribuir recursos eficientemente**.  
- Muitos desses problemas são **NP-Difíceis**, então são usados algoritmos aproximados.  

 **Inteligência Artificial** 🤖  
- Aprendizado de máquina envolve encontrar padrões em grandes conjuntos de dados.  
- O estudo da complexidade ajuda a criar **redes neurais e algoritmos de aprendizado mais eficientes**.  

---

## **Conclusão**
A **Complexidade Computacional** é essencial para entender **o que pode ser resolvido eficientemente e o que é impraticável**. Saber se um problema está em **P, NP, NP-Completo ou NP-Difícil** ajuda a determinar **se devemos buscar um algoritmo exato ou uma solução aproximada**.  

 **Resumo das principais ideias:**  
- **P:** Problemas fáceis de resolver.  
- **NP:** Problemas fáceis de verificar, mas difíceis de resolver.  
- **NP-Completos:** Os problemas mais difíceis dentro de NP.  
- **NP-Difíceis:** Problemas ainda mais difíceis, que podem nem ter solução computável.  
- **P vs NP:** Uma das maiores perguntas da ciência da computação.  

---

A **Teoria da Computação** ajuda a entender **o que um computador pode fazer, quais problemas são impossíveis e quais são difíceis**. Isso é essencial para criar novos algoritmos, linguagens de programação e até inteligência artificial!  


# Máquinas de Estados Finitos

As **máquinas de estados finitos (Finite State Machines - FSMs)** são sistemas matemáticos que modelam computação baseada em estados e transições. Elas podem ser usadas para modelar sistemas reativos, protocolos de comunicação e circuitos digitais (Hopcroft, Motwani & Ullman, 2007).

# **Máquinas de Estados Finitos (Finite State Machines - FSMs)**  

As **Máquinas de Estados Finitos (FSMs)** são modelos matemáticos usados para descrever sistemas que podem estar em diferentes estados e fazem transições entre eles com base em entradas específicas. FSMs são amplamente utilizadas na ciência da computação e engenharia para modelagem de sistemas digitais, controle de processos, reconhecimento de padrões e mais.

---

## **1. Definição Formal**  
Uma **Máquina de Estados Finitos (FSM)** é definida por uma quíntupla:  

$M = (Q, \Sigma, \delta, q_0, F)$

onde:  
- **$Q$**: Conjunto finito de estados.  
- **$\Sigma$**: Alfabeto finito de entrada.  
- **$\delta: Q \times \Sigma \to Q$**: Função de transição que mapeia um estado e uma entrada para um novo estado.  
- **$q_0 \in Q$**: Estado inicial.  
- **$F \subseteq Q$**: Conjunto de estados de aceitação (ou estados finais).  

---

## **2. Tipos de Máquinas de Estados Finitos**  

### **2.1. Autômato Finito Determinístico (AFD)**  
No **Autômato Finito Determinístico (AFD)**, cada estado tem exatamente **uma única transição** para cada símbolo de entrada. Isso significa que, dada uma entrada, sempre há um caminho bem definido para processá-la.  

#### **Exemplo: Detector de palavras "ab"**  
Imagine um autômato que reconhece a palavra "ab" dentro de uma sequência de caracteres. Ele pode ser definido pelos seguintes estados:  

- **Q** = {q0 (inicial), q1, q2 (final)}  
- **Σ** = {a, b}  
- **δ**:  
  - δ(q0, a) → q1  
  - δ(q1, b) → q2  
  - δ(q0, b) → q0  
  - δ(q1, a) → q1  
  - δ(q2, a) → q2  
  - δ(q2, b) → q2  
- **q0** = estado inicial  
- **F** = {q2}  

Isso significa que, ao receber a sequência "ab", o autômato termina no estado **q2**, que é um estado de aceitação.  

---

### **2.2. Autômato Finito Não Determinístico (AFND)**  
No **Autômato Finito Não Determinístico (AFND)**, um estado pode ter **várias transições possíveis** para um mesmo símbolo de entrada, ou até transições espontâneas (ε-movimentos).  

#### **Exemplo: Autômato que aceita "ab" ou "ba"**  
Se quisermos construir um autômato que reconhece tanto "ab" quanto "ba", podemos ter um AFND que permite múltiplas opções de transição:  

- **Q** = {q0, q1, q2, q3 (final)}  
- **Σ** = {a, b}  
- **δ**:  
  - δ(q0, a) → {q1}  
  - δ(q0, b) → {q2}  
  - δ(q1, b) → {q3}  
  - δ(q2, a) → {q3}  
- **q0** = estado inicial  
- **F** = {q3}  

Aqui, o AFND permite que diferentes sequências levem ao estado de aceitação.  

**Conversão para AFD:**  
Todo AFND pode ser convertido em um AFD equivalente, mas o número de estados pode crescer exponencialmente.  

---

## **3. Aplicações de Máquinas de Estados Finitos**  

### **3.1. Reconhecimento Lexical em Compiladores**  
Compiladores usam FSMs para identificar palavras-chave, operadores e identificadores na análise léxica de linguagens de programação.  

### **3.2. Controle de Protocolos de Comunicação**  
FSMs são usadas em protocolos de rede para gerenciar estados de conexão (como TCP, que tem estados como SYN-SENT, ESTABLISHED, FIN-WAIT).  

### **3.3. Inteligência Artificial e Jogos**  
Jogos eletrônicos usam FSMs para modelar comportamento de NPCs. Por exemplo, um inimigo pode estar nos estados **Patrulha**, **Perseguir** ou **Atacar**, dependendo da entrada (posição do jogador).  

### **3.4. Circuitos Digitais**  
FSMs são fundamentais para projetar circuitos sequenciais, como controladores de memória RAM e processadores.  

---

## **4. Implementação de um AFD em Python**  
Exemplo simples de um **Autômato Finito Determinístico (AFD)** em C, que reconhece a linguagem que aceita a sequência de símbolos "ab" repetidos, ou seja, ele aceita strings formadas pela repetição de "ab" (como "ab", "abab", "ababab", etc.).

### Exemplo de AFD em C

```c
#include <stdio.h>
#include <string.h>

// Definindo os estados
#define Q0 0 // Estado inicial
#define Q1 1 // Estado de aceitação após 'a'
#define Q2 2 // Estado de aceitação após 'ab'

int afd(char* input) {
    int estado = Q0; // Começa no estado inicial Q0
    int i = 0;
    
    while (input[i] != '\0') {
        switch (estado) {
            case Q0:
                if (input[i] == 'a') {
                    estado = Q1; // Vai para o estado Q1 se encontrar 'a'
                } else {
                    return 0; // Se o símbolo não for 'a', a string é rejeitada
                }
                break;

            case Q1:
                if (input[i] == 'b') {
                    estado = Q2; // Vai para o estado Q2 se encontrar 'b'
                } else {
                    return 0; // Se o símbolo não for 'b', a string é rejeitada
                }
                break;

            case Q2:
                if (input[i] == 'a') {
                    estado = Q1; // Volta para o estado Q1 se encontrar 'a'
                } else {
                    return 0; // Se o símbolo não for 'a', a string é rejeitada
                }
                break;
            
            default:
                return 0; // Estado inválido
        }
        i++;
    }

    // Se o último estado for Q1 ou Q2, significa que a string foi aceita
    return (estado == Q1 || estado == Q2);
}

int main() {
    char input[100];

    printf("Digite uma string: ");
    scanf("%s", input);

    if (afd(input)) {
        printf("A string '%s' é aceita pelo AFD.\n", input);
    } else {
        printf("A string '%s' não é aceita pelo AFD.\n", input);
    }

    return 0;
}
```

### Explicação do Código:

- **Estados**: O AFD tem três estados definidos:
  - **Q0**: Estado inicial.
  - **Q1**: Estado de aceitação após ver "a".
  - **Q2**: Estado de aceitação após ver "ab".
  
- **Função `afd`**:
  - A função `afd` percorre a string e faz a transição entre os estados conforme os símbolos de entrada.
  - No estado `Q0`, o AFD espera encontrar um 'a'. Se encontrar, transita para o estado `Q1`.
  - No estado `Q1`, espera um 'b' para transitar para o estado `Q2`.
  - No estado `Q2`, espera novamente um 'a' para voltar ao estado `Q1`.
  - A string é aceita se, ao final, o AFD terminar em `Q1` ou `Q2`.

- **Entrada**: O usuário deve fornecer uma string. O AFD verifica se a string segue o padrão "ab" repetido.
  
- **Exemplo de Execução**:
  - Entrada: `"abab"`
    - O AFD passa pelos estados Q0 -> Q1 -> Q2 -> Q1 e aceita a string.
  - Entrada: `"aabb"`
    - O AFD rejeita a string, pois não segue o padrão "ab" repetido.

### Como Funciona:
1. O AFD começa no estado Q0.
2. Ele espera que o primeiro caractere seja 'a'. Se for, ele vai para o estado Q1.
3. No estado Q1, ele espera que o próximo caractere seja 'b'. Se for, ele vai para o estado Q2.
4. Em Q2, ele espera que o próximo caractere seja 'a' para voltar ao estado Q1.
5. O AFD aceita a string se terminar em Q1 ou Q2, já que esses são os estados de aceitação.

Este é um exemplo básico de como um AFD pode ser implementado em C para verificar uma linguagem simples.
---

As **Máquinas de Estados Finitos** são modelos fundamentais na computação, usadas para resolver problemas como reconhecimento de padrões, processamento de linguagens e controle de sistemas digitais. Existem diferentes tipos de FSMs, como **AFDs** (determinísticos) e **AFNDs** (não determinísticos), cada um com suas vantagens e desvantagens. Além disso, esses conceitos formam a base para modelos mais avançados, como **Autômatos com Pilha** e **Máquinas de Turing**.  


As FSMs são amplamente utilizadas em inteligência artificial, reconhecimento de fala e controle de processos (Sipser, 2012).

## 3. Linguagens Formais e Classes de Linguagens

Uma **linguagem formal** é um conjunto de sequências de símbolos construídas a partir de um alfabeto finito e definidas por regras sintáticas bem especificadas (Hopcroft, Motwani & Ullman, 2007).

As **linguagens formais** são sistemas de símbolos e regras que definem padrões de estrutura e organização para expressar informações. Elas são fundamentais em áreas como ciência da computação, teoria da computação e linguagens de programação. A teoria das linguagens formais é uma área que estuda essas linguagens e a maneira como elas podem ser descritas e reconhecidas.

### 1. **Definição de Linguagem Formal**

Uma **linguagem formal** é um conjunto de cadeias (ou palavras) que são formadas a partir de um **alfabeto**. Um alfabeto é um conjunto finito de símbolos (por exemplo, $\Sigma = \{a, b\} $) e as palavras da linguagem são sequências desses símbolos. 

Formalmente, uma linguagem $L $ sobre um alfabeto $\Sigma $ é um conjunto de palavras, e uma palavra $w $ é uma sequência finita de símbolos de $\Sigma $. A **gramática** de uma linguagem formal define as regras para gerar todas as palavras dessa linguagem.

#### Exemplos de Linguagens Formais:

1. **Linguagem sobre o alfabeto $\Sigma = \{0, 1\} $**: A linguagem $L $ que contém todas as palavras de comprimento par, como $\{ \epsilon, 00, 11, 0101, 1001, \dots \} $.
   
2. **Linguagem de Parênteses Balanceados**: A linguagem formada por todas as palavras com parênteses corretamente balanceados, como $\{ \epsilon, (), (()), ()() \} $.

### 2. **Gramáticas Formais**

Uma **gramática formal** é uma maneira de descrever a estrutura de uma linguagem formal. Ela consiste em um conjunto de regras que descrevem como as palavras de uma linguagem podem ser formadas. Uma gramática formal é composta por:

- **Variáveis** ou **símbolos não terminais**: Representam partes da linguagem que podem ser expandidas.
- **Terminais**: São os símbolos do alfabeto.
- **Regras de Produção**: Descrevem como os símbolos podem ser substituídos ou gerados.
- **Símbolo inicial**: O símbolo a partir do qual todas as palavras podem ser geradas.

Uma gramática formal pode ser representada como uma quádrupla $G = (V, \Sigma, R, S) $, onde:
- $V $ é um conjunto de variáveis,
- $\Sigma $ é um alfabeto (conjunto de símbolos terminais),
- $R $ é um conjunto de regras de produção,
- $S $ é o símbolo inicial.

### 3. **Classes de Linguagens Formais**

A teoria das **classes de linguagens formais** trata da organização das linguagens com base em sua complexidade e na capacidade de serem reconhecidas ou geradas por diferentes tipos de máquinas. Essas classes são classificadas de acordo com a **hierarquia de Chomsky**, que descreve os diferentes tipos de linguagens formais e os modelos de máquinas que podem reconhecê-las.

#### 3.1 **Linguagens Tipo 0: Linguagens Recursivamente Enumeráveis (RE)**

- **Máquina de Turing**: Linguagens tipo 0 são aquelas que podem ser reconhecidas por uma Máquina de Turing, ou seja, podem ser geradas por um algoritmo que sempre termina com a aceitação de uma palavra ou entra em loop infinito.
- Não existe garantia de que uma Máquina de Turing que reconhece uma linguagem tipo 0 sempre pare, o que significa que essas linguagens podem ser **não decidíveis**.
  
#### 3.2 **Linguagens Tipo 1: Linguagens Sensíveis ao Contexto (CSL)**

- **Máquina Linearmente Limitada (LBA)**: Linguagens sensíveis ao contexto podem ser reconhecidas por uma Máquina Linearmente Limitada, que é uma Máquina de Turing com a restrição de usar uma quantidade limitada de espaço adicional em relação ao tamanho da entrada.
- Essas linguagens são mais poderosas do que as linguagens regulares e context-free, mas ainda assim têm limitações em comparação com as linguagens tipo 0.

#### 3.3 **Linguagens Tipo 2: Linguagens Livre de Contexto (CFL)**

- **Autômato de Pilha**: As linguagens tipo 2 podem ser reconhecidas por um autômato de pilha, que tem uma memória adicional na forma de uma pilha.
- **Gramáticas Livre de Contexto (CFG)**: As gramáticas para essas linguagens podem ser descritas por regras de produção do tipo $A \to \alpha $, onde $A $ é uma variável e $\alpha $ é uma string de variáveis e terminais. Linguagens livres de contexto são amplamente utilizadas para descrever a sintaxe de linguagens de programação.
  
#### 3.4 **Linguagens Tipo 3: Linguagens Regulares (RL)**

- **Autômato Finito Determinístico (DFA)**: As linguagens regulares podem ser reconhecidas por um autômato finito determinístico, que tem uma quantidade limitada de memória (apenas o estado atual).
- **Expressões regulares**: Linguagens regulares podem ser descritas por expressões regulares e são as linguagens mais simples na hierarquia de Chomsky. Elas são adequadas para a modelagem de padrões simples em textos, como validação de números de telefone ou endereços de e-mail.

### 4. **Hierarquia de Linguagens de Chomsky**

A hierarquia de Chomsky é uma classificação das linguagens formais em quatro tipos, do mais simples ao mais complexo:

- **Tipo 3**: Linguagens regulares (reconhecíveis por autômatos finitos).
- **Tipo 2**: Linguagens livres de contexto (reconhecíveis por autômatos de pilha).
- **Tipo 1**: Linguagens sensíveis ao contexto (reconhecíveis por máquinas linearmente limitadas).
- **Tipo 0**: Linguagens recursivamente enumeráveis (reconhecíveis por máquinas de Turing).

A hierarquia mostra que:
- As linguagens de tipo 3 (regulares) são um subconjunto das de tipo 2 (livres de contexto), que são um subconjunto das de tipo 1 (sensíveis ao contexto), que por sua vez são um subconjunto das de tipo 0 (recursivamente enumeráveis).

### 5. **Aplicações das Linguagens Formais**

As linguagens formais são fundamentais em várias áreas da computação:

- **Compiladores**: Utilizam linguagens livres de contexto para descrever a sintaxe de linguagens de programação.
- **Automação de Processos**: Linguagens regulares e expressões regulares são amplamente usadas em validação de dados e busca de padrões.
- **Processamento de Linguagem Natural (NLP)**: As linguagens formais ajudam na análise e geração de sentenças em linguagens naturais.
- **Teoria da Computação**: Estudo da complexidade computacional, decidibilidade e os limites do que pode ser computado.

### Conclusão

As linguagens formais são um dos pilares da teoria da computação e têm várias aplicações práticas em ciência da computação, especialmente no design de compiladores, processamento de linguagem natural e reconhecimento de padrões. A hierarquia de Chomsky e as máquinas associadas a diferentes classes de linguagens ajudam a entender a complexidade computacional e os limites de diferentes tipos de sistemas de reconhecimento e geração de linguagens.

## Referências

- Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D. (2006). *Compilers: Principles, Techniques, and Tools*. Pearson.
- Hopcroft, J. E., Motwani, R., & Ullman, J. D. (2007). *Introduction to Automata Theory, Languages, and Computation*. Addison-Wesley.
- Sipser, M. (2012). *Introduction to the Theory of Computation*. Cengage Learning.

# Autômato Finito Determinístico (AFD)

Um **Autômato Finito Determinístico (AFD)** é um modelo matemático fundamental na teoria de autômatos e na computação em geral. Ele serve para representar linguagens regulares e é essencial para entender a relação entre linguagens e máquinas. Vamos explorar seus conceitos de forma aprofundada. O **Autômato Finito Determinístico (AFD)** é uma ferramenta poderosa e essencial na teoria da computação, sendo capaz de reconhecer linguagens regulares de maneira eficiente e determinística. Seu estudo é fundamental para compreender como as máquinas de estados podem ser aplicadas em diversas áreas da computação, desde a análise de linguagens até a execução de tarefas práticas em softwares de processamento de texto e redes.


### 1. Definição Formal

Um AFD é uma 5-tupla $(Q, \Sigma, \delta, q_0, F)$, onde:

- **Q** é um conjunto finito de estados.
- **Σ** (ou $\Sigma$) é o conjunto finito de símbolos, chamado de alfabeto, que a máquina pode ler.
- **δ** (ou $\delta$) é a função de transição, que mapeia um par $(q, a)$ para um estado $q'$. Ou seja, dada uma entrada $a \in \Sigma$ e um estado $q \in Q$, $\delta(q, a) = q' \in Q$.
- **q₀** é o estado inicial, onde o autômato começa a sua execução ($q₀ \in Q$).
- **F** é o conjunto de estados finais ou de aceitação ($F \subseteq Q$).

### 2. Características do AFD

- **Determinismo**: A principal característica de um AFD é o **determinismo**, que significa que, para cada estado $q \in Q$ e cada símbolo de entrada $a \in \Sigma$, a função de transição $\delta(q, a)$ nos leva a um único estado $q' \in Q$. Não há ambiguidade em um AFD; para qualquer par de estado e símbolo de entrada, a transição é única.
  
- **Estados**: Cada estado em $Q$ pode ser interpretado como uma configuração possível do autômato enquanto ele processa a entrada. O autômato começa no estado inicial $q₀$ e faz transições com base nos símbolos da entrada.

- **Transições**: As transições $\delta$ descrevem como o autômato se move de um estado para outro. Em um AFD, a função de transição é totalmente definida para todos os pares $(q, a)$, ou seja, sempre existe uma transição para qualquer símbolo de entrada em qualquer estado.

- **Estados Finais**: O conjunto $F$ contém os estados nos quais o autômato pode terminar sua execução e aceitar a entrada. Se, após processar todos os símbolos da entrada, o autômato termina em um estado $q \in F$, a entrada é aceita. Caso contrário, a entrada é rejeitada.

### 3. Funcionamento do AFD

O funcionamento do AFD pode ser descrito como segue:

1. O autômato começa no estado inicial $q₀$.
2. O símbolo da entrada atual é lido.
3. A função de transição $\delta$ determina o próximo estado com base no estado atual e no símbolo lido.
4. O autômato se move para o próximo estado e repete esse processo até que todos os símbolos da entrada tenham sido lidos.
5. Se o autômato termina em um estado de aceitação, a entrada é aceita; caso contrário, é rejeitada.

### 4. Exemplo de AFD

Considere um AFD que reconhece a linguagem de todas as palavras sobre o alfabeto $\Sigma = \{a, b\}$ que terminam com a letra 'a'. A 5-tupla seria definida como:

- $Q = \{q₀, q₁\}$ (dois estados)
- $\Sigma = \{a, b\}$
- $δ$ (função de transição):

  $\delta(q₀, a) = q₁$
  $\delta(q₀, b) = q₀$
  $\delta(q₁, a) = q₁$
  $\delta(q₁, b) = q₀$

- $q₀$ é o estado inicial.
- $F = \{q₁\}$, ou seja, o estado final é $q₁$, que indica que a palavra termina com 'a'.

Este AFD funciona da seguinte maneira:

1. Começa em $q₀$.
2. Ao ler um 'a', transita para $q₁$.
3. Ao ler um 'b', permanece em $q₀$.
4. Se, no final da leitura da entrada, o autômato estiver em $q₁$, ele aceita a entrada (pois termina com 'a'); caso contrário, rejeita.

### 5. Propriedades Importantes

#### 5.1. Determinismo

Em um AFD, para cada par de estado e símbolo de entrada, sempre existe uma transição bem definida. Isso é diferente de um **Autômato Finito Não Determinístico (AFND)**, onde pode haver múltiplas transições para um mesmo símbolo em um dado estado.

#### 5.2. Linguagens Regulares

O AFD pode ser utilizado para reconhecer **linguagens regulares**, que são aquelas que podem ser descritas por expressões regulares. A classe das linguagens regulares é a mesma que a classe das linguagens aceitas por AFDs. Isso significa que qualquer linguagem regular pode ser reconhecida por um AFD.

#### 5.3. Equivalência entre AFD e AFND

Embora um AFD seja determinístico e um AFND seja não determinístico, **toda linguagem aceita por um AFND pode ser aceita por um AFD**. A construção de um AFD equivalente a um AFND envolve a **"determinização"** do AFND, um processo que pode ser feito usando o algoritmo de construção de subconjuntos.

### 6. Conversão de AFND para AFD

Um dos aspectos interessantes dos AFDs é que, apesar de serem determinísticos, eles podem ser obtidos a partir de autômatos não determinísticos (AFNDs). O processo de conversão de um AFND para um AFD é realizado pela construção de subconjuntos, onde o conjunto de estados do AFD é formado pelas combinações de estados do AFND.

### 7. Eficiência e Aplicações

AFDs são usados em diversas áreas da ciência da computação, especialmente em **compiladores** (para análise léxica), **redes de comunicação** (para análise de protocolos) e **ferramentas de processamento de texto** (para busca e substituição com expressões regulares).

Embora a construção de um AFD a partir de um AFND possa aumentar o número de estados exponencialmente, os AFDs são mais eficientes em termos de tempo de execução, pois não exigem retrocesso, como ocorre com os AFNDs.

### 8. Características Detalhadas e Expansão do AFD

O **Autômato Finito Determinístico (AFD)** é um dos modelos mais fundamentais na teoria da computação e da linguagem formal. Seu funcionamento é intuitivo, mas suas propriedades e estrutura exigem uma análise detalhada para que possamos compreender todo o potencial desse modelo.

### 8.1. Função de Transição $\delta$

A função de transição, $\delta: Q \times \Sigma \to Q$, é o coração do autômato, determinando como o autômato se move de um estado para outro com base no símbolo de entrada. Para um **AFD**, a transição é sempre determinística: para um estado e um símbolo, existe **apenas um estado de destino**. Essa é a diferença essencial em relação aos **autômatos não determinísticos (AFND)**, onde podem existir múltiplos estados de destino para um dado par (estado, símbolo de entrada).

#### Exemplo 1: Simples AFD

Considere o alfabeto $\Sigma = \{a, b\}$ e um AFD que reconhece a linguagem das palavras que **terminam com 'a'**. A 5-tupla do AFD seria:

$Q = \{q₀, q₁\}, \quad \Sigma = \{a, b\}, \quad \delta = \{(q₀, a) \to q₁, (q₀, b) \to q₀, (q₁, a) \to q₁, (q₁, b) \to q₀\}, \quad q₀ \text{ (estado inicial)}, \quad F = \{q₁\} \text{ (estado de aceitação)}$

Aqui, a máquina começa no estado $q₀$. A transição funciona da seguinte forma:

- Se o autômato está no estado $q₀$ e lê o símbolo **'a'**, ele transita para $q₁$.
- Se o autômato está no estado $q₀$ e lê **'b'**, ele permanece em $q₀$.
- Se o autômato está no estado $q₁$ e lê **'a'**, ele permanece em $q₁$.
- Se o autômato está no estado $q₁$ e lê **'b'**, ele volta para $q₀$.

Este AFD reconhece qualquer palavra sobre $\Sigma = \{a, b\}$ que termine com o símbolo **'a'**.

#### Como Funciona?

Considere a entrada "ab". O autômato segue os seguintes passos:

1. Começa no estado $q₀$.
2. Lê 'a', transita para $q₁$.
3. Lê 'b', transita para $q₀$.

Como o autômato termina no estado $q₀$ (que não é um estado de aceitação), a palavra **não é aceita**.

Agora, para a entrada "baa":

1. Começa no estado $q₀$.
2. Lê 'b', permanece em $q₀$.
3. Lê 'a', transita para $q₁$.
4. Lê 'a', permanece em $q₁$.

O autômato termina no estado $q₁$, que é um estado de aceitação, portanto, a palavra **é aceita**.

### 8.2. Processo de Leitura

Durante a execução do AFD, a entrada é lida um símbolo por vez. O autômato realiza transições de estado com base nos símbolos da entrada. O processamento termina quando toda a entrada é lida. Se o autômato termina em um estado final (aceitação), a entrada é aceita, caso contrário, é rejeitada.

#### Exemplo 2: AFD para Linguagem com Substring "aba"

Considere um AFD que reconhece palavras sobre o alfabeto $\Sigma = \{a, b\}$ que **contêm a substring "aba"** em algum ponto. A 5-tupla do AFD seria:

$Q = \{q₀, q₁, q₂, q₃\}, \quad \Sigma = \{a, b\}, \quad \delta = \{(q₀, a) \to q₁, (q₀, b) \to q₀, (q₁, a) \to q₁, (q₁, b) \to q₂, (q₂, a) \to q₃, (q₂, b) \to q₀, (q₃, a) \to q₃, (q₃, b) \to q₃\}, \quad q₀ \text{ (estado inicial)}, \quad F = \{q₃\}$

Neste autômato:

- O estado $q₀$ é o estado inicial, onde o autômato começa a leitura.
- O estado $q₃$ é o estado de aceitação, que é alcançado quando a substring "aba" é detectada.

**Funcionamento**:
- Se o autômato está em $q₀$ e lê 'a', ele vai para $q₁$.
- Se está em $q₁$ e lê 'b', ele vai para $q₂$.
- Se está em $q₂$ e lê 'a', ele vai para $q₃$.
- Quando o autômato chega em $q₃$, ele permanece lá, independentemente dos próximos símbolos.

**Exemplo de Execução**:
Entrada: "bababa"

1. Começa em $q₀$.
2. Lê 'b', permanece em $q₀$.
3. Lê 'a', transita para $q₁$.
4. Lê 'b', transita para $q₂$.
5. Lê 'a', transita para $q₃$.
6. Lê 'b', permanece em $q₃$.
7. Lê 'a', permanece em $q₃$.

O autômato termina em $q₃$, que é um estado de aceitação, então a entrada **é aceita**.

### 8.3. AFD e Expressões Regulares

Como mencionado anteriormente, **AFDs reconhecem linguagens regulares**, ou seja, linguagens que podem ser descritas por expressões regulares. Existe uma equivalência entre expressões regulares, AFDs e **autômatos finitos não determinísticos (AFND)**. Para qualquer expressão regular, pode-se construir um AFD que a reconhece.

#### Exemplo de Linguagem Regular

A expressão regular **"a(b|a)*"** descreve todas as palavras que começam com 'a' e podem ser seguidas por qualquer número de 'a' ou 'b'. O AFD para essa expressão seria algo como:

$Q = \{q₀, q₁\}, \quad \Sigma = \{a, b\}, \quad \delta = \{(q₀, a) \to q₁, (q₁, a) \to q₁, (q₁, b) \to q₁\}, \quad q₀ \text{ (estado inicial)}, \quad F = \{q₁\}$

Esse AFD vai aceitar qualquer entrada que comece com 'a' e seja seguida por qualquer número de 'a' ou 'b', como por exemplo: "a", "ab", "aa", "aab", "ababa", etc.

### 8.4. AFDs e Eficiência

Uma vantagem dos AFDs em relação aos **autômatos não determinísticos (AFND)** é que a execução do AFD é **determinística** e **não requer retrocessos**. Isso significa que, ao processar uma entrada, o AFD pode fazer isso de forma **linear**, ou seja, em tempo $O(n)$, onde $n$ é o comprimento da entrada. Isso torna os AFDs muito eficientes para reconhecimento de padrões, como ocorre em análise léxica de compiladores e mecanismos de busca de texto.

### 8.5. Conversão de AFND para AFD

A conversão de um AFND para um AFD é feita por meio de um processo chamado **determinização**. A ideia básica é tratar os subconjuntos de estados de um AFND como novos estados de um AFD. No entanto, a conversão pode aumentar exponencialmente o número de estados, já que, em um AFND, um único estado pode se dividir em múltiplos estados dependendo das escolhas de transição.

### 9. Detalhamento do Funcionamento do AFD

O funcionamento do **Autômato Finito Determinístico (AFD)** pode ser dividido em três partes principais:

1. **Leitura da Entrada**: O autômato processa a entrada um símbolo de cada vez.
2. **Transição entre Estados**: O autômato transita de um estado para outro com base no símbolo lido.
3. **Aceitação ou Rejeição**: O autômato aceita a entrada se, ao final da leitura de todos os símbolos, ele terminar em um **estado de aceitação**. Caso contrário, a entrada é rejeitada.

Para ilustrar esses passos de maneira mais clara, vamos explorar em mais detalhes a mecânica de um AFD com um exemplo.

#### Exemplo 1: AFD que Reconhece Palavras com Número Par de "a"s

Considere um AFD que reconhece palavras sobre o alfabeto $\Sigma = \{a, b\}$ que contêm um **número par de "a"s**. A 5-tupla do AFD seria:

$Q = \{q₀, q₁\}, \quad \Sigma = \{a, b\}, \quad \delta = \{(q₀, a) \to q₁, (q₀, b) \to q₀, (q₁, a) \to q₀, (q₁, b) \to q₁\}, \quad q₀ \text{ (estado inicial)}, \quad F = \{q₀\}$

**Funcionamento:**

- O estado $q₀$ é o estado inicial e também o estado de aceitação, que corresponde ao número par de 'a's.
- O estado $q₁$ indica que o número de 'a's lidos até aquele momento é ímpar.

Se uma entrada contém um número par de 'a's, o AFD terminará em $q₀$, e a entrada será aceita. Caso contrário, ele terminará em $q₁$, e a entrada será rejeitada.

**Execução para a entrada "ababa":**

1. Começa no estado $q₀$.
2. Lê 'a', transita para $q₁$ (número ímpar de 'a's).
3. Lê 'b', permanece em $q₁$.
4. Lê 'a', transita para $q₀$ (número par de 'a's).
5. Lê 'b', permanece em $q₀$.
6. Lê 'a', transita para $q₁$ (número ímpar de 'a's).

Como o autômato termina em $q₁$, que não é um estado de aceitação, a entrada **não é aceita**.

#### Execução para a entrada "abab":

1. Começa no estado $q₀$.
2. Lê 'a', transita para $q₁$.
3. Lê 'b', permanece em $q₁$.
4. Lê 'a', transita para $q₀$.
5. Lê 'b', permanece em $q₀$.

Como o autômato termina em $q₀$, que é um estado de aceitação, a entrada **é aceita**.

Os **autômatos finitos determinísticos (AFD)** são modelos matemáticos usados para processar e reconhecer linguagens formais. Eles consistem em um conjunto de estados e uma função de transição que determina como o autômato se move de um estado para outro com base no símbolo de entrada. Vamos analisar alguns exemplos práticos para ilustrar como os AFDs processam strings e reconhecem linguagens.

### Exemplo 1: Reconhecimento de strings que terminam com "ab"

Consideramos um autômato que reconhece strings que terminam com "ab". Isso significa que a máquina precisa verificar se a sequência final da string é "ab", independentemente do que aconteceu antes.

#### Diagrama do AFD

- Estados: `Q0` (inicial), `Q1`, `Q2` (estado de aceitação)
- Alfabeto: `{a, b}`
- Função de transição:

| Estado | Entrada 'a' | Entrada 'b' |
|--------|-------------|-------------|
| Q0     | Q1          | Q0          |
| Q1     | Q1          | Q2          |
| Q2     | Q1          | Q0          |

#### Descrição do AFD:
1. O autômato começa no estado `Q0`.
2. Se o símbolo de entrada for 'a', ele transita para o estado `Q1`.
3. Se o símbolo for 'b', ele transita de volta para `Q0`.
4. Quando o autômato estiver no estado `Q1` e ler 'b', ele transita para `Q2`, que é o estado de aceitação. Ou seja, o autômato aceita as strings que terminam com "ab".
5. Se o autômato estiver no estado `Q2` e ler qualquer entrada (seja 'a' ou 'b'), ele retorna ao estado `Q0` ou `Q1`, dependendo do símbolo.

#### Processamento de uma string: `"ab"`

- Estado inicial: `Q0`
  - Primeiro símbolo: 'a' → transita para `Q1`
  - Segundo símbolo: 'b' → transita para `Q2` (estado de aceitação)

Como o autômato terminou em um estado de aceitação (`Q2`), a string `"ab"` é aceita.

#### Processamento de outra string: `"aab"`

- Estado inicial: `Q0`
  - Primeiro símbolo: 'a' → transita para `Q1`
  - Segundo símbolo: 'a' → transita para `Q1` (permanece em `Q1`)
  - Terceiro símbolo: 'b' → transita para `Q2` (estado de aceitação)

A string `"aab"` também é aceita, porque o autômato termina no estado de aceitação `Q2`.

#### Processamento de outra string: `"ba"`

- Estado inicial: `Q0`
  - Primeiro símbolo: 'b' → transita para `Q0` (permanece em `Q0`)
  - Segundo símbolo: 'a' → transita para `Q1`

O autômato termina no estado `Q1`, que **não é um estado de aceitação**, portanto, a string `"ba"` **não é aceita**.

---

### Exemplo 2: Linguagem de strings que contêm um número par de 'a's

Agora, vamos criar um AFD para aceitar strings que contenham um número par de caracteres 'a'. Ou seja, queremos reconhecer a linguagem `{w | w contém um número par de 'a's}`.

#### Diagrama do AFD

- Estados: `Q0` (inicial e de aceitação), `Q1`
- Alfabeto: `{a, b}`
- Função de transição:

| Estado | Entrada 'a' | Entrada 'b' |
|--------|-------------|-------------|
| Q0     | Q1          | Q0          |
| Q1     | Q0          | Q1          |

#### Descrição do AFD:
1. O autômato começa no estado `Q0`, que é o estado de aceitação e representa um número par de 'a's.
2. Quando ele lê um 'a', ele transita para o estado `Q1`, representando que agora há um número ímpar de 'a's.
3. Quando ele lê outro 'a', ele volta para o estado `Q0`, representando que o número de 'a's voltou a ser par.
4. Se o autômato lê um 'b', ele permanece no estado atual (não afeta a contagem de 'a's).

#### Processamento de uma string: `"aab"`

- Estado inicial: `Q0`
  - Primeiro símbolo: 'a' → transita para `Q1`
  - Segundo símbolo: 'a' → transita para `Q0` (estado de aceitação)
  - Terceiro símbolo: 'b' → permanece em `Q0`

A string `"aab"` é aceita, porque o número de 'a's é par.

#### Processamento de outra string: `"ab"`

- Estado inicial: `Q0`
  - Primeiro símbolo: 'a' → transita para `Q1`
  - Segundo símbolo: 'b' → permanece em `Q1`

A string `"ab"` **não é aceita**, porque o número de 'a's é ímpar e o autômato termina no estado `Q1`.

#### Processamento de outra string: `"baab"`

- Estado inicial: `Q0`
  - Primeiro símbolo: 'b' → permanece em `Q0`
  - Segundo símbolo: 'a' → transita para `Q1`
  - Terceiro símbolo: 'a' → transita para `Q0` (estado de aceitação)
  - Quarto símbolo: 'b' → permanece em `Q0`

A string `"baab"` é aceita, pois o número de 'a's é par.

---

### Exemplo 3: Linguagem de strings que contêm "ab" como substring

Agora, vamos criar um AFD para reconhecer strings que contêm "ab" como substring. Ou seja, a máquina aceita qualquer string que tenha "ab" em algum ponto da sequência.

#### Diagrama do AFD

- Estados: `Q0` (inicial), `Q1`, `Q2` (estado de aceitação)
- Alfabeto: `{a, b}`
- Função de transição:

| Estado | Entrada 'a' | Entrada 'b' |
|--------|-------------|-------------|
| Q0     | Q1          | Q0          |
| Q1     | Q1          | Q2          |
| Q2     | Q2          | Q2          |

#### Descrição do AFD:
1. O autômato começa no estado `Q0`.
2. Se ele lê 'a', ele transita para `Q1`.
3. Se ele, em `Q1`, ler 'b', ele transita para `Q2` (estado de aceitação). Isso significa que "ab" foi encontrado.
4. Uma vez que o autômato tenha alcançado o estado `Q2`, ele permanece em `Q2` para todas as entradas subsequentes.

#### Processamento de uma string: `"aabb"`

- Estado inicial: `Q0`
  - Primeiro símbolo: 'a' → transita para `Q1`
  - Segundo símbolo: 'a' → permanece em `Q1`
  - Terceiro símbolo: 'b' → transita para `Q2` (estado de aceitação)
  - Quarto símbolo: 'b' → permanece em `Q2`

A string `"aabb"` é aceita, porque contém "ab" como substring.

#### Processamento de outra string: `"ba"`

- Estado inicial: `Q0`
  - Primeiro símbolo: 'b' → permanece em `Q0`
  - Segundo símbolo: 'a' → transita para `Q1`

A string `"ba"` **não é aceita**, porque não contém "ab" como substring.

---

Esses exemplos ilustram como os autômatos finitos determinísticos (AFDs) processam strings e reconhecem linguagens com base nas transições entre seus estados. A ideia central é que, ao processar uma string, o autômato transita de estado para estado de acordo com a entrada e, no final, aceita ou rejeita a string com base no estado em que termina.

### 10. Estrutura de Estados e Funcionalidade do AFD

A definição de um AFD é baseada em quatro componentes essenciais:

1. **Conjunto de Estados $Q$**: O conjunto de todos os estados possíveis do autômato. Cada estado representa uma configuração possível durante o processamento da entrada.

2. **Alfabeto $\Sigma$**: O conjunto de símbolos que o autômato pode ler. Em um AFD, o alfabeto é finito e discreto, como letras, números ou outros caracteres.

3. **Função de Transição $\delta$**: A função que define as transições entre estados. Para cada par de estado e símbolo de entrada, a função $\delta$ fornece um único estado de destino. Isso torna o autômato determinístico.

4. **Estado Inicial $q_0$**: O estado onde o autômato começa a execução. Quando a entrada começa a ser lida, o autômato começa nesse estado.

5. **Conjunto de Estados de Aceitação $F$**: O conjunto de estados que indicam que a entrada foi aceita. Se o autômato termina a execução em um desses estados, a entrada é aceita.

#### Propriedades dos AFDs

**Determinismo**: Como o nome sugere, um AFD é determinístico. Isso significa que para cada símbolo de entrada em um determinado estado, sempre há uma única transição possível. Isso se opõe aos autômatos não determinísticos (AFND), onde, para um estado e símbolo, podem existir múltiplas transições possíveis.

**Determinização**: A principal característica do AFD é que ele pode ser obtido a partir de um autômato não determinístico. Isso é feito por um processo chamado **determinização**, onde cada estado do AFD corresponde a um conjunto de estados do AFND.

### 11. AFD e Computação

Na prática, os AFDs são usados em diversas áreas da computação, principalmente quando se trabalha com linguagens regulares, como:

1. **Compiladores e Analisadores Léxicos**: Um AFD pode ser usado para reconhecer palavras-chave e identificadores em um código-fonte. Ele pode ser implementado para verificar se uma sequência de caracteres segue uma determinada expressão regular.

2. **Sistemas de Busca**: Motores de busca de texto frequentemente utilizam AFDs para verificar se uma consulta corresponde a padrões específicos, como correspondências exatas ou aproximações.

3. **Verificação de Padrões**: AFDs são comumente empregados em sistemas de monitoramento de redes ou sistemas de segurança para verificar padrões em fluxos de dados.

4. **Reconhecimento de Padrões em Linguagens Formais**: AFDs são frequentemente usados para reconhecer linguagens simples e previsíveis, como aquelas que podem ser expressas com expressões regulares.

### 12. Conversão de AFND para AFD

Embora os **Autômatos Finitos Não Determinísticos (AFND)** sejam mais flexíveis em termos de transições, eles podem ser convertidos em AFDs equivalentes. Isso é feito por um processo chamado **determinização**, onde o conjunto de estados possíveis é tratado como um único estado. Este processo pode, em casos extremos, resultar em um aumento exponencial no número de estados.

#### Exemplo de Conversão de AFND para AFD

Considere um AFND simples que tem dois estados ($q₀, q₁$) e transições de forma não determinística. Para o símbolo 'a', a máquina pode transitar para dois estados diferentes. Quando se faz a conversão para um AFD, um único estado é criado para representar o conjunto de estados possíveis que o AFND pode alcançar com a leitura de um símbolo.

A **determinização** envolve considerar todos os possíveis estados que o autômato pode estar ao mesmo tempo, transformando-os em um único estado no AFD.

### 13. AFDs e Expressões Regulares

**Expressões regulares** são um método compactado de descrever linguagens regulares, e elas são diretamente relacionadas aos **Autômatos Finitos Determinísticos (AFD)**. Qualquer expressão regular pode ser convertida em um AFD. O processo de conversão envolve criar um AFD que aceitaria exatamente a mesma linguagem definida pela expressão regular.

### 16. Vantagens do AFD

- **Eficiência**: Um AFD pode ser executado em tempo linear em relação ao tamanho da entrada, ou seja, $O(n)$, onde $n$ é o comprimento da string de entrada.
- **Simplicidade**: A estrutura do AFD é simples e direta. Como as transições são sempre determinísticas, o comportamento do autômato é previsível e fácil de implementar.
- **Determinismo**: Não há necessidade de retrocessos ou escolhas múltiplas de transição, o que torna a execução do AFD mais eficiente do que em um AFND.

# Conversão de AFD para AFND

Antes de falar sobre a conversão de um **Autômato Finito Determinístico (AFD)** para um **Autômato Finito Não Determinístico (AFND)**, é importante esclarecer que essa conversão não é necessária em todos os casos. O mais comum é que se converta **AFND para AFD** ou se utilize o conceito de **expressões regulares** para criar um AFD. 

Entretanto, se desejarmos **converter um AFD para um AFND**, é relativamente simples, pois o AFD é um caso especial de AFND. Um AFD já é determinístico, então, ao converter para um AFND, basicamente estamos adicionando mais flexibilidade às transições, mas isso geralmente não é necessário, já que o AFD já resolve o problema de reconhecimento de uma linguagem regular de maneira eficiente.

Agora, se você está se referindo à **conversão de uma expressão regular para um AFND**, ou mais precisamente para um **Autômato Finito Não Determinístico com Transições Epsilon (ε-AFND)**, o algoritmo que você está buscando é o **Algoritmo de Thompson**, que é muito utilizado para criar um AFND a partir de uma expressão regular.

## Algoritmo de Thompson

O **Algoritmo de Thompson** é um método que converte uma expressão regular em um **Autômato Finito Não Determinístico com Transições Epsilon (ε-AFND)**. Ele é fundamental para a construção de **autômatos** a partir de expressões regulares, e geralmente é utilizado para construir o autômato como parte do processo de **compilação** ou **análise léxica**.

#### Passos do Algoritmo de Thompson

1. **Para símbolos simples (como "a" ou "b")**:
   - Para cada símbolo terminal, criamos um autômato com dois estados: um inicial e um final.
   - Existe uma transição entre os dois estados para o símbolo.

2. **Para a operação de alternância (|)**:
   - Para uma expressão do tipo $A | B $, criamos um novo estado inicial que possui transições ε (transições que não consomem símbolo) para os estados iniciais de $A$ e $B$.
   - Os estados finais de $A$ e $B$ possuem transições ε para um novo estado final.

3. **Para a operação de concatenação (AB)**:
   - Para uma expressão do tipo $AB$, conectamos o estado final de $A$ ao estado inicial de $B$ com uma transição ε.

4. **Para a operação de Kleene star (A\*)**:
   - Para uma expressão do tipo $A^*$, criamos um novo estado inicial e um novo estado final.
   - O estado inicial tem uma transição ε para o estado final, e também tem uma transição ε para o estado inicial de $A$.
   - O estado final de $A$ tem uma transição ε para o estado final novo, além de uma transição ε de volta ao estado inicial de $A$, criando um loop.

Esse processo cria um **AFND** que aceita a mesma linguagem que a expressão regular. Esse AFND pode então ser convertido para um **AFD** usando técnicas de determinização.

### Exemplo de Algoritmo de Thompson

Considerando a expressão regular **(a|b)*a**, vamos criar o **AFND** usando o algoritmo de Thompson:

1. Para o símbolo **'a'**, criamos um autômato simples com dois estados $q₀ $ e $q₁ $, onde $q₀ $ é o estado inicial e $q₁ $ é o estado final, com uma transição de $q₀ $ para $q₁ $ com 'a'.

2. Para o símbolo **'b'**, o processo é o mesmo. Criamos outro autômato de dois estados $q₂ $ e $q₃ $, onde há uma transição de $q₂ $ para $q₃ $ com 'b'.

3. Para a alternância **'a|b'**, conectamos os estados iniciais $q₀ $ e $q₂ $ com transições ε para um novo estado $q₄ $. O estado final $q₁ $ e $q₃ $ têm transições ε para um novo estado final $q₅ $.

4. Para a operação Kleene star **(a|b)*,** criamos dois estados adicionais: um estado inicial $q₆ $ e um estado final $q₇ $. O estado $q₆ $ tem transições ε para o estado $q₄ $ (início da alternância) e para o estado $q₇ $ (aceitação imediata). O estado $q₅ $ (final da alternância) tem transições ε para $q₄ $ (reinicia o loop) e para $q₇ $ (aceitação).

5. Por fim, para a expressão **(a|b)*a**, criamos uma transição de $q₇ $ para o estado $q₈ $ com 'a' e, então, $q₈ $ é o estado final.

Esse é um esboço da construção do AFND correspondente à expressão regular **(a|b)*a**.

### Exemplo em C: Construção de um AFND para uma Expressão Regular Simples

Vamos criar um exemplo simples de **AFND** que reconhece a expressão regular **a(b|a)*b**. O AFND será construído para aceitar a sequência de caracteres que começa com **a**, seguida por zero ou mais **b** ou **a**, e termina com **b**.

#### Estrutura do Código C

Neste exemplo, vamos criar uma função básica que implementa um **AFND** simples que reconhece a expressão regular acima.

```c
#include <stdio.h>
#include <stdbool.h>

// Definindo os estados possíveis
typedef enum {q0, q1, q2, q3} Estado;

// Função para verificar se a entrada é aceita
bool afnd(char* entrada) {
    Estado estado_atual = q0;  // Começamos no estado inicial

    while (*entrada != '\0') {
        char simbolo = *entrada++;

        // Transições de acordo com o símbolo lido
        switch (estado_atual) {
            case q0:
                if (simbolo == 'a') {
                    estado_atual = q1;  // Transita para q1 ao ler 'a'
                } else {
                    return false;  // Rejeita se não for 'a'
                }
                break;
            case q1:
                if (simbolo == 'a') {
                    estado_atual = q1;  // Fica em q1 ao ler 'a'
                } else if (simbolo == 'b') {
                    estado_atual = q2;  // Transita para q2 ao ler 'b'
                } else {
                    return false;  // Rejeita se o símbolo não for 'a' ou 'b'
                }
                break;
            case q2:
                if (simbolo == 'a') {
                    estado_atual = q1;  // Retorna a q1 ao ler 'a'
                } else if (simbolo == 'b') {
                    estado_atual = q3;  // Transita para q3 ao ler 'b'
                } else {
                    return false;  // Rejeita se o símbolo não for 'a' ou 'b'
                }
                break;
            case q3:
                return false;  // Se já está em q3, a sequência foi aceita
        }
    }

    return estado_atual == q3;  // A entrada é aceita se terminarmos em q3
}

int main() {
    char entrada[] = "abbbab";  // Exemplo de entrada
    if (afnd(entrada)) {
        printf("Entrada aceita.\n");
    } else {
        printf("Entrada rejeitada.\n");
    }
    return 0;
}
```

### Explicação do Código

1. O código define quatro estados para o AFND: `q0`, `q1`, `q2`, `q3`. Estes estados representam diferentes etapas do processamento da entrada.
2. A função `afnd` lê os caracteres da entrada e realiza transições de acordo com os símbolos lidos.
3. O **AFND** funciona sem determinismo explícito neste código, porque ao ler o símbolo, ele pode transitar para diferentes estados dependendo de qual símbolo é lido.
4. O autômato aceita a entrada se, ao final da leitura, ele terminar no estado **`q3`**.