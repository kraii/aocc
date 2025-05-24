#include "files.h"
#include "strings.h"

#include <assert.h>

int main(const int argc, char *argv[]) {
  assert(argc > 1);

  vector *lines = read_file_lines(str_wrap_c(argv[1]));
}