#include <assert.h>
#include <stdio.h>

#include "strings.h"
#include "files.h"
#include "regex.h"

static pattern *compile_or_exit(const str pats) {
    int err_code;
    pattern *p = re_compile(pats, &err_code);
    if (p == NULL) {
        const str err = re_get_error_message(err_code);
        printf("Compile error %s\n", strc(err));
        str_free(err);
        exit(-1);
    }
    return p;
}

static void part1(const str input) {
    pattern *p = compile_or_exit(strlit("mul\\(([0-9]+),([0-9]+)\\)"));

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
}

static void part2(const str input) {
    pattern *p = compile_or_exit(strlit("mul\\(([0-9]+),([0-9]+)\\)|(do\\(\\))|(don't\\(\\))"));

    match_data *match = re_prepare_match(p, input);
    str buff = str_new_empty(20);
    long total = 0;
    bool enabled = true;
    while (re_next_match(match)) {
        if (re_get_group(match, &buff, 1) && enabled) {
            const long x = str_to_long(buff);
            re_get_group(match, &buff, 2);
            const long y = str_to_long(buff);
            total += x * y;
        } else if (re_get_group(match, &buff, 3)) {
            enabled = true;
        } else if (re_get_group(match, &buff, 4)) {
            enabled = false;
        }
    }
    printf("Part 1 %ld \n", total);
    re_free_match(match);
    re_free_pat(p);
}

int main(const int argc, char *argv[]) {
    assert(argc > 1);

    vector *lines = read_file_lines(str_wrap_c(argv[1]));

    size_t total_len = 0;
    for (size_t i = 0; i < vector_len(lines); i++) {
        const str line = vector_get_str(lines, i);
        total_len += str_len(line);
    }


    str input = str_new_empty(total_len);
    for (size_t i = 0; i < vector_len(lines); i++) {
        str_cat_l(&input, vector_get_str(lines, i));
    }

    part1(input);
    part2(input);

    str_free(input);
    str_vector_free(lines);
}
