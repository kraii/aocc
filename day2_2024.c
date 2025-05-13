#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
        diffs[i] = report->levels[i+1] - report->levels[i];
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

int main(const int argc, char *argv[]) {
    assert(argc > 1);

    vector *lines = read_file_lines(argv[1]);
    const size_t n = vector_len(lines);
    report *reports = calloc(n, sizeof(report));
    string *buffer = string_new_empty(30);
    const string delim = string_l(" ");

    for (size_t i = 0; i < n; i++) {
        report *report = &reports[i];
        size_t pos = 0;
        while (string_tok(buffer, vector_get_p(lines, i), &pos, &delim)) {
            report->levels[report->n++] = (unsigned) string_to_l(buffer);
        }
    }

    unsigned safe = 0;

    for (size_t i = 0; i < n; i++) {
        if (is_safe(&reports[i])) {
            safe++;
        }
    }

    printf("Part 1 %u\n", safe);

    vector_free(lines);
    string_free(buffer);
    free(reports);
}
