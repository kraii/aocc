#ifndef AOCC_GRID
#define AOCC_GRID
#include "strings.h"

typedef struct grid {
  int w;
  int h;
  char data[];
} grid;

typedef struct point {
  int x;
  int y;
} point;

constexpr point grid_dir_up = {0, -1};
constexpr point grid_dir_down = {0, 1};
constexpr point grid_dir_left = {-1, 0};
constexpr point grid_dir_right = {1, 0};
constexpr point grid_directions[4] = {grid_dir_up, grid_dir_right,
                                      grid_dir_down, grid_dir_left};

grid *grid_from_file(str filepath);

char grid_atp(const grid *g, point p);
char grid_at(const grid *g, int x, int y);
int grid_w(const grid *g);
int grid_h(const grid *g);

#endif
