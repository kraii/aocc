#include <criterion/criterion.h>
#include <stdio.h>

#include "regex.h"

Test(re_find, match_exists) {
  int re_compile_err = 0;
  pattern *re = re_compile(strlit("abc"), &re_compile_err);

  cr_assert_eq(re_find(re, strlit("123abc")), 3);
  cr_assert_eq(re_find(re, strlit("123abc123abc")), 3);
  cr_assert_eq(re_find(re, strlit("abc123abc")), 0);

  re_free_pat(re);
}

Test(re_find, no_match_exists) {
  int re_compile_err = 0;
  pattern *re = re_compile(strlit("[0-9]{2}"), &re_compile_err);

  cr_assert_eq(re_find(re, strlit("abc")), -1);
  cr_assert_eq(re_find(re, strlit("1foot")), -1);

  re_free_pat(re);
}

Test(re_match, loop_with_get_index) {
  int re_compile_err = 0;
  pattern *re = re_compile(strlit("[0-9]+"), &re_compile_err);
  match_data *md = re_prepare_match(re, strlit("a 123 1 456 789"));
  //                                              012345678901234
  int i = 0;
  while (re_next_match(md)) {
    constexpr int expected_matches[] = {2, 6, 8, 12};
    const long match_index = re_get_index(md);
    cr_expect_eq(match_index, expected_matches[i]);
    i++;
  }

  cr_expect_eq(i, 4);

  re_free_match(md);
  re_free_pat(re);
}

Test(re_match, loop_with_get_group) {
  int re_compile_err = 0;
  pattern *re = re_compile(strlit("([0-9]+)"), &re_compile_err);
  match_data *md = re_prepare_match(re, strlit("a 123 1 456 789"));
  //                                              012345678901234
  int i = 0;
  str buffer = str_new_empty(14);
  while (re_next_match(md)) {
    const char *expected_matches[] = {"123", "1", "456", "789"};
    cr_expect_eq(re_get_group(md, &buffer, 1), true);
    cr_expect_str_eq(strc(buffer), expected_matches[i]);
    cr_expect_eq(re_get_group(md, &buffer, 2), false);
    i++;
  }

  cr_expect_eq(i, 4);

  re_free_match(md);
  re_free_pat(re);
  str_free(buffer);
}
