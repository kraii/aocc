#include <assert.h>
#include <stdio.h>

#include "grid.h"
#include <stdlib.h>

static point find_start(const grid *grid) {
  for (int x = 0; x < grid_w(grid); x++) {
    for (int y = 0; y < grid_w(grid); y++) {
      if (grid_at(grid, x, y) == 'S') {
        const point result = {x, y};
        return result;
      }
    }
  }

  const point result = {-1, -1};
  return result;
}

void part_1(grid *grid, const point start) {
  printf("Start: %d, %d (%d,%d) \n", start.x, start.y, grid_w(grid), grid_h(grid));
  vector *beam_stack = vector_new(sizeof(point));
  vector_push(beam_stack, &start);
  u_int64_t part1 = 0;
  bool *visited = calloc(grid_h(grid) * grid_w(grid), sizeof(bool));

  while (vector_len(beam_stack) > 0) {
    point *current = vector_peek(beam_stack);

    if (!grid_in_boundsp(grid, *current) || visited[current->y * grid_w(grid) + current->x]) {
      vector_pop(beam_stack);
      continue;
    }
    if (grid_atp(grid, *current) != '^') {
      visited[current->y * grid_w(grid) + current->x] = true;
      current->y++;
      continue;
    }

    current->y++;
    part1++;
    // split beam, processing right side first
    point right_beam = {current->x + 1, current->y};
    current->x--;

    vector_push(beam_stack, &right_beam);
  }

  printf("Part 1 %ld\n", part1);

  free(visited);
}

void part_2(const grid *grid, const point start) {
  u_int64_t *beams = calloc(grid_h(grid), sizeof(u_int64_t));
  beams[start.x] = 1;
  for (int y = 0; y < grid_h(grid); y++) {
    for (int x = 0; x < grid_w(grid); x++) {
      const char current = grid_at(grid, x, y);
      if (current != '^') {
        continue;
      }
      if (x > 0) {
        beams[x - 1] += beams[x];
      }
      if (x < grid_w(grid) - 1) {
        beams[x + 1] += beams[x];
      }
      beams[x] = 0;
    }
  }

  u_int64_t part2 = 0;
  for (u_int32_t i = 0; i < grid_h(grid); i++) {
    part2 += beams[i];
  }
  printf("Part 2 %ld\n", part2);

  free(beams);
}

int main(const int argc, char *argv[]) {
  assert(argc > 1);
  grid *grid = grid_from_file(str_wrap_c(argv[1]));
  const point start = find_start(grid);
  part_1(grid, start);
  part_2(grid, start);
  free(grid);
}