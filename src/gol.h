#pragma once

#include <stdint.h>
#include <stdbool.h>

/* grid.c */

#define HISTORY_LENGTH 32

typedef struct {
  uint8_t *cells;
  int width;
  int height;
  
  // Stores a history of the last few hashes
  uint32_t history[HISTORY_LENGTH];
  int history_pos;
  // Whether the grid is in a cycle of less than HISTORY_LENGTH
  bool cycle;
} grid_t;

void grid_init(grid_t *grid, int width, int height);
void grid_destroy(grid_t *grid);
void grid_seed(grid_t *grid, long seed);
void grid_step(grid_t *grid);

/* game.c */

void game_launch(grid_t *grid);
void game_run();
void game_quit();

/* term.c */

void term_print(grid_t *grid);
