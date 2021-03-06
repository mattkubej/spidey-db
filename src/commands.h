#include "networking.h"

typedef void (*voidfunc)(Client);

struct spideyCommand {
  char name[32];
  int arity;
  voidfunc exec;
};

typedef struct spideyCommand *SpideyCommand;

void commandCommand(Client client);
void commandPing(Client client);
void commandSetEdge(Client client);
void commandSetVertex(Client client);
void commandGetVertex(Client client);
void commandGetNeighbors(Client client);
