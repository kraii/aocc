#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

#include "strings.c"

Test(strings, new_empty_string) {
    string* s = string_new_empty(5);

    cr_assert(eq(s->cap, 5));
    cr_assert(eq(s->len, 0));
    cr_assert_str_empty(s->buffer);

    free(s);
}

Test(strings, set_string) {
    string* s = string_new_empty(5);
    string_set(s, "12345", 5);
    cr_assert(eq(s->len, 5));
    cr_assert_str_eq(s->buffer, "12345");
    cr_assert(eq(strlen(s->buffer), 5));

    free(s);
}

Test(strings, set_string_oob) {
    string* s = string_new_empty(5);
    string_set(s, "123456", 6);
    cr_assert(eq(s->len, 5));
    cr_assert_str_eq(s->buffer, "12345");
    cr_assert(eq(strlen(s->buffer), 5));

    free(s);
}

Test(strings, new_string_from_literal) {
    string *s = string_new(5, "1234", 4);

    cr_assert(eq(s->len, 4));
    cr_assert_str_eq(s->buffer, "1234");
    cr_assert(eq(strlen(s->buffer), 4));

    free(s);
}

Test(strings, set_string_l) {
    string* s = string_new_empty(5);
    string_set_l(s, "12345");
    cr_assert(eq(s->len, 5));

    cr_assert_str_eq(s->buffer, "12345");
    cr_assert(eq(strlen(s->buffer), 5));

    free(s);
}


Test(strings, new_string_l) {
    string *s = string_new_l("1234");

    cr_assert(eq(s->len, 4));
    cr_assert_str_eq(s->buffer, "1234");
    cr_assert(eq(strlen(s->buffer), 4));

    free(s);
}

Test(strings, copy) {
    string *a = string_new_l("1234");
    string *b = string_copy(3, a);

    cr_assert(eq(b->len, 3));
    cr_assert_str_eq(b->buffer, "123");
    cr_assert(eq(strlen(b->buffer), 3));

    free(a);
    // ReSharper disable once CppDFADeletedPointer
    free(b);
}


Test(strings, concat_new) {
    string *a = string_new_l("abc ");
    string *b = string_new_l("123");

    string *result = string_cat_new(a, b);
    cr_assert_str_eq(result->buffer, "abc 123");
    cr_assert(eq(strlen(result->buffer), 7));

    free(a);
    free(b);
    free(result);
}

Test(strings, concat_l) {
    string *a = string_new(10, "abc ", 4);
    string *b = string_new_l("123");

    string_cat_l(a, b);
    cr_assert_str_eq(a->buffer, "abc 123");
    cr_assert(eq(strlen(a->buffer), 7));
    cr_assert(eq(a->len, 7));
    free(a);
    free(b);
}


Test(strings, concat_l_over_cap) {
    string *a = string_new(6, "abc ", 4);
    string *b = string_new_l("123");

    string_cat_l(a, b);
    cr_assert_str_eq(a->buffer, "abc 12");
    cr_assert(eq(strlen(a->buffer), 6));
    cr_assert(eq(a->len, 6));
    free(a);
    free(b);
}
