#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

#include "strings.c"

Test(strings, new_empty_string) {
    string* s = new_empty_string(5);

    cr_assert(eq(s->cap, 5));
    cr_assert(eq(s->len, 0));
    cr_assert_str_empty(s->buffer);

    free(s);
}

Test(strings, set_string) {
    string* s = new_empty_string(5);
    const int status = string_set(s, "12345", 5);
    cr_assert(eq(status, 0));
    cr_assert(eq(s->len, 5));

    cr_assert_str_eq(s->buffer, "12345");
    cr_assert(eq(strlen(s->buffer), 5));

    free(s);
}

Test(strings, set_string_oob, .signal = SIGTRAP) {
    string* s = new_empty_string(5);
    const int status = string_set(s, "123456", 6);
    cr_assert(eq(status, 1));
    cr_assert(eq(s->len, 0));

    free(s);
}

Test(strings, new_string_from_literal) {
    string *s = new_string(5, "1234", 4);

    cr_assert(eq(s->len, 4));
    cr_assert_str_eq(s->buffer, "1234");
    cr_assert(eq(strlen(s->buffer), 4));

    free(s);
}