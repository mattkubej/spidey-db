#include "server.h"

#include <stdio.h>
#include <stdlib.h>

#define COMMAND_ARRAY '*'
#define COMMAND_BULKSTRING '$'

typedef struct command {
  char *buf;
  size_t offset;
} command;

void processCommandType(command *cmd) {
}

int getLength(char *buf) {
  int len = 0;

  buf++;

  while (*buf != '\r') {
    len = (len * 10) + (*buf - '0');
    buf++;
  }

  return len;
}

void processBulkString(char *buf) {
  printf("%s\n", buf);
  printf("---\n");

  int len = getLength(buf);

  char cmd[len];

  for (int i = 0; i < len; i++) {
    cmd[i] = buf[i];
    buf++;
  }

  printf("%s\n", cmd);
}

void processArray(command *cmd) {
  printf("%s\n", cmd->buf);
}

void processBuffer(char *buf) {
  command *cmd = malloc(sizeof(command));
  cmd->buf = buf;
  cmd->offset = 0;

  char type = buf[0];

  switch(type) {
    case COMMAND_ARRAY:
      processArray(cmd);
      break;
    default:
      break;
  }

  int len = getLength(buf);
  printf("%d\n", len);

  processBulkString(buf);

  free(cmd);
}
