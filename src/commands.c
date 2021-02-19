#include "graph.h"
#include "networking.h"

#include <string.h>
#include <sys/socket.h>

#include <stdio.h>

void commandCommand(Client client) {
  // cheating here, so redis-cli responds back
  addArrayLength(client, 1);
  addArrayLength(client, 6);
  addBulkString(client, "command");
  addInteger(client, -1);
  addArrayLength(client, 2);
  addSimpleStringReply(client, "loading");
  addSimpleStringReply(client, "stale");
  addInteger(client, 0);
  addInteger(client, 0);
  addInteger(client, 0);

  send(client->fd, client->resp_buf, client->resp_offset, 0);
}

void commandPing(Client client) {
  addSimpleStringReply(client, "PONG");
  send(client->fd, client->resp_buf, client->resp_offset, 0);
}

void commandSetEdge(Client client) {
  addEdge(client->graph, client->req_args[1], client->req_args[2]);

  addSimpleStringReply(client, RESPONSE_OK);
  send(client->fd, client->resp_buf, client->resp_offset, 0);
}

void commandSetVertex(Client client) {
  addVertex(client->graph, client->req_args[1], client->req_args[2]);

  addSimpleStringReply(client, RESPONSE_OK);
  send(client->fd, client->resp_buf, client->resp_offset, 0);
}

void commandGetVertex(Client client) {
  Vertex v = getVertex(client->graph, client->req_args[1]);

  // TODO: more intelligently return this
  char msg[80];
  strcpy(msg, "+");
  strcat(msg, v->value);
  strcat(msg, CRLF);

  int len = strlen(msg);

  send(client->fd, msg, len, 0);
}

void commandGetNeighbors(Client client) {
  EdgeList el = getNeighbors(client->graph, client->req_args[1], 1);
  Edge e = el->head;

  while (e != NULL) {
    printf("{%s, %s}\n", e->src_key, e->dest_key);
    e = e->next;
  }

  // free edges?

  addSimpleStringReply(client, RESPONSE_OK);
  send(client->fd, client->resp_buf, client->resp_offset, 0);
}
