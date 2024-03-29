#ifndef NETWORKING_H
#define NETWORKING_H

#include "graph.h"

#include <stdlib.h>

#define MAX_REQUEST_ARGS 16

#define COMMAND_ARRAY '*'
#define COMMAND_BULKSTRING '$'

#define CRLF "\r\n"
#define REPLY_BUFFER_LENGTH (16 * 1024)
#define RESPONSE_OK "OK"

struct clientRequest {
  char **req_args;
  int req_arg_length;
  struct clientRequest *next;
};

struct client {
  char *req_buf;
  size_t req_offset;
  int fd;
  Graph graph;
  char reply_buf[REPLY_BUFFER_LENGTH];
  int reply_offset;
  struct clientRequest *clt_req, *clt_req_last;
};

typedef struct client *Client;
typedef struct clientRequest *ClientRequest;

Client buildClient(char *req_buf, int clt_fd, Graph graph);
void destroyClient(Client client);
void parse(Client client);
int addSimpleStringReply(Client client, char *str);
int addArrayLengthReply(Client client, int length);
int addBulkStringReply(Client client, char *str);
int addIntegerReply(Client client, int num);
int addErrorReply(Client client, char *err);

#endif
