#include <stdio.h>
#include <assert.h>

#include "files.h"
#include "strings.h"
#include "vector.h"

int main(const int argc, char *argv[]) {

  assert(argc > 1);

  vector *lines = read_file_lines(str_wrap_c(argv[1]));

  int current = 50;
  int total = 0;

  str buffer = str_new_empty(5);

  for (int i = 0; i < vector_len(lines); i++) {
    const str line = vector_get_str(lines, i);
    str_substr(&buffer, line, 1, str_len(line));
    const int val = str_to_long(buffer);
    int inc = 1;
    if (str_at(line, 0) == 'L') {
      inc = -1;
    }
    for (int j = 0; j < val; j++) {
      current -= inc;
      if (current < 0) {
        current = 99;
      }
      if (current > 99) {
        current = 0;
      }
    }
    if (current == 0) {
      total++;
    }
    printf("current %d %d %c\n", current, val, str_at(line, 0));
  }
  printf("Total %d\n", total);
  str_free(buffer);
  str_vec_free(lines);
}