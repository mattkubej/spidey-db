#include "server.h"
#include "commands.h"
#include "dict.h"
#include "networking.h"
#include "util.h"

#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

SpideyServer createServer() {
  server_t *server = malloc(sizeof(*server));

  if (server == NULL) {
    perror("failed to allocate memory for server");
    return NULL;
  }

  Dict cmd_dict = createDict();
  struct spideyCommand *spideyCommandTable = getSpideyCommandTable();
  int command_count = getSpideyCommandCount();

  for (int i = 0; i < command_count; i++) {
    SpideyCommand sc = spideyCommandTable + i;
    insertDictItem(cmd_dict, sc->name, sc);
  }

  server->commands = cmd_dict;
  server->master_fd = -1;
  server->graph = createGraph();

  return server;
}

int acceptConnection(server_t *server) {
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

void processClientBuffer(server_t *server, int clt_fd, char *clt_buf) {
  Client client = buildClient(clt_buf, clt_fd, server->graph);
  ClientRequest client_request = client->clt_req_last;

  if (client_request != NULL) {
    char *command = toLower(client_request->req_args[0]);

    SpideyCommand spidey_cmd = getDictItemValue(server->commands, command);

    if (spidey_cmd == NULL) {
      addErrorReply(client, "ERR unknown command");
      send(client->fd, client->reply_buf, client->reply_offset, 0);
    } else if (spidey_cmd->arity != client_request->req_arg_length) {
      addErrorReply(client, "ERR wrong number of arguments");
      send(client->fd, client->reply_buf, client->reply_offset, 0);
    } else {
      spidey_cmd->exec(client);
    }

    client->clt_req_last = client->clt_req_last->next;
  }

  destroyClient(client);
}

int receiveClientMessage(server_t *server, int clt_fd) {
  int read_size;

  char clt_buf[CLT_BUF_SZ];
  memset(clt_buf, 0, sizeof(clt_buf));

  if ((read_size = recv(clt_fd, clt_buf, CLT_BUF_SZ - 1, 0)) > 0) {
    clt_buf[read_size] = '\0';

    processClientBuffer(server, clt_fd, clt_buf);

    memset(clt_buf, 0, sizeof(clt_buf));
  } else {
    close(clt_fd);
    FD_CLR(clt_fd, &server->read_fds);
  }

  return 0;
}

void printBanner() {
  printf("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
  printf(":'######::'########::'####:'########::'########:'##:::'##:\n");
  printf("'##... ##: ##.... ##:. ##:: ##.... ##: ##.....::. ##:'##::\n");
  printf(" ##:::..:: ##:::: ##:: ##:: ##:::: ##: ##::::::::. ####:::\n");
  printf(". ######:: ########::: ##:: ##:::: ##: ######:::::. ##::::\n");
  printf(":..... ##: ##.....:::: ##:: ##:::: ##: ##...::::::: ##::::\n");
  printf("'##::: ##: ##::::::::: ##:: ##:::: ##: ##:::::::::: ##::::\n");
  printf(". ######:: ##::::::::'####: ########:: ########:::: ##::::\n");
  printf(":......:::..:::::::::....::........:::........:::::..:::::\n");
  printf(":::'########::'########:::::::::::::::::::::::::::::::::::\n");
  printf("::: ##.... ##: ##.... ##::::::::::::::::::::::::::::::::::\n");
  printf("::: ##:::: ##: ##:::: ##::::::::::::::::::::::::::::::::::\n");
  printf("::: ##:::: ##: ########:::::::::::: Port: %d :::::::::::\n", PORT);
  printf("::: ##:::: ##: ##.... ##::::::::::::::::::::::::::::::::::\n");
  printf("::: ##:::: ##: ##:::: ##::::::::::::::::::::::::::::::::::\n");
  printf("::: ########:: ########:::::::::::::::::::::::::::::::::::\n");
  printf(":::........:::........::::::::::::::::::::::::::::::::::::\n");
}

int serverListen(SpideyServer server) {
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

  printBanner();

  while (1) {
    fd_set copy_fds = server->read_fds;

    if (select(server->max_fd + 1, &copy_fds, NULL, NULL, NULL) < 0) {
      perror("select error");
      return 1;
    }

    for (int i = 0; i <= server->max_fd; i++) {
      if (FD_ISSET(i, &copy_fds)) {
        if (i == server->master_fd) {
          acceptConnection(server);
        } else {
          receiveClientMessage(server, i);
        }
      }
    }
  }

  return 0;
}

int destroyServer(SpideyServer server) {
  if (server->master_fd != -1) {
    int err = close(server->master_fd);
    if (err < 0) {
      perror("failed to close master fd");
      return err;
    }

    server->master_fd = -1;
  }

  destroyDict(server->commands);
  destroyGraph(server->graph);
  free(server);

  return 0;
}
