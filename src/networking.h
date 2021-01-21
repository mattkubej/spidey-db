#include <stdlib.h>

typedef struct command {
  char *buf;
  size_t offset;
  char *args[16];
  int arg_length;
} command;

void processBuffer(char *buf);
void parse(command *cmd);
