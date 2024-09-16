#include "bitreader.h"

#include <stdio.h>
#include <stdlib.h>
struct BitReader {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

BitReader *bit_read_open(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    BitReader *buf = malloc(sizeof(BitReader));
    if (buf == NULL) {
        fclose(file);
        perror("Error allocating memory for BitReader");
        return NULL;
    }

    buf->underlying_stream = file;
    buf->bit_position = 8; // Start with bit_position at 8 to force reading the first byte
    buf->byte = 0; // Initialize byte field

    return buf;
}
void bit_read_close(BitReader **pbuf) {
    if (*pbuf != NULL) {
        // Close the underlying_stream
        if (fclose((*pbuf)->underlying_stream) != 0) {
            // Error occurred while closing the stream
            fprintf(stderr, "Fatal error: Error closing file stream.\n");
            exit(1);
        }

        // Free *pbuf
        free(*pbuf);

        // Set *pbuf to NULL
        *pbuf = NULL;
    }
}
uint32_t bit_read_uint32(BitReader *buf) {
    uint32_t word = 0x00000000;
    for (int i = 0; i < 32; i++) {
        // Read a bit from the underlying_stream
        uint8_t bit = bit_read_bit(buf);
        // Check if reading failed
        if (bit == (uint8_t) EOF) {
            // Error reading from stream
            return (uint8_t) EOF;
        }
        // Set bit i of word to the value of bit
        word |= (unsigned int) ((bit & 1) << i);
    }
    return word;
}
uint16_t bit_read_uint16(BitReader *buf) {
    uint16_t word = 0x0000;
    for (int i = 0; i < 16; i++) {
        // Read a bit from the underlying_stream
        uint8_t bit = bit_read_bit(buf);
        // Check if reading failed
        if (bit == (uint8_t) EOF) {
            // Error reading from stream
            return (uint16_t) EOF;
        }
        // Set bit i of word to the value of bit
        word |= (bit & 1) << i;
    }
    return word;
}
uint8_t bit_read_bit(BitReader *buf) {
    // Check if bit_position is greater than or equal to 8
    if (buf->bit_position >= 8) {
        // Read the next byte from the stream
        int byte = fgetc(buf->underlying_stream);
        if (byte == EOF) {
            // Error reading from stream
            return (uint8_t) EOF;
        }
        // Update the byte field of the BitReader
        buf->byte = (uint8_t) byte;
        // Reset bit_position to 0
        buf->bit_position = 0;
    }
    // Extract the bit at the current bit_position
    uint8_t bit = (buf->byte >> buf->bit_position) & 1;
    // Increment bit_position
    buf->bit_position++;
    return bit;
}
uint8_t bit_read_uint8(BitReader *buf) {
    uint8_t byte = 0x00;
    // Read each bit and collect them into byte
    for (int i = 0; i < 8; i++) {
        uint8_t bit = bit_read_bit(buf);
        byte |= (bit << i);
    }
    return byte;
}
