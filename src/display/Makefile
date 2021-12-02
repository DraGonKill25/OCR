CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE
CFLAGS=-Wall -Wextra -std=c99 -O1 $$(pkg-config --cflags sdl SDL_image SDL_gfx)
LDFLAGS=
LDLIBS= -lm $$(pkg-config --libs sdl SDL_image SDL_gfx)
EXEC=

SRC= main.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: display

-include ${DEP}

display: ${OBJ}

clean:
	rm -f *.d *.o display

