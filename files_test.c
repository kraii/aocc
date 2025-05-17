#include <criterion/criterion.h>
#include "files.h"

Test(files, read_file_lines) {
    vector *result = read_file_lines("example.txt");
    cr_assert_eq(vector_len(result), 5);
    cr_assert_str_eq(strc(vector_get_str(result, 0)), "line 1");
    cr_assert_str_eq(strc(vector_get_str(result, 1)), "line 2 is longer than line 1");
    cr_assert_str_eq(strc(vector_get_str(result, 2)), "line 3");
    cr_assert_str_eq(strc(vector_get_str(result, 3)), "line 4");
    cr_assert_str_eq(strc(vector_get_str(result, 4)), "line 5 this is the last line");

    vector_free(result);
}