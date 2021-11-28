CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE
CFLAGS=-Wall -Wextra -Werror -std=c99 
LDFLAGS=
LDLIBS=-lm 
EXEC=

SRC= main.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: main

-include ${DEP}

xor: ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} xor neural_network
