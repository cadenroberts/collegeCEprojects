#include "item.h"

#include <stdio.h>
#include <string.h>

// implementation of the cmp() function on items, for when items are ints
bool cmp(item *i1, item *i2) {
    if (i1 == NULL || i2 == NULL)
        return 0;
    return strcmp(i1->key, i2->key) == 0 ? 1 : 0;
}
