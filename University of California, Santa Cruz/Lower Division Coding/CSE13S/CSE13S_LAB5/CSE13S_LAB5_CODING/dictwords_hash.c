#include "hash.h"

#include <stdio.h>
#include <string.h>

void *abstractwords(int, void *, void (*dsdo)(void *, char *, int));

void hashdo(void *ds, char *line, int i) {
    Hashtable *h = (Hashtable *) ds;
    int *ret = hash_get(h, line);
    if (ret == NULL) {
        hash_put(h, line, i);
    }
}

Hashtable *uniqwords(int iters) {
    Hashtable *h = hash_create();
    return (Hashtable *) abstractwords(iters, (void *) h, hashdo);
}
