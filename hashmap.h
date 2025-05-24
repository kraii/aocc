#ifndef AOCC_HASHMAP
#define AOCC_HASHMAP
#include <stddef.h>
#include <stdint.h>

typedef struct hashmap hashmap;
typedef uint64_t (*hash_func)(const void *);
typedef bool (*hash_eq)(const void *, const void *);

hashmap *hashmap_new(size_t key_size, size_t value_size, hash_func hasher, hash_eq eq);
bool hashmap_put(hashmap *map, const void *key, const void *value);
void *hashmap_get(const hashmap *map, const void *key);
uint64_t hashmap_fnv_hash(const uint8_t *bytes, size_t n);

void hashmap_free(hashmap *map);

#endif // HASHMAP_H
