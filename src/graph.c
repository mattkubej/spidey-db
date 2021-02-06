#include <stdio.h>
#include <stdlib.h>

struct Node {
  int key;
  struct Node *next;
};

struct List {
  struct Node *head;
};

struct Graph {
  int size;
  struct List *list;
};

struct Node *createNode(int key) {
  struct Node *node = malloc(sizeof(struct Node));

  node->key = key;
  node->next = NULL;

  return node;
}

struct Graph *createGraph(int size) {
  struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));

  graph->size= size;
  graph->list = (struct List*) malloc(size* sizeof(struct List));

  for (int i = 0; i < size; i++) {
    graph->list[i].head = NULL;
  }

  return graph;
}

void addEdge(struct Graph *graph, int src, int dest) {
  struct Node *node = createNode(dest);
  node->next = graph->list[src].head;
  graph->list[src].head = node;

  node = createNode(src);
  node->next = graph->list[dest].head;
  graph->list[dest].head = node;
}
