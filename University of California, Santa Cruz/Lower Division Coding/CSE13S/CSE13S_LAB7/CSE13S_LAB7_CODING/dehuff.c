#include "bitreader.h"
#include "node.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_STACK_SIZE 64
#define INPUTS         "i:o:h"
#define USAGE                                                                                      \
    "Usage: dehuff -i infile -o outfile\n"                                                         \
    "	dehuff -h\n"

void stack_push(Node *stack[], int *top, Node *node) {
    if (*top >= MAX_STACK_SIZE - 1) {
        fprintf(stderr, "stack overflow lol\n");
        exit(EXIT_FAILURE);
    }
    stack[++(*top)] = node;
}

Node *stack_pop(Node *stack[], int *top) {
    if (*top < 0) {
        fprintf(stderr, "stack overflow lol\n");
        exit(EXIT_FAILURE);
    }
    return stack[(*top)--];
}

void dehuff_decompress_file(FILE *fout, BitReader *inbuf) {
    uint8_t type1 = bit_read_uint8(inbuf);
    uint8_t type2 = bit_read_uint8(inbuf);
    uint32_t filesize = bit_read_uint32(inbuf);
    uint16_t num_leaves = bit_read_uint16(inbuf);
    assert(type1 == 'H' && type2 == 'C');
    int num_nodes = 2 * num_leaves - 1;
    Node *stack[MAX_STACK_SIZE];
    int stack_top = -1;
    for (int i = 0; i < num_nodes; ++i) {
        int bit = bit_read_bit(inbuf);
        Node *node;
        if (bit == 1) {
            uint8_t symbol = bit_read_uint8(inbuf);
            node = node_create(symbol, 0);
        } else {
            node = node_create(0, 0);
            if (stack_top >= 1) {
                node->right = stack_pop(stack, &stack_top);
                node->left = stack_pop(stack, &stack_top);
            }
        }
        stack_push(stack, &stack_top, node);
    }
    Node *code_tree = stack_pop(stack, &stack_top);
    for (uint32_t i = 0; i < filesize; ++i) {
        Node *node = code_tree;
        while (node->left != NULL || node->right != NULL) {
            int bit = bit_read_bit(inbuf);
            node = (bit == 0) ? node->left : node->right;
        }
        fputc(node->symbol, fout);
    }
    node_free(&code_tree);
}

int main(int argc, char *argv[]) {
    int opt;
    char *infile_path = NULL, *outfile_path = NULL;
    while ((opt = getopt(argc, argv, INPUTS)) != -1)
        switch (opt) {
        case 'h': fprintf(stdout, "%s", USAGE); return 0;
        case 'i': infile_path = optarg; break;
        case 'o': outfile_path = optarg; break;
        default: fprintf(stderr, "dehuff: Invalid option\n%s", USAGE); return 1;
        }
    if (infile_path == NULL || outfile_path == NULL) {
        fprintf(stderr, "dehuff: -i and -o options are required\n%s", USAGE);
        return 1;
    }
    BitReader *inbuf = bit_read_open(infile_path);
    if (!inbuf) {
        fprintf(stderr, "Error opening input file");
        return 1;
    }
    FILE *fout = fopen(outfile_path, "wb");
    if (!fout) {
        fprintf(stderr, "Error opening input file");
        bit_read_close(&inbuf);
        return 1;
    }
    dehuff_decompress_file(fout, inbuf);
    fclose(fout);
    bit_read_close(&inbuf);
    return 0;
}
