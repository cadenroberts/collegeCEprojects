#include "bitreader.h"
#include "bitwriter.h"
#include "node.h"
#include "pq.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define INPUTS "i:o:h"
#define USAGE                                                                                      \
    "Usage: huff -i infile -o outfile\n"                                                           \
    "       huff -v -i infile -o outfile\n"                                                        \
    "       huff -h\n"

typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} Code;

uint32_t fill_histogram(FILE *fin, uint32_t *histogram) {
    for (int i = 0; i < 256; ++i)
        histogram[i] = 0;
    ++histogram[0x00];
    ++histogram[0xff];
    uint32_t filesize = 0;
    int byte;
    while ((byte = fgetc(fin)) != EOF) {
        ++histogram[(unsigned char) byte];
        ++filesize;
    }
    return filesize;
}

Node *create_tree(uint32_t *histogram, uint16_t *num_leaves) {
    PriorityQueue *pq = pq_create();
    if (pq == NULL)
        return NULL;
    *num_leaves = 0;
    for (int i = 0; i < 256; ++i) {
        if (histogram[i] > 0) {
            Node *node = node_create((uint8_t) i, histogram[i]);
            if (node == NULL) {
                pq_free(&pq);
                return NULL;
            }
            enqueue(pq, node);
            (*num_leaves)++;
        }
    }
    while (!pq_size_is_1(pq)) {
        Node *left = dequeue(pq);
        Node *right = dequeue(pq);
        if (!left || !right) {
            node_free(&left);
            node_free(&right);
            pq_free(&pq);
            return NULL;
        }
        uint32_t parent_weight = left->weight + right->weight;
        Node *parent = node_create(0, parent_weight);
        if (parent == NULL) {
            node_free(&left);
            node_free(&right);
            pq_free(&pq);
            return NULL;
        }
        parent->left = left;
        parent->right = right;
        enqueue(pq, parent);
    }
    Node *root = dequeue(pq);
    pq_free(&pq);
    return root;
}

void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {
    if (node == NULL)
        return;
    if (node->left == NULL && node->right == NULL) {
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_length = code_length;
    } else {
        fill_code_table(code_table, node->left, code, code_length + 1);
        code |= (uint64_t) 1 << code_length;
        fill_code_table(code_table, node->right, code, code_length + 1);
    }
}

void huff_write_tree(BitWriter *outbuf, Node *node) {
    if (node == NULL)
        return;
    if (node->left == NULL && node->right == NULL) {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);
    } else {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    }
}

void huff_compress_file(BitWriter *outbuf, FILE *fin, uint32_t filesize, uint16_t num_leaves,
    Node *code_tree, Code *code_table) {
    bit_write_uint8(outbuf, 'H');
    bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, filesize);
    bit_write_uint16(outbuf, num_leaves);
    huff_write_tree(outbuf, code_tree);
    fseek(fin, 0, SEEK_SET);
    for (uint32_t i = 0; i < filesize; ++i) {
        int b = fgetc(fin);
        if (b == EOF)
            break;
        uint64_t code = code_table[b].code;
        uint8_t code_length = code_table[b].code_length;
        for (int j = 0; j < code_length; ++j) {
            bit_write_bit(outbuf, code & 1);
            code >>= 1;
        }
    }
}

int main(int argc, char *argv[]) {
    int opt;
    char *infile_path, *outfile_path;
    if (argc < 2) {
        fprintf(stderr, "Bad Input Files\n");
        fprintf(stderr, "%s", USAGE);
        return 1;
    }
    while ((opt = getopt(argc, argv, INPUTS)) != -1)
        switch (opt) {
        case 'h': fprintf(stdout, USAGE); return 0;

        case 'i': infile_path = optarg; break;

        case 'o': outfile_path = optarg; break;

        default:
            fprintf(stderr, "huff:  Invalid option\n");
            fprintf(stderr, USAGE);
            return 1;
        }
    if (infile_path == NULL || outfile_path == NULL) {
        fprintf(stderr, "huff: -i and -o options are required\n%s", USAGE);
        return 1;
    }
    FILE *fin = fopen(infile_path, "rb");
    uint32_t histogram[256] = { 0 };
    uint32_t filesize = fill_histogram(fin, histogram);
    uint16_t num_leaves;
    Node *code_tree = create_tree(histogram, &num_leaves);
    if (!code_tree) {
        fprintf(stderr, "Error creating Huffman tree\n");
        fclose(fin);
        return 1;
    }
    Code code_table[256] = { 0 };
    fill_code_table(code_table, code_tree, 0, 0);
    BitWriter *outbuf = bit_write_open(outfile_path);
    if (!outbuf) {
        fprintf(stderr, "Error opening input file");
        fclose(fin);
        node_free(&code_tree);
        return 1;
    }
    huff_compress_file(outbuf, fin, filesize, num_leaves, code_tree, code_table);
    node_print_tree(code_tree);
    fclose(fin);
    bit_write_close(&outbuf);
    node_free(&code_tree);
    return 0;
}
