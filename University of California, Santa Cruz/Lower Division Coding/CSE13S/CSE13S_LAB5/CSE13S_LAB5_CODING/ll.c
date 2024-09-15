#include "ll.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

LL *list_create() {
    LL *l = (LL *) malloc(sizeof(LL));
    if (l == NULL)
        return NULL;
    l->head = NULL;
    l->tail = NULL;
    return l;
}

bool list_add(LL *l, item *i) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n == NULL)
        return false;
    n->data = *i;
    n->next = NULL;
    if (l->head == NULL) {
        l->head = n;
        l->tail = n;
        return true;
    } else {
        Node *end = l->tail;
        end->next = n;
        l->tail = n;
        return true;
    }
}

item *list_find(LL *l, bool (*cmp)(item *, item *), item *i) {
    Node *n = l->head;
    while (n != NULL) {
        if (cmp(&n->data, i))
            return &n->data;
        n = n->next;
    }
    return NULL;
}

void list_destroy(LL **l) {
    if (l == NULL || *l == NULL)
        return;
    Node *current = (*l)->head, *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(*l);
    *l = NULL;
}

void list_remove(LL *l, bool (*cmp)(item *, item *), item *i) {
    if (l == NULL || l->head == NULL)
        return;
    Node *current = l->head, *prev = l->head;
    while (current != NULL) {
        if (cmp(&(current->data), i)) {
            if (current == l->head)
                l->head = (l->head)->next;
            if (current == l->tail) {
                l->tail = prev;
                if (l->tail != NULL)
                    (l->tail)->next = NULL;
            }
            if ((current != l->head) && (current != l->tail))
                prev->next = current->next;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
