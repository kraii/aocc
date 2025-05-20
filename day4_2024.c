#include <assert.h>
#include <stdio.h>

#include "grid.h"
#include "strings.h"

int find_xmas(const grid *grid, const int x, const int y) {
  const int w = grid_w(grid);
  const int h = grid_h(grid);
  int count = 0;
  // right
  if (x + 3 < w && grid_at(grid, x + 1, y) == 'M' &&
      grid_at(grid, x + 2, y) == 'A' && grid_at(grid, x + 3, y) == 'S')
    count++;

  // left
  if (x - 3 >= 0 && grid_at(grid, x - 1, y) == 'M' &&
      grid_at(grid, x - 2, y) == 'A' && grid_at(grid, x - 3, y) == 'S')
    count++;

  // up
  if (y - 3 >= 0 && grid_at(grid, x, y - 1) == 'M' &&
      grid_at(grid, x, y - 2) == 'A' && grid_at(grid, x, y - 3) == 'S')
    count++;

  // down
  if (y + 3 < h && grid_at(grid, x, y + 1) == 'M' &&
      grid_at(grid, x, y + 2) == 'A' && grid_at(grid, x, y + 3) == 'S')
    count++;

  // diagonal up-left
  if (y - 3 >= 0 && x - 3 >= 0 && grid_at(grid, x - 1, y - 1) == 'M' &&
      grid_at(grid, x - 2, y - 2) == 'A' && grid_at(grid, x - 3, y - 3) == 'S')
    count++;

  // diagonal up-right
  if (y - 3 >= 0 && x + 3 < w && grid_at(grid, x + 1, y - 1) == 'M' &&
      grid_at(grid, x + 2, y - 2) == 'A' && grid_at(grid, x + 3, y - 3) == 'S')
    count++;

  // diagonal down-left
  if (y + 3 < h && x - 3 >= 0 && grid_at(grid, x - 1, y + 1) == 'M' &&
      grid_at(grid, x - 2, y + 2) == 'A' && grid_at(grid, x - 3, y + 3) == 'S')
    count++;

  // diagonal down-right
  if (y + 3 < h && x + 3 < w && grid_at(grid, x + 1, y + 1) == 'M' &&
      grid_at(grid, x + 2, y + 2) == 'A' && grid_at(grid, x + 3, y + 3) == 'S')
    count++;

  return count;
}

int main(const int argc, char *argv[]) {
  assert(argc > 1);
  const grid *grid = grid_from_file(str_wrap_c(argv[1]));
  unsigned xmas = 0;
  for (int y = 0; y < grid_h(grid); y++) {
    for (int x = 0; x < grid_w(grid); x++) {
      const char tile = grid_at(grid, x, y);
      if (tile == 'X') {
        xmas += find_xmas(grid, x, y);
      }
    }
  }

  printf("Part 1 %u \n", xmas);
  assert(xmas == 2524);
}
