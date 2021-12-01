CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -Werror -std=c99 $$(pkg-config --cflags sdl)
LDFLAGS=
LDLIBS= -lm $$(pkg-config --libs sdl SDL_image SDL_gfx)
EXEC=imagetreatment

EXEC= main

SRC= main.c neural_network.c neural_network_tools.c sdl_tools.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: ${EXEC}

${EXEC}: ${OBJ}

	${CC} ${LDLIBS} ${LDFLAGS} -o $@ $^ 

clean:
	rm -f ${OBJ} ${DEP} ${EXEC} main
