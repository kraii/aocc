#include "files.h"
#include "strings.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  long l;
  long rs[20];
  int n;
} equation;

static equation *parse(const struct string file, size_t *out_count) {
  const str l_r_delim = strlit(":");
  const str r_delim = strlit(" ");
  str buffer = str_new_empty(100);
  str r_buffer = str_new_empty(100);

  str_vec *lines = read_file_lines(file);
  const size_t count = vector_len(lines);
  equation *result = calloc(count, sizeof(equation));

  for (size_t i = 0; i < count; i++) {
    const str line = vector_get_str(lines, i);
    size_t pos = 0;
    assert(str_tok(&buffer, line, &pos, l_r_delim));
    result[i].l = str_to_long(buffer);
    assert(str_tok(&buffer, line, &pos, l_r_delim));

    int n = 0;
    size_t r_pos = 0;
    while (str_tok(&r_buffer, buffer, &r_pos, r_delim)) {
      assert(n < 20);
      if (str_len(r_buffer) == 0) {
        continue;
      }
      result[i].rs[n] = str_to_long(r_buffer);
      n++;
    }
    result[i].n = n;
  }

  str_free(buffer);
  str_free(r_buffer);
  str_vec_free(lines);
  *out_count = count;
  return result;
}

static long un_cat(long acc, long r) {
    if (r >= acc) {
      return -1;
    }

  const int n_digits = log10(r) + 1;

  for (int i = n_digits - 1; i >= 0; i--) {
    if (r % 10 != acc % 10) {
      return -1;
    }
    r /= 10;
    acc /= 10;
  }
  return acc;
}

bool can_solve(const equation *equation, const long acc, const size_t index, const bool concat) {
  if (index == 0) {
    return acc == equation->rs[0];
  }
  const long r = equation->rs[index];
  if (acc >= r && can_solve(equation, acc - r, index - 1, concat)) {
    return true;
  }

  if (r != 0 && acc % r == 0 && can_solve(equation, acc / r, index - 1, concat)) {
    return true;
  }

  if (concat) {
    const long un_catted = un_cat(acc, r);
    if (un_catted >= 0 && can_solve(equation, un_catted, index - 1, concat)) {
      return true;
    }
  }

  return false;
}


int main(const int argc, char *argv[]) {
  assert(argc > 1);
  size_t equation_count;
  equation *equations = parse(str_wrap_c(argv[1]), &equation_count);
  unsigned long part_1 = 0;
  unsigned long part_2 = 0;
  for (size_t i = 0; i < equation_count; i++) {
    if (can_solve(&equations[i], equations[i].l, equations[i].n - 1, false)) {
      part_1 += equations[i].l;
    }
    if (can_solve(&equations[i], equations[i].l, equations[i].n - 1, true)) {
      part_2 += equations[i].l;
    }
  }
  printf("Part 1 %lu\n", part_1);
  printf("Part 2 %lu\n", part_2);
  assert(part_1 == 4364915411363);
  assert(part_2 == 38322057216320);
  free(equations);
  return 0;
}
