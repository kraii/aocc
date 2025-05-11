#include <stdio.h>
#include <string.h>
#include "vector.h"
#include "strings.h"

#define BUFFER_SIZE 200

vector *read_file_lines(const char *file_path) {
    FILE *f = fopen(file_path, "r");
    vector *result = vector_new(sizeof(string *));
    char buffer[BUFFER_SIZE];

    while (fgets(buffer, BUFFER_SIZE, f)) {
        size_t len = strlen(buffer);
        string *line = string_new(len, buffer, len);
        while (len > 0 && buffer[len - 1] != '\n' && fgets(buffer, BUFFER_SIZE, f)) {
            len = strlen(buffer);
            string_set_cap(line, string_cap(line) + len);
            string more = string_wrap(buffer, len);
            string_cat_l(line, &more);
        }
        string_trim(line);
        vector_push(result, &line);
    }

    fclose(f);
    return result;
}
