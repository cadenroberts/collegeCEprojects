#include "dictwords.h"
#include "hash.h"

#include <stdio.h>
#include <string.h>

int main() {
    LL *l = getwords();

    // some sanity checks.
    //
    item look;
    strcpy(look.key, "idempotent");
    item *ret = list_find(l, cmp, &look);
    if (ret == NULL || (*ret).id != 56613) {
        printf("something went wrong\n");
        return 1;
    }

    printf("SUCCESS!\n");
    return 0;
}
