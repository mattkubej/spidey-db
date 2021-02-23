#include "queue.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

struct Queue *createQueue(size_t size) {
  struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));

  queue->size = size;
  queue->front = queue->count = 0;

  queue->rear = size - 1;
  queue->array = malloc(queue->size * sizeof(char *));

  return queue;
}

int isQueueEmpty(struct Queue *queue) { return queue->count == 0; }

// TODO: introduce growth factor
void grow(struct Queue *queue) {
  struct Queue *temp = createQueue(queue->size * 2);

  for (size_t i = 0; i < queue->count; i++) {
    enqueue(temp, dequeue(queue));
  }

  struct Queue *swap = queue;
  queue = temp;
  temp = swap;

  // TODO: destroy
}

void enqueue(struct Queue *queue, char *item) {
  queue->rear = (queue->rear + 1) % queue->size;
  queue->array[queue->rear] = item;
  queue->count = queue->count + 1;

  if (queue->count >= queue->size) {
    grow(queue);
  }
}

char *dequeue(struct Queue *queue) {
  if (isQueueEmpty(queue)) {
    return NULL;
  }

  char *item = queue->array[queue->front];

  queue->front = (queue->front + 1) % queue->size;
  queue->count = queue->count - 1;

  return item;
}

char *front(struct Queue *queue) {
  if (isQueueEmpty(queue)) {
    return NULL;
  }

  return queue->array[queue->front];
}

char *rear(struct Queue *queue) {
  if (isQueueEmpty(queue)) {
    return NULL;
  }

  return queue->array[queue->rear];
}
