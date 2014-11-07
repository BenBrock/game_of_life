#include <stdio.h>
#include <assert.h>
#include "gol.h"


void term_print(grid_t *grid)
{
  printf("\033[H");
  for (int y = 0; y < grid->height; y++) {
    for (int x = 0; x < grid->width; x++) {
      printf(grid_at(grid, x, y) ? "\033[07m  \033[m" : "  ");
    }
    printf("\033[E");
  }
  
  fflush(stdout);
}
