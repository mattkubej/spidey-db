#include "graph.h"
#include "dict.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

EdgeList getNeighbors(Graph graph, char *key, int distance) {
  EdgeList el = malloc(sizeof(EdgeList));
  el->count = 0;
  el->head = NULL;

  Edge e_prev = NULL;

  Vertex v = getVertex(graph, key);
  Vertex dest = v->next;

  while (dest != NULL) {
    Edge e = malloc(sizeof(Edge));

    char *c_src_key = malloc(strlen(v->key) + 1);
    strcpy(c_src_key, v->key);
    e->src_key = c_src_key;

    char *c_dest_key = malloc(strlen(dest->key) + 1);
    strcpy(c_dest_key, dest->key);
    e->dest_key = c_dest_key;

    if (el->head == NULL) {
      el->head = e;
      e_prev = e;
    } else {
      e_prev->next = e;
      e_prev = e;
    }

    dest = dest->next;
  }

  return el;
}

EdgeList bfs(Graph graph, char *key, int distance) {
  EdgeList el = malloc(sizeof(EdgeList));
  Vertex v = getVertex(graph, key);
  Dict visited_dict = createDict();

  bool *has_visited = malloc(sizeof(bool));
  *has_visited = true;
  insertDictItem(visited_dict, v->key, has_visited);

  return el;
}

Graph createGraph() {
  Graph graph = (Graph)malloc(sizeof(Graph));

  graph->v_dict = createDict();

  return graph;
}
