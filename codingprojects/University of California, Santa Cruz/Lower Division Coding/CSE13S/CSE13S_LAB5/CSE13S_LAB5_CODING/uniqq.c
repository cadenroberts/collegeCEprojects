#include "hash.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
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
    FILE *file = fdopen(fd, "r");
    Hashtable *h = hash_create();
    if (h == NULL)
        return 1;
    int a = 0, i;
    char ch;
    char s[1000000];
    while ((i = fgetc(file)) != -1) {
        if (i > 31 && i < 126)
            ch = (char) i;
        else
            ch = '\n';
        if (ch != '\n')
            s[a++] = ch;
        else {
            s[a] = '\0';
            hash_put(h, s, 1);
            for (int i = 0; i <= a; i++)
                s[i] = '\0';
            a = 0;
        }
    }
    Node *n;
    a = 0;
    for (int i = 0; i < 1000; i++) {
        if ((h->table[i]) != NULL) {
            n = (h->table[i])->head;
            while (n != NULL) {
                a++;
                n = n->next;
            }
        }
    }
    printf("%d\n", a);
    if (fd != STDIN_FILENO) {
        close(fd);
    }
    return 0;
}
