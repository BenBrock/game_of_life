#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "SDL2/SDL.h"
#include "gol.h"


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
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
      uint8_t cell = grid_at(grid, x, y);
      /*
        0x101010ff
        0x10101000
        0x01010100 - disappear
        0x01108000 - trippy
      */
      pixels[x + y * pitch / 4] = cell ? cell * 0x01108000 : 0xffffffff;
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
