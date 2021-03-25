#include "graph.h"
#include "dict.h"
#include "queue.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Graph createGraph() {
  Graph graph = malloc(sizeof(*graph));

  graph->v_dict = createDict();

  return graph;
}

void destroyGraph(Graph graph) {
  destroyDict(graph->v_dict);
  free(graph);
}

void addVertex(Graph graph, char *key, char *value) {
  Vertex vertex = malloc(sizeof(*vertex));

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
  Vertex ref = malloc(sizeof(*ref));

  char *c_key = malloc(strlen(key) + 1);
  strcpy(c_key, key);
  ref->key = c_key;

  ref->value = NULL;
  ref->next = NULL;

  return ref;
}

void addEdge(Graph graph, char *v1_key, char *v2_key) {
  // creates a logical edge with adjacency list
  Vertex v1 = getVertex(graph, v1_key);
  Vertex v2_c = createVertexRef(v2_key);
  v2_c->next = v1->next;
  v1->next = v2_c;

  Vertex v2 = getVertex(graph, v2_key);
  Vertex v1_c = createVertexRef(v1_key);
  v1_c->next = v2->next;
  v2->next = v1_c;
}

// --- getNeighbors and helpers ---

Neighbors createNeighbors() {
  Neighbors neighbors = malloc(sizeof(*neighbors));

  neighbors->edge_count = 0;
  neighbors->vertex_count = 0;
  neighbors->edge_head = NULL;
  neighbors->vertex_head = NULL;

  return neighbors;
}

void destroyNeighbors(Neighbors neighbors) {
  while (neighbors->vertex_head != NULL) {
    Vertex v = neighbors->vertex_head;
    neighbors->vertex_head = neighbors->vertex_head->next;

    free(v->key);
    free(v->value);
    free(v);
  }

  while (neighbors->edge_head != NULL) {
    Edge e = neighbors->edge_head;
    neighbors->edge_head = neighbors->edge_head->next;

    free(e->src_key);
    free(e->dest_key);
    free(e);
  }

  free(neighbors);
}

Edge createEdge(Vertex src, Vertex dest) {
  Edge e = malloc(sizeof(*e));
  memset(e, 0, sizeof(*e));

  char *c_src_key = malloc(strlen(src->key) + 1);
  strcpy(c_src_key, src->key);
  e->src_key = c_src_key;

  char *c_dest_key = malloc(strlen(dest->key) + 1);
  strcpy(c_dest_key, dest->key);
  e->dest_key = c_dest_key;

  return e;
}

void enqueueUnvisitedVertex(Vertex v, Queue unvisited_queue) {
  char *v_key = malloc(strlen(v->key) + 1);
  strcpy(v_key, v->key);
  enqueue(unvisited_queue, v_key);
}

void flagUnvisitedVertex(Vertex v, Dict visited_dict) {
  bool *flag = malloc(sizeof(*flag));
  *flag = true;
  insertDictItem(visited_dict, v->key, flag);
}

Vertex shallowCopyVertex(Vertex v) {
  Vertex c_vertex = malloc(sizeof(*c_vertex));
  memset(c_vertex, 0, sizeof(*c_vertex));

  c_vertex->value = NULL;

  char *c_v_key = malloc(strlen(v->key) + 1);
  strcpy(c_v_key, v->key);
  c_vertex->key = c_v_key;

  return c_vertex;
}

Neighbors getNeighbors(Graph graph, char *key, int distance) {
  Vertex v = getVertex(graph, key);

  // flag unvisited vertices, only process once
  Dict unvisited_dict = createDict();
  unvisited_dict->flags |= DICT_FREE_VALUES;
  flagUnvisitedVertex(v, unvisited_dict);

  // queue of neighbors to visit
  Queue unvisited_queue = createQueue(1024);
  enqueueUnvisitedVertex(v, unvisited_queue);

  // list of neighbors
  Neighbors neighbors = createNeighbors();

  // dummy vertex header
  neighbors->vertex_head = malloc(sizeof(*neighbors->vertex_head));
  memset(neighbors->vertex_head, 0, sizeof(*neighbors->vertex_head));

  // dummy edge header
  neighbors->edge_head = malloc(sizeof(*neighbors->edge_head));
  memset(neighbors->edge_head, 0, sizeof(*neighbors->edge_head));

  Vertex v_tail = neighbors->vertex_head;
  Edge e_tail = neighbors->edge_head;
  int distance_from_root = 0;

  while (!isQueueEmpty(unvisited_queue) && distance_from_root <= distance) {
    char *unvisited_v_key = dequeue(unvisited_queue);
    Vertex v_src = getVertex(graph, unvisited_v_key);
    free(unvisited_v_key);

    // append new vertex
    Vertex c_vertex = shallowCopyVertex(v_src);
    v_tail->next = c_vertex;
    v_tail = c_vertex;
    neighbors->vertex_count++;

    // first neighbor to visit
    Vertex v_dest = v_src->next;

    while (v_dest != NULL) {
      // append new edge
      Edge e = createEdge(v_src, v_dest);
      e_tail->next = e;
      e_tail = e;
      neighbors->edge_count++;

      // if vertex not flagged, then add neighbor to unvisited queue
      if (!getDictItemValue(unvisited_dict, v_dest->key)) {
        flagUnvisitedVertex(v_dest, unvisited_dict);
        enqueueUnvisitedVertex(v_dest, unvisited_queue);
      }

      // move to next unvisted vertex
      v_dest = v_dest->next;
    }

    distance_from_root++;
  }

  // clean up dummy vertex on head
  Vertex v_temp = neighbors->vertex_head;
  neighbors->vertex_head = neighbors->vertex_head->next;
  free(v_temp);

  // clean up dummy edge on head
  Edge e_temp = neighbors->edge_head;
  neighbors->edge_head = neighbors->edge_head->next;
  free(e_temp);

  destroyDict(unvisited_dict);
  destroyQueue(unvisited_queue);

  return neighbors;
}
