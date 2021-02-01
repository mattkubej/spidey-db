#include <stdio.h>
#include <stdlib.h>

// based on https://www.geeksforgeeks.org/graph-and-its-representations/

struct ListNode {
  int dest;
  struct ListNode *next;
};

struct List {
  struct ListNode *head;
};

struct Graph {
  int V;
  struct List *array;
};

struct ListNode *createListNode(int dest) {
  struct ListNode *node = malloc(sizeof(struct ListNode));

  node->dest = dest;
  node->next = NULL;

  return node;
}

struct Graph *createGraph(int V) {
  struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));

  graph->V = V;
  graph->array = (struct List*) malloc(V * sizeof(struct List));

  for (int i = 0; i < V; i++) {
    graph->array[i].head = NULL;
  }

  return graph;
}

void addEdge(struct Graph *graph, int src, int dest) {
  struct ListNode *node = createListNode(dest);
  node->next = graph->array[src].head;
  graph->array[src].head = node;

  node = createListNode(src);
  node->next = graph->array[dest].head;
  graph->array[dest].head = node;
}
