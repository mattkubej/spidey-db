#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

struct queue {
  int front, rear, count;
  size_t size;
  char **array;
};

typedef struct queue *Queue;

Queue createQueue(size_t size);
int isQueueEmpty(Queue queue);
void enqueue(Queue queue, char *item);
char *dequeue(Queue queue);
char *front(Queue queue);
char *rear(Queue queue);

#endif
