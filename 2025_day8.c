#include <assert.h>
#include <stdio.h>

#include "files.h"
#include "strings.h"
#include "vector.h"

#include <inttypes.h>
#include <stdlib.h>
#include <sys/types.h>

#define EMPTY 0xFFFF

typedef struct {
  int x, y, z;
  uint16_t circuit, next;
} point;

typedef struct {
  u_int32_t ia, ib;
  u_int64_t dist;
} pair;

typedef struct {
  uint16_t size, start, end;
} circuit;

point points[1000];
u_int32_t points_count = 0;
pair pairs[1000000];
u_int64_t pairs_count = 0;

circuit circuits[1000];

static void init_circuits(void) {
  for (uint16_t i = 0; i < points_count; i++) {
    points[i].circuit = i;
    points[i].next = EMPTY;
    circuits[i].size = 1;
    circuits[i].start = i;
    circuits[i].end = i;
  }
}

static int cmp_pair_distance(const void *pair1_, const void *pair2_) {
  const pair *pair1 = pair1_;
  const pair *pair2 = pair2_;

  return (pair1->dist > pair2->dist) - (pair1->dist < pair2->dist);
}

int cmp_circuit_size_reverse(const void *c1p, const void *c2p) {
  const circuit *c1 = c1p;
  const circuit *c2 = c2p;

  return (c1->size < c2->size) - (c1->size > c2->size);
}

static void connect_pair(const pair pair) {
  const uint16_t c1 = points[pair.ia].circuit;
  const uint16_t c2 = points[pair.ib].circuit;

  if (c1 == c2)
    return;

  // merge c1 and c2
  circuits[c1].size += circuits[c2].size;
  points[circuits[c1].end].next = circuits[c2].start;
  circuits[c1].end = circuits[c2].end;

  // make all points from c2 point at c1
  for (uint16_t j = circuits[c2].start; j != EMPTY; j = points[j].next) {
    points[j].circuit = c1;
  }

  // empty c2
  circuits[c2].size = 0;
  circuits[c2].start = EMPTY;
  circuits[c2].end = EMPTY;
}

void part_1() {
  init_circuits();

  for (uint64_t i = 0; i < 1000 && i < pairs_count; i++) {
    connect_pair(pairs[i]);
  }

  qsort(circuits, sizeof(circuits) / sizeof(circuits[0]), sizeof(circuits[0]), cmp_circuit_size_reverse);

  int64_t part_1 = 1;
  part_1 *= 1 > circuits[0].size ? 1 : circuits[0].size;
  part_1 *= 1 > circuits[1].size ? 1 : circuits[1].size;
  part_1 *= 1 > circuits[2].size ? 1 : circuits[2].size;

  printf("Part 1 %ld \n", part_1);
}

static void part_2() {
  int64_t part_2 = 0;

  init_circuits();

  for (uint64_t i = 0; i < pairs_count; i++) {
    const pair pair = pairs[i];
    connect_pair(pair);


    const uint16_t c1 = points[pair.ia].circuit;
    if (circuits[c1].size == points_count) {
      part_2 = points[pair.ia].x * points[pair.ib].x;
      break;
    }
  }

  printf("Part 2 %ld\n", part_2);

}

int main(const int argc, char *argv[]) {
  assert(argc > 1);

  str_vec *lines = read_file_lines(str_wrap_c(argv[1]));

  const str delim = strlit(",");
  str buffer = str_new_empty(10);
  for (int i = 0; i < vector_len(lines); i++) {
    str line = vector_get_str(lines, i);
    size_t pos = 0;
    str_tok(&buffer, line, &pos, delim);
    points[i].x = str_to_long(buffer);
    str_tok(&buffer, line, &pos, delim);
    points[i].y = str_to_long(buffer);
    str_tok(&buffer, line, &pos, delim);
    points[i].z = str_to_long(buffer);
    points_count++;
  }

  for (uint16_t i = 0; i < points_count; i++) {
    for (uint16_t j = i + 1; j < points_count; j++) {
      const point a = points[i];
      const point b = points[j];
      const int64_t xr = a.x - b.x;
      const int64_t yr = a.y - b.y;
      const int64_t zr = a.z - b.z;

      pair pair = {0};
      pair.ia = i;
      pair.ib = j;
      pair.dist = xr * xr + yr * yr + zr * zr;
      pairs[pairs_count++] = pair;
    }
  }
  qsort(pairs, pairs_count, sizeof(pair), cmp_pair_distance);
  part_1();
  part_2();

  str_free(buffer);
  str_vec_free(lines);
}