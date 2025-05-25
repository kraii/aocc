#include "files.h"
#include "strings.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rule {
  int l;
  int r;
} rule;

int main(const int argc, char *argv[]) {
  assert(argc > 1);

  str_vec *lines = read_file_lines(str_wrap_c(argv[1]));
  const size_t line_count = vector_len(lines);
  rule *rules = calloc(line_count, sizeof(rule));
  size_t rule_count = 0;
  str buffer = str_new_empty(100);
  str delim = strlit("|");

  for (size_t i = 0; i < line_count; i++) {
    const str line = vector_get_str(lines, i);
    if (str_len(line) == 0) {
      break;
    }
    size_t pos = 0;
    assert(str_tok(&buffer, line, &pos, delim) == true);
    const long l = str_to_long(buffer);
    assert(str_tok(&buffer, line, &pos, delim) == true);
    const long r = str_to_long(buffer);
    rules[i].l = l;
    rules[i].r = r;
    rule_count++;
  }

  delim = strlit(",");
  unsigned part1 = 0;
  for (size_t i = rule_count + 1; i < line_count; i++) {
    const str line = vector_get_str(lines, i);
    str_vec *pages = str_split(line, delim);

    int indexes[100];
    memset(indexes, -1, sizeof(indexes));

    for (int j = 0; j < vector_len(pages); j++) {
      const long page = str_to_long(vector_get_str(pages, j));
      assert(page < 100);
      indexes[page] = j;
    }

    bool ordered = true;
    for (size_t j = 0; j < rule_count; j++) {
      const int left_i = indexes[rules[j].l];
      const int right_i = indexes[rules[j].r];
      if (left_i >= 0 && right_i >= 0 && right_i < left_i) {
        ordered = false;
        break;
      }
    }

    if (ordered) {
      const size_t mid = vector_len(pages) / 2;
      const long page = str_to_long(vector_get_str(pages, mid));
      assert(page >= 0);
      part1 += page;
    }

    str_vector_free(pages);
  }
  printf("part 1 %u\n", part1);

  str_free(buffer);
  str_vector_free(lines);
  free(rules);
}