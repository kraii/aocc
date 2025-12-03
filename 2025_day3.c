#include <assert.h>
#include <stdio.h>

#include "files.h"
#include "strings.h"
#include "vector.h"

#include <stdlib.h>



int main(const int argc, char *argv[]) {
  assert(argc > 1);

  vector *lines = read_file_lines(str_wrap_c(argv[1]));

  int part_1 = 0;

  for (int lineNumber = 0; lineNumber < vector_len(lines); lineNumber++) {
    const str line = vector_get_str(lines, lineNumber);
    int highest = 0;
    for (int i = 0; i < str_len(line); i++) {
      const int leftJolt = str_at(line, i) - 48;
      for (int j = i + 1; j < str_len(line); j++) {
        const int rightJolt = str_at(line, j) - 48;
        const int total = (leftJolt * 10) + rightJolt;
        if (total > highest) {
          highest = total;
        }
      }
    }
    printf("%s %d \n", strc(line), highest);
    part_1 += highest;
  }

  printf("Part 1 %d\n", part_1);
  str_vec_free(lines);
}