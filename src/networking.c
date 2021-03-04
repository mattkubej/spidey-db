#include "networking.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

// --- client construction and reply request processing ---

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
  // TODO: remove this printing
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
  client->reply_offset = 0;

  parse(client);

  return client;
}

void destroyClient(Client client) {
  // TODO: free cmd->args?

  free(client);
}

// --- reply handling ---

int addReply(Client client, char *str, size_t str_len) {
  memcpy(client->reply_buf + client->reply_offset, str, str_len);
  client->reply_offset += str_len;

  return 0;
}

int addSimpleStringReply(Client client, char *str) {
  addReply(client, "+", 1);
  addReply(client, str, strlen(str));
  addReply(client, CRLF, 2);

  return 0;
}

int addArrayLengthReply(Client client, int length) {
  // TODO: abstract out the buf size
  char buf[1024];
  intToString(length, buf);

  addReply(client, "*", 1);
  addReply(client, buf, strlen(buf));
  addReply(client, CRLF, 2);

  return 0;
}

int addBulkStringReply(Client client, char *str) {
  // TODO: abstract out the buf size
  char buf[1024];
  intToString(strlen(str), buf);

  addReply(client, "$", 1);
  addReply(client, buf, strlen(buf));
  addReply(client, CRLF, 2);
  addReply(client, str, strlen(str));
  addReply(client, CRLF, 2);

  return 0;
}

int addIntegerReply(Client client, int num) {
  // TODO: abstract out the buf size
  char buf[1024];
  intToString(num, buf);

  addReply(client, ":", 1);
  addReply(client, buf, strlen(buf));
  addReply(client, CRLF, 2);

  return 0;
}

int addErrorReply(Client client, char *err) {
  addReply(client, "-", 1);
  addReply(client, err, strlen(err));
  addReply(client, CRLF, 2);

  return 0;
}
