#ifndef NETWORKING_H
#define NETWORKING_H

#include "graph.h"

#include <stdlib.h>

struct client {
  char *req_buf;
  size_t req_offset;
  char **req_args;
  int req_arg_length;
  int fd;
  Graph graph;
};

typedef struct client *Client;

Client buildClient(char *req_buf, int clt_fd, Graph graph);
void destroyClient(Client client);
void parse(Client client);

#endif
