#include "ll.h"

#include <stdio.h>
#include <string.h>

void *abstractwords(int, void *, void (*dsdo)(void *, char *, int));

void listdo(void *ds, char *line, int i) {
    LL *l = (LL *) ds;
    item it;
    strcpy(it.key, line);
    it.id = i;
    list_add(l, &it);
}

LL *listwords(void) {
    LL *l = list_create();
    return (LL *) abstractwords(1, (void *) l, listdo);
}

LL *getwords(void) {
    return listwords();
}
