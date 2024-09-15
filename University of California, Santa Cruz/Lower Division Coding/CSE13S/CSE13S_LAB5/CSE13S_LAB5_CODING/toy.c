#include "ll.h"

#include <stdio.h>
#include <string.h>

int main() {
    LL *l = list_create();
    for (int i = 0; i < 1000; i++) {
        item a;
        a.id = i;
        a.key[0] = (char) ((i % 93) + 33);
        a.key[1] = '\0';
        list_add(l, &a);
    }
    item MAGICN;
    MAGICN.id = 457;
    strcpy(MAGICN.key, "MAGICN");
    item b = MAGICN;
    list_remove(l, cmp, &b);

    item *ret;
    for (int i = 0; i < 1000; i++) {
        if (i != MAGICN.id) {
            item a;
            a.id = i;
            a.key[0] = (char) ((i % 93) + 33);
            a.key[1] = '\0';
            ret = list_find(l, cmp, &a);
            if (ret == NULL) {
                printf("something went wrong. I inserted %d into the list but then could not find "
                       "it.\n",
                    i);
                return 1;
            }
        }
    }
    list_destroy(&l);
    return 0;
}
