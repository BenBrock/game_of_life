#pragma once

/* grid.c */

typedef struct {
  char *cells;
  int width;
  int height;
} grid_t;

void grid_init(grid_t *grid, int width, int height);
void grid_destroy(grid_t *grid);
void grid_seed(grid_t *grid, long seed);
char grid_at(grid_t *grid, int x, int y);
void grid_step(grid_t *grid);

/* game.c */

void game_launch(grid_t *grid);
void game_run();
void game_quit();

/* term.c */

void term_print(grid_t *grid);
