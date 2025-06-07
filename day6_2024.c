#include "grid.h"
#include "strings.h"
#include "vector.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

point find_start(const grid *grid) {
  for (int y = 0; y < grid_h(grid); y++) {
    for (int x = 0; x < grid_h(grid); x++) {
      if (grid_at(grid, x, y) == '^') {
        return (point){x,y};
      }
    }
  }
  return (point){-1,-1};
}

int main(const int argc, char *argv[]) {
  assert(argc > 1);
  grid *grid = grid_from_file(str_wrap_c(argv[1]));
  point current = find_start(grid);
  point dir = grid_dir_up;
  const int w = grid_w(grid);
  const int h = grid_h(grid);
  bool *visited = calloc(w * h, sizeof(char));

  printf("Start at %d, %d\n", current.x, current.y);

  while (true) {
    if (!grid_in_bounds(grid, current.x, current.y)) {
      break;
    }
    visited[(current.y * w) + current.x] = true;
    point next = point_add(current, dir);
    while (grid_in_bounds(grid, next.x, next.y) && grid_atp(grid, next) == '#') {
      dir = point_turn_clock(dir);
      next = point_add(current, dir);
    }
    current = next;
  }

  int part1_positions = 0;
  for (int i = 0; i < w * h; i++) {
    if (visited[i]) {
      part1_positions++;
    }
  }

  printf("Part 1: %d \n", part1_positions);

  free(grid);
  free(visited);
}