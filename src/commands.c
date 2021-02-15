#include "graph.h"
#include "networking.h"

#include <string.h>
#include <sys/socket.h>

#include <stdio.h>

void commandCommand(Client client) {
  char *msg = "*1\r\n*6\r\n$7\r\ncommand\r\n:-1\r\n*2\r\n+loading\r\n+"
              "stale\r\n:0\r\n:0\r\n:0\r\n";
  int len = strlen(msg);

  send(client->clt_fd, msg, len, 0);
}

void commandPing(Client client) {
  char *msg = "+PONG\r\n";

  int len = strlen(msg);

  send(client->clt_fd, msg, len, 0);
}

void commandSetEdge(Client client) {
  addEdge(client->graph, client->args[1], client->args[2]);

  char *msg = "+OK\r\n";

  int len = strlen(msg);

  send(client->clt_fd, msg, len, 0);
}

void commandSetVertex(Client client) {
  addVertex(client->graph, client->args[1], client->args[2]);

  char *msg = "+OK\r\n";

  int len = strlen(msg);

  send(client->clt_fd, msg, len, 0);
}

void commandGetVertex(Client client) {
  Vertex v = getVertex(client->graph, client->args[1]);

  // TODO: more intelligently return this
  char msg[80];
  strcpy(msg, "+");
  strcat(msg, v->value);
  strcat(msg, "\r\n");

  int len = strlen(msg);

  send(client->clt_fd, msg, len, 0);
}

void commandGetNeighbors(Client client) {
  getNeighbors(client->graph, client->args[1], 1);

  char *msg = "+OK\r\n";

  int len = strlen(msg);

  send(client->clt_fd, msg, len, 0);
}
