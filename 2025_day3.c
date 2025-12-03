#include <assert.h>
#include <stdio.h>

#include "files.h"
#include "strings.h"
#include "vector.h"

#include <stdlib.h>

constexpr int NUM_BATTERIES = 12;

int main(const int argc, char *argv[]) {
  assert(argc > 1);

  vector *lines = read_file_lines(str_wrap_c(argv[1]));

  long total_joltage = 0;

  for (int lineNumber = 0; lineNumber < vector_len(lines); lineNumber++) {
    const str line = vector_get_str(lines, lineNumber);

    int chosen[NUM_BATTERIES] = {0};
    int index = -1;

    for (int i = 0; i < NUM_BATTERIES; i++) {
      const int end = str_len(line) - (NUM_BATTERIES - i) + 1;
      for (int j = index + 1; j < end; j++) {
        const int next = str_at(line, j) - 48;
        if (next > chosen[i]) {
          chosen[i] = next;
          index = j;
        }
      }
    }

    long row_total = 0;
    long current_pow = 1;
    for (int i = NUM_BATTERIES - 1; i >= 0; i--) {
      row_total += (chosen[i] * current_pow);
      current_pow *= 10;
    }
    total_joltage += row_total;
  }

  printf("Total %ld\n", total_joltage);
  str_vec_free(lines);
}