#ifndef NETWORKING_H
#define NETWORKING_H

#include "graph.h"

#include <stdlib.h>

#define RESPONSE_BUFFER_LENGTH (16*1024)
#define CRLF "\r\n"
#define RESPONSE_OK "OK"

struct client {
  char *req_buf;
  size_t req_offset;
  char **req_args;
  int req_arg_length;
  int fd;
  Graph graph;
  char resp_buf[RESPONSE_BUFFER_LENGTH];
  int resp_offset;
};

typedef struct client *Client;

Client buildClient(char *req_buf, int clt_fd, Graph graph);
void destroyClient(Client client);
void parse(Client client);
int addSimpleStringReply(Client client, char *str);
int addArrayLength(Client client, int length);
int addBulkString(Client client, char *str);
int addInteger(Client client, int num);

#endif
