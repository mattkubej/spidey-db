#include "server.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>

server_t* create_server() {
  server_t* server = malloc(sizeof(*server));

  if (server == NULL) {
    perror("failed to allocate memory for server");
    return NULL;
  }

  server->master_fd = -1;

  return server;
}

int server_listen(server_t* server) {
  int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (serv_fd < 0) {
    perror("failed to create socket");
    return 1;
  }

  int enable = 1;
  if (setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
    perror("setsockopt(SO_REUSEADDR) failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  if (bind(serv_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("failed to bind socket");
    return 1;
  }

  if (listen(serv_fd, TCP_BACKLOG) < 0) {
    perror("failed to listen");
    return 1;
  }

  int buff_size = 1024;
  char client_buffer[buff_size];
  int cli_fd, read_size;
  struct sockaddr_in in_addr;
  int in_len = sizeof(in_addr);

  while (1) {
    printf("--- waiting for clients ---\n");
    if ((cli_fd = accept(serv_fd, (struct sockaddr*) &in_addr, (socklen_t*) &in_len)) < 0) {
      perror("failed to accept");
      return 1;
    }

    memset(client_buffer, 0, sizeof(client_buffer));
    while ((read_size = recv(cli_fd, client_buffer, buff_size, 0)) > 0) {
      printf("%s\n", client_buffer);
      memset(client_buffer, 0, sizeof(client_buffer));
    }
  }

  return 0;
}

int destroy_server(server_t* server) {
  if (server->master_fd != -1) {
    int err = close(server->master_fd);
    if (err < 0) {
      perror("failed to close master fd");
      return err;
    }

    server->master_fd = -1;
  }

  return 0;
}
