#include "hash.h"

#include "badhash.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Hashtable *hash_create() {
    Hashtable *ht = (Hashtable *) malloc(sizeof(Hashtable));
    if (ht == NULL)
        return NULL;
    for (size_t i = 0; i < 1000; i++)
        ht->table[i] = list_create();
    return ht;
}

bool hash_put(Hashtable *ht, char *key, int val) {
    if (ht == NULL || key == NULL)
        return false;
    item i;
    strcpy(i.key, key);
    i.id = val;
    item *d = list_find((ht->table[hash(key) % 1000]), cmp, &i);
    if (d != NULL) {
        d->id = val;
        return true;
    }
    return list_add((ht->table[hash(key) % 1000]), &i);
}

int *hash_get(Hashtable *ht, char *key) {
    if (ht == NULL || key == NULL)
        return NULL;
    item i;
    strcpy(i.key, key);
    item *d = list_find((ht->table[hash(key) % 1000]), cmp, &i);
    if (d != NULL)
        return &(d->id);
    else
        return NULL;
}

void hash_destroy(Hashtable **ht) {
    if (*ht == NULL)
        return;
    LL **l = NULL;
    for (int i = 0; i < 1000; i++) {
        l = &((*ht)->table[i]);
        list_destroy(l);
        free((*ht)->table[i]);
    }
    free(*ht);
    *ht = NULL;
}
