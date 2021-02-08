#ifndef SERVER_H
#define SERVER_H

#include "dict.h"
#include "graph.h"

#include <sys/time.h>
#include <time.h>

#define PORT 8080
#define TCP_BACKLOG 11
#define MAX_CLIENTS 11
#define CLT_BUF_SZ 1024

typedef struct spideyServer {
  int master_fd;
  fd_set read_fds;
  int max_fd;
  Dict commands;
  Graph graph;
} server_t;

server_t *create_server();
int server_listen(server_t *server);
int destroy_server(server_t *server);

#endif
