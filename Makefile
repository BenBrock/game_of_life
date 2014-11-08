CFLAGS = -g -O3
LDFLAGS = -lSDL2

SRCS = \
	src/game.c \
	src/term.c \
	src/grid.c \
	src/gol.c

all: gol

gol: $(SRCS)
	$(LINK.c) -o $@ $^

clean:
	@rm -fv gol $(OBJ)
