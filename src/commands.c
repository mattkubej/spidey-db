#include "networking.h"

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
  char *msg = "+SET VERTEX\r\n";

  int len = strlen(msg);

  send(req->clt_fd, msg, len, 0);
}

void commandGetVertex(Request req) {
  char *msg = "+GET VERTEX\r\n";

  int len = strlen(msg);

  send(req->clt_fd, msg, len, 0);
}
