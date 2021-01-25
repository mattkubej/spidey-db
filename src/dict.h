#include "networking.h"

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

Dict createDict();
void destroyDict(Dict d);
void insertDictItem(Dict d, char *key, commandExec *exec);
commandExec *getDictItem(Dict d, char *key);
void deleteDictItem(Dict d, char *key);
