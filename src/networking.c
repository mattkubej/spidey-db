#include "networking.h"
#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_ARRAY '*'
#define COMMAND_BULKSTRING '$'

void addArg(command *cmd, char *arg) {
  char c_arg[sizeof(arg)];
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

  switch(type) {
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

void processBuffer(char *buf) {
  printf("--- received ---\n");
  printf("%s", buf);
  printf("----------------\n\n");

  command *cmd = malloc(sizeof(command));
  cmd->buf = buf;
  cmd->offset = 0;
  cmd->arg_length = 0;

  parse(cmd);
  printf("done parsing\n\n");

  printf("--- args ---\n");
  for (int i = 0; i < cmd->arg_length; i++) {
    printf(" %s\n", cmd->args[i]);
  }
  printf("------------\n");

  free(cmd);
}
