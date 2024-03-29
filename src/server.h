#ifndef SERVER_H
#define SERVER_H

#include "dict.h"
#include "graph.h"

#include <sys/time.h>
#include <time.h>

#define PORT 6425
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

typedef struct spideyServer *SpideyServer;

SpideyServer createServer();
int serverListen(SpideyServer server);
int destroyServer(SpideyServer server);

#endif
