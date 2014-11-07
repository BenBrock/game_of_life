#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include "game.h"

static char *grid;
static int grid_width;
static int grid_height;

int main(int argc, char **argv)
{
  if (argc < 3) {
    fprintf(stderr, "usage: %s <grid_width> <grid_height>\n", argv[0]);
    exit(1);
  }

  grid_width = atoi(argv[1]);
  grid_height = atoi(argv[2]);

  if (grid_width <= 0 || grid_height <= 0) {
    fprintf(stderr, "error: invalid grid dimensions\n");
    exit(1);
  }

  game_init(grid_width, grid_height);
  grid = gen_grid(grid_width, grid_height);
  game_run(grid);
  game_destroy(grid);

  free(grid);
  return 0;
}

char *gen_grid(int grid_width, int grid_height)
{
  int i;
  char *grid;

  grid = malloc(grid_width * grid_height);

  srand48(time(0));

  for (i = 0; i < grid_width * grid_height; i++) {
    grid[i] = (drand48() > 0.5) ? 1 : 0;
  }

  return grid;
}

void step_grid()
{
  int i, j;
  int neighbors;
  char *tmp;

  tmp = malloc(grid_width * grid_height);

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



static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

void game_init(int width, int height)
{
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("GOL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_FULLSCREEN_DESKTOP);
  assert(window);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  assert(renderer);
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
  assert(texture);
}

void update_texture()
{
  uint32_t *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, (void**) &pixels, &pitch);
  
  int x, y;
  for (y = 0; y < grid_height; y++) {
    for (x = 0; x < grid_width; x++) {
      pixels[x + y * pitch / 4] = grid[x + grid_width * y] ? 0xff000000 : 0xffffffff;
    }
  }
  
  SDL_UnlockTexture(texture);
}

void game_run()
{
  while (1) {
    // Handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          return;
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE) {
            return;
          }
          break;
      }
    }
    
    // Step physics
    step_grid();
    
    // Update graphics
    update_texture();
        
    // Render window
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }
}

void game_destroy()
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
