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

void listdo(void *ds, char *line, int i) {
    LL *l = (LL *) ds;
    item it;
    strcpy(it.key, line);
    it.id = i;
    list_add(l, it);
}

Hashtable *uniqwords(int iters) {
    Hashtable *h = hash_create();
    return (Hashtable *) abstractwords(iters, (void *) h, hashdo);
}

LL *listwords(void) {
    LL *l = list_create();
    return (LL *) abstractwords(1, (void *) l, listdo);
}

void *abstractwords(int iters, void *ds, void (*dsdo)(void *, char *, int)) {
    ssize_t read;
    char *line = NULL;
    size_t len = 0;
    int i = 0;

    for (int times = 0; times < iters; times++) {
        FILE *fp = fopen("/usr/share/dict/words", "r");
        if (fp == NULL) {
            perror("no dictionary file\n");
            return NULL;
        }
        while ((read = getline(&line, &len, fp)) != -1) {
            line[read - 1] = '\0';
            dsdo(ds, line, i);
            i++;
        }
        fclose(fp);
    }
    return ds;
}

LL *getwords(void) {
    return listwords();
}
