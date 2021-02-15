#include "networking.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define COMMAND_ARRAY '*'
#define COMMAND_BULKSTRING '$'

void addArg(Client client, char *arg) {
  char *c_arg = malloc(strlen(arg) + 1);
  strcpy(c_arg, arg);

  client->args[client->arg_length] = c_arg;
  client->arg_length++;
}

int readLength(Client client) {
  int len = 0;

  while (client->buf[client->offset] != '\r') {
    len = (len * 10) + (client->buf[client->offset] - '0');
    client->offset++;
  }

  // iterate passed "\r\n"
  client->offset += 2;

  return len;
}

void processBulkString(Client client) {
  int length = readLength(client);
  char bulkString[length + 1];

  for (int i = 0; i < length; i++) {
    bulkString[i] = client->buf[client->offset];
    client->offset++;
  }

  bulkString[length] = '\0';
  addArg(client, bulkString);

  // iterate passed "\r\n"
  client->offset += 2;
}

void processArray(Client client) {
  int items = readLength(client);

  for (int i = 0; i <= items; i++) {
    parse(client);
  }
}

void parse(Client client) {
  char type = client->buf[client->offset];
  client->offset++;

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
  client->buf = buf;
  client->offset = 0;
  client->args = malloc(sizeof(char *) * 16); // TODO: do this better
  client->arg_length = 0;
  client->clt_fd = clt_fd;
  client->graph = graph;

  parse(client);

  return client;
}

void destroyClient(Client client) {
  // TODO: free cmd->args?

  free(client);
}
