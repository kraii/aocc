#include <criterion/criterion.h>
#include "grid.h"

Test(grid, grid_at) {
    grid *grid = grid_from_file(strlit("../test/example_grid.txt"));

    cr_expect_eq(grid_at(grid, 0,0), 'M');
    cr_expect_eq(grid_at(grid, 2,4), 'A');
    cr_expect_eq(grid_at(grid, 9,9), 'X');

    free(grid);
}