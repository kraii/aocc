#include "hashmap.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hashmap {
  size_t cap;
  size_t len;
  size_t key_size;
  size_t value_size;
  hash_func hash_func;
  hash_eq eq_func;
  hashmap_entry *entries;
  void *data;
};

constexpr size_t INITIAL_SIZE = 16;
constexpr float LOAD_FACTOR = 0.6f;

hashmap *hashmap_new(const size_t key_size, const size_t value_size, const hash_func hasher, const hash_eq eq) {
  hashmap *result = malloc(sizeof(hashmap));
  if (result == NULL) {
    return NULL;
  }

  hashmap_entry *entries = calloc(INITIAL_SIZE, sizeof(hashmap_entry) + key_size + value_size);
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
  result->data = &entries[INITIAL_SIZE];
  return result;
}

static int put_entry(hashmap_entry *entries, void *data, const size_t cap, const hashmap *map, const void *key,
                     const void *value) {
  unsigned attempts = 0;
  const uint64_t hash = map->hash_func(key);
  size_t index = hash & cap - 1;

  while (entries[index].key != NULL) {
    if (map->eq_func(key, entries[index].key)) {
      // update
      memcpy(entries[index].value, value, map->value_size);
      return 0;
    }
    index++;
    attempts++;
    if (attempts > cap) {
      return -1;
    }
    if (index >= cap) {
      index = 0;
    }
  }
  // Didn't find a match, insert new
  void *key_p = data + (map->value_size + map->key_size) * index;
  void *data_p = key_p + map->key_size;
  entries[index].key = key_p;
  entries[index].value = data_p;
  memcpy(data_p, value, map->value_size);
  memcpy(key_p, key, map->key_size);
  return 1;
}

static bool grow_if_req(hashmap *map) {
  const float factor = (float)(map->len + 1) / (float)map->cap;
  if (factor < LOAD_FACTOR) {
    return true;
  }

  const size_t cap = map->cap * 2;
  hashmap_entry *new_entries = calloc(cap, sizeof(hashmap_entry) + map->key_size + map->value_size);
  if (new_entries == NULL) {
    return false;
  }
  void *new_data = &new_entries[cap];

  hashmap_entry *old_entries = map->entries;
  for (size_t i = 0; i < map->cap; i++) {
    if (old_entries[i].key == NULL) {
      continue;
    }
    put_entry(new_entries, new_data, cap, map, old_entries[i].key, old_entries[i].value);
  }

  map->data = new_data;
  free(old_entries);
  old_entries = NULL;
  map->entries = new_entries;
  map->cap = cap;

  return true;
}

bool hashmap_put(hashmap *map, const void *key, const void *value) {
  if (key == NULL || value == NULL) {
    return false;
  }
  if (!grow_if_req(map))
    return false;

  const int code = put_entry(map->entries, map->data, map->cap, map, key, value);
  if (code == 1) {
    map->len++;
  }

  return code >= 0;
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
hashmap_entry hashmap_delete(hashmap *map, const void *key) {
  hashmap_entry result = {NULL, NULL};
  if (key == NULL) {
    return result;
  }

  const uint64_t hash = map->hash_func(key);
  size_t index = hash & map->cap - 1;
  unsigned attempts = 0;
  bool found = false;

  while (map->entries[index].key != NULL) {
    if (map->eq_func(key, map->entries[index].key)) {
      result.key = malloc(map->key_size);
      result.value = malloc(map->value_size);
      memcpy(result.key, map->entries[index].key, map->key_size);
      memcpy(result.value, map->entries[index].value, map->value_size);
      map->entries[index].key = NULL;
      map->entries[index].value = NULL;
      map->len--;
      found = true;
    } else if (found) {
      // any entries until gap is reach could now be out of place so reinsert them
      const void *tmp_key = map->entries[index].key;
      const void *tmp_value = map->entries[index].value;
      map->entries[index].key = NULL;
      map->entries[index].value = NULL;
      put_entry(map->entries, map->data, map->cap, map, tmp_key, tmp_value);
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
  return result;
}

size_t hashmap_len(const hashmap *map) {
  if (map == NULL)
    return 0;
  return map->len;
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

void hashmap_free_entry(const hashmap_entry entry) {
  free(entry.key);
  free(entry.value);
}
