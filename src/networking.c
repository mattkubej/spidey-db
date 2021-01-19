#include "server.h"

#include <stdio.h>

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

void processBuffer(char *buf) {
  printf("%s\n", buf);
  printf("---\n");

  int len = getLength(buf);
  printf("%d\n", len);

  processBulkString(buf);
}
