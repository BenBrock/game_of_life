CFLAGS = -g -O3

SRC = src/game.c

OBJ = $(SRC:.c=.o)

all: game

game: $(OBJ)
	$(LINK.c) -o $@ $^

%.o: %.c
	$(COMPILE.c) -o $@ $^

clean:
	@rm -fv game $(OBJ)
