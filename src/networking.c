#include "networking.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define COMMAND_ARRAY '*'
#define COMMAND_BULKSTRING '$'

void commandCommand(command *cmd, int clt_fd) {
  char *msg = "*1\r\n*6\r\n$7\r\ncommand\r\n:-1\r\n*2\r\n+loading\r\n+"
              "stale\r\n:0\r\n:0\r\n:0\r\n";
  int len = strlen(msg);

  send(clt_fd, msg, len, 0);
}

// --- buffer processing ---

void addArg(command *cmd, char *arg) {
  char *c_arg = malloc(strlen(arg) + 1);
  strcpy(c_arg, arg);

  cmd->args[cmd->arg_length] = c_arg;
  cmd->arg_length++;
}

int readLength(command *cmd) {
  int len = 0;

  while (cmd->buf[cmd->offset] != '\r') {
    len = (len * 10) + (cmd->buf[cmd->offset] - '0');
    cmd->offset++;
  }

  // iterate passed "\r\n"
  cmd->offset += 2;

  return len;
}

void processBulkString(command *cmd) {
  int length = readLength(cmd);
  char bulkString[length];

  for (int i = 0; i < length; i++) {
    bulkString[i] = cmd->buf[cmd->offset];
    cmd->offset++;
  }

  addArg(cmd, bulkString);
}

void processArray(command *cmd) {
  int items = readLength(cmd);

  for (int i = 0; i < items; i++) {
    parse(cmd);
  }
}

void parse(command *cmd) {
  char type = cmd->buf[cmd->offset];
  cmd->offset++;

  switch (type) {
  case COMMAND_ARRAY:
    processArray(cmd);
    break;
  case COMMAND_BULKSTRING:
    processBulkString(cmd);
    break;
  default:
    break;
  }
}

void processCmd(command *cmd, int clt_fd) {
  printf("--- args ---\n");
  for (int i = 0; i < cmd->arg_length; i++) {
    printf(" %s\n", cmd->args[i]);
  }
  printf("------------\n");

  commandCommand(cmd, clt_fd);
}

void processBuffer(char *buf, int clt_fd) {
  printf("--- received ---\n");
  printf("%s", buf);
  printf("----------------\n\n");

  command *cmd = malloc(sizeof(command));
  cmd->buf = buf;
  cmd->offset = 0;
  cmd->arg_length = 0;

  parse(cmd);
  printf("done parsing\n\n");

  processCmd(cmd, clt_fd);

  // TODO: free cmd->args?

  free(cmd);
}
