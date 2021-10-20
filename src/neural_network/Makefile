CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE
CFLAGS=-Wall -Wextra -Werror -std=c99 
LDFLAGS=
LDLIBS=-lm 
EXEC=

SRC= xor.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: xor

-include ${DEP}

xor: ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} xor
