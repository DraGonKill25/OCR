CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -Werror -std=c99
LDFLAGS=
LDLIBS=-lSDL_image -lSDL_gfx -lSDL -lm
EXEC=imagetreatment

SRC= image_treatment.c
OBJ= ${SRC:.c=.o}

all: ${EXEC}



${EXEC}: ${OBJ}

	${CC} ${LDLIBS} ${LDFLAGS} -o $@ $^ 



clean:
	rm -f ${OBJ} ${EXEC}
