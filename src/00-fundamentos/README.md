# 00 — Fundamentos para Compiladores

## Introdução

Bem-vindo ao mundo dos compiladores! Este documento foi criado especialmente para alunos do primeiro período que estão se preparando para estudar uma das disciplinas mais desafiadoras e fascinantes da Ciência da Computação: **Compiladores**.

A disciplina de compiladores é conhecida por sua complexidade, pois integra diversos conceitos fundamentais de programação, estruturas de dados, algoritmos e teoria da computação. Antes de mergulhar no estudo de como um compilador funciona, é essencial ter uma base sólida em alguns conceitos fundamentais.

Este guia tem como objetivo revisar e reforçar os **conhecimentos prévios necessários** para que você possa acompanhar a disciplina com confiança. Cada seção apresenta conceitos básicos com exemplos práticos e explicações simples, pensadas para facilitar seu aprendizado.

---

## Índice

1. [Conceitos Básicos de Programação](#1-conceitos-básicos-de-programação)
2. [Estruturas de Dados Fundamentais](#2-estruturas-de-dados-fundamentais)
3. [Ponteiros e Gerenciamento de Memória](#3-ponteiros-e-gerenciamento-de-memória)
4. [Strings e Manipulação de Texto](#4-strings-e-manipulação-de-texto)
5. [Algoritmos e Complexidade](#5-algoritmos-e-complexidade)
6. [Entrada e Saída de Dados](#6-entrada-e-saída-de-dados)
7. [Fundamentos Matemáticos](#7-fundamentos-matemáticos)
8. [Processo de Compilação](#8-processo-de-compilação)
9. [Exercícios Práticos](#9-exercícios-práticos)

---

## 1. Conceitos Básicos de Programação

### 1.1 Variáveis e Tipos de Dados

Uma **variável** é um espaço na memória do computador onde podemos armazenar dados. Em C, toda variável deve ter um **tipo** que define que tipo de dado ela pode armazenar.

#### Tipos Primitivos em C

```c
// Tipos inteiros
int idade = 25;              // Número inteiro
char letra = 'A';            // Caractere (internamente é um inteiro pequeno)
short pequeno = 100;         // Inteiro curto
long grande = 1000000L;      // Inteiro longo

// Tipos de ponto flutuante
float altura = 1.75f;        // Número decimal (precisão simples)
double pi = 3.14159265359;   // Número decimal (precisão dupla)

// Tipo lógico (em C, usa-se int)
int verdadeiro = 1;          // Qualquer valor diferente de 0 é verdadeiro
int falso = 0;               // 0 é falso
```

**Explicação simples**: Pense nas variáveis como "caixas" onde guardamos informações. O tipo da variável define que tipo de informação cabe naquela caixa (números inteiros, números decimais, letras, etc.).

### 1.2 Operadores

Operadores são símbolos que realizam operações sobre variáveis e valores.

#### Operadores Aritméticos

```c
int a = 10, b = 3;

int soma = a + b;           // 13
int subtracao = a - b;      // 7
int multiplicacao = a * b;  // 30
int divisao = a / b;        // 3 (divisão inteira)
int resto = a % b;          // 1 (resto da divisão)
```

#### Operadores Relacionais

```c
int x = 5, y = 10;

int igual = (x == y);          // 0 (falso)
int diferente = (x != y);      // 1 (verdadeiro)
int maior = (x > y);           // 0 (falso)
int menor = (x < y);           // 1 (verdadeiro)
int maior_igual = (x >= y);    // 0 (falso)
int menor_igual = (x <= y);    // 1 (verdadeiro)
```

#### Operadores Lógicos

```c
int a = 1, b = 0;  // 1 = verdadeiro, 0 = falso

int e_logico = (a && b);       // 0 (falso) - E: ambos devem ser verdadeiros
int ou_logico = (a || b);      // 1 (verdadeiro) - OU: pelo menos um verdadeiro
int negacao = !a;              // 0 (falso) - NÃO: inverte o valor
```

### 1.3 Estruturas de Controle

As estruturas de controle permitem que o programa tome decisões e repita ações.

#### Estrutura Condicional: if-else

```c
int nota = 75;

if (nota >= 90) {
    printf("Aprovado com A\n");
} else if (nota >= 70) {
    printf("Aprovado com B\n");
} else if (nota >= 50) {
    printf("Aprovado com C\n");
} else {
    printf("Reprovado\n");
}
```

**Explicação simples**: O `if` permite ao programa "pensar" e tomar decisões baseadas em condições. É como perguntar: "Se isso for verdade, faça aquilo; caso contrário, faça outra coisa".

#### Estrutura de Repetição: for

```c
// Imprimir números de 1 a 10
for (int i = 1; i <= 10; i++) {
    printf("%d ", i);
}
// Saída: 1 2 3 4 5 6 7 8 9 10
```

**Partes do for**:
- `int i = 1`: Inicialização (onde começa)
- `i <= 10`: Condição (até quando continuar)
- `i++`: Incremento (o que fazer a cada repetição)

#### Estrutura de Repetição: while

```c
int contador = 0;

while (contador < 5) {
    printf("Contador: %d\n", contador);
    contador++;
}
```

**Explicação simples**: O `while` repete um bloco de código enquanto uma condição for verdadeira. É útil quando não sabemos quantas vezes precisamos repetir.

#### Estrutura de Repetição: do-while

```c
int numero;

do {
    printf("Digite um número positivo: ");
    scanf("%d", &numero);
} while (numero <= 0);
```

**Diferença do while**: O `do-while` garante que o bloco de código seja executado **pelo menos uma vez**, pois a condição é verificada no final.

### 1.4 Funções

Funções são blocos de código reutilizáveis que executam uma tarefa específica.

```c
// Declaração da função
int somar(int a, int b) {
    return a + b;
}

// Uso da função
int main() {
    int resultado = somar(5, 3);
    printf("Resultado: %d\n", resultado);  // Saída: Resultado: 8
    return 0;
}
```

**Anatomia de uma função**:
- `int`: Tipo de retorno (o tipo de valor que a função devolve)
- `somar`: Nome da função
- `int a, int b`: Parâmetros (valores que a função recebe)
- `return a + b`: Valor retornado pela função

**Por que usar funções?**
- **Reutilização**: Escreva uma vez, use várias vezes
- **Organização**: Divide o programa em partes menores e mais fáceis de entender
- **Manutenção**: Facilita encontrar e corrigir erros
- **Abstração**: Oculta detalhes de implementação

---

## 2. Estruturas de Dados Fundamentais

### 2.1 Arrays (Vetores)

Um **array** é uma coleção de elementos do mesmo tipo armazenados em posições consecutivas de memória.

```c
// Declaração e inicialização
int numeros[5] = {10, 20, 30, 40, 50};

// Acesso aos elementos (índices começam em 0)
printf("Primeiro elemento: %d\n", numeros[0]);   // 10
printf("Terceiro elemento: %d\n", numeros[2]);   // 30

// Modificação de elementos
numeros[1] = 25;  // Agora o segundo elemento vale 25

// Percorrer um array
for (int i = 0; i < 5; i++) {
    printf("numeros[%d] = %d\n", i, numeros[i]);
}
```

**Importante**: 
- Os índices vão de 0 a (tamanho - 1)
- Acessar um índice fora dos limites causa **comportamento indefinido** (bug comum!)

#### Arrays Multidimensionais

```c
// Matriz 3x3
int matriz[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};

// Acesso: matriz[linha][coluna]
printf("Elemento central: %d\n", matriz[1][1]);  // 5

// Percorrer uma matriz
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        printf("%d ", matriz[i][j]);
    }
    printf("\n");
}
```

### 2.2 Structs (Estruturas)

Uma **struct** permite agrupar variáveis de tipos diferentes sob um único nome.

```c
// Definição da estrutura
struct Pessoa {
    char nome[50];
    int idade;
    float altura;
};

// Uso da estrutura
int main() {
    // Declaração e inicialização
    struct Pessoa aluno;
    
    strcpy(aluno.nome, "João Silva");
    aluno.idade = 20;
    aluno.altura = 1.75;
    
    // Acesso aos campos
    printf("Nome: %s\n", aluno.nome);
    printf("Idade: %d anos\n", aluno.idade);
    printf("Altura: %.2f m\n", aluno.altura);
    
    return 0;
}
```

**Quando usar structs?**
- Para representar entidades do mundo real (pessoa, produto, livro)
- Para agrupar dados relacionados
- Para criar tipos de dados mais complexos

### 2.3 Enumerações (Enums)

Enumerações criam tipos com conjunto fixo de valores possíveis.

```c
// Definição de enumeração
enum DiaSemana {
    DOMINGO = 0,
    SEGUNDA = 1,
    TERCA = 2,
    QUARTA = 3,
    QUINTA = 4,
    SEXTA = 5,
    SABADO = 6
};

// Uso
enum DiaSemana hoje = QUARTA;

if (hoje == SABADO || hoje == DOMINGO) {
    printf("Fim de semana!\n");
} else {
    printf("Dia útil\n");
}
```

**Vantagem**: Código mais legível. Melhor escrever `SEGUNDA` do que `1`.

---

## 3. Ponteiros e Gerenciamento de Memória

### 3.1 O que são Ponteiros?

Um **ponteiro** é uma variável que armazena o **endereço de memória** de outra variável.

```c
int numero = 42;        // Variável normal
int *ptr = &numero;     // Ponteiro que guarda o endereço de 'numero'

printf("Valor de numero: %d\n", numero);          // 42
printf("Endereço de numero: %p\n", &numero);      // Ex: 0x7fff5fbff5ac
printf("Valor de ptr: %p\n", ptr);                // Mesmo endereço
printf("Valor apontado por ptr: %d\n", *ptr);     // 42
```

**Operadores importantes**:
- `&`: Operador "endereço de" - obtém o endereço de uma variável
- `*`: Operador "conteúdo de" (desreferenciação) - acessa o valor no endereço

**Analogia**: Um ponteiro é como um "endereço postal". Ele não é a casa (variável), mas indica onde a casa está na memória.

### 3.2 Ponteiros e Arrays

Em C, arrays e ponteiros estão intimamente relacionados:

```c
int numeros[5] = {10, 20, 30, 40, 50};
int *ptr = numeros;  // Nome do array é um ponteiro para o primeiro elemento

// Estas formas são equivalentes:
printf("%d\n", numeros[0]);    // 10
printf("%d\n", *ptr);          // 10
printf("%d\n", *(ptr + 0));    // 10

printf("%d\n", numeros[2]);    // 30
printf("%d\n", *(ptr + 2));    // 30
```

**Aritmética de ponteiros**:
```c
int arr[3] = {100, 200, 300};
int *p = arr;

printf("%d\n", *p);       // 100
p++;                       // Avança para o próximo elemento
printf("%d\n", *p);       // 200
p++;
printf("%d\n", *p);       // 300
```

### 3.3 Alocação Dinâmica de Memória

Até agora, trabalhamos com variáveis em **memória estática** (definidas em tempo de compilação). Mas frequentemente precisamos de **memória dinâmica** (alocada em tempo de execução).

#### malloc - Memory Allocation

```c
#include <stdlib.h>

// Alocar memória para 10 inteiros
int *array = malloc(10 * sizeof(int));

if (array == NULL) {
    printf("Erro: memória insuficiente!\n");
    return 1;
}

// Usar a memória
for (int i = 0; i < 10; i++) {
    array[i] = i * 10;
}

// Liberar a memória (SEMPRE IMPORTANTE!)
free(array);
array = NULL;  // Boa prática: anular o ponteiro
```

#### calloc - Contiguous Allocation

```c
// calloc aloca e inicializa com zeros
int *array = calloc(10, sizeof(int));

// Todos os elementos começam com 0
for (int i = 0; i < 10; i++) {
    printf("%d ", array[i]);  // 0 0 0 0 0 0 0 0 0 0
}

free(array);
```

#### realloc - Re-allocation

```c
int *array = (int*) malloc(5 * sizeof(int));

// Depois, precisamos de mais espaço
int *temp = realloc(array, 10 * sizeof(int));
if (temp != NULL) {
    array = temp;  // realloc bem-sucedido, atualiza o ponteiro
} else {
    // tratar erro de alocação mantendo 'array' válido
    // por exemplo: free(array); lidar_com_erro();
}

// Agora temos espaço para 10 inteiros (se realloc tiver sucesso)
free(array);
```

**IMPORTANTE - Regras de Ouro**:
1. **Toda alocação (`malloc`, `calloc`) deve ter um `free` correspondente**
2. **Não usar memória após `free`** (dangling pointer)
3. **Não fazer `free` duas vezes no mesmo ponteiro** (double free)
4. **Sempre verificar se `malloc` retornou `NULL`**

### 3.4 Memória: Stack vs Heap

```c
void exemplo() {
    // STACK (Pilha) - automática, rápida, limitada
    int local = 10;           // Criada quando a função é chamada
    int array[100];           // Destruída quando a função retorna
    
    // HEAP (Monte) - manual, mais lenta, flexível
    int *dinamico = malloc(sizeof(int) * 1000);  // Permanece até o free()
    
    // ... usar dinamico ...
    
    free(dinamico);  // Liberar explicitamente
}  // 'local' e 'array' são destruídos automaticamente aqui
```

**Stack (Pilha)**:
- Alocação/desalocação automática
- Rápida
- Tamanho limitado (geralmente alguns MB)
- Variáveis locais e parâmetros de função

**Heap (Monte)**:
- Alocação/desalocação manual (`malloc`/`free`)
- Mais lenta
- Tamanho muito maior
- Dados que precisam sobreviver ao escopo da função

---

## 4. Strings e Manipulação de Texto

### 4.1 Strings em C

Em C, uma **string** é um array de caracteres terminado com o caractere nulo `'\0'`.

```c
// Várias formas de declarar strings
char str1[6] = {'H', 'e', 'l', 'l', 'o', '\0'};  // Manual
char str2[] = "Hello";                            // Automático (adiciona \0)
char str3[20] = "Hello";                          // Com espaço extra
char *str4 = "Hello";                             // Ponteiro para literal
```

**Importante**: O `'\0'` é essencial! Funções de string usam-no para saber onde a string termina.

```c
char palavra[6] = "Hello";
// Memória: ['H']['e']['l']['l']['o']['\0']
//  Índice:   0    1    2    3    4    5
```

### 4.2 Funções Básicas de String

```c
#include <string.h>

char origem[20] = "Olá";
char destino[20];

// strlen - comprimento da string (sem contar \0)
int tamanho = strlen(origem);  // 3

// strcpy - copiar string
// Atenção: strcpy NÃO verifica o tamanho do destino e pode causar buffer overflow
// em códigos reais. Prefira funções que limitam o número de caracteres copiados,
// como strncpy ou snprintf.
strncpy(destino, origem, sizeof(destino) - 1);
destino[sizeof(destino) - 1] = '\0';  // garante terminação em \0
// destino agora é "Olá"

// strcat - concatenar strings
strcat(destino, " Mundo");  // destino agora é "Olá Mundo"

// strcmp - comparar strings
// Retorna: 0 se iguais, negativo se str1 < str2, positivo se str1 > str2
int resultado = strcmp(str1, str2);
if (resultado == 0) {
    printf("Strings iguais\n");
} else if (resultado < 0) {
    printf("str1 vem antes de str2\n");
} else {
    printf("str1 vem depois de str2\n");
}

// strchr - buscar caractere
char *pos = strchr(origem, 'l');  // Retorna ponteiro para o primeiro 'l'

// strstr - buscar substring
char texto[] = "Aprender C é divertido";
char *resultado = strstr(texto, "C");  // Ponteiro para "C é divertido"
```

### 4.3 Manipulação Manual de Strings

```c
// Exemplo: converter string para maiúsculas
void para_maiuscula(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32;  // Diferença entre minúscula e maiúscula
        }
    }
}

// Uso
char texto[] = "hello world";
para_maiuscula(texto);
printf("%s\n", texto);  // HELLO WORLD
```

### 4.4 Leitura Segura de Strings

```c
#include <stdio.h>

char nome[50];

// Leitura com limite de tamanho (mais seguro)
printf("Digite seu nome: ");
fgets(nome, sizeof(nome), stdin);

// fgets inclui o \n no final, remover se necessário:
nome[strcspn(nome, "\n")] = '\0';

printf("Olá, %s!\n", nome);
```

**Cuidado com `gets()`**: NUNCA use `gets()` pois não verifica limites e pode causar buffer overflow (vulnerabilidade de segurança grave). Use `fgets()` sempre!

---

## 5. Algoritmos e Complexidade

### 5.1 O que é um Algoritmo?

Um **algoritmo** é uma sequência finita de passos bem definidos para resolver um problema. Pense em uma receita de bolo: tem ingredientes (entrada), passos (processamento) e o bolo pronto (saída).

**Características de um bom algoritmo**:
- **Correto**: Resolve o problema proposto
- **Eficiente**: Usa recursos (tempo, memória) de forma otimizada
- **Claro**: Fácil de entender e manter
- **Genérico**: Funciona para diferentes entradas

### 5.2 Notação Big-O

A **notação Big-O** expressa como o tempo de execução ou uso de memória de um algoritmo cresce conforme o tamanho da entrada aumenta.

#### Complexidades Comuns

| Notação | Nome | Exemplo | Descrição |
|---------|------|---------|-----------|
| O(1) | Constante | Acessar array[i] | Tempo fixo, não depende do tamanho da entrada |
| O(log n) | Logarítmica | Busca binária | Muito eficiente, divide o problema pela metade |
| O(n) | Linear | Busca sequencial | Proporcional ao tamanho da entrada |
| O(n log n) | Linearithmic (linearítmica) | Merge Sort, Quick Sort | Algoritmos de ordenação eficientes |
| O(n²) | Quadrática | Bubble Sort, Selection Sort | Um loop dentro de outro |
| O(2ⁿ) | Exponencial | Algumas recursões | Cresce muito rapidamente |

#### Exemplos Práticos

```c
// O(1) - Tempo constante
int primeiro_elemento(int arr[]) {
    return arr[0];  // Sempre 1 operação
}

// O(n) - Tempo linear
int somar_elementos(int arr[], int n) {
    int soma = 0;
    for (int i = 0; i < n; i++) {
        soma += arr[i];  // n operações
    }
    return soma;
}

// O(n²) - Tempo quadrático
void imprimir_pares(int arr[], int n) {
    for (int i = 0; i < n; i++) {           // n vezes
        for (int j = 0; j < n; j++) {       // n vezes para cada i
            printf("(%d, %d) ", arr[i], arr[j]);
        }
    }
    // Total: n × n = n² operações
}

// O(log n) - Tempo logarítmico
int busca_binaria(int arr[], int n, int alvo) {
    int esq = 0, dir = n - 1;
    
    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;  // Evita overflow
        
        if (arr[meio] == alvo) {
            return meio;  // Encontrado
        } else if (arr[meio] < alvo) {
            esq = meio + 1;  // Buscar na metade direita
        } else {
            dir = meio - 1;  // Buscar na metade esquerda
        }
    }
    return -1;  // Não encontrado
}
```

### 5.3 Algoritmos de Ordenação

#### Bubble Sort (O(n²))

```c
void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Trocar elementos
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
```

**Como funciona**: Compara pares adjacentes e troca se estiverem na ordem errada. O maior elemento "borbulha" para o final.

#### Selection Sort (O(n²))

```c
void selection_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        
        // Encontrar o menor elemento
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        
        // Trocar com a posição atual
        int temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }
}
```

**Como funciona**: Encontra o menor elemento e coloca na primeira posição, depois o segundo menor na segunda posição, e assim por diante.

### 5.4 Algoritmos de Busca

#### Busca Linear (O(n))

```c
int busca_linear(int arr[], int n, int alvo) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == alvo) {
            return i;  // Posição encontrada
        }
    }
    return -1;  // Não encontrado
}
```

**Quando usar**: Array não ordenado, busca simples

#### Busca Binária (O(log n))

```c
int busca_binaria(int arr[], int n, int alvo) {
    int esq = 0, dir = n - 1;
    
    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        
        if (arr[meio] == alvo) {
            return meio;
        }
        
        if (arr[meio] < alvo) {
            esq = meio + 1;
        } else {
            dir = meio - 1;
        }
    }
    
    return -1;
}
```

**Quando usar**: Array **ordenado**. Muito mais eficiente que busca linear!

**Exemplo**: Buscar 23 em [3, 7, 12, 23, 45, 67, 89]
1. Meio = 23 → Encontrado! ✓

---

## 6. Entrada e Saída de Dados

### 6.1 Entrada e Saída Padrão

#### printf - Saída Formatada

```c
#include <stdio.h>

int idade = 25;
float altura = 1.75;
char inicial = 'J';
char nome[] = "João";

// Especificadores de formato
printf("Inteiro: %d\n", idade);              // %d ou %i para int
printf("Float: %.2f\n", altura);             // %f para float/double (.2 = 2 casas decimais)
printf("Caractere: %c\n", inicial);          // %c para char
printf("String: %s\n", nome);                // %s para string
printf("Hexadecimal: %x\n", 255);           // %x para hexadecimal
printf("Ponteiro: %p\n", (void*)&idade);    // %p para endereço

// Múltiplos valores
printf("Nome: %s, Idade: %d, Altura: %.2f\n", nome, idade, altura);
```

#### scanf - Entrada Formatada

```c
int numero;
float preco;
char caractere;
char texto[50];

printf("Digite um número: ");
scanf("%d", &numero);  // Note o & para passar o endereço

printf("Digite um preço: ");
scanf("%f", &preco);

printf("Digite um caractere: ");
scanf(" %c", &caractere);  // Espaço antes de %c para ignorar whitespace

printf("Digite uma palavra: ");
scanf("%s", texto);  // Strings não precisam de &, mas cuidado com buffer overflow!
```

**Importante**: `scanf` é útil mas perigoso. Para strings, prefira `fgets`.

### 6.2 Arquivos

#### Leitura de Arquivo

```c
#include <stdio.h>

FILE *arquivo = fopen("dados.txt", "r");  // "r" = read (leitura)

if (arquivo == NULL) {
    printf("Erro ao abrir arquivo!\n");
    return 1;
}

char linha[100];
while (fgets(linha, sizeof(linha), arquivo) != NULL) {
    printf("%s", linha);
}

fclose(arquivo);  // SEMPRE fechar o arquivo!
```

#### Escrita em Arquivo

```c
FILE *arquivo = fopen("saida.txt", "w");  // "w" = write (escrita)

if (arquivo == NULL) {
    printf("Erro ao criar arquivo!\n");
    return 1;
}

fprintf(arquivo, "Linha 1\n");
fprintf(arquivo, "Linha 2\n");
fprintf(arquivo, "Número: %d\n", 42);

fclose(arquivo);
```

#### Modos de Abertura de Arquivo

| Modo | Significado | Descrição |
|------|-------------|-----------|
| "r" | Read | Leitura. Arquivo deve existir |
| "w" | Write | Escrita. Cria novo ou sobrescreve |
| "a" | Append | Adiciona ao final. Cria se não existir |
| "r+" | Read/Write | Leitura e escrita. Arquivo deve existir |
| "w+" | Write/Read | Leitura e escrita. Cria novo ou sobrescreve |
| "a+" | Append/Read | Leitura e adição. Cria se não existir |

### 6.3 Argumentos de Linha de Comando

Programas podem receber argumentos quando executados no terminal:

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    // argc = número de argumentos
    // argv = array de strings com os argumentos
    
    printf("Número de argumentos: %d\n", argc);
    
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    
    return 0;
}
```

**Exemplo de uso**:
```bash
$ ./programa arquivo.txt 123
Número de argumentos: 3
argv[0] = ./programa
argv[1] = arquivo.txt
argv[2] = 123
```

**Nota**: `argv[0]` sempre contém o nome do programa.

---

## 7. Fundamentos Matemáticos

### 7.1 Teoria de Conjuntos

Um **conjunto** é uma coleção de elementos distintos. É um conceito fundamental em ciência da computação.

#### Operações Básicas

```
A = {1, 2, 3, 4}
B = {3, 4, 5, 6}

União (A ∪ B):        {1, 2, 3, 4, 5, 6}    // Elementos em A ou B
Interseção (A ∩ B):   {3, 4}                 // Elementos em A e B
Diferença (A - B):    {1, 2}                 // Elementos em A mas não em B
Complemento:          Todos exceto A
```

**Implementação em C** (conjunto como array de booleanos):

```c
#define TAMANHO 10

// Conjuntos representados como arrays de 0s e 1s
int A[TAMANHO] = {0, 1, 1, 1, 1, 0, 0, 0, 0, 0};  // {1, 2, 3, 4}
int B[TAMANHO] = {0, 0, 0, 1, 1, 1, 1, 0, 0, 0};  // {3, 4, 5, 6}

// União
int uniao[TAMANHO];
for (int i = 0; i < TAMANHO; i++) {
    uniao[i] = A[i] || B[i];
}

// Interseção
int intersecao[TAMANHO];
for (int i = 0; i < TAMANHO; i++) {
    intersecao[i] = A[i] && B[i];
}
```

### 7.2 Lógica Booleana

#### Tabelas Verdade

**AND (E lógico) - &&**
| A | B | A && B |
|---|---|--------|
| 0 | 0 |   0    |
| 0 | 1 |   0    |
| 1 | 0 |   0    |
| 1 | 1 |   1    |

**OR (OU lógico) - ||**
| A | B | A \|\| B |
|---|---|--------|
| 0 | 0 |   0    |
| 0 | 1 |   1    |
| 1 | 0 |   1    |
| 1 | 1 |   1    |

**NOT (NÃO lógico) - !**
| A | !A |
|---|----|
| 0 | 1  |
| 1 | 0  |

#### Leis de De Morgan

```c
// !(A && B) é equivalente a (!A || !B)
// !(A || B) é equivalente a (!A && !B)

int A = 1, B = 0;

// Exemplo 1
int resultado1 = !(A && B);        // !(1 && 0) = !(0) = 1
int resultado2 = !A || !B;         // !1 || !0 = 0 || 1 = 1
// resultado1 == resultado2 ✓

// Exemplo 2
int resultado3 = !(A || B);        // !(1 || 0) = !(1) = 0
int resultado4 = !A && !B;         // !1 && !0 = 0 && 1 = 0
// resultado3 == resultado4 ✓
```

### 7.3 Grafos Básicos

Um **grafo** é uma estrutura composta por **vértices** (nós) e **arestas** (conexões).

```
Exemplo de grafo simples:

    A --- B
    |     |
    |     |
    C --- D

Vértices: {A, B, C, D}
Arestas: {(A,B), (A,C), (B,D), (C,D)}
```

#### Representação por Matriz de Adjacência

```c
#define N 4  // Número de vértices

// Matriz de adjacência (1 = existe aresta, 0 = não existe)
int grafo[N][N] = {
    {0, 1, 1, 0},  // A conecta com B e C
    {1, 0, 0, 1},  // B conecta com A e D
    {1, 0, 0, 1},  // C conecta com A e D
    {0, 1, 1, 0}   // D conecta com B e C
};

// Verificar se existe aresta entre vértice i e j
int tem_aresta(int i, int j) {
    return grafo[i][j];
}
```

#### Representação por Lista de Adjacências

```c
#include <stdlib.h>

// Nó da lista
struct No {
    int vertice;
    struct No *proximo;
};

// Grafo como array de listas
struct No *grafo[N];

// Adicionar aresta
void adicionar_aresta(int origem, int destino) {
    struct No *novo = malloc(sizeof(struct No));
    novo->vertice = destino;
    novo->proximo = grafo[origem];
    grafo[origem] = novo;
}
```

**Por que grafos são importantes?**
- Análise sintática usa **árvores** (um tipo especial de grafo)
- Otimização de compiladores usa **grafos de fluxo de controle**
- Algoritmos de parsing trabalham com **grafos direcionados**

### 7.4 Recursão

**Recursão** é quando uma função chama a si mesma. É fundamental em compiladores!

```c
// Exemplo clássico: Fatorial
int fatorial(int n) {
    // Caso base (condição de parada)
    if (n == 0 || n == 1) {
        return 1;
    }
    
    // Caso recursivo
    return n * fatorial(n - 1);
}

// fatorial(5) = 5 * fatorial(4)
//             = 5 * 4 * fatorial(3)
//             = 5 * 4 * 3 * fatorial(2)
//             = 5 * 4 * 3 * 2 * fatorial(1)
//             = 5 * 4 * 3 * 2 * 1
//             = 120
```

#### Exemplo: Fibonacci

```c
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// fibonacci(5) = fibonacci(4) + fibonacci(3)
//              = ... (muito cálculo recursivo)
//              = 5
```

#### Exemplo: Busca em Árvore

```c
struct No {
    int valor;
    struct No *esquerda;
    struct No *direita;
};

// Buscar valor em árvore binária (recursivo)
int buscar(struct No *raiz, int valor) {
    if (raiz == NULL) {
        return 0;  // Não encontrado
    }
    
    if (raiz->valor == valor) {
        return 1;  // Encontrado!
    }
    
    // Buscar nas subárvores
    return buscar(raiz->esquerda, valor) || 
           buscar(raiz->direita, valor);
}
```

**Cuidados com recursão**:
- Sempre ter um **caso base** (condição de parada)
- Cada chamada recursiva deve aproximar do caso base
- Recursão muito profunda pode causar **stack overflow**

---

## 8. Processo de Compilação

### 8.1 O que é Compilação?

**Compilação** é o processo de traduzir código escrito em linguagem de alto nível (como C) para linguagem de máquina (código que o processador entende).

```
Código C ──────> Compilador ──────> Código de Máquina
(legível)                            (executável)
```

### 8.2 Etapas da Compilação

```
programa.c ──┬──> [Pré-processador] ──> programa.i
             │
             ├──> [Compilador] ──> programa.s (assembly)
             │
             ├──> [Montador] ──> programa.o (código objeto)
             │
             └──> [Linkeditor] ──> programa (executável)
```

#### 1. Pré-processamento

O **pré-processador** processa diretivas que começam com `#`:

```c
// Exemplo de código original
#include <stdio.h>
#define PI 3.14159
#define QUADRADO(x) ((x) * (x))

int main() {
    printf("PI = %f\n", PI);
    int area = QUADRADO(5);
    return 0;
}
```

Após o pré-processamento:
```c
// stdio.h é incluído (milhares de linhas)
// ...

int main() {
    printf("PI = %f\n", 3.14159);          // PI substituído
    int area = ((5) * (5));                 // Macro expandida
    return 0;
}
```

**Comando**: `gcc -E programa.c -o programa.i`

#### 2. Compilação

O **compilador** traduz o código C para **assembly** (linguagem de baixo nível, mas ainda legível):

```c
// C
int soma(int a, int b) {
    return a + b;
}
```

Vira assembly (exemplo simplificado x86):
```asm
soma:
    push    rbp
    mov     rbp, rsp
    mov     eax, edi      ; move 'a' para eax
    add     eax, esi      ; adiciona 'b' a eax
    pop     rbp
    ret
```

**Comando**: `gcc -S programa.c -o programa.s`

#### 3. Montagem (Assembly)

O **montador** (assembler) converte assembly em **código objeto** (binário):

```
Assembly (.s) ──> Montador ──> Código Objeto (.o)
```

Código objeto contém instruções de máquina, mas ainda não está pronto para executar (faltam bibliotecas).

**Comando**: `gcc -c programa.c -o programa.o`

#### 4. Ligação (Linking)

O **linkeditor** (linker) junta todos os códigos objeto e bibliotecas em um **executável** final:

```
programa.o + biblioteca.o + libc ──> Linkeditor ──> executável
```

**Comando**: `gcc programa.o -o programa`

#### Processo Completo em Um Comando

```bash
gcc programa.c -o programa
```

Este comando executa todas as 4 etapas automaticamente!

### 8.3 Makefile Básico

Um **Makefile** automatiza a compilação:

```makefile
# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c99

# Nome do executável
TARGET = programa

# Arquivos fonte
SOURCES = main.c funcoes.c

# Arquivos objeto
OBJECTS = $(SOURCES:.c=.o)

# Regra padrão
all: $(TARGET)

# Como criar o executável
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Como criar arquivos .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
	rm -f $(OBJECTS) $(TARGET)

# Recompilar tudo
rebuild: clean all
```

**Uso**:
```bash
make           # Compilar
make clean     # Remover arquivos gerados
make rebuild   # Limpar e recompilar
```

### 8.4 Flags Úteis do GCC

```bash
# Avisos (warnings)
gcc -Wall -Wextra programa.c       # Ativar avisos importantes

# Depuração
gcc -g programa.c                  # Incluir informações de debug

# Otimização
gcc -O2 programa.c                 # Otimizar performance
gcc -O3 programa.c                 # Otimização agressiva

# Padrão C
gcc -std=c99 programa.c            # Usar padrão C99
gcc -std=c11 programa.c            # Usar padrão C11

# Incluir diretórios e bibliotecas
gcc programa.c -I./include         # Adicionar diretório de headers
gcc programa.c -L./lib -lminhalib  # Adicionar biblioteca

# Definir macros
gcc -DDEBUG programa.c             # Define DEBUG
```

---

## 9. Exercícios Práticos

### Exercício 1: Manipulação de Strings

Implemente uma função que recebe uma string e retorna uma nova string com todos os espaços removidos.

```c
// Exemplo:
// Entrada: "Olá Mundo"
// Saída: "OláMundo"

char* remover_espacos(const char *str) {
    // Seu código aqui
}
```

**Dica**: Aloque memória dinâmica, percorra a string original copiando apenas caracteres que não são espaço.

### Exercício 2: Busca em Array

Implemente busca linear e busca binária. Compare o tempo de execução para arrays grandes.

```c
int busca_linear(int arr[], int n, int alvo);
int busca_binaria(int arr[], int n, int alvo);
```

**Teste**: Crie um array de 100000 elementos e meça o tempo das duas buscas.

### Exercício 3: Lista Ligada

Implemente uma lista ligada simples com as operações:

```c
struct No {
    int valor;
    struct No *proximo;
};

void inserir_inicio(struct No **cabeca, int valor);
void inserir_fim(struct No **cabeca, int valor);
void remover(struct No **cabeca, int valor);
void imprimir(struct No *cabeca);
void liberar(struct No **cabeca);
```

### Exercício 4: Calculadora de Expressões

Crie uma calculadora que lê uma expressão matemática simples e calcula o resultado:

```c
// Exemplo:
// Entrada: "10 + 5 * 2"
// Saída: 20

int calcular(const char *expressao);
```

**Desafio**: Implemente respeitando a precedência de operadores (* e / antes de + e -).

### Exercício 5: Leitura de Arquivo CSV

Implemente um programa que lê um arquivo CSV (valores separados por vírgula) e armazena os dados em uma estrutura:

```c
struct Aluno {
    char nome[50];
    int idade;
    float nota;
};

struct Aluno* ler_csv(const char *arquivo, int *num_alunos);
```

### Exercício 6: Analisador Léxico Simples

Crie um mini-analisador léxico que identifica tokens em uma expressão:

```c
// Entrada: "x = 10 + y"
// Saída:
// IDENTIFIER: x
// EQUAL: =
// NUMBER: 10
// PLUS: +
// IDENTIFIER: y

void analisar_lexico(const char *entrada);
```

**Isso é um preview do que faremos em Compiladores!**

---

## Conclusão

Parabéns por revisar todos esses conceitos fundamentais! Você agora tem uma base sólida para começar a estudar compiladores.

### Checklist de Conhecimentos

Antes de iniciar a disciplina, certifique-se de que você:

- ✅ Entende variáveis, tipos de dados e operadores
- ✅ Domina estruturas de controle (if, for, while)
- ✅ Sabe criar e usar funções
- ✅ Conhece arrays, structs e enums
- ✅ Compreende ponteiros e alocação dinâmica
- ✅ Consegue manipular strings em C
- ✅ Entende notação Big-O e algoritmos básicos
- ✅ Sabe trabalhar com arquivos
- ✅ Conhece conceitos de conjuntos, lógica e grafos
- ✅ Entende o processo de compilação

### Próximos Passos

Agora você está pronto para:

1. **02-linguagens-formais**: Aprender sobre gramáticas e hierarquia de Chomsky
2. **03-automatos**: Estudar autômatos finitos (fundamentais para análise léxica)
3. **04-expressao-regular**: Trabalhar com expressões regulares
4. **07-analisador-lexico**: Implementar seu primeiro compilador (a parte léxica)!

### Recursos Adicionais

**Livros Recomendados**:
- "The C Programming Language" - Kernighan & Ritchie (K&R)
- "Estruturas de Dados e Seus Algoritmos" - Szwarcfiter & Markenzon
- "Introduction to Algorithms" - Cormen, Leiserson, Rivest & Stein (CLRS)

**Prática Online**:
- [LeetCode](https://leetcode.com/) - Problemas de algoritmos
- [HackerRank](https://www.hackerrank.com/) - Desafios de programação
- [Exercism](https://exercism.org/) - Exercícios com mentoria

**Dica Final**: A melhor forma de aprender é **praticando**! Não apenas leia o código, digite-o, modifique-o, quebre-o e conserte-o. É assim que se aprende de verdade.

Bons estudos e sucesso na disciplina de Compiladores! 🚀

---

## Referências

1. Kernighan, B. W., & Ritchie, D. M. (1988). *The C Programming Language* (2nd ed.). Prentice Hall.
2. Aho, A. V., Sethi, R., & Ullman, J. D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.). Addison-Wesley.
3. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
4. Hopcroft, J. E., Motwani, R., & Ullman, J. D. (2006). *Introduction to Automata Theory, Languages, and Computation* (3rd ed.). Pearson.
5. Sipser, M. (2013). *Introduction to the Theory of Computation* (3rd ed.). Cengage Learning.
