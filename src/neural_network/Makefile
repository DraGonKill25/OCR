CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE `pkg-config --cflags sdl`
CFLAGS=-Wall -Wextra -Werror -std=c99 
LDFLAGS=
LDLIBS=-lm `pkg-config --libs sdl SDL_image`
EXEC=

SRC= main.c neural_network.c neural_network_tools.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: main 

main: neural_network.o main.o neural_network_tools.o

-include ${DEP}

xor: ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} main
