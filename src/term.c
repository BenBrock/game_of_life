#include <stdio.h>
#include <assert.h>
#include "gol.h"


void term_print(grid_t *grid)
{
  int i, j;

  puts("\033[H");
  for (j = 0; j < grid->height; j++) {
    for (i = 0; i < grid->width; i++) {
      uint8_t cell = grid->cells[i + j * grid->width];
      puts(cell ? "\033[46m  \033[m" : "  ");
    }
    puts("\033[E");
  }
  
  fflush(stdout);
}
