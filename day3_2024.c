#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "files.h"
#include "regex.h"

int main(const int argc, const char *argv[]) {
    assert(argc > 1);

    vector *lines = read_file_lines(argv[1]);

    size_t total_len = 0;
    for (size_t i = 0; i < vector_len(lines); i++) {
        const str line = vector_get_str(lines, i);
        total_len += str_len(line);
    }


    str input = str_new_empty(total_len);
    for (size_t i = 0; i < vector_len(lines); i++) {
        str_cat_l(&input, vector_get_str(lines, i));
    }

    int err_code;
    pattern *p = re_compile(str_l("mul\\(([0-9]+),([0-9]+)\\)"), &err_code);
    if (p == NULL) {
        char err[100];
        pcre2_get_error_message(err_code, err, 100);
        printf("Compile error %s\n", err);
        return -1;
    }

    match_data *match = re_prepare_match(p, input);
    str buff = str_new_empty(20);
    long total = 0;
    while (re_next_match(match)) {
        re_get_group(match, &buff, 1);
        const long x = str_to_long(buff);
        re_get_group(match, &buff, 2);
        const long y = str_to_long(buff);
        total += x * y;
    }
    printf("Part 1 %ld \n", total);

    re_free_match(match);
    re_free_pat(p);
    str_free(input);
    str_vector_free(lines);
}

