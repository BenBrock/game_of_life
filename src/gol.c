#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "gol.h"


int main(int argc, char **argv)
{
  if (argc < 3) {
    fprintf(stderr, "usage: %s <width> <height>\n", argv[0]);
    exit(1);
  }
  
  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  
  if (width <= 0 || height <= 0) {
    fprintf(stderr, "error: invalid grid dimensions\n");
    exit(1);
  }
  
  grid_t grid;
  grid_init(&grid, width, height);
  grid_seed(&grid, time(0));
  
  game_launch(&grid);
  game_run();
  game_quit();
  
  grid_destroy(&grid);

  return 0;
}
