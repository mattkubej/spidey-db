#include "commands.h"
#include "graph.h"
#include "networking.h"

#include <string.h>
#include <sys/socket.h>

#include <stdio.h>

struct spideyCommand spideyCommandTable[] = {
  {"command", 1, commandCommand},
  {"ping", 1, commandPing},
  {"setedge", 3, commandSetEdge},
  {"setvertex", 3, commandSetVertex},
  {"getvertex", 2, commandGetVertex},
  {"getneighbors", 3, commandGetNeighbors}
};

void commandCommand(Client client) {
  // cheating here, so redis-cli responds back
  addArrayLengthReply(client, 1);
  addArrayLengthReply(client, 6);
  addBulkStringReply(client, "command");
  addIntegerReply(client, -1);
  addArrayLengthReply(client, 2);
  addSimpleStringReply(client, "loading");
  addSimpleStringReply(client, "stale");
  addIntegerReply(client, 0);
  addIntegerReply(client, 0);
  addIntegerReply(client, 0);

  send(client->fd, client->reply_buf, client->reply_offset, 0);
}

void commandPing(Client client) {
  addSimpleStringReply(client, "PONG");
  send(client->fd, client->reply_buf, client->reply_offset, 0);
}

void commandSetEdge(Client client) {
  addEdge(client->graph, client->req_args[1], client->req_args[2]);

  addSimpleStringReply(client, RESPONSE_OK);
  send(client->fd, client->reply_buf, client->reply_offset, 0);
}

void commandSetVertex(Client client) {
  addVertex(client->graph, client->req_args[1], client->req_args[2]);

  addSimpleStringReply(client, RESPONSE_OK);
  send(client->fd, client->reply_buf, client->reply_offset, 0);
}

void commandGetVertex(Client client) {
  Vertex v = getVertex(client->graph, client->req_args[1]);

  char msg[strlen(v->value) + 4];
  strcpy(msg, "+");
  strcat(msg, v->value);
  strcat(msg, CRLF);

  int len = strlen(msg);

  send(client->fd, msg, len, 0);
}

void commandGetNeighbors(Client client) {
  int distance = atoi(client->req_args[2]);
  Neighbors neighbors = getNeighbors(client->graph, client->req_args[1], distance);

  addArrayLengthReply(client, 2);

  addArrayLengthReply(client, neighbors->vertex_count);
  Vertex v = neighbors->vertex_head;
  while (v != NULL) {
    addBulkStringReply(client, v->key);
    v = v->next;
  }

  addArrayLengthReply(client, neighbors->edge_count);
  Edge e = neighbors->edge_head;
  while (e != NULL) {
    addArrayLengthReply(client, 2);
    addBulkStringReply(client, e->src_key);
    addBulkStringReply(client, e->dest_key);
    e = e->next;
  }

  // free edges?

  send(client->fd, client->reply_buf, client->reply_offset, 0);
}

struct spideyCommand* getSpideyCommandTable() {
  return spideyCommandTable;
}
