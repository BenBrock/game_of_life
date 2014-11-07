#include "SDL2/SDL.h"

char *gen_grid(int grid_width, int grid_height);
void step_grid();
int num_neighbors(char *grid, int grid_width, int grid_height, int x, int y);

void game_init(int width, int height);
void game_run();
void game_destroy();

void print_grid(char *grid, int grid_width, int grid_height);
