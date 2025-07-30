#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Estrutura para representar um nó da árvore de derivação
typedef struct Node {
    char symbol;
    struct Node *left;
    struct Node *right;
} Node;

// Função para criar um novo nó
Node* createNode(char symbol) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->symbol = symbol;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Função para imprimir a árvore em ordem (in-order traversal)
void printTree(Node* root) {
    if (root == NULL) return;
    printTree(root->left);
    printf("%c ", root->symbol);
    printTree(root->right);
}

// Função para liberar a memória da árvore
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Função para simular a tradução dirigida por sintaxe
// Exemplo: Avaliação de uma expressão aritmética simples (E -> E + T | T, T -> T * F | F, F -> (E) | id)
Node* parseExpression(const char** input);
Node* parseTerm(const char** input);
Node* parseFactor(const char** input);

// Função para consumir espaços em branco
void skipWhitespace(const char** input) {
    while (isspace(**input)) (*input)++;
}

// Função para analisar um fator (F -> (E) | id)
Node* parseFactor(const char** input) {
    skipWhitespace(input);
    if (**input == '(') {
        (*input)++; // Consome '('
        Node* node = parseExpression(input);
        skipWhitespace(input);
        if (**input == ')') {
            (*input)++; // Consome ')'
        } else {
            printf("Erro: ')' esperado\n");
            exit(1);
        }
        return node;
    } else if (isalnum(**input)) {
        Node* node = createNode(**input);
        (*input)++; // Consome o identificador
        return node;
    } else {
        printf("Erro: Fator inválido\n");
        exit(1);
    }
}

// Função para analisar um termo (T -> T * F | F)
Node* parseTerm(const char** input) {
    Node* node = parseFactor(input);
    skipWhitespace(input);
    while (**input == '*') {
        char op = **input;
        (*input)++; // Consome '*'
        Node* right = parseFactor(input);
        Node* newNode = createNode(op);
        newNode->left = node;
        newNode->right = right;
        node = newNode;
        skipWhitespace(input);
    }
    return node;
}

// Função para analisar uma expressão (E -> E + T | T)
Node* parseExpression(const char** input) {
    Node* node = parseTerm(input);
    skipWhitespace(input);
    while (**input == '+') {
        char op = **input;
        (*input)++; // Consome '+'
        Node* right = parseTerm(input);
        Node* newNode = createNode(op);
        newNode->left = node;
        newNode->right = right;
        node = newNode;
        skipWhitespace(input);
    }
    return node;
}

int main() {
    const char* input = "a + b * c + d";
    printf("Entrada: %s\n", input);

    const char* inputPtr = input;
    Node* syntaxTree = parseExpression(&inputPtr);

    printf("Árvore de derivação (in-order): ");
    printTree(syntaxTree);
    printf("\n");

    freeTree(syntaxTree);
    return 0;
}