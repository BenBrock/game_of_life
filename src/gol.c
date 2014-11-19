#include <stdlib.h>
#include <stdio.h>
#include <string.h> // for strcmp
#include <time.h> // for time
#include <unistd.h> // for usleep
#include "gol.h"


int main(int argc, char **argv)
{
  if (argc < 4) {
    fprintf(stderr, "usage: %s <width> <height> <display>\n", argv[0]);
    fprintf(stderr, "\tdisplay types: sdl, term, none (for benchmarking)\n");
    exit(1);
  }
  
  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  char *display = argv[3];
  
  if (width <= 0 || height <= 0) {
    fprintf(stderr, "error: invalid grid dimensions\n");
    exit(1);
  }
  
  grid_t grid;
  grid_init(&grid, width, height);
  grid_seed(&grid, time(0));
  
  if (strcmp(display, "sdl") == 0) {
    // sdl
    game_launch(&grid);
    game_run();
    game_quit();
  }
  else if (strcmp(display, "term") == 0) {
    // term
    while (1) {
      term_print(&grid);
      usleep(1000000/30);
      grid_step(&grid);
    }
  }
  else {
    // none
    printf("Stepping 1000 generations...\n");
    int gen;
    for (gen = 0; gen < 1000; gen++) {
      grid_step(&grid);
    }
  }
  
  grid_destroy(&grid);
  return 0;
}
