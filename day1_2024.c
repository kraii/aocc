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
    const long min = rs[0];
    const long max = rs[n - 1];
    long *counts = calloc(max - min, sizeof(long));
    if (counts == NULL) return;

    for (size_t i = 0; i < n; i++) {
        counts[rs[i] - min]++;
    }

    long similarity = 0;
    for (size_t i = 0; i < n; i++) {
        const long l = ls[i];
        const long ci = l - min;
        if (ci < 0 || ci > max - min - 1) continue;
        similarity += l * counts[ci];
    }

    free(counts);
    printf("Part 2 %ld\n", similarity);
}

int main(const int argc, char *argv[]) {
    assert(argc > 1);

    vector *lines = read_file_lines(argv[1]);
    const str delim = str_l("   ");
    str buffer = str_new_empty(5);

    const size_t n = vector_len(lines);
    long *ls = calloc(n, sizeof(long));
    long *rs = calloc(n, sizeof(long));

    if (ls == NULL || rs == NULL || lines == NULL || str_is_null(buffer)) {
        return -1;
    }

    for (size_t i = 0; i < n; i++) {
        size_t pos = 0;
        const str line = vector_get_str(lines, i);
        assert(str_tok(&buffer, line, &pos, delim) == true);
        const long l = str_to_long(buffer);
        assert(str_tok(&buffer, line, &pos, delim) == true);
        const long r = str_to_long(buffer);
        ls[i] = l;
        rs[i] = r;
    }

    qsort(ls, n, sizeof(long), cmp);
    qsort(rs, n, sizeof(long), cmp);

    part_1(n, ls, rs);
    part_2(n, ls, rs);

    str_free(buffer);
    vector_free_all(lines);
    free(ls);
    free(rs);
    return 0;
}
