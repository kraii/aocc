#include "hashmap.h"

#include <criterion/criterion.h>
#include <stdio.h>

uint64_t int_hash(const void *v) { return hashmap_fnv_hash(v, sizeof(int) / sizeof(uint8_t)); }

bool int_eq(const void *a, const void *b) { return *(int *)a == *(int *)b; }

Test(hashmap, put_and_get_ints) {
  // int main(...) {
  hashmap *map = hashmap_new(sizeof(int), sizeof(int), int_hash, int_eq);

  int k = 1;
  int v = 10;
  bool success = hashmap_put(map, &k, &v);
  cr_assert(success);
  k = 453;
  v = 24;
  success = hashmap_put(map, &k, &v);
  cr_assert(success);

  k = 1;
  int *r = hashmap_get(map, &k);
  cr_assert_not_null(r);
  cr_expect_eq(*r, 10);

  k = 453;
  r = hashmap_get(map, &k);
  cr_assert_not_null(r);
  cr_expect_eq(*r, 24, "Expected 24 got %d", *r);

  hashmap_free(map);
}