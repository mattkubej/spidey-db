#include "networking.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define COMMAND_ARRAY '*'
#define COMMAND_BULKSTRING '$'

void addArg(Request req, char *arg) {
  char *c_arg = malloc(strlen(arg) + 1);
  strcpy(c_arg, arg);

  req->args[req->arg_length] = c_arg;
  req->arg_length++;
}

int readLength(Request req) {
  int len = 0;

  while (req->buf[req->offset] != '\r') {
    len = (len * 10) + (req->buf[req->offset] - '0');
    req->offset++;
  }

  // iterate passed "\r\n"
  req->offset += 2;

  return len;
}

void processBulkString(Request req) {
  int length = readLength(req);
  char bulkString[length + 1];

  for (int i = 0; i < length; i++) {
    bulkString[i] = req->buf[req->offset];
    req->offset++;
  }

  bulkString[length] = '\0';
  addArg(req, bulkString);

  // iterate passed "\r\n"
  req->offset += 2;
}

void processArray(Request req) {
  int items = readLength(req);

  for (int i = 0; i <= items; i++) {
    parse(req);
  }
}

void parse(Request req) {
  char type = req->buf[req->offset];
  req->offset++;

  switch (type) {
  case COMMAND_ARRAY:
    processArray(req);
    break;
  case COMMAND_BULKSTRING:
    processBulkString(req);
    break;
  default:
    break;
  }
}

Request buildRequest(char *buf, int clt_fd) {
  printf("--- received ---\n");
  printf("%s", buf);
  printf("----------------\n\n");

  Request req = malloc(sizeof(Request));
  req->buf = buf;
  req->offset = 0;
  req->arg_length = 0;
  req->clt_fd = clt_fd;

  parse(req);

  return req;
}

void destroyRequest(Request req) {
  // TODO: free cmd->args?

  free(req);
}
