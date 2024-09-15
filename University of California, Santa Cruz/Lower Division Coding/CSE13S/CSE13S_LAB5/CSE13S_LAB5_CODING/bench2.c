#include "dictwords.h"
#include "hash.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    int times = 1;
    if (argc > 1) {
        for (size_t i = 0; i < strlen(argv[1]); i++) {
            if (!isdigit(argv[1][i])) {
                perror("invalid input");
                return 1;
            }
        }
        times = atoi(argv[1]);
    }

    Hashtable *h = uniqwords(times);

    // some sanity checks.
    //
    int *ret = hash_get(h, "idempotent");
    if (ret == NULL || *ret != 56613) {
        printf("something went wrong\n");
        return 1;
    }

    ret = hash_get(h, "fractal");
    if (ret == NULL || *ret != 49781) {
        printf("something went wrong %d\n", *ret);
        return 1;
    }

    printf("SUCCESS!\n");
    return 0;
}
