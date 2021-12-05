CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE
CFLAGS=-Wall -Wextra -std=c99 -O1 $$(pkg-config --cflags sdl gtk+-3.0)
LDFLAGS=
LDLIBS=-lm $$(pkg-config --libs sdl SDL_image SDL_gfx gtk+-3.0) -lSDL2 -lSDL2_image -lm `sdl2-config --cflags --libs`
EXEC=

SRC= main.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: solver

-include ${DEP}

solver: ${OBJ}

clean:
	rm -f *.d *.o *.result solver

