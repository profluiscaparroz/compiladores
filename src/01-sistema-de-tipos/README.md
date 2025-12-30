# 01 — Sistema de Tipos em Compiladores

## Índice

1. [Introdução](#introdução)
2. [História dos Sistemas de Tipos](#história-dos-sistemas-de-tipos)
3. [Por Que Tipos Existem](#por-que-tipos-existem)
4. [Fundamentos Teóricos](#fundamentos-teóricos)
5. [Classificação de Sistemas de Tipos](#classificação-de-sistemas-de-tipos)
6. [Padrões IEEE e Representação de Tipos](#padrões-ieee-e-representação-de-tipos)
7. [Teoria de Tipos](#teoria-de-tipos)
8. [Inferência de Tipos](#inferência-de-tipos)
9. [Polimorfismo e Generics](#polimorfismo-e-generics)
10. [Sistemas de Tipos Avançados](#sistemas-de-tipos-avançados)
11. [Implementação em Compiladores](#implementação-em-compiladores)
12. [Verificação de Tipos](#verificação-de-tipos)
13. [Desafios e Limitações](#desafios-e-limitações)
14. [O Que Todo Arquiteto de Compiladores Deve Saber](#o-que-todo-arquiteto-de-compiladores-deve-saber)
15. [Referências Acadêmicas](#referências-acadêmicas)

---

## Introdução

Um **sistema de tipos** é uma das fundações mais importantes da Ciência da Computação e peça central no design e implementação de compiladores modernos. Sistemas de tipos são métodos formais e sintáticos para classificar programas de acordo com os tipos de valores que computam, com o objetivo primário de **prevenir erros de execução** através de verificações em tempo de compilação.

### Definição Formal

Segundo **Benjamin Pierce** em "Types and Programming Languages" (2002):

> "Um sistema de tipos é um método tratável e sintático para provar a ausência de certos comportamentos de programa, classificando frases de acordo com os tipos de valores que computam."

Esta definição destaca três aspectos cruciais:
- **Tratável**: O sistema deve ser decidível e eficiente o suficiente para ser implementado em um compilador
- **Sintático**: As regras são baseadas na estrutura sintática do programa, não em sua semântica operacional
- **Provar a ausência**: Sistemas de tipos são conservadores — podem rejeitar programas corretos, mas garantem que programas aceitos não terão certos erros

### Objetivos dos Sistemas de Tipos

Os sistemas de tipos servem múltiplos propósitos fundamentais:

1. **Detecção de Erros em Tempo de Compilação**
   - Prevenir operações inválidas (adicionar número a string)
   - Detectar uso de variáveis não inicializadas
   - Verificar compatibilidade de argumentos em chamadas de função

2. **Abstração e Modularidade**
   - Interfaces tipadas permitem que módulos sejam usados independentemente de sua implementação
   - Contratos explícitos entre componentes do software

3. **Documentação**
   - Tipos servem como documentação formal e verificável
   - Assinaturas de tipos descrevem o comportamento esperado de funções

4. **Otimização**
   - Informações de tipos permitem que compiladores gerem código mais eficiente
   - Eliminação de verificações dinâmicas desnecessárias
   - Especialização de código para tipos específicos

5. **Segurança de Memória**
   - Prevenir acesso inválido à memória
   - Evitar vulnerabilidades como buffer overflows
   - Garantir que ponteiros apontam para dados válidos

### Importância para Compiladores

Para um **arquiteto de compiladores**, o sistema de tipos é crucial porque:

- **Define a interface do compilador**: O que é um programa válido?
- **Guia o design da linguagem**: Quais abstrações são expressáveis?
- **Determina a complexidade da análise**: Que verificações são necessárias?
- **Influencia a geração de código**: Como representar valores em tempo de execução?
- **Afeta a experiência do programador**: Como erros são reportados?

---

## História dos Sistemas de Tipos

A evolução dos sistemas de tipos reflete a história da própria Ciência da Computação, desde os primeiros computadores até as linguagens modernas.

### Anos 1940-1950: Os Primórdios

#### Programação sem Tipos (1940s)

Os primeiros computadores, como o **ENIAC** (1945) e o **EDVAC** (1949), eram programados diretamente em **linguagem de máquina**, onde não havia distinção formal entre diferentes tipos de dados. Todos os dados eram simplesmente sequências de bits, e era responsabilidade exclusiva do programador garantir que operações fossem aplicadas a dados apropriados.

**Problemas dessa abordagem**:
- **Erros catastróficos**: Interpretar um endereço como número ou vice-versa causava crashes imprevisíveis
- **Dificuldade de manutenção**: Impossível verificar correção sem executar o código
- **Falta de abstração**: Todo código lidava com representação de baixo nível

#### Primeiros Conceitos de Tipos (1950s)

**FORTRAN I** (1957), desenvolvido por **John Backus** e sua equipe na IBM, introduziu o primeiro sistema de tipos prático em uma linguagem de alto nível:

```fortran
C FORTRAN I - Tipos implícitos baseados no primeiro caractere
      INTEGER I, J, K    ! Variáveis inteiras
      REAL X, Y, Z       ! Variáveis de ponto flutuante
      I = 5
      X = 3.14
```

**Inovações do FORTRAN**:
- **Tipagem implícita**: Variáveis começando com I-N eram inteiras; outras eram reais
- **Conversão automática**: `REAL + INTEGER` resultava em `REAL`
- **Verificação limitada**: Detectava alguns erros de tipo em tempo de compilação

**COBOL** (1959), liderado por **Grace Hopper**, introduziu tipos de dados mais orientados a negócios:
- **Tipos numéricos decimais**: `PIC 9(5)V99` (números com precisão decimal exata)
- **Tipos alfanuméricos**: `PIC X(30)` (strings de caracteres)
- **Tipos de data e moeda**: Formatos específicos para aplicações comerciais

### Anos 1960: Formalização Matemática

#### ALGOL 60 e a Tipagem Estrutural

**ALGOL 60** (1960) foi revolucionário na formalização de tipos:

```algol
begin
  integer i;
  real x;
  Boolean flag;
  
  procedure soma(a, b: integer): integer;
  begin
    soma := a + b
  end;
  
  i := soma(5, 10);  // OK: tipos compatíveis
  x := soma(5, 10);  // OK: conversão implícita de integer para real
end
```

**Contribuições do ALGOL 60**:
- **Declarações explícitas de tipo**: Todos os identificadores devem ser declarados
- **Tipos de procedimentos**: Procedimentos e funções têm assinaturas tipadas
- **Blocos com escopo léxico**: Tipos são verificados respeitando escopo
- **Verificação estática**: Muitos erros de tipo detectados em compilação

#### Teoria dos Tipos Lambda

Paralelamente, matemáticos e lógicos desenvolviam a **teoria formal de tipos**:

**Alonzo Church** e o **Cálculo Lambda Tipado** (1940s-1960s):
- **Simply Typed Lambda Calculus** (λ→): Sistema de tipos mais simples
- **Curry-Howard Correspondence** (1960s): Descoberta profunda da conexão entre tipos e lógica:
  - **Tipos** correspondem a **proposições**
  - **Programas** correspondem a **provas**
  - **Avaliação** corresponde a **normalização de provas**

Esta correspondência estabeleceu que:
```
Programas com tipo A → B são provas construtivas de "A implica B"
```

**Per Martin-Löf** desenvolveu a **Teoria de Tipos Intuicionista** (1970s), unificando lógica e computação de forma elegante.

### Anos 1970: Consolidação e Tipagem Forte

#### Pascal e Tipagem Forte

**Pascal** (1970), criado por **Niklaus Wirth**, enfatizava tipagem forte e verificação rigorosa:

```pascal
program TypeExample;
type
  Temperature = Real;
  Distance = Real;
  
var
  temp: Temperature;
  dist: Distance;
  
begin
  temp := 36.5;
  dist := 100.0;
  { temp := dist; - Erro conceitual, mesmo ambos sendo Real }
end.
```

**Princípios do Pascal**:
- **Tipagem forte**: Conversões explícitas obrigatórias
- **Tipos nomeados**: `type Temperature = Real` cria tipo distinto
- **Verificação em tempo de compilação**: Máxima detecção de erros antes da execução
- **Segurança**: Prevenir operações sem sentido

#### C e Tipagem Pragmática

**C** (1972), desenvolvido por **Dennis Ritchie**, adotou abordagem mais pragmática:

```c
/* C - Tipagem mais permissiva */
int x = 10;
float y = 3.14;
char c = 'A';
void *ptr;  /* Ponteiro genérico - tipo "escape hatch" */

ptr = &x;   /* OK: qualquer ponteiro pode ser atribuído a void* */
y = x;      /* OK: conversão implícita de int para float */
x = y;      /* OK: conversão implícita de float para int (com perda) */
```

**Filosofia do C**:
- **Confiança no programador**: Permitir conversões que podem ser inseguras
- **Eficiência**: Evitar overhead de verificações excessivas
- **Flexibilidade**: Manipulação de baixo nível quando necessário
- **Ponteiros void\***: "Escape hatch" para contornar o sistema de tipos

#### ML e Inferência de Tipos

**ML** (Meta Language, 1973), criado por **Robin Milner** na Universidade de Edimburgo, introduziu **inferência de tipos automática**:

```ml
(* ML - Tipos inferidos automaticamente *)
fun factorial n =
  if n = 0 then 1
  else n * factorial (n - 1)
  
(* O compilador infere: factorial : int -> int *)

fun map f [] = []
  | map f (x::xs) = (f x) :: (map f xs)
  
(* O compilador infere: map : ('a -> 'b) -> 'a list -> 'b list *)
```

**Algoritmo de Hindley-Milner** (1969-1978):
- **Inferência completa**: O compilador deduz todos os tipos automaticamente
- **Tipos polimórficos**: Funções podem operar em múltiplos tipos
- **Let-polymorphism**: Generalização de tipos em definições locais
- **Unificação**: Algoritmo baseado em unificação de termos

Esta foi uma descoberta revolucionária: combinar verificação estática forte com ergonomia de linguagens dinâmicas.

### Anos 1980: Orientação a Objetos e Subtipos

#### C++ e Tipos Orientados a Objetos

**C++** (1983), por **Bjarne Stroustrup**, adicionou orientação a objetos ao C:

```cpp
// C++ - Herança e polimorfismo
class Shape {
public:
    virtual double area() const = 0;  // Método virtual puro
    virtual ~Shape() {}
};

class Circle : public Shape {
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const override { return 3.14159 * radius * radius; }
};

class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double area() const override { return width * height; }
};

// Polimorfismo de subtipagem
void printArea(const Shape& shape) {
    std::cout << "Área: " << shape.area() << std::endl;
}

Circle c(5.0);
Rectangle r(4.0, 6.0);
printArea(c);  // OK: Circle é um subtipo de Shape
printArea(r);  // OK: Rectangle é um subtipo de Shape
```

**Conceitos introduzidos**:
- **Herança**: Relação "é-um" entre tipos
- **Polimorfismo de subtipagem**: Usar subtipos onde o supertipo é esperado
- **Métodos virtuais**: Despacho dinâmico baseado no tipo em tempo de execução
- **Templates** (adicionados depois): Polimorfismo paramétrico em C++

#### Sistemas de Tipos com Subtipos

**Barbara Liskov** formulou o **Princípio de Substituição de Liskov** (1987):

> "Se S é um subtipo de T, então objetos de tipo T podem ser substituídos por objetos de tipo S sem alterar as propriedades desejáveis do programa."

Este princípio formal define quando a relação de subtipagem é sã:
- **Covariância nos resultados**: Subtipo pode retornar tipo mais específico
- **Contravariância nos argumentos**: Subtipo pode aceitar tipo mais geral
- **Invariantes preservados**: Subtipo deve manter todas as garantias do supertipo

### Anos 1990-2000: Generics, Traits e Tipagem Gradual

#### Java e Generics

**Java** (1995) inicialmente não tinha generics, levando ao uso extensivo de `Object`:

```java
// Java pré-1.5 - Sem generics
List list = new ArrayList();
list.add("texto");
list.add(Integer.valueOf(42));
String s = (String) list.get(0);  // Cast era necessário, propenso a erros
```

**Java 5** (2004) introduziu **generics**:

```java
// Java 5+ - Com generics
List<String> list = new ArrayList<String>();
list.add("texto");
// list.add(42);  // ERRO DE COMPILAÇÃO
String s = list.get(0);  // Sem cast necessário
```

**Type erasure**: Java implementa generics com **erasure** — informações de tipo são removidas em tempo de execução, mantendo compatibilidade binária com código antigo.

#### Haskell e Type Classes

**Haskell** (1990) introduziu **type classes**, um mecanismo elegante para polimorfismo ad-hoc:

```haskell
-- Type class define interface
class Eq a where
  (==) :: a -> a -> Bool
  (/=) :: a -> a -> Bool

-- Instância para tipo específico
instance Eq Int where
  x == y = primEqInt x y
  x /= y = not (x == y)

-- Função polimórfica com restrição
elem :: Eq a => a -> [a] -> Bool
elem x [] = False
elem x (y:ys) = x == y || elem x ys
```

**Vantagens das type classes**:
- **Polimorfismo ad-hoc**: Diferentes implementações para diferentes tipos
- **Inferência**: Compilador deduz restrições automaticamente
- **Extensibilidade**: Novos tipos podem implementar type classes existentes
- **Composição**: Type classes podem ter dependências

### Anos 2010-presente: Tipagem Moderna

#### Rust e Ownership Types

**Rust** (2010) introduziu **ownership types** e **lifetime tracking**:

```rust
// Rust - Sistema de tipos com ownership
fn main() {
    let s1 = String::from("hello");
    let s2 = s1;  // s1 é "movido" para s2
    // println!("{}", s1);  // ERRO: s1 não pode mais ser usado
    
    let s3 = String::from("world");
    let len = calculate_length(&s3);  // Empresta s3 (borrow)
    println!("Length of '{}' is {}", s3, len);  // OK: s3 ainda é válido
}

fn calculate_length(s: &String) -> usize {  // Referência imutável
    s.len()
}  // s sai de escopo, mas não é dealocada (apenas emprestada)
```

**Conceitos do Rust**:
- **Ownership**: Cada valor tem exatamente um dono
- **Borrowing**: Referências temporárias (mutáveis ou imutáveis)
- **Lifetimes**: Anotações que garantem que referências são sempre válidas
- **Verificação em tempo de compilação**: Segurança de memória sem garbage collection

#### TypeScript e Tipagem Estrutural

**TypeScript** (2012) adicionou tipagem gradual opcional ao JavaScript:

```typescript
// TypeScript - Tipagem estrutural
interface Point {
  x: number;
  y: number;
}

function distance(p: Point): number {
  return Math.sqrt(p.x * p.x + p.y * p.y);
}

let p1 = { x: 3, y: 4, color: "red" };
console.log(distance(p1));  // OK: p1 tem pelo menos x e y
```

**Características**:
- **Tipagem estrutural**: Compatibilidade baseada em estrutura, não em nomes
- **Tipagem gradual**: Código JavaScript puro é TypeScript válido
- **Union types**: `string | number`
- **Intersection types**: `A & B`
- **Type guards**: Refinamento de tipos em runtime

### Linha do Tempo Resumida

| Ano | Linguagem | Inovação em Tipos |
|-----|-----------|-------------------|
| 1957 | FORTRAN | Primeiros tipos (INTEGER, REAL) |
| 1960 | ALGOL 60 | Declarações explícitas, procedimentos tipados |
| 1970 | Pascal | Tipagem forte, tipos nomeados |
| 1972 | C | Ponteiros tipados, conversões flexíveis |
| 1973 | ML | Inferência de tipos (Hindley-Milner) |
| 1983 | C++ | Herança, polimorfismo de subtipagem |
| 1990 | Haskell | Type classes, tipos de ordem superior |
| 1995 | Java | Subtipos nominais, verificação em tempo de execução |
| 2004 | Java 5 | Generics com type erasure |
| 2010 | Rust | Ownership types, lifetimes |
| 2012 | TypeScript | Tipagem estrutural gradual |

---

## Por Que Tipos Existem

A existência de sistemas de tipos não é acidental — eles surgiram como respostas a problemas fundamentais em computação e continuam evoluindo para enfrentar novos desafios.

### Motivação Histórica

#### Problema 1: Erros Catastróficos

Nos primeiros computadores, interpretar bits incorretamente causava:
- **Crashes imprevisíveis**: Tratar dados como endereço de memória
- **Corrupção de dados**: Sobrescrever memória aleatoriamente
- **Vulnerabilidades de segurança**: Buffer overflows, code injection

**Exemplo em Assembly (sem tipos)**:
```assembly
; Sem tipos, o programador deve lembrar que:
; R1 contém um endereço de memória
; R2 contém um número inteiro
ADD R3, R1, R2    ; Somar endereço + número?!
                  ; Resultado é nonsensico, mas aceito
```

**Com tipos**, o compilador rejeita operações inválidas:
```c
int* ptr = ...;
int num = 42;
int result = ptr + num;  // ERRO: operação inválida entre ponteiro e inteiro
```

#### Problema 2: Representações Diferentes

Diferentes tipos de dados requerem representações diferentes em hardware:

**Inteiros** (complemento de dois):
```
42 decimal = 00000000 00000000 00000000 00101010 (32 bits)
```

**Ponto flutuante IEEE 754**:
```
42.0 float = 01000010 00101000 00000000 00000000 (32 bits)
             [sign][exponent][mantissa]
```

**Interpretar 42 (int) como float**:
```
00000000 00000000 00000000 00101010 como float = 5.88e-44
```

Resultado completamente errado! Tipos garantem que valores sejam interpretados corretamente.

#### Problema 3: Operações com Custos Diferentes

Diferentes tipos têm diferentes custos operacionais:

| Operação | Inteiro 32-bit | Float 32-bit | Double 64-bit | Custo (ciclos) |
|----------|----------------|--------------|---------------|----------------|
| Adição   | ADD            | FADD         | DADD          | 1 / 3 / 5 |
| Multiplicação | MUL       | FMUL         | DMUL          | 3 / 5 / 8 |
| Divisão  | DIV            | FDIV         | DDIV          | 20 / 25 / 40 |

Compiladores usam informações de tipos para escolher instruções corretas e otimizar performance.

### Razões Fundamentais

#### 1. Correção: Prevenir Erros

**Princípio**: "Se o programa compila, está livre de certos erros."

Sistemas de tipos garantem propriedades como:
- **Type safety**: Programas bem-tipados não tentam operações inválidas
- **Memory safety**: Acesso à memória é sempre válido
- **Ausência de null pointer dereference**: Em linguagens com tipos não-nulos

**Teorema de Soundness (Solidez)**:
```
Se ⊢ e : T (expressão e tem tipo T)
então e ⇓ v (e avalia para algum valor v de tipo T)
ou e diverge (loop infinito)
mas NUNCA e resulta em "operação inválida"
```

Este teorema, provado formalmente para muitas linguagens, garante que programas bem-tipados não "ficam presos" (stuck) em estados inválidos.

#### 2. Abstração: Encapsular Detalhes

Tipos permitem **abstração de dados** — esconder detalhes de implementação:

```typescript
// TypeScript - Interface abstrata
interface Stack<T> {
  push(item: T): void;
  pop(): T | undefined;
  isEmpty(): boolean;
}

// Usuários da Stack não precisam saber como é implementada
// (array, lista ligada, etc.)
```

**Benefícios**:
- **Modularidade**: Componentes podem ser desenvolvidos independentemente
- **Manutenibilidade**: Implementação pode mudar sem afetar clientes
- **Reutilização**: Código genérico funciona com múltiplos tipos

#### 3. Documentação: Comunicar Intenções

Tipos são **documentação executável** e **verificável**:

```haskell
-- Haskell - Assinatura de tipo documenta comportamento
quicksort :: Ord a => [a] -> [a]
```

Esta assinatura comunica:
- **Entrada**: Uma lista de elementos do tipo `a`
- **Restrição**: `a` deve ser comparável (type class `Ord`)
- **Saída**: Uma lista de elementos do tipo `a`
- **Garantia**: Mesmos tipos na entrada e saída

Diferente de comentários, tipos são **verificados pelo compilador** — nunca ficam desatualizados.

#### 4. Otimização: Geração de Código Eficiente

Informações de tipos permitem otimizações agressivas:

**Exemplo 1: Especialização de código**
```rust
// Rust - Monomorphization
fn add<T: Add<Output=T>>(a: T, b: T) -> T {
    a + b
}

let x = add(5, 10);      // Compilador gera versão especializada para i32
let y = add(5.0, 10.0);  // Compilador gera versão especializada para f64
```

Cada versão especializada usa instruções nativas ótimas para o tipo.

**Exemplo 2: Eliminação de verificações**
```java
// Java - Com tipos, verificações podem ser eliminadas
List<Integer> numbers = new ArrayList<>();
numbers.add(42);
int x = numbers.get(0);  // Sem cast, sem verificação de tipo em runtime
```

**Exemplo 3: Representação eficiente**
```c
// C - Tipos determinam layout de memória
struct Point {
    float x;  // 4 bytes
    float y;  // 4 bytes
};  // Total: 8 bytes, alinhamento conhecido em tempo de compilação
```

#### 5. Segurança: Prevenir Vulnerabilidades

Sistemas de tipos são cruciais para segurança:

**Buffer Overflow Prevention**:
```rust
// Rust - Acesso a arrays é verificado
let arr = [1, 2, 3, 4, 5];
let x = arr[10];  // PÂNICO EM RUNTIME (verificação inserida pelo compilador)
```

Vs. C sem verificação:
```c
// C - Acesso sem verificação
int arr[5] = {1, 2, 3, 4, 5};
int x = arr[10];  // COMPORTAMENTO INDEFINIDO - pode executar código malicioso
```

#### 6. Manutenibilidade: Facilitar Evolução

Tipos facilitam refatorações seguras:

**Cenário**: Mudar tipo de um campo em uma estrutura grande.

**Com tipos estáticos**:
```java
class User {
    // int id;          // Antes
    UUID id;            // Depois
}

// Compilador DETECTA TODOS OS USOS que precisam ser atualizados
```

**Sem tipos estáticos**:
```python
class User:
    def __init__(self, id):
        self.id = id  # int? str? UUID? Não sabemos!

# Erros só aparecem em execução, e podem estar em caminhos raramente testados
```

### Conclusão: Tipos Existem Para...

Sistemas de tipos existem fundamentalmente para **aumentar confiabilidade e produtividade** do software:

1. **Confiabilidade**: Detectar erros cedo, prevenir vulnerabilidades
2. **Performance**: Otimizações baseadas em tipos
3. **Manutenibilidade**: Evolução segura de código
4. **Comunicação**: Documentação verificável
5. **Abstração**: Modularidade e reutilização

A tendência histórica é clara: linguagens sem tipos (Assembly) → tipagem fraca (C) → tipagem forte (Java) → tipos avançados (Rust, Haskell). Cada passo adiciona mais garantias em tempo de compilação.

---

## Classificação de Sistemas de Tipos

Os sistemas de tipos podem ser classificados segundo diversas dimensões ortogonais, cada uma refletindo decisões de design fundamentais na criação de linguagens de programação.

### Tipagem Estática vs Dinâmica

#### Tipagem Estática

**Definição**: Tipos são verificados em **tempo de compilação** antes do programa ser executado.

**Linguagens**: Java, C, C++, Rust, Haskell, TypeScript, Go

**Características**:
- Todos os tipos devem ser conhecidos ou inferíveis antes da execução
- Erros de tipo são detectados pelo compilador
- Possibilita otimizações agressivas

**Exemplo (Java)**:
```java
// Java - Tipagem estática
int x = 10;
String s = "texto";
// x = s;  // ERRO DE COMPILAÇÃO: incompatibilidade de tipos
```

**Vantagens**:
- **Detecção precoce de erros**: Bugs detectados antes de rodar o programa
- **Melhor performance**: Compilador pode otimizar baseado em tipos
- **Ferramentas melhores**: IDEs podem oferecer autocompletar, refatoração segura
- **Documentação**: Assinaturas de tipos servem como contratos

**Desvantagens**:
- **Verbosidade**: Pode exigir muitas anotações de tipo
- **Rigidez**: Alguns padrões dinâmicos são difíceis de expressar
- **Curva de aprendizado**: Sistemas de tipos complexos podem ser intimidadores

#### Tipagem Dinâmica

**Definição**: Tipos são verificados em **tempo de execução** quando operações são executadas.

**Linguagens**: Python, JavaScript, Ruby, Lisp, Smalltalk

**Características**:
- Variáveis não têm tipos fixos; valores têm tipos
- Erros de tipo aparecem durante execução
- Maior flexibilidade em trocar tipos

**Exemplo (Python)**:
```python
# Python - Tipagem dinâmica
x = 10        # x é int
x = "texto"   # Agora x é str - OK!
# print(x + 5)  # ERRO EM RUNTIME: não pode somar str e int
```

**Vantagens**:
- **Prototipação rápida**: Menos cerimônia, desenvolvimento mais ágil
- **Flexibilidade**: Metaprogramação e reflexão são mais fáceis
- **Simplicidade**: Menos conceitos para aprender inicialmente

**Desvantagens**:
- **Erros em produção**: Bugs de tipo só aparecem quando código é executado
- **Performance**: Verificações em runtime têm overhead
- **Manutenção**: Refatorações são mais arriscadas
- **Tooling**: IDEs têm menos informações para auxiliar

#### Tipagem Gradual

**Tipagem gradual** combina aspectos de tipagem estática e dinâmica, permitindo que desenvolvedores escolham onde aplicar verificação estática.

**Linguagens**: TypeScript, Python (com mypy), Dart, Hack (Facebook)

**Exemplo (TypeScript)**:
```typescript
// TypeScript - Tipagem gradual
let x: number = 10;       // Tipado estaticamente
let y: any = "texto";     // any - bypass do sistema de tipos
y = 42;                    // OK
x = y;                     // OK, mas potencialmente inseguro

// Inferência automática
let z = 100;               // z inferido como number
// z = "texto";            // ERRO: tipo inferido impede mudança
```

**Vantagens**:
- **Adoção incremental**: Adicionar tipos gradualmente a código existente
- **Flexibilidade quando necessária**: Bypass do sistema de tipos com `any`
- **Migração**: Facilita transição de linguagens dinâmicas para estáticas

### Tipagem Forte vs Fraca

#### Tipagem Forte

**Definição**: A linguagem **não permite** operações entre tipos incompatíveis sem conversão explícita.

**Linguagens**: Python, Java, Haskell, Rust

**Exemplo (Python)**:
```python
# Python - Tipagem forte
x = 5
s = "texto"
# print(x + s)  # ERRO: não pode somar int e str sem conversão explícita
print(str(x) + s)  # OK: "5texto"
```

**Características**:
- **Conversões explícitas**: Programador deve converter tipos manualmente
- **Segurança**: Previne operações sem sentido
- **Clareza**: Intenção do programador é explícita

#### Tipagem Fraca

**Definição**: A linguagem permite ou **força conversões implícitas** (coerção) entre tipos.

**Linguagens**: C, JavaScript, PHP, Perl

**Exemplo (JavaScript)**:
```javascript
// JavaScript - Tipagem fraca (coerção agressiva)
let x = 5;
let s = "texto";
console.log(x + s);       // "5texto" - number coagido para string
console.log(s - x);       // NaN - string coagida para number (falha)
console.log("5" - 2);     // 3 - string coagida para number
console.log("5" + 2);     // "52" - number coagido para string
```

**Problemas da tipagem fraca**:
- **Comportamento imprevisível**: Regras de coerção podem ser complexas
- **Bugs sutis**: Conversões implícitas podem esconder erros
- **Dificuldade de raciocínio**: Difícil prever resultado de expressões

### Tipagem Nominal vs Estrutural

#### Tipagem Nominal

**Definição**: Compatibilidade de tipos é baseada em **nomes**. Dois tipos são compatíveis apenas se tiverem o mesmo nome (ou se há relação de herança explícita).

**Linguagens**: Java, C++, C#

**Exemplo (Java)**:
```java
// Java - Tipagem nominal
class Celsius {
    double value;
    Celsius(double v) { value = v; }
}

class Fahrenheit {
    double value;
    Fahrenheit(double v) { value = v; }
}

// Estruturas idênticas, mas tipos incompatíveis!
Celsius c = new Celsius(25.0);
// Fahrenheit f = c;  // ERRO: tipos nominalmente diferentes
```

**Vantagens**:
- **Intenção clara**: Tipos diferentes mesmo com estrutura similar
- **Segurança**: Previne confusão acidental entre tipos similares
- **Performance**: Verificação de tipos mais rápida

#### Tipagem Estrutural

**Definição**: Compatibilidade de tipos é baseada na **estrutura**. Dois tipos são compatíveis se tiverem membros compatíveis, independente dos nomes.

**Linguagens**: TypeScript, Go (interfaces), OCaml, ML

**Exemplo (TypeScript)**:
```typescript
// TypeScript - Tipagem estrutural
interface Point2D {
    x: number;
    y: number;
}

interface Vector2D {
    x: number;
    y: number;
}

function distance(p: Point2D): number {
    return Math.sqrt(p.x * p.x + p.y * p.y);
}

let v: Vector2D = { x: 3, y: 4 };
console.log(distance(v));  // OK: Vector2D compatível estruturalmente com Point2D
```

**Vantagens**:
- **Duck typing estático**: "Se parece com pato e grasna como pato..."
- **Flexibilidade**: Código funciona com qualquer estrutura compatível
- **Interoperabilidade**: Mais fácil integrar bibliotecas

**Desvantagens**:
- **Erros acidentais**: Tipos conceitualmente diferentes podem ser acidentalmente compatíveis
- **Performance**: Verificação pode ser mais custosa

### Tipagem Explícita vs Inferida

#### Tipagem Explícita (Manifesta)

**Definição**: O programador deve **declarar explicitamente** os tipos.

**Linguagens**: C, Java (pré-10), Go

**Exemplo (Java)**:
```java
// Java - Tipos explícitos
public int soma(int a, int b) {
    int resultado = a + b;
    return resultado;
}
```

**Vantagens**:
- **Documentação**: Tipos servem como documentação local
- **Clareza**: Intenção sempre explícita
- **Simplicidade do compilador**: Verificação mais direta

#### Tipagem Inferida (Implícita)

**Definição**: O compilador **deduz automaticamente** os tipos.

**Linguagens**: Haskell, OCaml, Rust, F#, Kotlin

**Exemplo (Rust)**:
```rust
// Rust - Tipos inferidos
fn soma(a: i32, b: i32) -> i32 {  // Tipos nos parâmetros são necessários
    let resultado = a + b;         // Tipo de `resultado` inferido como i32
    resultado                      // Tipo de retorno verificado
}
```

**Algoritmo de Hindley-Milner**:
```ocaml
(* OCaml - Inferência completa *)
let rec map f = function
  | [] -> []
  | x::xs -> (f x) :: (map f xs)
  
(* Compilador infere: ('a -> 'b) -> 'a list -> 'b list *)
```

**Vantagens**:
- **Concisão**: Menos verbosidade, código mais limpo
- **Manutenção**: Mudanças de tipo se propagam automaticamente
- **Ergonomia**: Menos anotações para escrever

**Desvantagens**:
- **Mensagens de erro**: Podem ser menos claras quando inferência falha
- **Leitura**: Às vezes difícil entender tipos sem anotações

### Tipagem Segura (Type-Safe) vs Insegura

#### Tipagem Segura

**Definição**: A linguagem garante que programas bem-tipados **nunca terão erros de tipo em execução**.

**Linguagens**: Java, Haskell, Rust, ML

**Propriedades garantidas**:
- **Progress**: Programas não ficam "presos" em estados inválidos
- **Preservation**: Tipos são preservados durante execução
- **Memory safety**: Acesso à memória é sempre válido

#### Tipagem Insegura

**Definição**: A linguagem permite **bypasses** do sistema de tipos ou não garante ausência de erros.

**Linguagens**: C (unsafe), C++ (casts), Java (reflection)

**Exemplo (C - comportamento indefinido)**:
```c
// C - Tipagem insegura
int x = 42;
float *f = (float*)&x;  // Cast forçado
printf("%f\n", *f);     // Interpreta int como float - INDEFINIDO
```

### Polimorfismo e Tipos

#### Monomorfismo

**Um tipo, uma implementação**.

```java
// Java - Monomorphic
int soma(int a, int b) {
    return a + b;
}
```

#### Polimorfismo Paramétrico (Generics)

**Uma implementação, múltiplos tipos**.

```java
// Java - Polimorfismo paramétrico
<T> T primeiro(List<T> lista) {
    return lista.get(0);
}
```

#### Polimorfismo Ad-hoc (Overloading)

**Múltiplas implementações, mesmo nome**.

```java
// Java - Overloading
int soma(int a, int b) { return a + b; }
double soma(double a, double b) { return a + b; }
String soma(String a, String b) { return a + b; }
```

#### Polimorfismo de Subtipagem (Herança)

**Subtipos podem ser usados onde supertipos são esperados**.

```java
// Java - Polimorfismo de subtipagem
void processar(Animal animal) {
    animal.emitirSom();
}

processar(new Cachorro());  // OK: Cachorro é subtipo de Animal
processar(new Gato());      // OK: Gato é subtipo de Animal
```

### Tabela Comparativa

| Característica | Estático | Dinâmico |
|----------------|----------|----------|
| **Verificação** | Compilação | Execução |
| **Exemplos** | Java, Rust, Haskell | Python, JavaScript, Ruby |
| **Performance** | Alta (otimizações) | Média (overhead) |
| **Segurança** | Alta (erros cedo) | Média (erros tarde) |
| **Flexibilidade** | Média | Alta |

| Característica | Forte | Fraca |
|----------------|-------|-------|
| **Conversões** | Explícitas | Implícitas (coerção) |
| **Exemplos** | Python, Rust, Haskell | C, JavaScript, PHP |
| **Segurança** | Alta | Baixa (bugs sutis) |
| **Previsibilidade** | Alta | Baixa |

| Característica | Nominal | Estrutural |
|----------------|---------|------------|
| **Compatibilidade** | Por nome | Por estrutura |
| **Exemplos** | Java, C# | TypeScript, Go (interfaces) |
| **Intencionalidade** | Explícita | Implícita |
| **Flexibilidade** | Baixa | Alta |

---

## Padrões IEEE e Representação de Tipos

Os padrões **IEEE** (Institute of Electrical and Electronics Engineers) definem representações padronizadas para tipos de dados, especialmente cruciais para **ponto flutuante** e **aritmética de precisão**.

### IEEE 754: Aritmética de Ponto Flutuante

O padrão **IEEE 754** (1985, revisado em 2008 e 2019) define a representação e operações para números de ponto flutuante, usado universalmente em processadores modernos.

#### Formato de Precisão Simples (32 bits - float)

```
├─────┬──────────┬───────────────────────┐
│  S  │ Expoente │      Mantissa         │
├─────┼──────────┼───────────────────────┤
│ 1bit│  8 bits  │      23 bits          │
└─────┴──────────┴───────────────────────┘

Valor = (-1)^S × 1.Mantissa × 2^(Expoente-127)
```

**Exemplo**: Representar 42.5 em float IEEE 754

```
42.5 em binário = 101010.1
Normalizado: 1.010101 × 2^5

S (sinal) = 0 (positivo)
Expoente = 5 + 127 = 132 = 10000100
Mantissa = 01010100000000000000000

Representação completa:
0 10000100 01010100000000000000000
```

#### Formato de Precisão Dupla (64 bits - double)

```
├─────┬──────────┬────────────────────────────────┐
│  S  │ Expoente │         Mantissa               │
├─────┼──────────┼────────────────────────────────┤
│ 1bit│ 11 bits  │         52 bits                │
└─────┴──────────┴────────────────────────────────┘

Valor = (-1)^S × 1.Mantissa × 2^(Expoente-1023)
```

#### Valores Especiais IEEE 754

| Valor | Expoente | Mantissa | Significado |
|-------|----------|----------|-------------|
| Zero positivo | 0 | 0 | +0.0 |
| Zero negativo | 0 | 0 (S=1) | -0.0 |
| Infinity | 11..1 | 0 | ±∞ |
| NaN (Not a Number) | 11..1 | ≠0 | Indefinido (0/0, ∞-∞, √-1) |
| Subnormal | 0 | ≠0 | Números muito pequenos |

**Exemplo em C**:
```c
#include <stdio.h>
#include <math.h>

int main() {
    float a = 1.0 / 0.0;        // +INF
    float b = -1.0 / 0.0;       // -INF
    float c = 0.0 / 0.0;        // NaN
    float d = sqrt(-1.0);       // NaN
    
    printf("a = %f (isinf: %d)\n", a, isinf(a));  // inf
    printf("c = %f (isnan: %d)\n", c, isnan(c));  // nan
    
    return 0;
}
```

#### Operações e Arredondamento

IEEE 754 define **cinco modos de arredondamento**:

1. **Round to nearest, ties to even** (padrão)
2. **Round toward +∞**
3. **Round toward -∞**
4. **Round toward zero (truncate)**
5. **Round to nearest, ties away from zero**

**Exemplo de arredondamento**:
```
Valor exato: 1.0000000000000002
Round to nearest: 1.0
Round to +∞: 1.0000000000000003
Round to -∞: 1.0
```

### IEEE 1003 (POSIX): Tipos de Dados Inteiros

O padrão **POSIX** define tipos de inteiros portáveis:

```c
#include <stdint.h>

// Tipos de tamanho exato
int8_t    i8;   // Exatamente 8 bits com sinal
uint8_t   u8;   // Exatamente 8 bits sem sinal
int16_t   i16;  // Exatamente 16 bits com sinal
uint16_t  u16;  // Exatamente 16 bits sem sinal
int32_t   i32;  // Exatamente 32 bits com sinal
uint32_t  u32;  // Exatamente 32 bits sem sinal
int64_t   i64;  // Exatamente 64 bits com sinal
uint64_t  u64;  // Exatamente 64 bits sem sinal

// Tipos de tamanho mínimo
int_least8_t   il8;   // Pelo menos 8 bits
int_least16_t  il16;  // Pelo menos 16 bits
int_least32_t  il32;  // Pelo menos 32 bits
int_least64_t  il64;  // Pelo menos 64 bits

// Tipos mais rápidos
int_fast8_t    if8;   // Mais rápido com pelo menos 8 bits
int_fast16_t   if16;  // Mais rápido com pelo menos 16 bits
int_fast32_t   if32;  // Mais rápido com pelo menos 32 bits
int_fast64_t   if64;  // Mais rápido com pelo menos 64 bits

// Tipos para ponteiros
intptr_t   iptr;  // Inteiro que pode conter um ponteiro
uintptr_t  uptr;  // Unsigned inteiro que pode conter um ponteiro

// Tipo de tamanho máximo
intmax_t   imax;  // Maior tipo inteiro disponível
uintmax_t  umax;  // Maior tipo inteiro sem sinal disponível
```

### Representação de Inteiros

#### Complemento de Dois (Two's Complement)

Padrão universal para representar inteiros com sinal:

```
int8_t (8 bits):
  Faixa: -128 a 127
  
  +5  = 00000101
  -5  = 11111011  (complemento de um: 11111010, + 1)
  
  +127 = 01111111  (maior positivo)
  -128 = 10000000  (menor negativo)
```

**Vantagens do complemento de dois**:
- **Zero único**: Apenas uma representação de zero
- **Aritmética simples**: Soma e subtração usam mesma circuitaria
- **Extensão de sinal**: Fácil converter para tipos maiores

**Exemplo de overflow**:
```c
int8_t a = 127;
a = a + 1;  // Overflow: resultado é -128
```

### Alinhamento e Padding

Compiladores inserem **padding** para alinhar dados segundo requisitos da arquitetura:

```c
struct Example1 {
    char c;      // 1 byte
    // 3 bytes de padding
    int i;       // 4 bytes (requer alinhamento de 4)
    char c2;     // 1 byte
    // 3 bytes de padding
};  // Total: 12 bytes (não 6!)

struct Example2 {
    int i;       // 4 bytes
    char c;      // 1 byte
    char c2;     // 1 byte
    // 2 bytes de padding
};  // Total: 8 bytes

// Reordenar campos pode economizar memória!
```

**Regras de alinhamento** (tipicamente):
- `char` (1 byte): alinhamento de 1
- `short` (2 bytes): alinhamento de 2
- `int` (4 bytes): alinhamento de 4
- `long` (8 bytes em 64-bit): alinhamento de 8
- `float` (4 bytes): alinhamento de 4
- `double` (8 bytes): alinhamento de 8
- Ponteiros: alinhamento de 4 (32-bit) ou 8 (64-bit)

### Endianness (Ordem de Bytes)

Ordem em que bytes são armazenados na memória:

#### Big-Endian

Byte **mais significativo** primeiro (redes, alguns processadores).

```
Valor: 0x12345678

Endereço   Byte
0x1000     0x12  (MSB)
0x1001     0x34
0x1002     0x56
0x1003     0x78  (LSB)
```

**Arquiteturas**: SPARC, PowerPC (mode), redes (network byte order)

#### Little-Endian

Byte **menos significativo** primeiro (x86, ARM).

```
Valor: 0x12345678

Endereço   Byte
0x1000     0x78  (LSB)
0x1001     0x56
0x1002     0x34
0x1003     0x12  (MSB)
```

**Arquiteturas**: x86, x86-64, ARM (usual)

**Detecção em C**:
```c
int is_little_endian() {
    int x = 1;
    char *c = (char*)&x;
    return (*c == 1);  // true se little-endian
}
```

### Impacto no Compilador

Compiladores devem:

1. **Respeitar representações IEEE 754** para operações de ponto flutuante
2. **Gerenciar alinhamento** para performance e correção
3. **Lidar com endianness** em conversões e comunicação
4. **Detectar overflow** ou gerar código para tratamento
5. **Otimizar** baseado em propriedades dos tipos
6. **Gerar código portável** que funciona em diferentes arquiteturas

**Exemplo de código gerado (x86-64)**:
```c
// Código C
float soma(float a, float b) {
    return a + b;
}

// Assembly gerado (x86-64)
soma:
    addss   xmm0, xmm1    ; Soma SSE de precisão simples
    ret
```

```c
// Código C
double soma_double(double a, double b) {
    return a + b;
}

// Assembly gerado (x86-64)
soma_double:
    addsd   xmm0, xmm1    ; Soma SSE de precisão dupla
    ret
```

Diferentes tipos geram diferentes instruções! O sistema de tipos guia a geração de código.

---

## Fundamentos Teóricos de Tipos

### Lambda Calculus e Tipagem

O **cálculo lambda** fornece a base matemática para sistemas de tipos.

#### Simply Typed Lambda Calculus (λ→)

**Sintaxe de tipos**:
```
τ ::= B              (tipo base: int, bool, etc.)
    | τ₁ → τ₂        (tipo função)
```

**Regras de tipagem**:
```
────────────── (VAR)
Γ, x:τ ⊢ x : τ


Γ, x:τ₁ ⊢ e : τ₂
─────────────────── (ABS)
Γ ⊢ λx.e : τ₁ → τ₂


Γ ⊢ e₁ : τ₁ → τ₂    Γ ⊢ e₂ : τ₁
──────────────────────────── (APP)
Γ ⊢ e₁ e₂ : τ₂
```

**Exemplo de derivação**:
```
Provar que λx.λy.x : int → bool → int

1. Assumir Γ = {x:int, y:bool}
2. Por (VAR), Γ ⊢ x : int
3. Por (ABS), Γ - {y:bool} ⊢ λy.x : bool → int
4. Por (ABS), ∅ ⊢ λx.λy.x : int → bool → int
```

### Curry-Howard Correspondence

**Correspondência fundamental entre tipos e lógica**:

| Lógica | Tipos | Programação |
|--------|-------|-------------|
| A ⊃ B | A → B | Função |
| A ∧ B | A × B | Par (produto) |
| A ∨ B | A + B | União (soma) |
| ⊤ | Unit | Tipo unitário () |
| ⊥ | Void | Tipo vazio (impossível construir) |
| ∀x.P(x) | ∀α.τ | Polimorfismo universal |

**Exemplo**:
```haskell
-- Proposição: A ∧ B ⊃ A
-- Tipo: (A, B) -> A
-- Prova construtiva:
fst :: (a, b) -> a
fst (x, y) = x
```

Este programa **prova** que se temos A e B, então temos A.

### System F (Polimorfismo Paramétrico)

**System F** estende λ→ com **quantificação universal**:

```
τ ::= α                    (variável de tipo)
    | τ₁ → τ₂              (tipo função)
    | ∀α.τ                 (tipo polimórfico)
    
e ::= x                    (variável)
    | λx:τ.e              (abstração)
    | e₁ e₂                (aplicação)
    | Λα.e                 (abstração de tipo)
    | e [τ]                (aplicação de tipo)
```

**Exemplo**:
```
identidade polimórfica:
id = Λα. λx:α. x
tipo: ∀α. α → α

Uso:
id [Int] 5          // aplica identidade a Int, depois a 5
id [Bool] true      // aplica identidade a Bool, depois a true
```

---

## Inferência de Tipos

### Algoritmo de Hindley-Milner

O **algoritmo de Hindley-Milner** (HM) permite inferência automática de tipos polimórficos.

#### Processo de Inferência

1. **Atribuir variáveis de tipo** a todas as subexpressões
2. **Gerar restrições** baseadas nas regras de tipagem
3. **Unificar** as restrições para encontrar tipos mais gerais
4. **Generalizar** para obter tipos polimórficos

**Exemplo passo a passo**:

```ocaml
let f x = x + 1
```

**Passo 1: Atribuir variáveis**
```
f : α
x : β
(+) : int → int → int
1 : int
```

**Passo 2: Gerar restrições**
```
x + 1 requer:
  x : int        (operando esquerdo de +)
  1 : int        (operando direito de +)
  resultado : int

Logo:
  β = int
  α = int → int
```

**Passo 3: Unificação**
```
Resolvendo:
  β = int
  α = int → int
```

**Resultado**: `f : int → int`

**Exemplo com polimorfismo**:
```ocaml
let id x = x
```

```
id : α
x : β
Restrição: tipo de x = tipo do retorno
β = β (sempre verdadeiro)

Generalização:
id : ∀α. α → α
```

#### Limitações do Hindley-Milner

**Não infere**:
- **Polimorfismo de subtipagem**: Requer anotações
- **Tipos de ordem superior**: Requer System F ou extensions
- **Tipos dependentes**: Requer linguagens como Idris, Agda

---

## Polimorfismo e Generics

### Tipos de Polimorfismo

#### 1. Polimorfismo Paramétrico (Generics)

**Uma implementação, múltiplos tipos**.

```java
// Java - Generics
class Box<T> {
    private T value;
    
    public void set(T value) {
        this.value = value;
    }
    
    public T get() {
        return value;
    }
}

Box<Integer> intBox = new Box<>();
intBox.set(42);
int x = intBox.get();  // Sem cast!

Box<String> strBox = new Box<>();
strBox.set("texto");
String s = strBox.get();
```

#### 2. Polimorfismo Ad-hoc (Overloading)

**Múltiplas implementações, mesmo nome**.

```cpp
// C++ - Overloading
int max(int a, int b) {
    return (a > b) ? a : b;
}

double max(double a, double b) {
    return (a > b) ? a : b;
}

std::string max(std::string a, std::string b) {
    return (a > b) ? a : b;
}
```

#### 3. Polimorfismo de Subtipagem

**Subtipos podem ser usados onde supertipos são esperados**.

```java
// Java - Herança
interface Animal {
    void emitirSom();
}

class Cachorro implements Animal {
    public void emitirSom() {
        System.out.println("Au au!");
    }
}

class Gato implements Animal {
    public void emitirSom() {
        System.out.println("Miau!");
    }
}

void processar(Animal animal) {
    animal.emitirSom();  // Despacho dinâmico
}
```

### Variância em Tipos Genéricos

#### Covariância

**Subtipo preserva ordem**: Se `Dog <: Animal`, então `List<Dog> <: List<Animal>`?

```java
// Java - Arrays são covariantes (INSEGURO!)
Animal[] animals = new Dog[10];
animals[0] = new Cat();  // ERRO EM RUNTIME! ArrayStoreException
```

#### Contravariância

**Subtipo inverte ordem**: Se `Dog <: Animal`, então `Consumer<Animal> <: Consumer<Dog>`?

```java
// Java - Contravariância com wildcards
Consumer<Animal> animalConsumer = (animal) -> animal.emitirSom();
Consumer<Dog> dogConsumer = animalConsumer;  // OK com ? super
```

#### Invariância

**Sem relação de subtipagem**: `List<Dog>` e `List<Animal>` são incompatíveis.

```java
// Java - Generics são invariantes (SEGURO)
List<Dog> dogs = new ArrayList<>();
// List<Animal> animals = dogs;  // ERRO DE COMPILAÇÃO
```

---

## Sistemas de Tipos Avançados

### Tipos Dependentes

**Tipos que dependem de valores**.

```idris
-- Idris - Tipos dependentes
data Vect : Nat -> Type -> Type where
  Nil  : Vect 0 a
  (::) : a -> Vect n a -> Vect (S n) a

-- Tipo garante que os vetores têm mesmo tamanho
append : Vect n a -> Vect m a -> Vect (n + m) a
append Nil       ys = ys
append (x :: xs) ys = x :: append xs ys
```

**Aplicações**:
- **Verificação formal**: Provar propriedades de programas
- **Sistemas críticos**: Software para aviação, medicina
- **Segurança**: Garantir ausência de bugs em nível de tipo

### Linear Types

**Cada valor usado exatamente uma vez**.

```rust
// Rust - Linear types via ownership
let s = String::from("hello");
let s2 = s;  // s é "movido" (linear)
// println!("{}", s);  // ERRO: s foi consumido
```

**Benefícios**:
- **Gerenciamento de recursos**: Arquivos, conexões de rede
- **Segurança**: Prevenir use-after-free
- **Concorrência**: Evitar data races

### Refinement Types

**Tipos com predicados lógicos**.

```haskell
-- Liquid Haskell
{-@ type Pos = {v:Int | v > 0} @-}
{-@ type Nat = {v:Int | v >= 0} @-}

{-@ divide :: Int -> Pos -> Int @-}
divide :: Int -> Int -> Int
divide x y = x `div` y  -- Garantido que y > 0
```

---

## Implementação em Compiladores

### Representação de Tipos em Tempo de Compilação

```c
// C - Estrutura de dados para tipos
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_STRUCT,
    TYPE_FUNCTION,
    TYPE_POINTER,
    TYPE_VOID
} TypeKind;

typedef struct Type {
    TypeKind kind;
    union {
        struct {  // Arrays
            struct Type* elem_type;
            int size;
        } array;
        
        struct {  // Funções
            struct Type* return_type;
            struct Type** param_types;
            int num_params;
        } function;
        
        struct {  // Ponteiros
            struct Type* pointed_type;
        } pointer;
        
        struct {  // Estruturas
            char** field_names;
            struct Type** field_types;
            int num_fields;
        } structure;
    } data;
} Type;
```

### Verificação de Tipos

```c
// Verificação de compatibilidade de tipos
bool types_compatible(Type* t1, Type* t2) {
    if (t1->kind != t2->kind) return false;
    
    switch (t1->kind) {
        case TYPE_INT:
        case TYPE_FLOAT:
        case TYPE_BOOL:
            return true;  // Tipos primitivos: comparação por kind
            
        case TYPE_ARRAY:
            return t1->data.array.size == t2->data.array.size &&
                   types_compatible(t1->data.array.elem_type,
                                   t2->data.array.elem_type);
                                   
        case TYPE_FUNCTION:
            if (t1->data.function.num_params != t2->data.function.num_params)
                return false;
            if (!types_compatible(t1->data.function.return_type,
                                 t2->data.function.return_type))
                return false;
            for (int i = 0; i < t1->data.function.num_params; i++) {
                if (!types_compatible(t1->data.function.param_types[i],
                                     t2->data.function.param_types[i]))
                    return false;
            }
            return true;
            
        case TYPE_POINTER:
            return types_compatible(t1->data.pointer.pointed_type,
                                   t2->data.pointer.pointed_type);
        
        default:
            return false;
    }
}
```

### Type Checking de Expressões

```c
// Type checking de uma expressão binária
Type* check_binary_expr(BinaryExpr* expr, SymbolTable* symtab) {
    Type* left_type = check_expr(expr->left, symtab);
    Type* right_type = check_expr(expr->right, symtab);
    
    switch (expr->op) {
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
            // Operações aritméticas: int ou float
            if ((left_type->kind == TYPE_INT || left_type->kind == TYPE_FLOAT) &&
                (right_type->kind == TYPE_INT || right_type->kind == TYPE_FLOAT)) {
                // Promoção: int + float = float
                if (left_type->kind == TYPE_FLOAT || right_type->kind == TYPE_FLOAT)
                    return make_float_type();
                return make_int_type();
            }
            error("Operação aritmética com tipos incompatíveis");
            return make_error_type();
            
        case OP_LT:
        case OP_GT:
        case OP_LE:
        case OP_GE:
            // Operações relacionais: retornam bool
            if ((left_type->kind == TYPE_INT || left_type->kind == TYPE_FLOAT) &&
                (right_type->kind == TYPE_INT || right_type->kind == TYPE_FLOAT)) {
                return make_bool_type();
            }
            error("Comparação entre tipos incompatíveis");
            return make_error_type();
            
        case OP_EQ:
        case OP_NE:
            // Igualdade: tipos devem ser compatíveis
            if (types_compatible(left_type, right_type)) {
                return make_bool_type();
            }
            error("Comparação de igualdade entre tipos incompatíveis");
            return make_error_type();
            
        case OP_AND:
        case OP_OR:
            // Operações lógicas: apenas bool
            if (left_type->kind == TYPE_BOOL && right_type->kind == TYPE_BOOL) {
                return make_bool_type();
            }
            error("Operador lógico requer operandos booleanos");
            return make_error_type();
            
        default:
            error("Operador desconhecido");
            return make_error_type();
    }
}
```

---

## Verificação de Tipos

### Type Soundness (Solidez)

Um sistema de tipos é **sólido** (sound) se satisfaz:

**Teorema (Progress)**:
```
Se ⊢ e : τ então:
  - e é um valor, OU
  - existe e' tal que e → e'
```

**Teorema (Preservation)**:
```
Se ⊢ e : τ e e → e' então ⊢ e' : τ
```

**Corolário (Type Safety)**:
```
Programas bem-tipados não ficam "presos" (stuck) em estados inválidos
```

### Completude

Um sistema é **completo** (complete) se aceita todos os programas corretos.

**Trade-off**: Sistemas sólidos são necessariamente **incompletos** — rejeitam alguns programas corretos para garantir ausência de erros.

**Exemplo**:
```java
// Programa correto mas rejeitado
if (false) {
    String s = null;
    int len = s.length();  // Nunca executado, mas erro de compilação
}
```

---

## Desafios e Limitações

### 1. Decidibilidade

**Inferência de tipos é indecidível** em sistemas poderosos (System F, tipos dependentes).

**Consequências**:
- Linguagens podem exigir anotações de tipo
- Compiladores podem ter timeouts
- Mensagens de erro podem ser obscuras

### 2. Expressividade vs Segurança

**Trade-off fundamental**:
- Sistemas mais expressivos são mais difíceis de verificar
- Sistemas mais restritivos rejeitam mais programas válidos

### 3. Performance de Compilação

**Type checking pode ser custoso**:
- Templates C++: compilação lenta
- Tipos dependentes: verificação pode ser arbitrariamente complexa

### 4. Ergonomia

**Sistemas de tipos complexos**:
- Curva de aprendizado íngreme
- Mensagens de erro difíceis de entender
- Muito código "para satisfazer o compilador"

---

## O Que Todo Arquiteto de Compiladores Deve Saber

### 1. Design do Sistema de Tipos

**Decisões fundamentais**:
- **Estático ou dinâmico?** Trade-off entre segurança e flexibilidade
- **Forte ou fraco?** Quanto controle dar ao programador?
- **Nominal ou estrutural?** Como expressar compatibilidade?
- **Inferência?** Quanto inferir vs. anotar explicitamente?

### 2. Implementação Eficiente

**Técnicas essenciais**:
- **Hash consing**: Compartilhar representações de tipos idênticos
- **Caching**: Memoizar resultados de type checking
- **Normalização**: Simplificar tipos antes de comparar
- **Incremental**: Re-verificar apenas código modificado

### 3. Mensagens de Erro

**Erros de tipo devem ser**:
- **Localizados**: Apontar para a causa raiz
- **Explicativos**: Dizer o que está errado e por quê
- **Construtivos**: Sugerir como corrigir

**Exemplo ruim**:
```
Error: type mismatch
```

**Exemplo bom**:
```
Error: Type mismatch in assignment
  Expected: int
  Found: string
  Location: file.c:42:5
  
  42 |     int x = "texto";
                  ^~~~~~~
  Suggestion: Did you mean to convert the string to int?
    Try: int x = atoi("texto");
```

### 4. Otimizações Baseadas em Tipos

**Tipos permitem**:
- **Especialização**: Gerar código específico para cada tipo
- **Inlining**: Substituir chamadas de função baseado em tipos
- **Eliminação de verificações**: Remover bounds checks quando provável seguro
- **Representação eficiente**: Escolher layout de memória ótimo

### 5. Interoperabilidade

**Compiladores devem**:
- **FFI (Foreign Function Interface)**: Chamar código em outras linguagens
- **ABI (Application Binary Interface)**: Layout compatível de dados
- **Marshalling**: Converter entre representações de tipos

### 6. Evolução

**Linguagens evoluem**:
- **Backward compatibility**: Novos recursos não devem quebrar código antigo
- **Gradual typing**: Migração de dinâmico para estático
- **Versioning**: Suportar múltiplas versões do sistema de tipos

### 7. Ferramental

**Sistemas de tipos alimentam**:
- **IDEs**: Autocompletar, navegação, refatoração
- **Documentação**: Geração automática de docs
- **Testing**: Geração de casos de teste baseado em tipos
- **Análise estática**: Detectar bugs sem executar

---

## Referências Acadêmicas

### Livros Fundamentais

1. **Pierce, Benjamin C.** (2002). *Types and Programming Languages*. MIT Press.
   - Referência definitiva sobre teoria de tipos
   - Cobre desde λ-calculus até tipos avançados

2. **Harper, Robert** (2016). *Practical Foundations for Programming Languages*. Cambridge University Press.
   - Fundação teórica moderna
   - Ênfase em semântica operacional

3. **Aho, Alfred V. et al.** (2006). *Compilers: Principles, Techniques, and Tools* (Dragon Book). Addison-Wesley.
   - Capítulo 6: Type Checking
   - Implementação prática de type checkers

4. **Appel, Andrew W.** (2004). *Modern Compiler Implementation in ML/Java/C*. Cambridge University Press.
   - Implementação completa de compilador com tipos

### Papers Clássicos

5. **Milner, Robin** (1978). "A Theory of Type Polymorphism in Programming". *Journal of Computer and System Sciences*.
   - Algoritmo de inferência de tipos Hindley-Milner

6. **Cardelli, Luca & Wegner, Peter** (1985). "On Understanding Types, Data Abstraction, and Polymorphism". *ACM Computing Surveys*.
   - Taxonomia clássica de sistemas de tipos

7. **Liskov, Barbara H. & Wing, Jeannette M.** (1994). "A Behavioral Notion of Subtyping". *ACM Transactions on Programming Languages and Systems*.
   - Princípio de Substituição de Liskov

8. **Wadler, Philip** (1989). "Theorems for Free!". *Functional Programming Languages and Computer Architecture*.
   - Propriedades garantidas por tipos polimórficos

### Padrões e Especificações

9. **IEEE Standard 754** (2019). *IEEE Standard for Floating-Point Arithmetic*.
   - Padrão para aritmética de ponto flutuante

10. **ISO/IEC 9899** (2018). *Programming Languages — C*.
    - Especificação formal da linguagem C, incluindo tipos

### Recursos Online

11. **Software Foundations** (Pierce et al.). https://softwarefoundations.cis.upenn.edu/
    - Curso interativo sobre fundamentos formais, incluindo tipos

12. **Type Theory and Functional Programming** (Thompson, Simon). http://www.cs.kent.ac.uk/people/staff/sjt/TTFP/
    - Introdução acessível à teoria de tipos

---

## Conclusão

Sistemas de tipos são fundamentais para compiladores modernos, fornecendo:

1. **Correção**: Detecção de erros em tempo de compilação
2. **Performance**: Otimizações baseadas em tipos
3. **Documentação**: Interfaces verificáveis
4. **Abstração**: Modularidade e reutilização
5. **Segurança**: Prevenção de vulnerabilidades

Para arquitetos de compiladores, dominar sistemas de tipos é essencial para:
- **Projetar linguagens** expressivas e seguras
- **Implementar verificadores** eficientes e precisos
- **Gerar código** otimizado e correto
- **Criar ferramentas** que auxiliem programadores

A evolução contínua dos sistemas de tipos — de tipagem simples a tipos dependentes, de verificação manual a inferência automática, de sistemas nominais a estruturais — reflete a busca perpétua por linguagens que combinem **segurança**, **expressividade** e **performance**.

