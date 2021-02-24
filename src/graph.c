#include "graph.h"
#include "dict.h"
#include "queue.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// TODO: clean this code up
EdgeList bfs(Graph graph, char *key, int distance) {
  EdgeList el = malloc(sizeof(EdgeList));
  Vertex v = getVertex(graph, key);
  Dict visited_dict = createDict();

  bool *has_visited = malloc(sizeof(bool));
  *has_visited = true;
  insertDictItem(visited_dict, v->key, has_visited);

  Queue queue = createQueue(1024);
  char *v_key = malloc(strlen(v->key) + 1);
  strcpy(v_key, v->key);
  enqueue(queue, v_key);
  Edge e_prev = NULL;

  while (!isQueueEmpty(queue)) {
    char* item = front(queue);
    Vertex test = getVertex(graph, item);
    dequeue(queue);

    Vertex dest = test->next;

    while (dest != NULL) {
      Edge e = malloc(sizeof(Edge));

      char *c_src_key = malloc(strlen(test->key) + 1);
      strcpy(c_src_key, test->key);
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

      if (!getDictItemValue(visited_dict, c_dest_key)) {
        char *vstd_key = malloc(strlen(c_dest_key) + 1);
        strcpy(vstd_key, c_dest_key);
        insertDictItem(visited_dict, vstd_key, has_visited);
        enqueue(queue, vstd_key);
      }

      dest = dest->next;
    }
  }

  return el;
}

Graph createGraph() {
  Graph graph = (Graph)malloc(sizeof(Graph));

  graph->v_dict = createDict();

  return graph;
}
