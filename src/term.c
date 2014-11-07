#include <stdio.h>
#include <assert.h>
#include "gol.h"


void term_print(grid_t *grid)
{
  int i, j;

  printf("\033[H");
  for (j = 0; j < grid->height; j++) {
    for (i = 0; i < grid->width; i++) {
      printf(grid_at(grid, i, j) ? "\033[07m  \033[m" : "  ");
    }
    printf("\033[E");
  }
  
  fflush(stdout);
}
