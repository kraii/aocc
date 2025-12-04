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
constexpr point grid_dir_up_left = {-1, -1};
constexpr point grid_dir_up_right = {1, -1};
constexpr point grid_dir_down_left = {-1, 1};
constexpr point grid_dir_down_right = {1, 1};
constexpr point grid_directions[4] = {grid_dir_up, grid_dir_right, grid_dir_down, grid_dir_left};
constexpr point grid_directions_8[8] = {grid_dir_up,   grid_dir_up_right,   grid_dir_right, grid_dir_down_right,
                                        grid_dir_down, grid_dir_down_left, grid_dir_left,  grid_dir_up_left};

grid *grid_from_file(str filepath);
grid *grid_new_copy(const grid *other);
void grid_copy(grid *dest, const grid *src);

char grid_atp(const grid *g, point p);
char grid_at(const grid *g, int x, int y);
void grid_set(grid *g, int x, int y, char c);

int grid_w(const grid *g);
int grid_h(const grid *g);
bool grid_in_bounds(const grid *grid, int x, int y);
bool grid_in_boundsp(const grid *grid, point p);

point point_add(point a, point b);
point point_turn_clock(point p);
bool point_eq(point a, point b);

#endif
