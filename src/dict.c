#include "dict.h"

#include <string.h>

// https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html

Dict internalCreateDict(int size) {
  Dict d = malloc(sizeof(Dict));

  d->size = size;
  d->count = 0;
  d->table = malloc(sizeof(Item) * d->size);

  for (int i = 0; i < d->size; i++) {
    d->table[i] = 0;
  }

  return d;
}

Dict createDict() {
  return internalCreateDict(INITIAL_SIZE);
}

void destroyDict(Dict d) {
  for (int i = 0; i < d->size; i++) {
    Item next;

    for (Item it = d->table[i]; it != 0; it = next) {
      next = it->next;

      free(it->key);
      free(it->exec);
      free(it);
    }
  }

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

void grow(Dict d) {
  Dict temp = internalCreateDict(d->size * GROWTH_FACTOR);

  for (int i = 0; i < d->size; i++) {
    for (Item it = d->table[i]; it != 0; it = it->next) {
      insertDictItem(temp, it->key, it->exec);
    }
  }

  Dict swap = d;
  d = temp;
  temp = swap;

  destroyDict(temp);
}

void insertDictItem(Dict d, char *key, commandExec *exec) {
  Item it = malloc(sizeof(Item));

  char *c_key = malloc(strlen(key) + 1);
  strcpy(c_key, key);
  it->key = key;
  it->exec = exec;

  int hkey = hash(key);

  it->next = d->table[hkey];
  d->table[hkey] = it;
  d->count++;

  if (d->count >= d->size) {
    grow(d);
  }
}

commandExec *getDictItem(Dict d, char *key) {
  for (Item it = d->table[hash(key)]; it != 0; it = it->next) {
    if (!strcmp(it->key, key)) {
      return it->exec;
    }
  }

  return NULL;
}

void deleteDictItem(Dict d, char *key) {
  for (Item it = d->table[hash(key)]; it != 0; it = it->next) {
    if (!strcmp(it->key, key)) {
      Item match = it;
      it = match->next;

      free(match->key);
      free(match->exec);
      free(match);

      return;
    }
  }
}
