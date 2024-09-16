#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define INPUTS "i:o:dh"
#define USAGE                                                                                      \
    "Usage: tsp [options]\n\n"                                                                     \
    "-i infile    Specify the input file path containing the cities and edges\n"                   \
    "             of a graph. If not specified, the default input should be\n"                     \
    "             set as stdin.\n\n"                                                               \
    "-o outfile   Specify the output file path to print to. If not specified,\n"                   \
    "             the default output should be set as stdout.\n\n"                                 \
    "-d           Specifies the graph to be directed.\n\n"                                         \
    "-h           Prints out a help message describing the purpose of the\n"                       \
    "             graph and the command-line options it accepts, exiting the\n"                    \
    "             program afterwards.\n"

void dfs(Graph *g, uint32_t vertex, Path *p, Path *optimal) {
    path_add(p, vertex, g);
    graph_visit_vertex(g, vertex);

    if ((path_vertices(p) == graph_vertices(g)) && graph_get_weight(g, vertex, START_VERTEX)) {
        if (path_vertices(optimal) == 0)
            path_copy(optimal, p);
        else if (path_distance(optimal) > path_distance(p))
            path_copy(optimal, p);
    }
    for (uint32_t i = 0; i < graph_vertices(g); i++)
        if (!graph_visited(g, i) && graph_get_weight(g, vertex, i) > 0)
            dfs(g, i, p, optimal);
    path_remove(p, g);
    graph_unvisit_vertex(g, vertex);
}

int main(int argc, char *argv[]) {
    FILE *infile = stdin;
    FILE *outfile = stdout;
    bool d = false;
    int opt;
    while ((opt = getopt(argc, argv, INPUTS)) != -1) {
        switch (opt) {
        case 'h': fprintf(stdout, USAGE); return 1;
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'd': d = true; break;
        case '?': fprintf(stderr, USAGE); return 1;
        }
    }
    if (infile == NULL || outfile == NULL) {
        printf("error opening file\n");
        return 1;
    }
    uint32_t vertices, edges;
    if (fscanf(infile, "%u\n", &vertices) != 1) {
        fprintf(stderr, "tsp: error reading number of vertices\n");
        return 1;
    }

    Graph *g = graph_create(vertices, d);
    char name[1000];
    for (uint32_t i = 0; i < vertices; i++) {
        if (fgets(name, sizeof(name), infile) == NULL)
            return 1;
        name[strlen(name) - 1] = '\0';
        graph_add_vertex(g, name, i);
    }
    if (fscanf(infile, "%u\n", &edges) != 1) {
        fprintf(stderr, "tsp: must provide number of edges\n");
        return 1;
    }
    uint32_t x, y, z;
    for (uint32_t i = 0; i < edges; i++) {
        if (fscanf(infile, "%u %u %u\n", &x, &y, &z) != 3) {
            fprintf(stderr, "tsp: error reading edge\n");
            return 1;
        }
        graph_add_edge(g, x, y, z);
    }

    Path *p = path_create(vertices + 1);
    Path *optimal = path_create(vertices + 1);
    dfs(g, START_VERTEX, p, optimal);

    if (path_vertices(optimal) != 0) {
        path_add(optimal, START_VERTEX, g);
        fprintf(outfile, "Alissa starts at:\n");
        path_print(optimal, outfile, g);
        fprintf(outfile, "Total Distance: %u\n", path_distance(optimal));
    } else {
        fprintf(outfile, "No path found! Alissa is lost!\n");
    }
    if (outfile != stdout)
        fclose(outfile);
    if (infile != stdin)
        fclose(infile);
    path_free(&optimal);
    path_free(&p);
    graph_free(&g);

    return 0;
}
