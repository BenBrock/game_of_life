#include "game.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

SDL_Window *make_grid(char *grid, int grid_width, int grid_height)
{
  SDL_Window* window = NULL;
  SDL_Surface* screen_surface = NULL;

  window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  screen_surface = SDL_GetWindowSurface(window);

  SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window);

  return window;
}

void update_grid(char *grid, int grid_width, int grid_height, SDL_Window *window)
{
}

void destroy_grid(SDL_Window *window)
{
  SDL_DestroyWindow(window);
  SDL_Quit();
}
