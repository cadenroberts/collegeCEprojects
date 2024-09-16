#include "node.h"

#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, uint32_t weight) {
    // Allocate memory for a new Node
    Node *new_node = (Node *) malloc(sizeof(Node));
    if (new_node == NULL) {
        // Memory allocation failed
        return NULL;
    }
    // Set the symbol and weight fields of the new Node
    new_node->symbol = symbol;
    new_node->weight = weight;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}
void node_free(Node **node) {
    if (*node != NULL) {
        // Recursively free the children nodes
        node_free(&(*node)->left);
        node_free(&(*node)->right);
        // Free the current node
        free(*node);
        // Set *pnode to NULL
        *node = NULL;
    }
}

void node_print_node(Node *tree, char ch, int indentation) {
    if (tree == NULL)
        return;
    node_print_node(tree->right, '/', indentation + 3);
    printf("%*cweight = %d", indentation + 1, ch, tree->weight);
    if (tree->left == NULL && tree->right == NULL) {
        if (' ' <= tree->symbol && tree->symbol <= '~')
            printf(", symbol = '%c'", tree->symbol);
        else
            printf(", symbol = 0x%02x", tree->symbol);
    }
    printf("\n");
    node_print_node(tree->left, '\\', indentation + 3);
}

void node_print_tree(Node *tree) {
    node_print_node(tree, '<', 2);
}
