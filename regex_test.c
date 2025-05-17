#include "regex.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    int re_compile_err = 0;
    pattern *re = re_compile(str_l("abc"), &re_compile_err);

    printf("result: %d \n", re_matches(re, str_l("abcdefg")));

    re_free(re);
}
