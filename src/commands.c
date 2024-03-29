#include "commands.h"
#include "graph.h"
#include "networking.h"

#include <string.h>
#include <sys/socket.h>

struct spideyCommand spideyCommandTable[] = {
    {"command", 1, commandCommand},
    {"ping", 1, commandPing},
    {"setedge", 3, commandSetEdge},
    {"deledge", 3, commandDeleteEdge},
    {"setvertex", 3, commandSetVertex},
    {"delvertex", 2, commandDeleteVertex},
    {"getvertex", 2, commandGetVertex},
    {"getneighbors", 3, commandGetNeighbors},
    {"getgraph", 1, commandGetGraph}};

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
  ClientRequest clt_req = client->clt_req;

  char *src_key = clt_req->req_args[1];
  char *dest_key = clt_req->req_args[2];

  Vertex src = getVertex(client->graph, src_key);
  if (src == NULL) {
    addErrorReply(client, "ERR src vertex not found");
    send(client->fd, client->reply_buf, client->reply_offset, 0);
    return;
  }

  Vertex dest = getVertex(client->graph, dest_key);
  if (dest == NULL) {
    addErrorReply(client, "ERR dest vertex not found");
    send(client->fd, client->reply_buf, client->reply_offset, 0);
    return;
  }

  addEdge(client->graph, src_key, dest_key);

  addSimpleStringReply(client, RESPONSE_OK);
  send(client->fd, client->reply_buf, client->reply_offset, 0);
}

void commandDeleteEdge(Client client) {
  ClientRequest clt_req = client->clt_req;

  deleteEdge(client->graph, clt_req->req_args[1], clt_req->req_args[2]);

  addSimpleStringReply(client, RESPONSE_OK);
  send(client->fd, client->reply_buf, client->reply_offset, 0);
}

void commandSetVertex(Client client) {
  ClientRequest clt_req = client->clt_req;

  addVertex(client->graph, clt_req->req_args[1], clt_req->req_args[2]);

  addSimpleStringReply(client, RESPONSE_OK);
  send(client->fd, client->reply_buf, client->reply_offset, 0);
}

void commandDeleteVertex(Client client) {
  ClientRequest clt_req = client->clt_req;

  deleteVertex(client->graph, clt_req->req_args[1]);

  addSimpleStringReply(client, RESPONSE_OK);
  send(client->fd, client->reply_buf, client->reply_offset, 0);
}

void commandGetVertex(Client client) {
  ClientRequest clt_req = client->clt_req;

  Vertex v = getVertex(client->graph, clt_req->req_args[1]);

  if (v == NULL) {
    addErrorReply(client, "ERR vertex not found");
    send(client->fd, client->reply_buf, client->reply_offset, 0);
    return;
  }

  char msg[strlen(v->value) + 4];
  strcpy(msg, "+");
  strcat(msg, v->value);
  strcat(msg, CRLF);

  int len = strlen(msg);

  send(client->fd, msg, len, 0);
}

void commandGetNeighbors(Client client) {
  ClientRequest clt_req = client->clt_req;

  char *vertex_key = clt_req->req_args[1];
  Vertex root = getVertex(client->graph, vertex_key);

  if (root == NULL) {
    addErrorReply(client, "ERR vertex not found");
    send(client->fd, client->reply_buf, client->reply_offset, 0);
    return;
  }

  int distance = atoi(clt_req->req_args[2]);
  VertexEdgeLists vertex_edge_lists =
      getNeighbors(client->graph, vertex_key, distance);

  addArrayLengthReply(client, 2);

  addArrayLengthReply(client, vertex_edge_lists->vertex_count);
  Vertex v = vertex_edge_lists->vertex_head;
  while (v != NULL) {
    addBulkStringReply(client, v->key);
    v = v->next;
  }

  addArrayLengthReply(client, vertex_edge_lists->edge_count);
  Edge e = vertex_edge_lists->edge_head;
  while (e != NULL) {
    addArrayLengthReply(client, 2);
    addBulkStringReply(client, e->src_key);
    addBulkStringReply(client, e->dest_key);
    e = e->next;
  }

  destroyVertexEdgeLists(vertex_edge_lists);

  send(client->fd, client->reply_buf, client->reply_offset, 0);
}

void commandGetGraph(Client client) {
  VertexEdgeLists vertex_edge_lists = toVertexEdgeLists(client->graph);

  addArrayLengthReply(client, 2);

  addArrayLengthReply(client, vertex_edge_lists->vertex_count);
  Vertex v = vertex_edge_lists->vertex_head;
  while (v != NULL) {
    addBulkStringReply(client, v->key);
    v = v->next;
  }

  addArrayLengthReply(client, vertex_edge_lists->edge_count);
  Edge e = vertex_edge_lists->edge_head;
  while (e != NULL) {
    addArrayLengthReply(client, 2);
    addBulkStringReply(client, e->src_key);
    addBulkStringReply(client, e->dest_key);
    e = e->next;
  }

  destroyVertexEdgeLists(vertex_edge_lists);

  send(client->fd, client->reply_buf, client->reply_offset, 0);
}

struct spideyCommand *getSpideyCommandTable() {
  return spideyCommandTable;
}

int getSpideyCommandCount() {
  return sizeof(spideyCommandTable) / sizeof(struct spideyCommand);
}
