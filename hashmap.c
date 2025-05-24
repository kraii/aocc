#include "hashmap.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct map_entry {
  void *key;
  void *value;
} map_entry;

struct hashmap {
  size_t cap;
  size_t len;
  size_t key_size;
  size_t value_size;
  hash_func hash_func;
  hash_eq eq_func;
  map_entry *entries;
};

constexpr size_t INITIAL_SIZE = 16;
constexpr float LOAD_FACTOR = 0.6f;

hashmap *hashmap_new(const size_t key_size, const size_t value_size, const hash_func hasher, const hash_eq eq) {

  hashmap *result = malloc(sizeof(hashmap));
  if (result == NULL) {
    return NULL;
  }

  map_entry *entries = calloc(INITIAL_SIZE, sizeof(map_entry) + key_size + value_size);
  if (entries == NULL) {
    free(result);
    return NULL;
  }

  result->cap = INITIAL_SIZE;
  result->len = 0;
  result->key_size = key_size;
  result->value_size = value_size;
  result->hash_func = hasher;
  result->eq_func = eq;
  result->entries = entries;
  return result;
}

static void grow_if_req(hashmap *map) {
  // TODO
}

bool hashmap_put(hashmap *map, const void *key, const void *value) {
  if (key == NULL || value == NULL) {
    return false;
  }
  grow_if_req(map);
  const uint64_t hash = map->hash_func(key);
  size_t index = hash & map->cap - 1;
  unsigned attempts = 0;

  while (map->entries[index].key != NULL) {
    if (map->eq_func(key, map->entries[index].key)) {
      // update
      memcpy(map->entries[index].value, value, map->value_size);
      return true;
    }
    index++;
    attempts++;
    if (attempts > map->cap) {
      return false;
    }
    if (index >= map->cap) {
      index = 0;
    }
  }
  // Didn't find a match, insert new
  map->entries[index].key = &map->entries[index] + sizeof(map_entry);
  map->entries[index].value = &map->entries[index] + sizeof(map_entry) + map->key_size;
  memcpy(map->entries[index].value, value, map->value_size);
  memcpy(map->entries[index].key, key, map->key_size);
  map->len++;
  return true;
}

void *hashmap_get(const hashmap *map, const void *key) {
  if (key == NULL) {
    return NULL;
  }

  const uint64_t hash = map->hash_func(key);
  size_t index = hash & map->cap - 1;
  unsigned attempts = 0;

  while (map->entries[index].key != NULL) {
    if (map->eq_func(key, map->entries[index].key)) {
      return map->entries[index].value;
    }
    index++;
    attempts++;
    if (attempts > map->cap) {
      break;
    }
    if (index >= map->cap) {
      index = 0;
    }
  }
  return NULL;
}

uint64_t hashmap_fnv_hash(const uint8_t *bytes, const size_t n) {
  constexpr uint64_t offset = 14695981039346656037UL;

  uint64_t hash = offset;
  for (size_t i = 0; i < n; i++) {
    constexpr uint64_t prime = 1099511628211UL;
    hash ^= bytes[i];
    hash *= prime;
  }
  return hash;
}
void hashmap_free(hashmap *map) {
  free(map->entries);
  free(map);
}
