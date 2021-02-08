#include "graph.h"
#include "dict.h"

#include <stdio.h>
#include <stdlib.h>

void addVertex(Graph graph, char *key) {
  Vertex vertex = malloc(sizeof(Vertex));

  vertex->key = key;
  vertex->next = NULL;

  insertDictItem(graph->v_dict, key, vertex);
}

Vertex getVertex(Graph graph, char *key) {
  Vertex vertex = getDictItemValue(graph->v_dict, key);

  return vertex;
}

Graph createGraph() {
  Graph graph = (Graph) malloc(sizeof(Graph));

  graph->v_dict = createDict();

  return graph;
}
