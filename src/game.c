#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "game.h"

int main(int argc, char **argv)
{
  if (argc < 3) {
    fprintf(stderr, "usage: %s [grid_width] [grid_height]\n", argv[0]);
    exit(1);
  }

  int grid_width, grid_height;
  char *grid;

  grid_width = atoi(argv[1]);
  grid_height = atoi(argv[2]);

  if (grid_width <= 0 || grid_height <= 0) {
    fprintf(stderr, "error: invalid grid dimensions\n");
    exit(1);
  }

  grid = gen_grid(grid_width, grid_height);

  print_grid(grid, grid_width, grid_height);

  free(grid);

  return 0;
}

char *gen_grid(int grid_width, int grid_height)
{
  int i;
  char *grid;

  grid = (char *) malloc(sizeof(char) * grid_width * grid_height);

  srand48(time(0));

  for (i = 0; i < grid_width * grid_height; i++) {
    grid[i] = (drand48() > 0.5) ? 1 : 0;
  }

  return grid;
}

void print_grid(char *grid, int grid_width, int grid_height)
{
  int i, j;

  for (j = 0; j < grid_height; j++) {
    for (i = 0; i < grid_width; i++) {
      printf((grid[j * grid_width + i]) ? "1" : "0");
    }
    printf("\n");
  }
}

void step_grid(char *grid, int grid_width, int grid_height)
{
  int i, j;
  int neighbors;
  char *tmp;

  tmp = (char *) malloc(sizeof(char) * grid_width * grid_height);

  for (i = 0; i < grid_width * grid_height; i++) {
    tmp[i] = grid[i];
  }

  for (j = 0; j < grid_height; j++) {
    for (i = 0; i < grid_width; i++) {
      neighbors = num_neighbors(tmp, grid_width, grid_height, i, j);

      if (neighbors < 2 || neighbors > 3)
        grid[j * grid_width + i] = 0;
      if (neighbors == 3)
        grid[j * grid_width + i] = 1;
    }
  }

  free(tmp);
}

int num_neighbors(char *grid, int grid_width, int grid_height, int x, int y)
{
  int i, j;
  int neighbors;

  neighbors = 0;

  neighbors += (y * grid_width + x+1 >= 0 && y * grid_width + x+1 < grid_width * grid_height) ? grid[y * grid_width + x+1] : 0;

  neighbors += (y * grid_width + x-1 >= 0 && y * grid_width + x-1 < grid_width * grid_height) ? grid[y * grid_width + x-1] : 0;

  neighbors += ((y+1) * grid_width + x >= 0 && (y+1) * grid_width + x < grid_width * grid_height) ? grid[(y+1) * grid_width + x] : 0;

  neighbors += ((y-1) * grid_width + x >= 0 && (y-1) * grid_width + x < grid_width * grid_height) ? grid[(y-1) * grid_width + x] : 0;

  neighbors += ((y+1) * grid_width + x+1 >= 0 && (y+1) * grid_width + x+1 < grid_width * grid_height) ? grid[(y+1) * grid_width + x+1] : 0;

  neighbors += ((y+1) * grid_width + x-1 >= 0 && (y+1) * grid_width + x-1 < grid_width * grid_height) ? grid[(y+1) * grid_width + x-1] : 0;

  neighbors += ((y-1) * grid_width + x+1 >= 0 && (y-1) * grid_width + x+1 < grid_width * grid_height) ? grid[(y-1) * grid_width + x+1] : 0;

  neighbors += ((y-1) * grid_width + x-1 >= 0 && (y-1) * grid_width + x-1 < grid_width * grid_height) ? grid[(y-1) * grid_width + x-1] : 0;

  return neighbors;
}
