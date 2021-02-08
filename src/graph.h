#ifndef GRAPH_H
#define GRAPH_H

#include "dict.h"

struct Vertex {
  char *key;
  struct Vertex *next;
};

struct List {
  struct Vertex *head;
};

struct Graph {
  Dict v_dict;
};

struct Graph *createGraph();
struct Vertex *getVertex(struct Graph *graph, char *key);

#endif
