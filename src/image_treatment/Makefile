CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -Werror -std=c99 $$(pkg-config --cflags sdl SDL_image SDL_gfx)
LDFLAGS=
LDLIBS=-lm $$(pkg-config --libs sdl SDL_image SDL_gfx)
EXEC=imagetreatment

SRC= image_treatment.c main.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: ${EXEC}

-include ${DEP}

${EXEC}: ${OBJ}
	${CC} ${LDLIBS} ${LDFLAGS} -o $@ $^


clean:
	rm -f ${OBJ} ${EXEC}
