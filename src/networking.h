#ifndef NETWORKING_H
#define NETWORKING_H

#include "graph.h"

#include <stdlib.h>

struct client {
  char *buf;
  size_t offset;
  char **args;
  int arg_length;
  int clt_fd;
  Graph graph;
};

typedef struct client *Client;

Client buildClient(char *buf, int clt_fd, Graph graph);
void destroyClient(Client client);
void parse(Client client);

#endif
