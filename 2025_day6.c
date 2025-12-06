#include <assert.h>
#include <stdio.h>

#include "files.h"
#include "inttypes.h"
#include "strings.h"
#include "vector.h"

#include <stdlib.h>

static int max(const int a, const int b) { return b > a ? b : a; }

static int digits_to_i(const int *digits, const int n) {
  int result = 0;
  int mult = 1;
  for (int i = n - 1; i >= 0; i--) {
    result += digits[i] * mult;
    mult *= 10;
  }
  return result;
}

static int64_t apply_op(const int64_t l, const int64_t r, const char op) {
  if (op == '*') {
    return l * r;
  }
  if (op == '+') {
    return l + r;
  }

  printf("Dun goofed\n");
  exit(1);
}

void part_1(char **argv) {
  vector *lines = read_file_lines(str_wrap_c(argv[1]));
  const int ele_count = vector_len(lines);
  vector *sums[ele_count];
  const str delim = strlit(" ");

  for (int i = 0; i < vector_len(lines); i++) {
    const str line = vector_get_str(lines, i);
    sums[i] = str_split(line, delim);
  }

  int64_t part_1 = 0;
  for (int sum_i = 0; sum_i < vector_len(sums[0]); sum_i++) {

    const str ops = vector_get_str(sums[ele_count - 1], sum_i);
    const char op = str_at(ops, 0);

    const str first_ele = vector_get_str(sums[0], sum_i);
    int64_t calc = str_to_long(first_ele);
    for (int ele_i = 1; ele_i < ele_count - 1; ele_i++) {
      const str ele = vector_get_str(sums[ele_i], sum_i);
      calc = apply_op(calc, str_to_long(ele), op);
    }
    part_1 += calc;
  }

  str_vec_free(lines);
  printf("part 1: %ld\n", part_1);
}

void part_2(char **argv) {
  int64_t part_2 = 0;
  vector *untrimmed = read_file_lines_no_trim(str_wrap_c(argv[1]));
  const int max_y = vector_len(untrimmed);
  int operands[max_y];
  int ops_count = 0;
  int longest_line_length = 0;
  for (int i = 0; i < vector_len(untrimmed); i++) {
    const str line = vector_get_str(untrimmed, i);
    longest_line_length = max(longest_line_length, str_len(line));
  }

  for (int x = longest_line_length - 2; x >= 0; x--) {
    int digit_count = 0;
    int digits[max_y];

    for (int y = 0; y < vector_len(untrimmed); y++) {
      const str line = vector_get_str(untrimmed, y);
      if (x >= str_len(line)) {
        continue;
      }
      const char c = str_at(line, x);
      if (c >= '0' && c <= '9') {
        digits[digit_count++] = c - '0';
      }
    }

    if (digit_count == 0) {
      continue;
    }
    const int operand = digits_to_i(digits, digit_count);

    operands[ops_count++] = operand;

    const char operator= str_at(vector_get_str(untrimmed, max_y - 1), x);

    if (operator!= '*' && operator!= '+') {
      continue;
    }
    int64_t calc = operands[0];
    for (int i = 1; i < ops_count; i++) {
      calc = apply_op(calc, operands[i], operator);
    }
    part_2 += calc;
    ops_count = 0;
    // skip empty line
    x--;
  }

  printf("part 2: %ld\n", part_2);
  str_vec_free(untrimmed);
}

int main(const int argc, char *argv[]) {
  assert(argc > 1);

  part_1(argv);
  part_2(argv);
}