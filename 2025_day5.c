#include <assert.h>
#include <stdio.h>

#include "files.h"
#include "strings.h"
#include "vector.h"

constexpr int NUM_BATTERIES = 12;

typedef struct range {
  long low;
  long high;
} range;

int main(const int argc, char *argv[]) {
  assert(argc > 1);

  vector *lines = read_file_lines(str_wrap_c(argv[1]));

  long part_1 = 0;
  vector *ranges = vector_new(sizeof(range));
  const str delim = strlit("-");
  str buffer = str_new_empty(20);
  bool parsingRanges = true;
  for (int i = 0; i < vector_len(lines); i++) {
    const str line = vector_get_str(lines, i);
    if (str_eq_l(line, "")) {
      parsingRanges = false;
      continue;
    }
    if (parsingRanges) {
      size_t pos = 0;
      assert(str_tok(&buffer, line, &pos, delim) == true);
      const long l = str_to_long(buffer);
      assert(str_tok(&buffer, line, &pos, delim) == true);
      const long h = str_to_long(buffer);
      range range = {l, h};
      vector_push(ranges, &range);
      continue;
    }

    const long id = str_to_long(line);
    for (int j = 0; j < vector_len(ranges); j++) {
      const range *r = vector_get(ranges, j);
      if (id >= r->low && id <= r->high) {
        part_1++;
        break;
      }
    }
  }

  printf("part 1: %ld\n", part_1);
  str_free(buffer);
  str_vec_free(lines);
  vector_free(ranges);
}