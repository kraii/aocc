#include "grid.h"
#include "hashmap.h"
#include "strings.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct location {
  point pos;
  point dir;
}location;

static point find_start(const grid *grid) {
  for (int y = 0; y < grid_h(grid); y++) {
    for (int x = 0; x < grid_h(grid); x++) {
      if (grid_at(grid, x, y) == '^') {
        return (point){x,y};
      }
    }
  }
  return (point){-1,-1};
}

static bool has_loop(const grid *maze, point current, hashmap *visited) {
  if (visited == NULL) {
    printf("Visited is null\n");
    exit(1);
  }
  point dir = grid_dir_up;

  bool result = false;

  while (true) {
    if (!grid_in_boundsp(maze, current)) {
      result = false;
      break;
    }

    location loc = {current, dir};

    if (hashmap_get(visited, &loc)) {
      result = true;
      break;
    }
    constexpr bool v = true;
    hashmap_put(visited, &loc, &v);

    point next = point_add(current, dir);
    while (grid_in_boundsp(maze, next) && grid_atp(maze, next) == '#') {
      dir = point_turn_clock(dir);
      next = point_add(current, dir);
    }
    current = next;
  }

  hashmap_clear(visited);
  return result;
}

static uint64_t location_hash(const void *v) {
  const location *loc = (location *)v;
  const int vals[4] = {loc->dir.x, loc->dir.y, loc->pos.x, loc->pos.y};
  // ReSharper disable once CppRedundantCastExpression
  return hashmap_fnv_hash((u_int8_t*)vals, (sizeof(int) * 4) / sizeof(u_int8_t));
}

static bool location_eq(const void *ap, const void *bp) {
  const location *a = (location *)ap;
  const location *b = (location *)bp;
  return point_eq(a->dir, b->dir) && point_eq(a->pos, b->pos);
}

int main(const int argc, char *argv[]) {
  assert(argc > 1);
  grid *maze = grid_from_file(str_wrap_c(argv[1]));
  const point start = find_start(maze);
  point current = start;
  point dir = grid_dir_up;
  const int w = grid_w(maze);
  const int h = grid_h(maze);
  bool *visited = calloc(w * h, sizeof(bool));

  printf("Start at %d, %d\n", current.x, current.y);

  while (true) {
    if (!grid_in_boundsp(maze, current)) {
      break;
    }
    visited[(current.y * w) + current.x] = true;
    point next = point_add(current, dir);
    while (grid_in_boundsp(maze, next) && grid_atp(maze, next) == '#') {
      dir = point_turn_clock(dir);
      next = point_add(current, dir);
    }
    current = next;
  }

  grid *maze2 = grid_new_copy(maze);
  int part1_positions = 0;
  int part2 = 0;
  hashmap *part2_visited = hashmap_new(sizeof(location), sizeof(bool), location_hash, location_eq);

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      if (visited[y * w + x]) {
        part1_positions++;

        grid_set(maze2, x, y, '#');
        if (has_loop(maze2, start, part2_visited)) {
          part2++;
        }
        grid_copy(maze2, maze);
      }
    }

  }

  printf("Part 1: %d \n", part1_positions);
  printf("Part 2: %d \n", part2);

  free(maze);
  free(maze2);
  free(visited);
  hashmap_free(part2_visited);
}