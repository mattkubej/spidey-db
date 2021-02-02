#include "networking.h"

void commandCommand(Request req, int clt_fd);
void commandPing(Request req, int clt_fd);
void commandSetEdge(Request req, int clt_fd);
void commandGetEdge(Request req, int clt_fd);
void commandSetVertex(Request req, int clt_fd);
void commandGetVertex(Request req, int clt_fd);
