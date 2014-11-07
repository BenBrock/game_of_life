#include "SDL.h"

char *gen_grid(int grid_width, int grid_height);
void print_grid(char *grid, int grid_width, int grid_height);
void step_grid(char *grid, int grid_width, int grid_height);
int num_neighbors(char *grid, int grid_width, int grid_height, int x, int y);

SDL_Window *make_grid(char *grid, int grid_width, int grid_height);
void update_grid(char *grid, int grid_width, int grid_height, SDL_Window *window);
void destroy_grid(SDL_Window *window);
