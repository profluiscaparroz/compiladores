#include <stdio.h>
#include <stdlib.h>

// Exemplo de program dependence graph
// com um loop while
// e uma condição if dentro do loop.
// O código é um loop que soma os números de 1 a 10
// e imprime o resultado.

int main()
{
    int i = 1;
    int soma = 0;
    while(i < 11 ){
        if (i < soma){
            soma += 1;
        }
        ++i;
    }
    return i;
}

// Função exemplo utilizando goto
void exemploGoto()
{
    int i = 1;
    int soma = 0;

inicio:
    if (i >= 11)
        goto fim;

    if (i < soma)
        soma += 1;

    ++i;
    goto inicio;

fim:
    printf("Soma: %d\n", soma);
}

// Outro exemplo utilizando goto e if-else if
void exemploGotoComIfElseIf()
{
    int valor = 5;
inicio:
    if (valor < 0)
    {
        printf("Valor é negativo.\n");
        goto fim;
    }
    else if (valor >= 0 && valor <= 10)
    {
        printf("Valor está entre 1 e 10.\n");
        goto fim;
    }
    else
    {
        printf("Valor é maior que 10.\n");
        goto fim;
    }
fim:
    printf("Fim do exemplo.\n");
}

void funcao1()
{
    int valor = 5;         // Bloco B1
    inicio:
        if (valor < 0)         // Bloco B2
        {
            printf("Valor é negativo.\n");  // Bloco B3
            goto fim;
        }
        else if (valor >= 0 && valor <= 10) // ainda B2
        {
            printf("Valor está entre 1 e 10.\n");  // Bloco B4
            goto fim;
        }
        else
        {
            printf("Valor é maior que 10.\n"); // Bloco B5
            goto fim;
        }
    fim:                       // Bloco B6 (fim)
        printf("Fim do exemplo.\n"); // Bloco B7
}