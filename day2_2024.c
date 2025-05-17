#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "strings.h"
#include "files.h"

struct report {
    unsigned n;
    unsigned levels[10];
};

typedef struct report report;

bool is_safe(const report *report) {
    int diffs[9];
    for (unsigned i = 0; i < report->n - 1; i++) {
        diffs[i] = report->levels[i + 1] - report->levels[i];
    }
    const bool positive = diffs[0] > 0;
    for (unsigned i = 0; i < report->n - 1; i++) {
        if (diffs[i] == 0 || abs(diffs[i]) > 3)
            return false;
        if (positive && diffs[i] < 0)
            return false;
        if (!positive && diffs[i] > 0)
            return false;
    }
    return true;
}

void remove_at(report *dest, const report *src, const int i) {
    memcpy(dest, src, sizeof(report));
    if (i != src->n - 1) {
        memmove(&dest->levels[i], &dest->levels[i + 1], sizeof(unsigned) * src->n - i);
    }
    dest->n--;
}

int main(const int argc, char *argv[]) {
    assert(argc > 1);

    vector *lines = read_file_lines(argv[1]);
    const size_t n = vector_len(lines);
    report *reports = calloc(n, sizeof(report));
    str buffer = str_new_empty(30);
    const str delim = str_l(" ");

    for (size_t i = 0; i < n; i++) {
        report *report = &reports[i];
        size_t pos = 0;
        while (str_tok(&buffer, vector_get_str(lines, i), &pos, delim)) {
            report->levels[report->n++] = (unsigned) str_to_long(buffer);
        }
    }

    unsigned safe = 0;
    unsigned safe_with_dampener = 0;
    report temp = {0, {}};
    for (size_t i = 0; i < n; i++) {
        if (is_safe(&reports[i])) {
            safe++;
        } else {
            const size_t len = reports[i].n;
            for (size_t j = 0; j < len; j++) {
                remove_at(&temp, &reports[i], j);
                if (is_safe(&temp)) {
                    safe_with_dampener++;
                    break;
                }
            }
        }
    }

    printf("Part 1 %u\n", safe);
    printf("Part 2 %u\n", safe + safe_with_dampener);

    vector_free(lines);
    str_free(buffer);
    free(reports);
}
