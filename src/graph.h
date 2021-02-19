#ifndef GRAPH_H
#define GRAPH_H

#include "dict.h"

struct vertex {
  char *key;
  char *value;
  struct vertex *next;
};

struct edge {
  char *src_key;
  char *dest_key;
  struct edge *next;
};

struct edge_list {
  size_t count;
  struct edge *head;
};

struct graph {
  Dict v_dict;
};

typedef struct vertex *Vertex;
typedef struct edge *Edge;
typedef struct edge_list *EdgeList;
typedef struct graph *Graph;

Graph createGraph();
void addVertex(Graph graph, char *key, char *value);
Vertex getVertex(Graph graph, char *key);
void addEdge(Graph graph, char *v1_key, char *v2_key);
EdgeList getNeighbors(Graph graph, char *key, int distance);

#endif
