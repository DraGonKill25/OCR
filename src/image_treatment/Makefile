CC=gcc
CPPFLAGS= -MMD -D_XOPEN_SOURCE
CFLAGS=-D_REENTRANT -I/usr/include/SDL2
LDFLAGS=
LDLIBS=-lSDL2_image -lSDL2
EXEC=

SRC= image_treatment.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: imagetreatment

-include ${DEP}

imagetreatment: ${OBJ}

clean:
	rm -f ${OBJ} ${DEP} imagetreatment
