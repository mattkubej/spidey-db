#include "server.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
  int err;

  SpideyServer server = createServer();
  if (server == NULL) {
    printf("failed to create server\n");
    return 1;
  }

  err = serverListen(server);
  if (err > 0) {
    printf("failed to listen on 0.0.0.0:%d\n", PORT);
    return err;
  }

  err = destroyServer(server);
  if (err > 0) {
    printf("failed to destroy server");
    return err;
  }

  return 0;
}
