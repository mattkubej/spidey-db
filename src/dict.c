#include "dict.h"

#include <stdlib.h>
#include <string.h>

// Dict implementation based on
// https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html

Dict internalCreateDict(unsigned int size) {
  Dict d = malloc(sizeof(*d));

  d->size = size;
  d->count = 0;

  d->table = malloc(sizeof(Item) * d->size);
  memset(d->table, 0, sizeof(Item) * d->size);

  return d;
}

Dict createDict() { return internalCreateDict(INITIAL_SIZE); }

void destroyDict(Dict d) {
  for (size_t i = 0; i < d->size; i++) {
    Item next;

    for (Item it = d->table[i]; it != 0; it = next) {
      next = it->next;

      free(it->key);
      free(it->value);
      free(it);
    }
  }

  free(d->table);
  free(d);
}

// djb2 hash function - http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(char *str, unsigned int size) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % size;
}

void growDict(Dict d) {
  Dict temp = internalCreateDict(d->size * GROWTH_FACTOR);

  for (size_t i = 0; i < d->size; i++) {
    for (Item it = d->table[i]; it != 0; it = it->next) {
      insertDictItem(temp, it->key, it->value);
    }
  }

  Dict swap = d;
  d = temp;
  temp = swap;

  destroyDict(temp);
}

void insertDictItem(Dict d, char *key, void *value) {
  Item it = malloc(sizeof(*it));

  char *c_key = malloc(strlen(key) + 1);
  strcpy(c_key, key);

  it->key = c_key;
  it->value = value;

  int hkey = hash(c_key, d->size);

  it->next = d->table[hkey];
  d->table[hkey] = it;
  d->count++;

  if (d->count >= d->size) {
    growDict(d);
  }
}

Item getDictItem(Dict d, char *key) {
  for (Item it = d->table[hash(key, d->size)]; it != 0; it = it->next) {
    if (!strcmp(it->key, key)) {
      return it;
    }
  }

  return NULL;
}

void *getDictItemValue(Dict d, char *key) {
  Item it = getDictItem(d, key);
  return it ? it->value : NULL;
}

void deleteDictItem(Dict d, char *key) {
  for (Item it = d->table[hash(key, d->size)]; it != 0; it = it->next) {
    if (!strcmp(it->key, key)) {
      Item match = it;
      it = match->next;

      free(match->key);
      free(match->value);
      free(match);

      return;
    }
  }
}
