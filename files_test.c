#include <criterion/criterion.h>
#include "files.c"

Test(files, read_file_lines) {
    vector *result = read_file_lines("example.txt");
    cr_assert_eq(vector_len(result), 5);
    cr_assert_str_eq(string_c(vector_get_p(result, 0)), "line 1");
    cr_assert_str_eq(string_c(vector_get_p(result, 1)), "line 2 is longer than line 1");
    cr_assert_str_eq(string_c(vector_get_p(result, 2)), "line 3");
    cr_assert_str_eq(string_c(vector_get_p(result, 3)), "line 4");
    cr_assert_str_eq(string_c(vector_get_p(result, 4)), "line 5 this is the last line");

    vector_free_all(result);
}