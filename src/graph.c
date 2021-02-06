#include <stdio.h>
#include <stdlib.h>

struct Node {
  int vertex;
  struct Node *next;
};

struct List {
  struct Node *head;
};

struct Graph {
  int size;
  struct List *array;
};

struct Node *createNode(int vertex) {
  struct Node *node = malloc(sizeof(struct Node));

  node->vertex = vertex;
  node->next = NULL;

  return node;
}

struct Graph *createGraph(int size) {
  struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));

  graph->size= size;
  graph->array = (struct List*) malloc(size* sizeof(struct List));

  for (int i = 0; i < size; i++) {
    graph->array[i].head = NULL;
  }

  return graph;
}

void addEdge(struct Graph *graph, int src, int dest) {
  struct Node *node = createNode(dest);
  node->next = graph->array[src].head;
  graph->array[src].head = node;

  node = createNode(src);
  node->next = graph->array[dest].head;
  graph->array[dest].head = node;
}
