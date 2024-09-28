#include "path.h"

#include <stdlib.h>
/*
Path *path_create(uint32_t capacity);

void path_free(Path **pp);

uint32_t path_vertices(const Path *p);

uint32_t path_distance(const Path *p);

void path_add(Path *p, uint32_t val, const Graph *g);

uint32_t path_remove(Path *p, const Graph *g);

void path_copy(Path *dst, const Path *src);

void path_print(const Path *p, FILE *f, const Graph *g);
*/

typedef struct path {
    uint32_t total_weight;
    Stack *vertices;
} Path;

Path *path_create(uint32_t capacity) {
    Path *p = (Path *) malloc(sizeof(Path));
    p->total_weight = 0;
    p->vertices = stack_create(capacity);
    return p;
}

void path_free(Path **pp) {
    stack_free(&((*pp)->vertices));
    free(*pp);
    *pp = NULL;
}

uint32_t path_vertices(const Path *p) {
    if (p == NULL || p->vertices == NULL)
        return 0;
    return stack_size(p->vertices);
}

uint32_t path_distance(const Path *p) {
    if (p == NULL)
        return 0;
    return p->total_weight;
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    if (p == NULL || g == NULL || p->vertices == NULL)
        return;
    uint32_t a = 0;
    if (stack_peek(p->vertices, &a))
        p->total_weight += graph_get_weight(g, a, val);
    stack_push(p->vertices, val);
}

uint32_t path_remove(Path *p, const Graph *g) {
    if (p == NULL || g == NULL || p->vertices == NULL)
        return 264918264;
    uint32_t a = 0, b = 0;
    if (stack_pop(p->vertices, &a)) {
        if (stack_peek(p->vertices, &b)) {
            p->total_weight -= graph_get_weight(g, b, a);
        }
        return a;
    }
    return 264918264;
}

void path_copy(Path *dst, const Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->total_weight = src->total_weight;
}

void path_print(const Path *p, FILE *f, const Graph *g) {
    Stack *s = stack_create(stack_size(p->vertices));
    while (stack_empty(p->vertices) == false) {
        uint32_t a;
        stack_pop(p->vertices, &a);
        stack_push(s, a);
    }
    while (stack_empty(s) == false) {
        uint32_t a;
        stack_pop(s, &a);
        fprintf(f, "%s", graph_get_vertex_name(g, a));
        if (stack_empty(s) == false)
            fprintf(f, "\n");
    }
    stack_free(&s);
    fprintf(f, "\n");
}
