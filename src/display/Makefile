CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE
CFLAGS=-Wall -Werror -Wextra -std=c99 -O1
LDFLAGS=
LDLIBS=
EXEC=

SRC= main.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: display

-include ${DEP}

solver: ${OBJ}

clean:
	rm -f *.d *.o *.result solver

