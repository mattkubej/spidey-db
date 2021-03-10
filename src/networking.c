#include "networking.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include <stdio.h>

// --- request processing ---

void addReqArg(Client client, char *req_arg) {
  char *c_req_arg = malloc(strlen(req_arg) + 1);
  strcpy(c_req_arg, req_arg);

  ClientRequest clt_req = client->clt_req_last;

  clt_req->req_args[clt_req->req_arg_length] = c_req_arg;
  clt_req->req_arg_length++;
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

  ClientRequest clt_req = malloc(sizeof(*clt_req));
  memset(clt_req, 0, sizeof(*clt_req));

  clt_req->req_args = malloc(sizeof(char *) * MAX_REQUEST_ARGS);
  clt_req->req_arg_length = 0;
  clt_req->next = NULL;

  // set clt_req on client
  client->clt_req_last->next = clt_req;
  client->clt_req_last = clt_req;

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

// --- client construction ---

Client buildClient(char *buf, int clt_fd, Graph graph) {
  printf("---- received ----\n");
  printf("%s", buf);
  printf("------------------\n");

  Client client = malloc(sizeof(*client));
  client->req_buf = buf;
  client->req_offset = 0;
  client->fd = clt_fd;
  client->graph = graph;
  client->reply_offset = 0;

  // dummy
  ClientRequest clt_req = malloc(sizeof(*clt_req));
  memset(clt_req, 0, sizeof(*clt_req));
  client->clt_req = clt_req;
  client->clt_req_last = clt_req;

  parse(client);

  // ditch dummy
  client->clt_req = client->clt_req->next;
  free(clt_req);

  // reset last to beginning for processing
  client->clt_req_last = client->clt_req;

  return client;
}

void destroyClient(Client client) {
  while (client->clt_req != NULL) {
    ClientRequest clt_req = client->clt_req;
    client->clt_req = client->clt_req->next;

    for (int i = 0; i < clt_req->req_arg_length; i++) {
      free(clt_req->req_args[i]);
    }

    free(clt_req->req_args);
    free(clt_req);
  }

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
  char buf[16];
  intToString(length, buf);

  addReply(client, "*", 1);
  addReply(client, buf, strlen(buf));
  addReply(client, CRLF, 2);

  return 0;
}

int addBulkStringReply(Client client, char *str) {
  char buf[16];
  intToString(strlen(str), buf);

  addReply(client, "$", 1);
  addReply(client, buf, strlen(buf));
  addReply(client, CRLF, 2);
  addReply(client, str, strlen(str));
  addReply(client, CRLF, 2);

  return 0;
}

int addIntegerReply(Client client, int num) {
  char buf[16];
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
