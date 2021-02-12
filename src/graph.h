#ifndef GRAPH_H
#define GRAPH_H

#include "dict.h"

struct vertex {
  char *key;
  char *value;
  struct vertex *next;
};

struct graph {
  Dict v_dict;
};

typedef struct vertex *Vertex;
typedef struct graph *Graph;

Graph createGraph();
void addVertex(Graph graph, char *key, char *value);
Vertex getVertex(Graph graph, char *key);
void addEdge(Graph graph, char *v1_key, char *v2_key);
void getNeighbors(Graph graph, char *key, int distance);

#endif
