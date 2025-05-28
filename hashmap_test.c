#include "hashmap.h"

#include <criterion/criterion.h>
#include <stdio.h>

static uint64_t int_hash(const void *v) { return hashmap_fnv_hash(v, sizeof(int) / sizeof(uint8_t)); }

static bool int_eq(const void *a, const void *b) { return *(int *)a == *(int *)b; }

Test(hashmap_int, put_and_get_ints) {
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

Test(hashmap_int, grow) {
  // int main(...) {
  hashmap *map = hashmap_new(sizeof(int), sizeof(int), int_hash, int_eq);

  for (int i = 4000; i < 4024; i++) {
    int v = i * 2;
    const bool success = hashmap_put(map, &i, &v);
    cr_assert(success);
  }

  for (int i = 4000; i < 4024; i++) {
    const int *v = hashmap_get(map, &i);
    cr_assert_not_null(v, "%d was null", i);
    cr_expect_eq(*v, i * 2);
  }

  hashmap_free(map);
}

Test(hashmap_int, update_value) {
  hashmap *map = hashmap_new(sizeof(int), sizeof(int), int_hash, int_eq);

  int k = 1;
  int v = 10;
  bool success = hashmap_put(map, &k, &v);
  cr_assert(success);

  k = 1;
  v = 176;
  success = hashmap_put(map, &k, &v);
  cr_assert(success);

  k = 453;
  v = 24;
  success = hashmap_put(map, &k, &v);
  cr_assert(success);

  k = 1;
  int *r = hashmap_get(map, &k);
  cr_assert_not_null(r);
  cr_expect_eq(*r, 176);

  k = 453;
  r = hashmap_get(map, &k);
  cr_assert_not_null(r);
  cr_expect_eq(*r, 24, "Expected 24 got %d", *r);

  hashmap_free(map);
}

static uint64_t str_hash(const void *v) {
  const str val = *(str *)v;
  return hashmap_fnv_hash((u_int8_t *)strc(val), sizeof(char) / sizeof(uint8_t) * str_len(val));
}

static bool str_map_eq(const void *a, const void *b) {
  const str str_a = *(str *)a;
  const str str_b = *(str *)b;
  return str_eq(str_a, str_b);
}

Test(hashmap_str, get_set_update) {
  hashmap *map = hashmap_new(sizeof(str), sizeof(int), str_hash, str_map_eq);

  const str abc = strlit("abc");
  int v = 10;
  hashmap_put(map, &abc, &v);
  v = 20;
  hashmap_put(map, &abc, &v);

  const str moose = strlit("rafl the moose");
  v = 30;
  hashmap_put(map, &moose, &v);
  const str other_moose = str_new_l("rafl the moose");

  const int *result = hashmap_get(map, &abc);
  cr_assert(result != NULL);
  cr_expect_eq(*result, 20);

  result = hashmap_get(map, &other_moose);
  cr_assert(result != NULL);
  cr_expect_eq(*result, 30);

  cr_assert_eq(hashmap_len(map), 2);

  str_free(other_moose);
  hashmap_free(map);
}

Test(hashmap_str, delete_item) {
  hashmap *map = hashmap_new(sizeof(str), sizeof(int), str_hash, str_map_eq);
  const str abc = strlit("abc");
  int v = 10;
  hashmap_put(map, &abc, &v);

  const str moose = strlit("rafl the moose");
  v = 30;
  hashmap_put(map, &moose, &v);
  cr_assert_eq(hashmap_len(map), 2);

  const hashmap_entry deleted = hashmap_delete(map, &abc);
  cr_assert(deleted.key);
  cr_assert(deleted.value);
  const str deleted_key = *(str *)deleted.key;
  const int deleted_value = *(int *)deleted.value;
  cr_assert_str_eq(strc(deleted_key), "abc");
  cr_assert_eq(deleted_value, 10);

  cr_assert_eq(hashmap_len(map), 1);
}

// force collisions
static uint64_t bad_hash(const void *_) { return 1; }

Test(hashmap_str, delete_item_after_collision) {
  hashmap *map = hashmap_new(sizeof(str), sizeof(int), bad_hash, str_map_eq);
  const str abc = strlit("abc");
  int v = 10;
  hashmap_put(map, &abc, &v);

  const str xyz = strlit("xyz");
  v = 20;
  hashmap_put(map, &xyz, &v);

  const str moose = strlit("rafl the moose");
  v = 30;
  hashmap_put(map, &moose, &v);
  cr_assert_eq(hashmap_len(map), 3);

  const int *result = hashmap_get(map, &xyz);
  cr_assert(result != NULL);
  cr_expect_eq(*result, 20);

  const hashmap_entry deleted = hashmap_delete(map, &abc);
  cr_assert(deleted.key);
  cr_assert(deleted.value);
  const str deleted_key = *(str *)deleted.key;
  const int deleted_value = *(int *)deleted.value;
  cr_assert_str_eq(strc(deleted_key), "abc");
  cr_assert_eq(deleted_value, 10);
  hashmap_free_entry(deleted);

  cr_assert_eq(hashmap_len(map), 2);

  result = hashmap_get(map, &xyz);
  cr_assert(result != NULL);
  cr_expect_eq(*result, 20);

  result = hashmap_get(map, &moose);
  cr_assert(result != NULL);
  cr_expect_eq(*result, 30);
}