#include <stdio.h>
#include <string.h>
#include "vector.h"
#include "strings.h"
#include "files.h"

#include <assert.h>

#define BUFFER_SIZE 200

str_vec *read_file_lines(const str file_path) {
    FILE *f = fopen(strc(file_path), "r");
    assert(f != NULL);
    str_vec *result = vector_new(sizeof(str));
    char buffer[BUFFER_SIZE];

    while (fgets(buffer, BUFFER_SIZE, f)) {
        size_t len = strlen(buffer);
        str line = str_new(len, buffer, len);
        while (len > 0 && buffer[len - 1] != '\n' && fgets(buffer, BUFFER_SIZE, f)) {
            len = strlen(buffer);
            str_set_cap(&line, str_cap(line) + len);
            const str more = str_wrap(buffer, len);
            str_cat_l(&line, more);
        }
        str_trim(&line);
        vector_push(result, &line);
    }

    fclose(f);
    return result;
}
