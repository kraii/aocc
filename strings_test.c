#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

#include <stdarg.h>
#include "strings.h"

Test(strings, new_empty_string) {
    string *s = string_new_empty(5);

    cr_expect(eq(string_cap(s), 5));
    cr_expect(eq(string_len(s), 0));
    cr_assert_str_empty(string_c(s));

    string_free(s);
}

Test(strings, set_string) {
    string *s = string_new_empty(5);
    string_set(s, "12345", 5);
    cr_expect(eq(string_len(s), 5));
    cr_assert_str_eq(string_c(s), "12345");
    cr_expect(eq(strlen(string_c(s)), 5));

    string_free(s);
}

Test(strings, set_string_oob) {
    string *s = string_new_empty(5);
    string_set(s, "123456", 6);
    cr_expect(eq(string_len(s), 5));
    cr_assert_str_eq(string_c(s), "12345");
    cr_expect(eq(strlen(string_c(s)), 5));

    string_free(s);
}

Test(strings, new_string_from_literal) {
    string *s = string_new(5, "1234", 4);

    cr_expect(eq(string_len(s), 4));
    cr_assert_str_eq(string_c(s), "1234");
    cr_expect(eq(strlen(string_c(s)), 4));

    string_free(s);
}


Test(strings, set_string_l) {
    string *s = string_new_empty(5);
    string_set_l(s, "12345");
    cr_expect(eq(string_len(s), 5));

    cr_assert_str_eq(string_c(s), "12345");
    cr_expect(eq(strlen(string_c(s)), 5));

    string_free(s);
}

Test(strings, new_string_l) {
    string *s = string_new_l("1234");

    cr_expect(eq(string_len(s), 4));
    cr_assert_str_eq(string_c(s), "1234");
    cr_expect(eq(strlen(string_c(s)), 4));

    string_free(s);
}

Test(strings, string_len) {
    string *s = string_new_l("123");
    cr_expect(eq(string_len(s), 3));
    string_free(s);
}


Test(strings, copy) {
    string *a = string_new_l("1234");
    string *b = string_copy(3, a);

    cr_expect(eq(string_len(b), 3));
    cr_assert_str_eq(b->buffer, "123");
    cr_expect(eq(strlen(b->buffer), 3));

    string_free(a);
    string_free(b);
}


Test(strings, concat_new) {
    string *a = string_new_l("abc ");
    string *b = string_new_l("123");

    string *result = string_cat_new(a, b);
    cr_assert_str_eq(result->buffer, "abc 123");
    cr_expect(eq(strlen(result->buffer), 7));

    string_free(a);
    string_free(b);
    string_free(result);
}

Test(strings, concat_l) {
    string *a = string_new(10, "abc ", 4);
    string *b = string_new_l("123");

    string_cat_l(a, b);
    cr_assert_str_eq(a->buffer, "abc 123");
    cr_expect(eq(strlen(a->buffer), 7));
    cr_expect(eq(string_len(a), 7));
    string_free(a);
    string_free(b);
}

Test(strings, concat_l_over_cap) {
    string *a = string_new(6, "abc ", 4);
    string *b = string_new_l("123");

    string_cat_l(a, b);
    cr_assert_str_eq(a->buffer, "abc 12");
    cr_expect(eq(strlen(a->buffer), 6));
    cr_expect(eq(string_len(a), 6));
    string_free(a);
    string_free(b);
}

Test(strings, string_eq) {
    string *a = string_new_l("123");
    string *b = string_new_l("123");

    cr_expect(string_eq(a, b), "strings should be equal");

    string_free(a);
    string_free(b);
}

Test(strings, string_not_eq) {
    string *a = string_new_l("abc");
    string *b = string_new_l("123");

    cr_expect(not(string_eq(a, b)), "non strings should not be equal");

    string_free(a);
    string_free(b);
}

Test(strings, string_find) {
    string *haystack = string_new_l("a big potato farm was very nice");
    string *needle = string_new_l("potato");
    const int result = string_find(haystack, needle);
    cr_expect(eq(result, 6), "result should be index 6 but was %d", result);
    string_free(haystack);
    string_free(needle);
}

Test(strings, string_find_not_found) {
    string *haystack = string_new_l("a big potato farm was very nice");
    string *needle = string_new_l("cake");
    const int result = string_find(haystack, needle);
    cr_expect(eq(result, -1));
    string_free(haystack);
    string_free(needle);
}

Test(strings, string_find_l) {
    string *haystack = string_new_l("a big potato farm was very nice");
    const int result = string_find_l(haystack, "potato");
    cr_expect(eq(result, 6), "result should be index 6 but was %d", result);
    string_free(haystack);
}

Test(strings, string_find_l_not_found) {
    string *haystack = string_new_l("a big potato farm was very nice");
    const int result = string_find_l(haystack, "cake");
    cr_expect(eq(result, -1));
    string_free(haystack);
}

Test(strings, string_contains_yes) {
    string *haystack = string_new_l("a crap load of sausages");
    string *needle = string_new_l("saus");
    const int result = string_contains(haystack, needle);
    cr_expect(result, "string does contain needle");
    string_free(haystack);
    string_free(needle);
}

Test(strings, string_contains_no) {
    string *haystack = string_new_l("meaty things");
    string *needle = string_new_l("M");
    const int result = string_contains(haystack, needle);
    cr_expect(not(result), "string does not contain needle");
    string_free(haystack);
    string_free(needle);
}

Test(strings, substring) {
    string *val = string_new_l("I am Batman");
    string *sub = string_new_substring(val, 5, 11);
    cr_expect_str_eq(string_c(sub), "Batman");
    string_free(val);
    string_free(sub);
}

Test(strings, substring_too_long_end) {
    string *val = string_new_l("I am Batman");
    string *sub = string_new_substring(val, 5, 450);
    cr_expect_str_eq(string_c(sub), "Batman");
    string_free(val);
    string_free(sub);
}

Test(strings, substring_errors) {
    string *val = string_new_l("I am Batman");
    const string *sub = string_new_substring(val, 11, 15);
    cr_expect_null(sub);
    sub = string_new_substring(val, 15, 14);
    cr_expect_null(sub);
    free(val);
}

Test(strings, find_single_char) {
    const string src = string_l("a big potato farm was very nice");
    const string delim = string_l("b");
    const int result = string_find(&src, &delim);
    cr_expect_eq(result, 2);
}

Test(strings, string_split) {
    const string src = string_l("Waffle man likes to iron");
    const string delim = string_l(" ");
    vector *result = string_split(&src, &delim);
    const size_t tokens = vector_len(result);
    cr_assert_eq(tokens, 5, "Expect 5 tokens got %lu", tokens);

    cr_assert(string_eq_l(vector_get_p(result, 0), "Waffle"));
    cr_assert(string_eq_l(vector_get_p(result, 1), "man"));
    cr_assert(string_eq_l(vector_get_p(result, 2), "likes"));
    cr_assert(string_eq_l(vector_get_p(result, 3), "to"));
    cr_assert(string_eq_l(vector_get_p(result, 4), "iron"));
    vector_free_all(result);
}

Test(string, string_split_no_match) {
    const string src = string_l("Waffle man likes to iron");
    const string delim = string_l("Z");
    vector *result = string_split(&src, &delim);
    const size_t tokens = vector_len(result);
    cr_assert_eq(tokens, 1, "Expect 1 tokens got %lu", tokens);
    cr_assert(string_eq(vector_get_p(result, 0), &src));
    free(result);
}

Test(string, string_trim_start) {
    string *target = string_new_l("   Waffle man likes to iron");
    string_trim(target);
    cr_assert_str_eq(string_c(target), "Waffle man likes to iron");
    cr_assert_eq(string_len(target), 24);
    free(target);
}

Test(string, string_trim_end) {
    string *target = string_new_l("Waffle man likes to iron   ");
    string_trim(target);
    cr_assert_str_eq(string_c(target), "Waffle man likes to iron");
    cr_assert_eq(string_len(target), 24);
    free(target);
}

Test(string, string_trim_both) {
    string *target = string_new_l(" Waffle man likes to iron   ");
    string_trim(target);
    cr_assert_str_eq(string_c(target), "Waffle man likes to iron");
    cr_assert_eq(string_len(target), 24);
    free(target);
}

Test(string, string_trim_blank) {
    string *target = string_new_l("  ");
    string_trim(target);
    cr_assert_str_eq(string_c(target), "");
    cr_assert_eq(string_len(target), 0);
    free(target);
}

Test(string, string_to_l) {
    string *target = string_new_l("12345");
    cr_assert_eq(string_to_l(target), 12345L);
    free(target);
}

Test(string_tok, success) {
    string *dest = string_new_empty(16);
    const string src = string_l("Waffle man rules");
    const string delim = string_l(" ");

    size_t pos = 0;
    bool complete = string_tok(dest, &src, &pos, &delim);
    cr_assert_eq(complete, true);
    cr_assert_str_eq(string_c(dest), "Waffle");
    cr_assert_eq(pos, 7);

    complete = string_tok(dest, &src, &pos, &delim);
    cr_assert_eq(complete, true);
    cr_assert_str_eq(string_c(dest), "man");
    cr_assert_eq(pos, 11);

    complete = string_tok(dest, &src, &pos, &delim);
    cr_assert_eq(complete, false);
    cr_assert_str_eq(string_c(dest), "rules");
    cr_assert_eq(pos, 16);

    free(dest);
}