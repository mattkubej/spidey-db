#include "server.h"

#include <stdio.h>

void processBuffer(char *buf) {
  int len = 0;

  buf++;

  while (*buf != '\r') {
    len = (len * 10) + (*buf - '0');
    buf++;
  }

  printf("%d\n", len);
}
