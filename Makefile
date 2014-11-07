CFLAGS = -g -O3
LDFLAGS = -lSDL2

SRC = src/game.c src/print_grid.c

OBJ = $(SRC:.c=.o)

all: game

game: $(OBJ)
	$(LINK.c) -o $@ $^

%.o: %.c
	$(COMPILE.c) -o $@ $^

clean:
	@rm -fv game $(OBJ)
