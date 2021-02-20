#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

struct Queue {
  int front, rear, count;
  size_t size;
  char** array;
};

struct Queue* createQueue(size_t size) {
  struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));

  queue->size = size;
  queue->front = queue->count = 0;

  queue->rear = size - 1;
  queue->array = malloc(queue->size * sizeof(char*));

  return queue;
}

int isQueueEmpty(struct Queue* queue) {
  return queue->count == 0;
}

// TODO: build grow functionality into this
void enqueue(struct Queue* queue, char* item) {
  queue->rear = (queue->rear + 1) % queue->size;
  queue->array[queue->rear] = item;
  queue->count = queue->count + 1;
}

char* dequeue(struct Queue* queue) {
  if (isQueueEmpty(queue)) {
    return NULL;
  }

  char* item = queue->array[queue->front];

  queue->front = (queue->front + 1) % queue->size;
  queue->count = queue->count - 1;

  return item;
}

char* front(struct Queue* queue) {
  if (isQueueEmpty(queue)) {
    return NULL;
  }

  return queue->array[queue->front];
}


char* rear(struct Queue* queue) {
  if (isQueueEmpty(queue)) {
    return NULL;
  }

  return queue->array[queue->rear];
}
