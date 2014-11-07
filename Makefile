CFLAGS = -g -O3 -std=c99
LDFLAGS = -lSDL2

SRCS = \
	src/game.c \
	src/term.c \
	src/grid.c \
	src/gol.c

all: game

game: $(SRCS)
	$(LINK.c) -o $@ $^

clean:
	@rm -fv game $(OBJ)
