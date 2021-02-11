#include "networking.h"
#include "graph.h"

#include <string.h>
#include <sys/socket.h>

#include <stdio.h>

void commandCommand(Request req) {
  char *msg = "*1\r\n*6\r\n$7\r\ncommand\r\n:-1\r\n*2\r\n+loading\r\n+"
              "stale\r\n:0\r\n:0\r\n:0\r\n";
  int len = strlen(msg);

  send(req->clt_fd, msg, len, 0);
}

void commandPing(Request req) {
  char *msg = "+PONG\r\n";

  int len = strlen(msg);

  send(req->clt_fd, msg, len, 0);
}

void commandSetEdge(Request req) {
  char *msg = "+SET EDGE\r\n";

  int len = strlen(msg);

  send(req->clt_fd, msg, len, 0);
}

void commandGetEdge(Request req) {
  char *msg = "+GET EDGE\r\n";

  int len = strlen(msg);

  send(req->clt_fd, msg, len, 0);
}

void commandSetVertex(Request req) {
  addVertex(req->graph, req->args[1], req->args[2]);

  char *msg = "+OK\r\n";

  int len = strlen(msg);

  send(req->clt_fd, msg, len, 0);
}

void commandGetVertex(Request req) {
  Vertex v = getVertex(req->graph, req->args[1]);

  char msg[80];
  strcpy(msg, "+");
  strcat(msg, v->value);
  strcat(msg, "\r\n");

  int len = strlen(msg);

  send(req->clt_fd, msg, len, 0);
}
