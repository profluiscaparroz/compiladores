# 09 — Analisador Semântico

## Índice
1. [Introdução](#introdução)
2. [Conceitos Fundamentais](#conceitos-fundamentais)
3. [Fundamentos Teóricos](#fundamentos-teóricos)
4. [Tabela de Símbolos](#tabela-de-símbolos)
5. [Sistema de Tipos](#sistema-de-tipos)
6. [Análise de Escopo](#análise-de-escopo)
7. [Verificações Semânticas](#verificações-semânticas)
8. [Implementação em C](#implementação-em-c)
9. [Tratamento de Erros Semânticos](#tratamento-de-erros-semânticos)
10. [Exemplos Práticos](#exemplos-práticos)
11. [Como Compilar e Usar](#como-compilar-e-usar)
12. [Referências Acadêmicas](#referências-acadêmicas)

---

## Introdução

A **análise semântica** é a **terceira fase** de um compilador, situada imediatamente após as análises léxica e sintática. Enquanto o analisador léxico verifica a estrutura dos tokens e o analisador sintático valida a estrutura gramatical do programa, o analisador semântico verifica se o programa **faz sentido** do ponto de vista de significado, tipos e contexto.

### Importância da Análise Semântica

Segundo **Aho, Lam, Sethi e Ullman** (2006), no clássico "Compiladores: Princípios, Técnicas e Ferramentas":

> "O analisador semântico usa a árvore sintática e as informações na tabela de símbolos para verificar a consistência semântica do programa fonte com a definição da linguagem."

A análise semântica é fundamental porque:
- **Detecta erros que a sintaxe não captura**: Uso de variáveis não declaradas, incompatibilidade de tipos
- **Garante correção do programa**: Verifica se operações fazem sentido no contexto
- **Prepara para geração de código**: Coleta informações necessárias para as fases posteriores
- **Melhora a qualidade do código**: Detecta avisos como variáveis não utilizadas
- **Implementa regras da linguagem**: Aplica semântica estática definida pela especificação

### Analogia com Linguagem Natural

Considere as frases em português:

1. **Sintaticamente correta, semanticamente incorreta**:
   - "As ideias verdes incolores dormem furiosamente."
   - Gramaticalmente perfeita, mas sem sentido lógico

2. **Sintaticamente e semanticamente correta**:
   - "O compilador detecta erros no código."
   - Correta em ambos os aspectos

Da mesma forma, em programação:

```c
// Sintaticamente correto, semanticamente incorreto
int x;
x = "texto";  // Erro: tentando atribuir string a inteiro

// Correto em ambos os aspectos
int x;
x = 42;  // OK: tipo compatível
```

### Objetivos deste Módulo

- Implementar tabela de símbolos com suporte a escopos aninhados
- Realizar verificação de tipos estática
- Analisar escopo e visibilidade de identificadores
- Detectar erros semânticos em tempo de compilação
- Compreender sistemas de tipos e suas propriedades
- Desenvolver estratégias de recuperação de erros semânticos

---

## Conceitos Fundamentais

### O que é Análise Semântica?

A análise semântica é o processo de verificar se um programa sintaticamente correto também é **semanticamente válido**, ou seja, se respeita as regras de significado da linguagem.

**Entrada**: Árvore sintática abstrata (AST) do analisador sintático
```
[Atribuição]
  ├── Variável: x (tipo: int)
  └── Valor: "texto" (tipo: string)
```

**Saída**: AST anotada com tipos ou mensagens de erro
```
ERRO SEMÂNTICO: Incompatibilidade de tipos
  Esperado: int
  Obtido: string
  Linha: 5, Coluna: 8
```

### Semântica Estática vs Dinâmica

**Louden** (1997) distingue claramente dois tipos de semântica:

| Aspecto | Semântica Estática | Semântica Dinâmica |
|---------|-------------------|-------------------|
| **Quando** | Tempo de compilação | Tempo de execução |
| **Verificações** | Tipos, declarações, escopos | Valores, divisão por zero, limites |
| **Exemplos** | `int x = "texto";` (erro de tipo) | `int y = x / 0;` (divisão por zero) |
