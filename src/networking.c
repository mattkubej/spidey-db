#include "networking.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define COMMAND_ARRAY '*'
#define COMMAND_BULKSTRING '$'

void addReqArg(Client client, char *req_arg) {
  char *c_req_arg = malloc(strlen(req_arg) + 1);
  strcpy(c_req_arg, req_arg);

  client->req_args[client->req_arg_length] = c_req_arg;
  client->req_arg_length++;
}

int readLength(Client client) {
  int len = 0;

  while (client->req_buf[client->req_offset] != '\r') {
    len = (len * 10) + (client->req_buf[client->req_offset] - '0');
    client->req_offset++;
  }

  // iterate passed "\r\n"
  client->req_offset += 2;

  return len;
}

void processBulkString(Client client) {
  int length = readLength(client);
  char bulkString[length + 1];

  for (int i = 0; i < length; i++) {
    bulkString[i] = client->req_buf[client->req_offset];
    client->req_offset++;
  }

  bulkString[length] = '\0';
  addReqArg(client, bulkString);

  // iterate passed "\r\n"
  client->req_offset += 2;
}

void processArray(Client client) {
  int items = readLength(client);

  for (int i = 0; i <= items; i++) {
    parse(client);
  }
}

void parse(Client client) {
  char type = client->req_buf[client->req_offset];
  client->req_offset++;

  switch (type) {
  case COMMAND_ARRAY:
    processArray(client);
    break;
  case COMMAND_BULKSTRING:
    processBulkString(client);
    break;
  default:
    break;
  }
}

Client buildClient(char *buf, int clt_fd, Graph graph) {
  printf("--- received ---\n");
  printf("%s", buf);
  printf("----------------\n\n");

  Client client = malloc(sizeof(Client));
  client->req_buf = buf;
  client->req_offset = 0;
  client->req_args = malloc(sizeof(char *) * 16); // TODO: do this better
  client->req_arg_length = 0;
  client->fd = clt_fd;
  client->graph = graph;

  parse(client);

  return client;
}

void destroyClient(Client client) {
  // TODO: free cmd->args?

  free(client);
}
