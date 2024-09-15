/*
#ifndef GRAPH
#define GRAPH

struct graph;
typedef struct graph Graph;

Graph *graph_create(uint32_t vertices, bool directed);

void graph_free(Graph **gp);

uint32_t graph_vertices(const Graph *g);

void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight);

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end);

void graph_visit_vertex(Graph *g, uint32_t v);

void graph_unvisit_vertex(Graph *g, uint32_t v);

bool graph_visited(const Graph *g, uint32_t v);

char **graph_get_names(const Graph *g);

void graph_add_vertex(Graph *g, const char *name, uint32_t v);

const char *graph_get_vertex_name(const Graph *g, uint32_t v);

void graph_print(const Graph *g);
*/

#include "graph.h"

#include "string.h"
#include "vertices.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct graph {
    uint32_t vertices;
    bool directed;
    bool *visited;
    char **names;
    uint32_t **weights;
} Graph;

Graph *graph_create(uint32_t vertices, bool directed) {
    Graph *g = calloc(1, sizeof(Graph));
    g->vertices = vertices;
    g->directed = directed;
    // use calloc to initialize everything with zeroes
    g->visited = calloc(vertices, sizeof(bool));
    g->names = calloc(vertices, sizeof(char *));
    // allocate g->weights with a pointer for each row
    g->weights = calloc(vertices, sizeof(g->weights[0]));
    // allocate each row in the adjacency matrix
    for (uint32_t i = 0; i < vertices; ++i)
        g->weights[i] = calloc(vertices, sizeof(g->weights[0][0]));
    return g;
}

void graph_free(Graph **gp) {
    if (gp == NULL || *gp == NULL)
        return; // Nothing to free

    Graph *g = *gp;

    // Free the visited array
    free(g->visited);

    // Free the names array and the memory for each individual string
    if (g->names != NULL) {
        for (uint32_t i = 0; i < g->vertices; ++i)
            free(g->names[i]);
        free(g->names);
    }

    // Free the weights array and the memory for each individual row
    if (g->weights != NULL) {
        for (uint32_t i = 0; i < g->vertices; ++i)
            free(g->weights[i]);
        free(g->weights);
    }

    // Free the Graph struct itself
    free(g);

    // Set the pointer to NULL to indicate that memory has been freed
    *gp = NULL;
}

uint32_t graph_vertices(const Graph *g) {
    return g->vertices;
}

void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight) {
    g->weights[start][end] = weight;
    if (!g->directed)
        g->weights[end][start] = weight;
}

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    return g->weights[start][end];
}

void graph_visit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = true;
}

void graph_unvisit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = false;
}

bool graph_visited(const Graph *g, uint32_t v) {
    return g->visited[v];
}

char **graph_get_names(const Graph *g) {
    return g->names;
}

void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    if (g->names[v])
        free(g->names[v]);
    g->names[v] = strdup(name);
}

const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    return g->names[v];
}

void graph_print(const Graph *g) {
    if (g == NULL) {
        printf("Graph is NULL.\n");
        return;
    }

    printf("Graph:\n");
    printf("Number of vertices: %u\n", g->vertices);
    printf("Directed: %s\n", g->directed ? "Yes" : "No");

    // Print vertex names
    printf("Vertex Names:\n");
    for (uint32_t i = 0; i < g->vertices; ++i) {
        printf("%u: %s\n", i, g->names[i]);
    }

    // Print adjacency matrix
    printf("Adjacency Matrix:\n");
    for (uint32_t i = 0; i < g->vertices; ++i) {
        for (uint32_t j = 0; j < g->vertices; ++j) {
            printf("%u ", g->weights[i][j]);
        }
        printf("\n");
    }
}
