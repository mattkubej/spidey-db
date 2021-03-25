#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_SIZE 32
#define GROWTH_FACTOR 2

#define DICT_DEFAULT (1<<0)
#define DICT_FREE_VALUES (1<<1)

struct item {
  struct item *next;
  char *key;
  void *value;
};

struct dict {
  size_t size;
  size_t count;
  int flags;
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
