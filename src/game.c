#include <stdlib.h>
#include <stdio.h>
// #include <unistd.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "game.h"

typedef struct {
  char *cells;
  int width;
  int height;
} grid_t;


void grid_init(grid_t *grid, int width, int height)
{
  grid->cells = malloc(width * height);
  grid->width = width;
  grid->height = height;
}

void grid_destroy(grid_t *grid)
{
  free(grid->cells);
}

void grid_seed(grid_t *grid, long seed)
{
  // Sorry, srand48 not in standard C99.
  srand(seed);
  for (int y = 0; y < grid->height; y++) {
    for (int x = 0; x < grid->width; x++) {
      grid->cells[x + grid->width * y] = (rand() & 0x1);
    }
  }
}

char grid_at(grid_t *grid, int x, int y)
{
  if (x < 0 || x >= grid->width) {
    return 0;
  }
  if (y < 0 || y >= grid->height) {
    return 0;
  }
  return grid->cells[x + grid->width * y];
}

int grid_neighbors(grid_t *grid, int x, int y)
{
  int neighbors = 0;
  
  for (int dy = -1; dy <= 1; dy++) {
    for (int dx = -1; dx <= 1; dx++) {
      if (dx == 0 && dy == 0) {
        continue;
      }
      
      if (grid_at(grid, x + dx, y + dy)) {
        neighbors++;
      }
    }
  }
  
  return neighbors;
}

void grid_step(grid_t *grid)
{
  char *new_cells = malloc(grid->width * grid->height);
  
  for (int y = 0; y < grid->height; y++) {
    for (int x = 0; x < grid->width; x++) {
      char cell = grid_at(grid, x, y);
      int neighbors = grid_neighbors(grid, x, y);
      char new_cell = cell;
      
      if (cell) {
        if (neighbors < 2 || neighbors > 3) {
          new_cell = 0;
        }
      }
      else {
        if (neighbors == 3) {
          new_cell = 1;
        }
      }
      
      new_cells[x + grid->width * y] = new_cell;
    }
  }
  
  memcpy(grid->cells, new_cells, grid->width * grid->height);
  free(new_cells);
}



static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture_cells;
static grid_t *the_grid;

void game_launch(grid_t *grid)
{
  the_grid = grid;
  
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("GOL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, grid->width, grid->height, SDL_WINDOW_FULLSCREEN_DESKTOP);
  assert(window);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  assert(renderer);
  texture_cells = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, grid->width, grid->height);
  assert(texture_cells);
}

void grid_stream_to_texture(grid_t *grid, SDL_Texture *texture)
{
  uint32_t *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, (void**) &pixels, &pitch);
  
  int x, y;
  for (y = 0; y < grid->height; y++) {
    for (x = 0; x < grid->width; x++) {
      char cell = grid_at(grid, x, y);
      pixels[x + y * pitch / 4] = cell ? 0xff000000 : 0xffffffff;
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
    grid_step(the_grid);
    
    // Update graphics
    grid_stream_to_texture(the_grid, texture_cells);
    
    // Render window
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture_cells, NULL, NULL);
    SDL_RenderPresent(renderer);
  }
}

void game_quit()
{
  SDL_DestroyTexture(texture_cells);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}


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
  grid_seed(&grid, time(NULL));
  
  game_launch(&grid);
  game_run();
  game_quit();
  
  grid_destroy(&grid);
  return 0;
}
