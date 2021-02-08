#include "server.h"
#include "commands.h"
#include "dict.h"
#include "networking.h"

#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <ctype.h>

char *toLower(char *s) {
  for (char *l = s; *l; l++) {
    *l = tolower(*l);
  }

  return s;
}

server_t *create_server() {
  server_t *server = malloc(sizeof(*server));

  if (server == NULL) {
    perror("failed to allocate memory for server");
    return NULL;
  }

  Dict cmd_dict = createDict();
  insertDictItem(cmd_dict, "command", commandCommand);
  insertDictItem(cmd_dict, "ping", commandPing);
  insertDictItem(cmd_dict, "setedge", commandSetEdge);
  insertDictItem(cmd_dict, "getedge", commandGetEdge);
  insertDictItem(cmd_dict, "setvertex", commandSetVertex);
  insertDictItem(cmd_dict, "getvertex", commandGetVertex);

  server->commands = cmd_dict;
  server->master_fd = -1;
  server->graph = createGraph();

  return server;
}

int accept_connection(server_t *server) {
  struct sockaddr_in in_addr;
  int in_len = sizeof(in_addr);

  int clt_fd = accept(server->master_fd, (struct sockaddr *)&in_addr,
                      (socklen_t *)&in_len);

  if (clt_fd < 0) {
    perror("failed to accept");
    return 1;
  }

  FD_SET(clt_fd, &server->read_fds);
  if (clt_fd > server->max_fd) {
    server->max_fd = clt_fd;
  }

  return 0;
}

void proc_clt_buf(char *clt_buf, server_t *server, int clt_fd) {
  // TODO: consider adding clt_fd to request
  Request req = buildRequest(clt_buf);

  if (req->args[0] != NULL) {
    char *command = toLower(req->args[0]);

    void (*req_cmd)(Request, int) =
        getDictItemValue(server->commands, toLower(command));

    if (req_cmd != NULL) {
      req_cmd(req, clt_fd);
    }
  }

  destroyRequest(req);
}

int recv_clt_msg(server_t *server, int clt_fd) {
  int read_size;

  char clt_buf[CLT_BUF_SZ];
  memset(clt_buf, 0, sizeof(clt_buf));

  if ((read_size = recv(clt_fd, clt_buf, CLT_BUF_SZ - 1, 0)) > 0) {
    clt_buf[read_size] = '\0';

    proc_clt_buf(clt_buf, server, clt_fd);

    memset(clt_buf, 0, sizeof(clt_buf));
  } else {
    close(clt_fd);
    FD_CLR(clt_fd, &server->read_fds);
  }

  return 0;
}

int server_listen(server_t *server) {
  if ((server->master_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("failed to create socket");
    return 1;
  }

  int enable = 1;
  if (setsockopt(server->master_fd, SOL_SOCKET, SO_REUSEADDR, &enable,
                 sizeof(enable)) < 0) {
    perror("setsockopt(SO_REUSEADDR) failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  if (bind(server->master_fd, (struct sockaddr *)&serv_addr,
           sizeof(serv_addr)) < 0) {
    perror("failed to bind socket");
    return 1;
  }

  if (listen(server->master_fd, TCP_BACKLOG) < 0) {
    perror("failed to listen");
    return 1;
  }

  FD_ZERO(&server->read_fds);
  FD_SET(server->master_fd, &server->read_fds);
  server->max_fd = server->master_fd;

  printf("--- waiting for clients ---\n");

  while (1) {
    fd_set copy_fds = server->read_fds;

    if (select(server->max_fd + 1, &copy_fds, NULL, NULL, NULL) < 0) {
      perror("select error");
      return 1;
    }

    for (int i = 0; i <= server->max_fd; i++) {
      if (FD_ISSET(i, &copy_fds)) {
        if (i == server->master_fd) {
          accept_connection(server);
        } else {
          recv_clt_msg(server, i);
        }
      }
    }
  }

  return 0;
}

int destroy_server(server_t *server) {
  if (server->master_fd != -1) {
    int err = close(server->master_fd);
    if (err < 0) {
      perror("failed to close master fd");
      return err;
    }

    server->master_fd = -1;
  }

  // TODO: close all client fds?

  return 0;
}
