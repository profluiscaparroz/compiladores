# 07 — Analisador Léxico

Objetivos:
- Implementar um analisador léxico completo em C.
- Reconhecer tokens de uma linguagem didática.
- Aplicar conceitos de autômatos e expressões regulares.

Conteúdo:
- Tokenização: identificadores, números, palavras-chave, operadores.
- Estados do lexer: leitura, reconhecimento, geração de tokens.
- Tratamento de comentários, strings e caracteres especiais.
- Relatório de erros com linha e coluna.

Linguagem didática suportada:
- Palavras-chave: `if`, `else`, `while`, `for`, `int`, `float`, `return`
- Identificadores: letras e dígitos, iniciando por letra
- Números: inteiros e ponto flutuante
- Operadores: `+`, `-`, `*`, `/`, `=`, `==`, `!=`, `<`, `>`, `<=`, `>=`
- Delimitadores: `(`, `)`, `{`, `}`, `;`, `,`
- Comentários: `//` (linha) e `/* */` (bloco)

Estrutura:
- `include/`: arquivos de cabeçalho (.h)
- `src/`: implementação do lexer (.c)
- `tests/`: arquivos de teste (.tl)
- `Makefile`: compilação e testes

## Como compilar e usar

```bash
make                    # compila o lexer
make test              # roda todos os testes
./build/lexer file.tl  # analisa um arquivo específico
```

## Exemplo de uso

Arquivo `tests/exemplo1.tl`:
```
int x = 42;
if (x > 0) {
    // comentário
    return x + 1;
}
```

Saída:
```
TOKEN_INT       "int"           1:1
TOKEN_ID        "x"             1:5
TOKEN_ASSIGN    "="             1:7
TOKEN_NUMBER    "42"            1:9
TOKEN_SEMICOLON ";"             1:11
...
```