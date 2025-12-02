#include <assert.h>
#include <stdio.h>

#include "files.h"
#include "strings.h"
#include "vector.h"

#include <string.h>

bool repeated_digits(const str s, const size_t test_len, const size_t segments)
{
  for (size_t segment = 1; segment < segments; segment++) {
    const char *buffer = strc(s);
    if (strncmp(buffer, buffer + segment * test_len * sizeof(char), test_len) != 0) {
      return false;
    }
  }

  return true;
}

int main(const int argc, char *argv[]) {
  assert(argc > 1);

  str_vec *lines = read_file_lines(str_wrap_c(argv[1]));
  assert(vector_len(lines) == 1);
  const str data = vector_get_str(lines, 0);

  str group = str_new_empty(30);
  str l = str_new_empty(30);
  str r = str_new_empty(30);
  str n = str_new_empty(30);
  long part_1 = 0;
  long part_2 = 0;
  const str delim = strlit(",");
  size_t pos = 0;
  while (str_tok(&group, data, &pos, delim)) {
    const int delim_index = str_find_l(group, "-");
    str_substr(&l, group, 0, delim_index);
    str_substr(&r, group, delim_index + 1, str_len(group));

    const long low = str_to_long(l);
    const long high = str_to_long(r);
    for (long i = low; i <= high; i++) {
      assert(str_from_long(&n, i));
      const size_t len = str_len(n);

      const size_t mid = len / 2;
      if (len % 2 == 0 && memcmp(strc(n), strc(n) + sizeof(char) * mid, sizeof(char) * mid) == 0) {
        part_1 += i;
      }

      for (size_t test_len = 1; test_len <= len / 2; test_len++) {
        if (len % test_len != 0)
          continue;
        const size_t segments = len / test_len;
        if (repeated_digits(n, test_len, segments)) {
          part_2 += i;
          break;
        }
      }
    }
  }

  printf("Part 1 %ld \n", part_1);
  printf("Part 2 %ld \n", part_2);
  str_free(group);
  str_free(l);
  str_free(r);
  str_free(n);

  str_vec_free(lines);
}