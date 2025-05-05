#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

#include "strings.c"

Test(strings, new_empty_string) {
    string *s = string_new_empty(5);

    cr_expect(eq(s->cap, 5));
    cr_expect(eq(string_len(s), 0));
    cr_assert_str_empty(s->buffer);

    free(s);
}

Test(strings, set_string) {
    string *s = string_new_empty(5);
    string_set(s, "12345", 5);
    cr_expect(eq(string_len(s), 5));
    cr_assert_str_eq(s->buffer, "12345");
    cr_expect(eq(strlen(s->buffer), 5));

    free(s);
}

Test(strings, set_string_oob) {
    string *s = string_new_empty(5);
    string_set(s, "123456", 6);
    cr_expect(eq(string_len(s), 5));
    cr_assert_str_eq(s->buffer, "12345");
    cr_expect(eq(strlen(s->buffer), 5));

    free(s);
}

Test(strings, new_string_from_literal) {
    string *s = string_new(5, "1234", 4);

    cr_expect(eq(string_len(s), 4));
    cr_assert_str_eq(s->buffer, "1234");
    cr_expect(eq(strlen(s->buffer), 4));

    free(s);
}


Test(strings, set_string_l) {
    string *s = string_new_empty(5);
    string_set_l(s, "12345");
    cr_expect(eq(string_len(s), 5));

    cr_assert_str_eq(s->buffer, "12345");
    cr_expect(eq(strlen(s->buffer), 5));

    free(s);
}

Test(strings, new_string_l) {
    string *s = string_new_l("1234");

    cr_expect(eq(string_len(s), 4));
    cr_assert_str_eq(s->buffer, "1234");
    cr_expect(eq(strlen(s->buffer), 4));

    free(s);
}

Test(strings, string_len) {
    string *s = string_new_l("123");
    cr_expect(eq(string_len(s), 3));
    free(s);
}


Test(strings, copy) {
    string *a = string_new_l("1234");
    string *b = string_copy(3, a);

    cr_expect(eq(string_len(b), 3));
    cr_assert_str_eq(b->buffer, "123");
    cr_expect(eq(strlen(b->buffer), 3));

    free(a);
    // ReSharper disable once CppDFADeletedPointer
    free(b);
}


Test(strings, concat_new) {
    string *a = string_new_l("abc ");
    string *b = string_new_l("123");

    string *result = string_cat_new(a, b);
    cr_assert_str_eq(result->buffer, "abc 123");
    cr_expect(eq(strlen(result->buffer), 7));

    free(a);
    free(b);
    free(result);
}

Test(strings, concat_l) {
    string *a = string_new(10, "abc ", 4);
    string *b = string_new_l("123");

    string_cat_l(a, b);
    cr_assert_str_eq(a->buffer, "abc 123");
    cr_expect(eq(strlen(a->buffer), 7));
    cr_expect(eq(string_len(a), 7));
    free(a);
    free(b);
}


Test(strings, concat_l_over_cap) {
    string *a = string_new(6, "abc ", 4);
    string *b = string_new_l("123");

    string_cat_l(a, b);
    cr_assert_str_eq(a->buffer, "abc 12");
    cr_expect(eq(strlen(a->buffer), 6));
    cr_expect(eq(string_len(a), 6));
    free(a);
    free(b);
}

Test(strings, string_eq) {
    string *a = string_new_l("123");
    string *b = string_new_l("123");

    cr_expect(string_eq(a, b), "strings should be equal");

    free(a);
    free(b);
}

Test(strings, string_not_eq) {
    string *a = string_new_l("abc");
    string *b = string_new_l("123");

    cr_expect(not(string_eq(a, b)), "non strings should not be equal");

    free(a);
    free(b);
}

Test(strings, string_find) {
    string *haystack = string_new_l("a big potato farm was very nice");
    string *needle = string_new_l("potato");
    const int result = string_find(haystack, needle);
    cr_expect(eq(result, 6), "result should be index 6 but was %d", result);
    free(haystack);
    free(needle);
}

Test(strings, string_find_not_found) {
    string *haystack = string_new_l("a big potato farm was very nice");
    string *needle = string_new_l("cake");
    const int result = string_find(haystack, needle);
    cr_expect(eq(result, -1));
    free(haystack);
    free(needle);
}

Test(strings, string_find_l) {
    string *haystack = string_new_l("a big potato farm was very nice");
    const int result = string_find_l(haystack, "potato");
    cr_expect(eq(result, 6), "result should be index 6 but was %d", result);
    free(haystack);
}

Test(strings, string_find_l_not_found) {
    string *haystack = string_new_l("a big potato farm was very nice");
    const int result = string_find_l(haystack, "cake");
    cr_expect(eq(result, -1));
    free(haystack);
}

Test(strings, string_contains_yes) {
    string *haystack = string_new_l("a crap load of sausages");
    string *needle = string_new_l("saus");
    const int result = string_contains(haystack, needle);
    cr_expect(result, "string does contain needle");
    free(haystack);
    free(needle);
}

Test(strings, string_contains_no) {
    string *haystack = string_new_l("meaty things");
    string *needle = string_new_l("M");
    const int result = string_contains(haystack, needle);
    cr_expect(not(result), "string does not contain needle");
    free(haystack);
    free(needle);
}
