#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdlib.h>

struct request {
  char *buf;
  size_t offset;
  char *args[16];
  int arg_length;
};

typedef struct request *Request;

Request buildRequest(char *buf);
void destroyRequest(Request request);
void parse(Request req);

#endif
