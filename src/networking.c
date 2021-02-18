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
  client->resp_offset = 0;

  parse(client);

  return client;
}

void destroyClient(Client client) {
  // TODO: free cmd->args?

  free(client);
}

int addReply(Client client, char *str, size_t str_len) {
  memcpy(client->resp_buf + client->resp_offset, str, str_len);
  client->resp_offset += str_len;

  return 0;
}

int addSimpleStringReply(Client client, char *str) {
  addReply(client, "+", 1);
  addReply(client, str, strlen(str));
  addReply(client, CRLF, 2);

  return 0;
}

// https://stackoverflow.com/a/29544416/14610143

void reverse(char s[]) {
  char c;
  int i, j;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void itoa(int n, char s[]) {
  int sign = n;

  if (sign < 0) {
    n = -n;
  }

  int i = 0;

  do {
    s[i++] = n % 10 + '0';
  } while ((n / 10) > 0);

  if (sign < 0) {
    s[i++] = '-';
  }

  s[i] = '\0';

  reverse(s);
}

int addArrayLength(Client client, int length) {
  char buf[1024];
  itoa(length, buf);

  addReply(client, "*", 1);
  addReply(client, buf, strlen(buf));
  addReply(client, CRLF, 2);

  return 0;
}

int addBulkString(Client client, char *str) {
  char buf[1024];
  itoa(strlen(str), buf);

  addReply(client, "$", 1);
  addReply(client, buf, strlen(buf));
  addReply(client, CRLF, 2);
  addReply(client, str, strlen(str));
  addReply(client, CRLF, 2);

  return 0;
}

int addInteger(Client client, int num) {
  char buf[1024];
  itoa(num, buf);

  addReply(client, ":", 1);
  addReply(client, buf, strlen(buf));
  addReply(client, CRLF, 2);

  return 0;
}
