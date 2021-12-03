CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE
CFLAGS=-Wall -Wextra -std=c99 -O1 $$(pkg-config --cflags sdl SDL_image SDL_gfx)
LDFLAGS=
LDLIBS= -lm $$(pkg-config --libs sdl SDL_image SDL_gfx)
EXEC=main

SRC= main.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: main

-include ${DEP}

main: ${OBJ}

clean:
	rm -f *.d *.o main

