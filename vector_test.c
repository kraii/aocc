#include <criterion/criterion.h>

#include "vector.h"

static void test_get_i(const vector *vec, const size_t i, const int expected) {
  const int result = vector_get_i(vec, i);
  cr_expect_eq(result, expected, "expected %d got %d", expected, result);
}

Test(vector, create_new_vector_and_add_to_it) {
  vector *vec = vector_new(sizeof(int));
  cr_assert_(vector_push_i(vec, 1));
  vector_push_i(vec, 2);
  vector_push_i(vec, 3);

  test_get_i(vec, 0, 1);
  test_get_i(vec, 1, 2);
  test_get_i(vec, 2, 3);
  cr_expect_eq(vector_len(vec), 3);

  vector_free(vec);
}

Test(vector, grows_if_required) {
  vector *vec = vector_newc(sizeof(int), 2);
  cr_expect_eq(vector_capacity(vec), 2);
  vector_push_i(vec, 1);
  vector_push_i(vec, 2);
  vector_push_i(vec, 3);

  test_get_i(vec, 0, 1);
  test_get_i(vec, 1, 2);
  test_get_i(vec, 2, 3);
  cr_expect_eq(vector_capacity(vec), 4);
  cr_expect_eq(vector_len(vec), 3);

  vector_free(vec);
}

Test(vector, set_index) {
  vector *vec = vector_new(sizeof(int));
  cr_assert_(vector_push_i(vec, 1));
  vector_push_i(vec, 2);

  vector_set_i(vec, 1, 227);

  test_get_i(vec, 0, 1);
  test_get_i(vec, 1, 227);

  cr_expect_eq(vector_len(vec), 2);

  vector_free(vec);
}

Test(vector, pop) {
  vector *vec = vector_new(sizeof(int));
  vector_push_i(vec, 1);
  vector_push_i(vec, 2);

  int p = vector_pop_i(vec);
  cr_assert_eq(2, p);
  cr_assert_eq(vector_len(vec), 1);
  p = vector_pop_i(vec);
  cr_assert_eq(1, p);
  cr_assert_eq(vector_len(vec), 0);

  vector_free(vec);
}