#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

#include <stdarg.h>
#include "strings.h"

#include <stdio.h>

Test(strings, new_empty_string) {
    str s = str_new_empty(5);

    cr_expect(eq(str_cap(s), 5));
    cr_expect(eq(str_len(s), 0));
    cr_assert_str_empty(strc(s));

    str_free(s);
}

Test(strings, set_string) {
    str s = str_new_empty(5);
    str_set(&s, "12345", 5);
    cr_expect(eq(str_len(s), 5));
    cr_assert_str_eq(strc(s), "12345");
    cr_expect(eq(strlen(strc(s)), 5));

    str_free(s);
}

Test(strings, set_string_oob) {
    str s = str_new_empty(5);
    str_set(&s, "123456", 6);
    cr_expect(eq(str_len(s), 5));
    cr_assert_str_eq(strc(s), "12345");
    cr_expect(eq(strlen(strc(s)), 5));

    str_free(s);
}

Test(strings, new_string_from_literal) {
    str s = str_new(5, "1234", 4);

    cr_expect(eq(str_len(s), 4));
    cr_assert_str_eq(strc(s), "1234");
    cr_expect(eq(strlen(strc(s)), 4));

    str_free(s);
}


Test(strings, set_string_l) {
    str s = str_new_empty(5);
    str_set_l(&s, "12345");
    cr_expect(eq(str_len(s), 5));

    cr_assert_str_eq(strc(s), "12345");
    cr_expect(eq(strlen(strc(s)), 5));

    str_free(s);
}

Test(strings, new_string_l) {
    str s = str_new_l("1234");

    cr_expect(eq(str_len(s), 4));
    cr_assert_str_eq(strc(s), "1234");
    cr_expect(eq(strlen(strc(s)), 4));

    str_free(s);
}

Test(strings, string_len) {
    const str s = str_new_l("123");
    cr_expect(eq(str_len(s), 3));
    str_free(s);
}


Test(strings, copy) {
    const str a = str_new_l("1234");
    const str b = str_copy(3, a);

    cr_expect(eq(str_len(b), 3));
    cr_assert_str_eq(strc(b), "123");

    str_free(a);
    str_free(b);
}


Test(strings, concat_new) {
    const str a = str_new_l("abc ");
    const str b = str_new_l("123");

    const str result = str_cat_new(a, b);
    cr_assert_str_eq(strc(result), "abc 123");
    cr_expect(eq(strlen(strc(result)), 7));

    str_free(a);
    str_free(b);
    str_free(result);
}

Test(strings, concat_l) {
    str a = str_new(10, "abc ", 4);
    const str b = str_new_l("123");

    str_cat_l(&a, b);
    cr_assert_str_eq(strc(a), "abc 123");
    cr_expect(eq(str_len(a), 7));
    str_free(a);
    str_free(b);
}

Test(strings, concat_l_over_cap) {
    str a = str_new(6, "abc ", 4);
    str b = str_new_l("123");

    str_cat_l(&a, b);
    cr_assert_str_eq(strc(a), "abc 12");
    cr_expect(eq(strlen(strc(a)), 6));
    cr_expect(eq(str_len(a), 6));
    str_free(a);
    str_free(b);
}

Test(strings, string_eq) {
    const str a = str_new_l("123");
    const str b = str_new_l("123");

    cr_expect(str_eq(a, b), "strings should be equal");

    str_free(a);
    str_free(b);
}

Test(strings, string_not_eq) {
    const str a = str_new_l("abc");
    const str b = str_new_l("123");

    cr_expect(not(str_eq(a, b)), "non strings should not be equal");

    str_free(a);
    str_free(b);
}

Test(strings, string_find) {
    const str haystack = str_new_l("a big potato farm was very nice");
    const str needle = str_new_l("potato");
    const int result = str_find(haystack, needle);
    cr_expect(eq(result, 6), "result should be index 6 but was %d", result);
    str_free(haystack);
    str_free(needle);
}

Test(strings, string_find_not_found) {
    const str haystack = str_new_l("a big potato farm was very nice");
    const str needle = str_new_l("cake");
    const int result = str_find(haystack, needle);
    cr_expect(eq(result, -1));
    str_free(haystack);
    str_free(needle);
}

Test(strings, string_find_l) {
    const str haystack = str_new_l("a big potato farm was very nice");
    const int result = str_find_l(haystack, "potato");
    cr_expect(eq(result, 6), "result should be index 6 but was %d", result);
    str_free(haystack);
}

Test(strings, string_find_l_not_found) {
    const str haystack = str_new_l("a big potato farm was very nice");
    const int result = str_find_l(haystack, "cake");
    cr_expect(eq(result, -1));
    str_free(haystack);
}

Test(strings, string_contains_yes) {
    const str haystack = str_new_l("a crap load of sausages");
    const str needle = str_new_l("saus");
    const int result = str_contains(haystack, needle);
    cr_expect(result, "string does contain needle");
    str_free(haystack);
    str_free(needle);
}

Test(strings, string_contains_no) {
    const str haystack = str_new_l("meaty things");
    const str needle = str_new_l("M");
    const int result = str_contains(haystack, needle);
    cr_expect(not(result), "string does not contain needle");
    str_free(haystack);
    str_free(needle);
}

Test(strings, substring) {
    const str val = str_new_l("I am Batman");
    const str sub = str_new_substr(val, 5, 11);
    cr_expect_str_eq(strc(sub), "Batman");
    str_free(val);
    str_free(sub);
}

Test(strings, substring_too_long_end) {
    const str val = str_new_l("I am Batman");
    const str sub = str_new_substr(val, 5, 450);
    cr_expect_str_eq(strc(sub), "Batman");
    str_free(val);
    str_free(sub);
}

Test(strings, substring_errors) {
    const str val = str_new_l("I am Batman");
    str sub = str_new_substr(val, 11, 15);
    cr_assert_(str_is_null(sub));
    sub = str_new_substr(val, 15, 14);
    cr_assert_(str_is_null(sub));
    str_free(val);
}

Test(strings, find_single_char) {
    const str src = str_l("a big potato farm was very nice");
    const str delim = str_l("b");
    const int result = str_find(src, delim);
    cr_expect_eq(result, 2);
}

Test(strings, string_split) {
    const str src = str_l("Waffle man likes to iron");
    const str delim = str_l(" ");
    vector *result = str_split(src, delim);
    const size_t tokens = vector_len(result);
    cr_assert_eq(tokens, 5, "Expect 5 tokens got %lu", tokens);

    cr_assert_str_eq(strc(vector_get_str(result, 0)), "Waffle");
    cr_assert(str_eq_l(vector_get_str(result, 1), "man"));
    cr_assert(str_eq_l(vector_get_str(result, 2), "likes"));
    cr_assert(str_eq_l(vector_get_str(result, 3), "to"));
    cr_assert(str_eq_l(vector_get_str(result, 4), "iron"));
    vector_free(result);
}

Test(string, string_split_no_match) {
    const str src = str_l("Waffle man likes to iron");
    const str delim = str_l("Z");
    vector *result = str_split(src, delim);
    const size_t tokens = vector_len(result);
    cr_assert_eq(tokens, 1, "Expect 1 tokens got %lu", tokens);
    cr_assert(str_eq(vector_get_str(result, 0), src));
    vector_free(result);
}

Test(string, string_trim_start) {
    str target = str_new_l("   Waffle man likes to iron");
    str_trim(&target);
    cr_assert_str_eq(strc(target), "Waffle man likes to iron");
    cr_assert_eq(str_len(target), 24);
    str_free(target);
}

Test(string, string_trim_end) {
    str target = str_new_l("Waffle man likes to iron   ");
    str_trim(&target);
    cr_assert_str_eq(strc(target), "Waffle man likes to iron");
    cr_assert_eq(str_len(target), 24);
    str_free(target);
}

Test(string, string_trim_both) {
    str target = str_new_l(" Waffle man likes to iron   ");
    str_trim(&target);
    cr_assert_str_eq(strc(target), "Waffle man likes to iron");
    cr_assert_eq(str_len(target), 24);
    str_free(target);
}

Test(string, string_trim_blank) {
    str target = str_new_l("  ");
    str_trim(&target);
    cr_assert_str_eq(strc(target), "");
    cr_assert_eq(str_len(target), 0);
    str_free(target);
}

Test(string, string_to_l) {
    const str target = str_new_l("12345");
    cr_assert_eq(str_to_long(target), 12345L);
    str_free(target);
}

Test(string_tok, success) {
    str dest = str_new_empty(16);
    const str src = str_l("Waffle man rules");
    const str delim = str_l(" ");

    size_t pos = 0;
    bool string_left = str_tok(&dest, src, &pos, delim);
    cr_assert_eq(string_left, true);
    cr_assert_str_eq(strc(dest), "Waffle");
    cr_assert_eq(pos, 7);

    string_left = str_tok(&dest, src, &pos, delim);
    cr_assert_eq(string_left, true);
    cr_assert_str_eq(strc(dest), "man");
    cr_assert_eq(pos, 11);

    string_left = str_tok(&dest, src, &pos, delim);
    cr_assert_eq(string_left, true);
    cr_assert_str_eq(strc(dest), "rules");
    cr_assert_eq(pos, 16);

    string_left = str_tok(&dest, src, &pos, delim);
    cr_assert_eq(string_left, false);

    str_free(dest);
}

Test(string_tok, example_2) {
    str dest = str_new_empty(16);
    const str delim = str_l(" ");
    size_t pos = 0;
    const str src = str_l("8 6 4 4 1");
    unsigned i = 0;

    while (str_tok(&dest, src, &pos, delim)) {
        const char *expected[5] = { "8", "6", "4", "4", "1"};
        cr_assert_str_eq(strc(dest), expected[i]);
        i++;
    }
    cr_assert_eq(i, 5);

    str_free(dest);

}
