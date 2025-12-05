#include <assert.h>
#include <stdio.h>

#include "files.h"
#include "strings.h"
#include "vector.h"
#include "inttypes.h"

typedef struct range {
  int64_t low;
  int64_t high;
} range;

static int range_cmp(const void *r1, const void *r2) {
  const range a = *((const range *)r1);
  const range b = *((const range *)r2);

  if (a.low == b.low) {
    if (a.high == b.high) {
      return 0;
    }
    return a.high - b.high < 0 ? -1 : 1;
  }
  return a.low - b.low < 0 ? -1 : 1;
}

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

  vector_qsort(ranges, range_cmp);

  const range *first = vector_get(ranges, 0);
  long open_h = first->high;
  int64_t part_2 = open_h - first->low + 1;
  for (int i = 1; i < vector_len(ranges); i++) {
    const range *r = vector_get(ranges, i);
    long current_l = r->low;
    const long current_h = r->high;

    if (current_l <= open_h) {
      current_l = open_h + 1;
    }
    if (current_l <= current_h) {
      part_2 += current_h - current_l + 1;
      open_h = current_h;
    }
  }

  printf("part 1: %ld\n", part_1);
  printf("part 2: %ld\n", part_2);
  str_free(buffer);
  str_vec_free(lines);
  vector_free(ranges);
}