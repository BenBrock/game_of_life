#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "SDL2/SDL.h"
#include "gol.h"


// Set to 0 for off, 255 max, lower is more intense
#define CRT_FACTOR 0

// The number of vsync frames per automata timestep
#define FRAMES_PER_STEP 1

static struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture_raw;
  SDL_Texture *texture_target;
  grid_t *grid;
  int frame;
} game;


void game_launch(grid_t *grid)
{
  game.grid = grid;
  
  int err = SDL_Init(SDL_INIT_VIDEO);
  assert(!err);
  
  game.window = SDL_CreateWindow("GOL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, grid->width, grid->height, 0);
  assert(game.window);
  
  game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  assert(game.renderer);
  
  game.texture_raw = SDL_CreateTexture(game.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, grid->width, grid->height);
  assert(game.texture_raw);
  
#if CRT_FACTOR
  SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);
  SDL_SetTextureBlendMode(game.texture_raw, SDL_BLENDMODE_BLEND);
  
  game.texture_target = SDL_CreateTexture(game.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, grid->width, grid->height);
  assert(game.texture_target);
#endif
}

static void draw_grid(grid_t *grid, SDL_Texture *texture)
{
  uint32_t *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, (void**) &pixels, &pitch);
  
  int x, y;
  for (y = 0; y < grid->height; y++) {
    for (x = 0; x < grid->width; x++) {
      uint8_t cell = grid->cells[x + y * grid->width];
      // Background is only relevant if CRT effect is disabled
      pixels[x + y * pitch / 4] = cell ? 0x000000ff : 0xffffff00;
    }
  }
  
  SDL_UnlockTexture(texture);
}

void paint(int x, int y)
{
  int sx, sy;
  SDL_GetWindowSize(game.window, &sx, &sy);
  x = x * game.grid->width / sx;
  y = y * game.grid->height / sy;
  
  if (0 <= x && x < game.grid->width && 0 <= y && y < game.grid->height) {
    game.grid->cells[x + y * game.grid->width] = 1;
  }
}

void game_run()
{
  game.frame = 0;
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
        case SDL_MOUSEMOTION:
          if (event.motion.state & SDL_BUTTON_LMASK) {
            paint(event.motion.x, event.motion.y);
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          if (event.button.button == SDL_BUTTON_LEFT) {
            paint(event.button.x, event.button.y);
          }
      }
    }
    
    if (game.frame % FRAMES_PER_STEP == 0) {
      // Step physics
      grid_step(game.grid);
      
      // Update graphics
      draw_grid(game.grid, game.texture_raw);
    }
    
    // Render target texture
#if CRT_FACTOR
    SDL_SetRenderTarget(game.renderer, game.texture_target);
    SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, CRT_FACTOR);
    SDL_RenderFillRect(game.renderer, NULL);
#endif
    
    SDL_RenderCopy(game.renderer, game.texture_raw, NULL, NULL);
    
    // Render window
    SDL_SetRenderTarget(game.renderer, NULL);
    SDL_RenderCopy(game.renderer, game.texture_target, NULL, NULL);
    SDL_RenderPresent(game.renderer);
    
    game.frame++;
  }
}

void game_quit()
{
  SDL_DestroyTexture(game.texture_target);
  SDL_DestroyTexture(game.texture_raw);
  SDL_DestroyRenderer(game.renderer);
  SDL_DestroyWindow(game.window);
  SDL_Quit();
}
