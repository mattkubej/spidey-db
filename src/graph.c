#include "graph.h"
#include "dict.h"

#include <stdio.h>
#include <stdlib.h>

void addVertex(struct Graph *graph, char *key) {
  struct Vertex *vertex = malloc(sizeof(struct Vertex));

  vertex->key = key;
  vertex->next = NULL;

  insertDictItem(graph->v_dict, key, vertex);
}

struct Vertex *getVertex(struct Graph *graph, char *key) {
  struct Vertex *vertex = getDictItemValue(graph->v_dict, key);

  return vertex;
}

struct Graph *createGraph() {
  struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));

  graph->v_dict = createDict();

  return graph;
}
