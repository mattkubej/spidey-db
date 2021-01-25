#ifndef DICT_H
#define DICT_H

#include "networking.h"

#define INITIAL_SIZE 32
#define GROWTH_FACTOR 2

struct item {
  struct item *next;
  char *key;
  void *value;
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
void insertDictItem(Dict d, char *key, void *value);
Item getDictItem(Dict d, char *key);
void *getDictItemValue(Dict d, char *key);
void deleteDictItem(Dict d, char *key);

#endif
