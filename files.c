#include "files.h"
#include "strings.h"
#include "vector.h"
#include <stdio.h>
#include <string.h>

#include <assert.h>

#define BUFFER_SIZE 200

static str_vec *do_read_file_lines(const str file_path, bool trim) {
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
    if (trim) {
      str_trim(&line);
    }

    vector_push(result, &line);
  }

  fclose(f);
  return result;
}

vector *read_file_lines(const str file_path) {
  return do_read_file_lines(file_path, true);
}

vector *read_file_lines_no_trim(const str file_path) {
  return do_read_file_lines(file_path, false);
}