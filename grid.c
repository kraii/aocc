#include "grid.h"
#include "files.h"

inline grid * grid_from_file(str filepath) {
    read_file_lines(filepath);
}