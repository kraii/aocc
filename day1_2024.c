#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "strings.h"
#include "files.h"

static int cmp(const void *l1, const void *l2) {
    return *((const long *) l1) - *((const long *) l2);
}

void part_1(const size_t n, const long *ls, const long *rs) {
    long distance = 0;

    for (size_t i = 0; i < n; i++) {
        distance += abs(ls[i] - rs[i]);
    }

    printf("Part 1 %ld\n", distance);
}

void part_2(const size_t n, const long *ls, const long *rs) {
    long *counts = calloc(rs[n-1], sizeof(long));
    if (counts == NULL) return;

    for (size_t i = 0; i < n; i++) {
        counts[rs[i]]++;
    }

    long similarity = 0;
    for (size_t i = 0; i < n; i++) {
        const long l = ls[i];
        similarity += l * counts[l];
    }

    free(counts);
    printf("Part 2 %ld\n", similarity);
}

int main(const int argc, char *argv[]) {
    assert(argc > 1);

    vector *lines = read_file_lines(argv[1]);
    const string delim = string_l("   ");
    string *buffer = string_new_empty(5);

    const size_t n = vector_len(lines);
    long *ls = calloc(n, sizeof(long));
    long *rs = calloc(n, sizeof(long));

    if (ls == NULL || rs == NULL || lines == NULL || buffer == NULL) {
        return -1;
    }

    for (size_t i = 0; i < n; i++) {
        size_t pos = 0;
        const string *line = vector_get_p(lines, i);
        assert(string_tok(buffer, line, &pos, &delim) == true);
        const long l = string_to_l(buffer);
        assert(string_tok(buffer, line, &pos, &delim) == false);
        const long r = string_to_l(buffer);
        ls[i] = l;
        rs[i] = r;
    }

    qsort(ls, n, sizeof(long), cmp);
    qsort(rs, n, sizeof(long), cmp);

    part_1(n, ls, rs);
    part_2(n, ls, rs);

    free(buffer);
    vector_free_all(lines);
    free(ls);
    free(rs);
    return 0;
}
