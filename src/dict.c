#include "networking.h"

// https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html

#define INITIAL_SIZE 32
#define GROWTH_FACTOR 2

typedef void commandExec(command *cmd);

struct item {
  struct item *next;
  char *key;
  commandExec *exec; // TODO: can this be generic?
};

struct dict {
  int size;
  int count;
  struct item **table;
};

typedef struct dict *Dict;
typedef struct item *Item;

Dict createDict() {
  Dict d = malloc(sizeof(Dict));

  d->size = INITIAL_SIZE;
  d->count = 0;
  d->table = malloc(sizeof(Item) * d->size);

  for (int i = 0; i < d->size; i++) {
    d->table[i] = 0;
  }

  return d;
}

void destroyDict(Dict d) {
  // iterate through items

  free(d->table);
  free(d);
}

// djb2 - http://www.cse.yorku.ca/~oz/hash.html
int hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash;
}
