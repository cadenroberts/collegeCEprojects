#include "bitwriter.h"

#include <stdio.h>
#include <stdlib.h>

struct BitWriter {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

BitWriter *bit_write_open(const char *filename) {
    BitWriter *b = calloc(1, sizeof(BitWriter));
    if (b == NULL)
        return NULL;
    b->underlying_stream = fopen(filename, "w");
    if (b->underlying_stream == NULL) {
        free(b);
        return NULL;
    }
    b->byte = b->bit_position = 0;
    return b;
}

void bit_write_close(BitWriter **pbuf) {
    if (*pbuf != NULL) {
        BitWriter *buf = *pbuf;
        if (buf->bit_position > 0) {
            // Flush any remaining bits in the byte to the underlying_stream
            if (fputc(buf->byte, buf->underlying_stream) == EOF) {
                perror("Error writing byte to stream");
                exit(EXIT_FAILURE);
            }
        }
        // Close the underlying_stream
        if (fclose(buf->underlying_stream) != 0) {
            perror("Error closing stream");
            exit(EXIT_FAILURE);
        }
        // Free the BitWriter object
        free(buf);
        // Set the pointer to NULL
        *pbuf = NULL;
    }
}
void bit_write_bit(BitWriter *buf, uint8_t bit) {
    if (buf->bit_position > 7) {
        // Write the byte to the underlying_stream using fputc()
        int result = fputc(buf->byte, buf->underlying_stream);
        // Check if fputc failed
        if (result == EOF) {
            // Report a fatal error
            fprintf(stderr, "Fatal error: Writing byte failed\n");
            exit(1); // or handle the error as appropriate
        }
        // Clear the byte and bit_position fields of the BitWriter
        buf->byte = 0;
        buf->bit_position = 0;
    }
    // Set the bit at bit_position of the byte to the value of bit
    buf->byte |= (bit & 1) << buf->bit_position;
    // Increment bit_position
    buf->bit_position++;
}
void bit_write_uint16(BitWriter *buf, uint16_t x) {
    for (int i = 0; i < 16; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}
void bit_write_uint32(BitWriter *buf, uint32_t x) {
    for (int i = 0; i < 32; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}
void bit_write_uint8(BitWriter *buf, uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        bit_write_bit(buf, (byte >> i) & 1);
    }
}
