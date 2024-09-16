#ifndef _ITEM
#define _ITEM

#include <stdbool.h>

typedef struct item item;

struct item {
    char key[255];
    int id;
};

bool cmp(item *, item *);

#endif
