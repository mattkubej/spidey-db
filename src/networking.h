#include <stdlib.h>

typedef struct command {
  char *buf;
  size_t offset;
} command;


void processBuffer(char *buf);
void parse(command *cmd);
