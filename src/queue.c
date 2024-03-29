#include "queue.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

Queue createQueue(size_t size) {
  Queue queue = malloc(sizeof(*queue));

  queue->size = size;
  queue->front = queue->count = 0;
  queue->rear = queue->size - 1;

  queue->array = malloc(sizeof(char *) * queue->size);

  return queue;
}

void destroyQueue(Queue queue) {
  while (!isQueueEmpty(queue)) {
    char *item = dequeue(queue);
    free(item);
  }

  free(queue->array);
  free(queue);
}

int isQueueEmpty(Queue queue) { return queue->count == 0; }

void growQueue(Queue queue) {
  Queue temp = createQueue(queue->size * GROWTH_FACTOR);

  for (size_t i = 0; i < queue->count; i++) {
    enqueue(temp, dequeue(queue));
  }

  Queue swap = queue;
  queue = temp;
  temp = swap;

  destroyQueue(temp);
}

void enqueue(Queue queue, char *item) {
  queue->rear = (queue->rear + 1) % queue->size;
  queue->array[queue->rear] = item;
  queue->count = queue->count + 1;

  if (queue->count >= queue->size) {
    growQueue(queue);
  }
}

char *dequeue(Queue queue) {
  if (isQueueEmpty(queue)) {
    return NULL;
  }

  char *item = queue->array[queue->front];

  queue->front = (queue->front + 1) % queue->size;
  queue->count = queue->count - 1;

  return item;
}

char *front(Queue queue) {
  if (isQueueEmpty(queue)) {
    return NULL;
  }

  return queue->array[queue->front];
}

char *rear(Queue queue) {
  if (isQueueEmpty(queue)) {
    return NULL;
  }

  return queue->array[queue->rear];
}
