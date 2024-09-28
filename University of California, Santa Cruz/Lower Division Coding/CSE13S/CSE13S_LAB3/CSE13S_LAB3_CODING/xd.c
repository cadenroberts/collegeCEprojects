#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd;
    if (argc == 1) {
        fd = STDIN_FILENO;
    } else if (argc == 2) {
        fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
            return 1;
        }
    } else {
        return 1;
    }

    unsigned char buffer[16];
    ssize_t bytes;
    unsigned long offset = 0;
    while (1) {
        bytes = read(fd, buffer, sizeof(buffer));
        if (bytes == 0)
            break;
        if (bytes < 0) {
            if (bytes == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    usleep(100000);
                    continue;
                }
            } else
                return 1;
        }

        printf("%08lx: ", offset);

        for (ssize_t i = 0; i < 16; i++) {
            if (i < bytes) {
                printf("%02x", buffer[i]);
            } else {
                printf("  ");
            }

            if (i % 2) {
                printf(" ");
            }
        }
        printf(" ");
        for (ssize_t i = 0; i < bytes; i++) {
            unsigned char c = buffer[i];
            if (c >= 32 && c <= 126) {
                printf("%c", c);
            } else {
                printf(".");
            }
        }

        printf("\n");
        offset += 16;
    }

    if (fd != STDIN_FILENO) {
        close(fd);
    }

    return 0;
}
