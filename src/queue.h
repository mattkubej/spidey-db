#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

#define GROWTH_FACTOR 2

struct queue {
  size_t size;
  size_t count;
  size_t front;
  size_t rear;
  char **array;
};

typedef struct queue *Queue;

Queue createQueue(size_t size);
void destroyQueue(Queue queue);
int isQueueEmpty(Queue queue);
void enqueue(Queue queue, char *item);
char *dequeue(Queue queue);
char *front(Queue queue);
char *rear(Queue queue);

#endif
