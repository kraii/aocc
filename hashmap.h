#ifndef AOCC_HASHMAP
#define AOCC_HASHMAP
#include <stddef.h>
#include <stdint.h>

typedef struct hashmap hashmap;
typedef struct hashmap_entry {
  void *key;
  void *value;
} hashmap_entry;

typedef uint64_t (*hash_func)(const void *);
typedef bool (*hash_eq)(const void *, const void *);
hashmap *hashmap_newc(size_t initial_capacity, size_t key_size, size_t value_size, hash_func hasher, hash_eq eq);
hashmap *hashmap_new(size_t key_size, size_t value_size, hash_func hasher, hash_eq eq);
bool hashmap_put(hashmap *map, const void *key, const void *value);
void *hashmap_get(const hashmap *map, const void *key);
bool hashmap_delete(hashmap *map, const void *key);
size_t hashmap_len(const hashmap *map);

uint64_t hashmap_fnv_hash(const uint8_t *bytes, size_t n);

void hashmap_clear(hashmap *map);
void hashmap_free(hashmap *map);

#endif // HASHMAP_H
