#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gol.h"

inline int euc_mod(int p, int q)
{
  int r = p % q;
  return (r < 0) ? r + q : r;
}

void grid_init(grid_t *grid, int width, int height)
{
  grid->cells = (uint8_t *) malloc(sizeof(uint8_t) * width * height);
  grid->width = width;
  grid->height = height;
}

void grid_destroy(grid_t *grid)
{
  free(grid->cells);
}

void grid_seed(grid_t *grid, long seed)
{
  int i, j;

  srand48(seed);

  for (j = 0; j < grid->height; j++) {
    for (i = 0; i < grid->width; i++) {
      grid->cells[i + grid->width * j] = (lrand48() & 0x1);
    }
  }
}

uint8_t grid_at(grid_t *grid, int x, int y)
{
  x = euc_mod(x, grid->width);
  y = euc_mod(y, grid->height);
  return grid->cells[x + grid->width * y];
}

inline
int block_neighbors(uint8_t *block, int pitch, int x, int y)
{
  // Assumes a little endian system
  // No branching
  // Lots of aliasing
  uint8_t *c = &block[(x + 1) + (y + 1) * pitch - 1];
  uint32_t r0 = *((uint32_t*) (c - pitch)) & 0x00ffffff;
  uint32_t r1 = *((uint32_t*) (c        )) & 0x00ff00ff;
  uint32_t r2 = *((uint32_t*) (c + pitch)) & 0x00ffffff;
  uint32_t r = r0 + r1 + r2;
  return (r + (r>>8) + (r>>16)) & 0xff;
}

void grid_step(grid_t *grid)
{
  int i, j;
  
  // Copy the grid cells into a block, but pad the sides
  // 0 0 0 0
  // 0 a a 0
  // 0 b b 0
  // 0 0 0 0
  // or in 1D,
  // 0 . . 0 0 a a 0 0 b b 0 0 c c 0 . . 0
  int pitch = grid->width + 2;
  // We need 1 extra byte since the uint32 will access it at the end.
  uint8_t *block = calloc(pitch * (grid->height + 2) + 1, 1);
  for (j = 0; j < grid->height; j++) {
    memcpy(&block[1 + (j + 1) * pitch], &grid->cells[j * grid->width], grid->width);
  }
  
  // Wrapping can be done here if needed, by simply copying
  // the edge cells to the other side of the padded grid.
  
  for (j = 0; j < grid->height; j++) {
    for (i = 0; i < grid->width; i++) {
      int neighbors = block_neighbors(block, pitch, i, j);
      switch (neighbors) {
        case 2:
          break;
        case 3:
          grid->cells[i + j * grid->width] = 1;
          break;
        default:
          // This kills the cell :'(
          grid->cells[i + j * grid->width] = 0;
          break;
      }
    }
  }
  
  free(block);
}
