#include "graph.h"
#include "dict.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void addVertex(Graph graph, char *key, char *value) {
  Vertex vertex = malloc(sizeof(Vertex));

  char *c_key = malloc(strlen(key) + 1);
  strcpy(c_key, key);
  vertex->key = c_key;

  char *c_value = malloc(strlen(value) + 1);
  strcpy(c_value, value);
  vertex->value = c_value;

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
