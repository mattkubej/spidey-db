#ifndef SERVER_H
#define SERVER_H

#define PORT 8080
#define TCP_BACKLOG 11

typedef struct spideyServer {
  int master_fd;
} server_t;

server_t* create_server();
int server_listen(server_t* server);
int destroy_server(server_t* server);

#endif
