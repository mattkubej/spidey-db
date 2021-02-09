#ifndef NETWORKING_H
#define NETWORKING_H

#include "graph.h"

#include <stdlib.h>

struct request {
  char *buf;
  size_t offset;
  char *args[16];
  int arg_length;
  int clt_fd;
  Graph graph;
};

typedef struct request *Request;

Request buildRequest(char *buf, int clt_fd, Graph graph);
void destroyRequest(Request request);
void parse(Request req);

#endif
