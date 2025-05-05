#include <criterion/criterion.h>
#include <string.h>

#include "vector.c"

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
}

