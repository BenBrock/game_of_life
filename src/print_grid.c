#include <assert.h>
#include "game.h"

void print_grid(char *grid, int grid_width, int grid_height)
{
  int i, j;

  printf("\033[H");
  for (j = 0; j < grid_height; j++) {
    for (i = 0; i < grid_width; i++) {
      printf((grid[j * grid_width + i]) ? "\033[07m  \033[m" : "  ");
    }
    printf("\033[E");
  }
  fflush(stdout);
}
