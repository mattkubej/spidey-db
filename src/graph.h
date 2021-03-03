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

struct neighbors {
  size_t vertex_count;
  struct vertex *vertex_head;
  size_t edge_count;
  struct edge *edge_head;
};

struct graph {
  Dict v_dict;
};

typedef struct vertex *Vertex;
typedef struct edge *Edge;
typedef struct neighbors *Neighbors;
typedef struct graph *Graph;

Graph createGraph();
void destroyGraph(Graph graph);
void addVertex(Graph graph, char *key, char *value);
Vertex getVertex(Graph graph, char *key);
void addEdge(Graph graph, char *v1_key, char *v2_key);
Neighbors getNeighbors(Graph graph, char *key, int distance);

#endif
