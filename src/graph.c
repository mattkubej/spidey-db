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

Vertex createVertexRef(char *key) {
  Vertex ref = malloc(sizeof(Vertex));

  char *c_key = malloc(strlen(key) + 1);
  strcpy(c_key, key);
  ref->key = c_key;

  ref->value = NULL;
  ref->next = NULL;

  return ref;
}

void addEdge(Graph graph, char *v1_key, char *v2_key) {
  Vertex v1 = getVertex(graph, v1_key);
  Vertex v2_c = createVertexRef(v2_key);
  v2_c->next = v1->next;
  v1->next = v2_c;

  Vertex v2 = getVertex(graph, v2_key);
  Vertex v1_c = createVertexRef(v1_key);
  v1_c->next = v2->next;
  v2->next = v1_c;
}

Graph createGraph() {
  Graph graph = (Graph) malloc(sizeof(Graph));

  graph->v_dict = createDict();

  return graph;
}
