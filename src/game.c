#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "SDL2/SDL.h"
#include "gol.h"


// Set to 0 for off, 255 max, lower is more intense
#define CRT_FACTOR 0

// The number of vsync frames per automata timestep
#define FRAMES_PER_STEP 1

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture_raw;
static SDL_Texture *texture_target;
static grid_t *the_grid;


void game_launch(grid_t *grid)
{
  the_grid = grid;
  
  int err = SDL_Init(SDL_INIT_VIDEO);
  assert(!err);
  
  window = SDL_CreateWindow("GOL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, grid->width, grid->height, 0);
  assert(window);
  
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  assert(renderer);
#if CRT_FACTOR
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
#endif
  
  texture_raw = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, grid->width, grid->height);
  assert(texture_raw);
#if CRT_FACTOR
  SDL_SetTextureBlendMode(texture_raw, SDL_BLENDMODE_BLEND);
#endif
  
  texture_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, grid->width, grid->height);
  assert(texture_target);
}

static void draw_grid(grid_t *grid, SDL_Texture *texture)
{
  uint32_t *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, (void**) &pixels, &pitch);
  
  int x, y;
  for (y = 0; y < grid->height; y++) {
    for (x = 0; x < grid->width; x++) {
      uint8_t cell = grid_at(grid, x, y);
      // Background is only relevant if CRT effect is disabled
      pixels[x + y * pitch / 4] = cell ? 0x000000ff : 0xffffff00;
    }
  }
  
  SDL_UnlockTexture(texture);
}

void game_run()
{
  int frame = 0;
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
    
    if (frame % FRAMES_PER_STEP == 0) {
      // Step physics
      grid_step(the_grid);
      
      // Update graphics
      draw_grid(the_grid, texture_raw);
    }
    
    // Render target texture
    SDL_SetRenderTarget(renderer, texture_target);
#if CRT_FACTOR
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, CRT_FACTOR);
    SDL_RenderFillRect(renderer, NULL);
#endif
    SDL_RenderCopy(renderer, texture_raw, NULL, NULL);
    
    // Render window
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture_target, NULL, NULL);
    SDL_RenderPresent(renderer);
    
    frame++;
  }
}

void game_quit()
{
  SDL_DestroyTexture(texture_target);
  SDL_DestroyTexture(texture_raw);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
