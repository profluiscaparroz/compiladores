#include <stdio.h>
#include <string.h>

#define MAX 100

// Estrutura da Pilha
typedef struct {
    char dados[MAX];
    int topo;
} Pilha;

// Inicializa a pilha
void inicializar_pilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está vazia
int pilha_vazia(Pilha *p) {
    return (p->topo == -1);
}

// Adiciona um elemento na pilha
void empilhar(Pilha *p, char c) {
    if (p->topo < MAX - 1) {
        p->dados[++p->topo] = c;
    }
}

// Remove um elemento da pilha
char desempilhar(Pilha *p) {
    if (!pilha_vazia(p)) {
        return p->dados[p->topo--];
    }
    return '\0'; // Retorna vazio se a pilha estiver vazia
}

// Função que verifica se a string pertence à linguagem a^n b^n
int reconhece_anbn(const char *str) {
    Pilha pilha;
    inicializar_pilha(&pilha);
    int i, len = strlen(str);

    // Primeira fase: empilhar todos os 'a's
    for (i = 0; i < len; i++) {
        if (str[i] == 'a') {
            empilhar(&pilha, 'a'); // Armazena os 'a's na pilha
        } else if (str[i] == 'b') {
            if (pilha_vazia(&pilha)) {
                return 0; // Se não há 'a' para desempilhar, a string é inválida
            }
            desempilhar(&pilha); // Remove um 'a' para cada 'b'
        } else {
            return 0; // Caracter inválido na entrada
        }
    }

    // Se a pilha estiver vazia no final, a string pertence à linguagem
    return pilha_vazia(&pilha);
}

int main() {
    // Testes
    char *testes[] = { "ab", "aabb", "aaabbb", "aaaabbbb", "abb", "ba", "aab", "aaaaabb", "" };
    int num_testes = sizeof(testes) / sizeof(testes[0]);

    for (int i = 0; i < num_testes; i++) {
        if (reconhece_anbn(testes[i]))
            printf("A string \"%s\" pertence à linguagem a^n b^n\n", testes[i]);
        else
            printf("A string \"%s\" NÃO pertence à linguagem a^n b^n\n", testes[i]);
    }

    return 0;
}
