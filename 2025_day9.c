#include "files.h"

#include <assert.h>
#include <stdio.h>

#include <stdlib.h>

typedef struct {
  int64_t x, y;
} point;

typedef struct {
  point a, b;
  int64_t area;
} rect;

point red[1000];
u_int32_t red_count = 0;
rect rects[100000];
u_int32_t rect_count = 0;

static int cmp_area(const void *rect1, const void *rect2) {
  const rect *a = rect1;
  const rect *b = rect2;
  return (a->area < b->area) - (a->area > b->area);
}

static bool in_range(const int64_t a1, const int64_t a2, const int64_t b1, const int64_t b2) {
  return !(a1 <= b1 && a1 <= b2 && a2 <= b1 && a2 <= b2) && !(a1 >= b1 && a1 >= b2 && a2 >= b1 && a2 >= b2);
}

int main(const int argc, char *argv[]) {
  assert(argc > 1);

  str_vec *lines = read_file_lines(str_wrap_c(argv[1]));
  const str delim = strlit(",");
  str buffer = str_new_empty(20);

  for (int i = 0; i < vector_len(lines); i++) {
    const str line = vector_get_str(lines, i);
    size_t pos = 0;
    str_tok(&buffer, line, &pos, delim);
    red[i].x = str_to_long(buffer);
    str_tok(&buffer, line, &pos, delim);
    red[i].y = str_to_long(buffer);
    red_count++;
  }

  for (u_int32_t i = 0; i < red_count; i++) {
    for (u_int32_t j = 0; j < red_count; j++) {
      const int64_t w = labs(red[i].x - red[j].x) + 1;
      const int64_t h = labs(red[i].y - red[j].y) + 1;
      rects[rect_count].a = red[i];
      rects[rect_count].b = red[j];
      rects[rect_count++].area = w * h;
    }
  }
  qsort(rects, rect_count, sizeof(rect), cmp_area);
  printf("Part 1 %ld\n", rects[0].area);

  for (u_int32_t i = 0; i < rect_count; i++) {
    bool found_intersect = false;
    for (u_int32_t j = 0; j < red_count; j++) {
      const u_int32_t next_red = j + 1 == red_count ? 0 : j + 1;
      if (in_range(red[j].x, red[next_red].x, rects[i].a.x, rects[i].b.x) &&
          in_range(red[j].y, red[next_red].y, rects[i].a.y, rects[i].b.y)) {
        found_intersect = true;
        break;
      }
    }
    if (!found_intersect) {
      assert(rects[i].area == 1474477524);
      printf("Part 2 %ld\n", rects[i].area);
      break;
    }
  }

  str_free(buffer);
  free(lines);
}