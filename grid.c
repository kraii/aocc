#include "grid.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "strings.h"
#include "vector.h"

grid *grid_from_file(const str filepath) {
  str_vec *lines = read_file_lines(filepath);
  const size_t y = vector_len(lines);
  assert(y > 0);
  const size_t x = str_len(vector_get_str(lines, 0));
  assert(x > 0);
  grid *result = malloc(offsetof(grid, data) + sizeof(char) * (x * y));
  memset(result->data, 0, sizeof(char) * (x * y));
  result->w = (unsigned)x;
  result->h = (unsigned)y;

  for (size_t i = 0; i < y; i++) {
    const str line = vector_get_str(lines, i);
    assert(str_len(line) == x);
    memcpy(&result->data[i * x], strc(line), sizeof(char) * x);
  }

  str_vec_free(lines);
  return result;
}

grid *grid_new_copy(const grid *other) {
  const size_t size = offsetof(grid, data) + sizeof(char) * (other->w * other->h);
  grid *result = malloc(size);
  memcpy(result, other, size);
  return result;
}

void grid_copy(grid *dest, const grid *src) {
  assert(dest->w >= src->w && dest->h >= src->h);
  const size_t size = offsetof(grid, data) + sizeof(char) * (src->w * src->h);
  memcpy(dest, src, size);
}

char grid_atp(const grid *g, const point p) { return grid_at(g, p.x, p.y); }

char grid_at(const grid *g, const int x, const int y) {
  if (x < 0 || x >= g->w || y < 0 || y > g->h) {
    fprintf(stderr, "out of bounds grid access (%u, %u)\n", x, y);
    return '\0';
  }
  return g->data[y * g->w + x];
}

void grid_set(grid *g, const int x, const int y, const char c) {
  assert(x >= 0 && x < g->w && y >= 0 && y < g->h);
  g->data[y * g->w + x] = c;
}

int grid_w(const grid *g) { return g->w; }

int grid_h(const grid *g) { return g->h; }

bool grid_in_bounds(const grid *grid, const int x, const int y) {
  return x >= 0 && y >= 0 && x < grid->w && y < grid->h;
}

bool grid_in_boundsp(const grid *grid, const point p) { return grid_in_bounds(grid, p.x, p.y); }

point point_add(const point a, const point b) { return (point){a.x + b.x, a.y + b.y}; }

point point_turn_clock(const point p) { return (point){-p.y, p.x}; }

bool point_eq(const point a, const point b) {
  return a.x == b.x && a.y == b.y;
}