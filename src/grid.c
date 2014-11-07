#include <stdlib.h>
#include <string.h>
#include "gol.h"


void grid_init(grid_t *grid, int width, int height)
{
  grid->cells = malloc(width * height);
  grid->width = width;
  grid->height = height;
}

void grid_destroy(grid_t *grid)
{
  free(grid->cells);
}

void grid_seed(grid_t *grid, long seed)
{
  // Sorry, srand48 not in standard C99.
  srand(seed);
  for (int y = 0; y < grid->height; y++) {
    for (int x = 0; x < grid->width; x++) {
      grid->cells[x + grid->width * y] = (rand() & 0x1);
    }
  }
}

char grid_at(grid_t *grid, int x, int y)
{
  if (x < 0 || x >= grid->width) {
    return 0;
  }
  if (y < 0 || y >= grid->height) {
    return 0;
  }
  return grid->cells[x + grid->width * y];
}

int grid_neighbors(grid_t *grid, int x, int y)
{
  int neighbors = 0;
  
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      if (dx == 0 && dy == 0) {
        continue;
      }
      
      if (grid_at(grid, x + dx, y + dy)) {
        neighbors++;
      }
    }
  }
  
  return neighbors;
}

void grid_step(grid_t *grid)
{
  char *new_cells = malloc(grid->width * grid->height);
  
  for (int y = 0; y < grid->height; y++) {
    for (int x = 0; x < grid->width; x++) {
      char cell = grid_at(grid, x, y);
      int neighbors = grid_neighbors(grid, x, y);
      char new_cell = cell;
      
      if (cell) {
        if (neighbors < 2 || neighbors > 3) {
          new_cell = 0;
        }
      }
      else {
        if (neighbors == 3) {
          new_cell = 1;
        }
      }
      
      new_cells[x + grid->width * y] = new_cell;
    }
  }
  
  memcpy(grid->cells, new_cells, grid->width * grid->height);
  free(new_cells);
}
