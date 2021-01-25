#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdlib.h>

typedef struct command {
  char *buf;
  size_t offset;
  char *args[16];
  int arg_length;
} command;

void processBuffer(char *buf, int clt_fd);
void parse(command *cmd);

#endif
