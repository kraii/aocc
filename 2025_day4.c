#include <assert.h>
#include <stdio.h>

#include "grid.h"

#include <stdlib.h>

static void count_adj_paper(const grid *grid, const point current, int *adj_paper) {
  *adj_paper = 0;
  for (int dir = 0; dir < 8; dir++) {
    const point adj = point_add(current, grid_directions_8[dir]);
    if (grid_in_boundsp(grid, adj) && grid_atp(grid, adj) == '@') {
      (*adj_paper)++;
    }
  }
}

static void do_part_1(const grid *grid) {
  int part_1 = 0;

  for (int y = 0; y < grid_h(grid); y++) {
    for (int x = 0; x < grid_w(grid); x++) {
      const point current = {x, y};
      if (grid_atp(grid, current) == '.') {
        continue;
      }

      int adj_paper;
      count_adj_paper(grid, current, &adj_paper);
      if (adj_paper < 4) {
        part_1++;
      }
    }
  }

  printf("part 1: %d\n", part_1);
}

static void do_part_2(grid *grid) {
  int part_2 = 0;

  for (;;) {
    int removed = 0;
    for (int y = 0; y < grid_h(grid); y++) {
      for (int x = 0; x < grid_w(grid); x++) {
        const point current = {x, y};
        if (grid_atp(grid, current) == '.') {
          continue;
        }

        int adj_paper;
        count_adj_paper(grid, current, &adj_paper);
        if (adj_paper < 4) {
          removed++;
          part_2++;
          grid_set(grid, x, y, '.');
        }
      }
    }
    if (removed <= 0) {
      break;
    }
  }

  printf("part 2: %d\n", part_2);
}



int main(const int argc, char *argv[]) {
  assert(argc > 1);

  grid *grid = grid_from_file(str_wrap_c(argv[1]));
  do_part_1(grid);
  do_part_2(grid);

  free(grid);
}