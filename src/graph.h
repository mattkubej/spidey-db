#ifndef GRAPH_H
#define GRAPH_H

#include "dict.h"

struct vertex {
  char *key;
  struct vertex *next;
};

struct graph {
  Dict v_dict;
};

typedef struct vertex *Vertex;
typedef struct graph *Graph;

Graph createGraph();
void addVertex(Graph graph, char *key);
Vertex getVertex(Graph graph, char *key);

#endif
