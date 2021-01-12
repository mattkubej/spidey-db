#include "server.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
  int err;

  server_t *server = create_server();
  if (server == NULL) {
    printf("failed to create server\n");
    return 1;
  }

  err = server_listen(server);
  if (err > 0) {
    printf("failed to listen on 0.0.0.0:%d\n", PORT);
    return err;
  }

  err = destroy_server(server);
  if (err > 0) {
    printf("failed to destroy server");
    return err;
  }

  free(server);

  return 0;
}
