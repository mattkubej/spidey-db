#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

struct Queue {
  int front, rear, count;
  size_t size;
  char **array;
};

struct Queue *createQueue(size_t size);
int isQueueEmpty(struct Queue *queue);
void enqueue(struct Queue *queue, char *item);
char *dequeue(struct Queue *queue);
char *front(struct Queue *queue);
char *rear(struct Queue *queue);

#endif
