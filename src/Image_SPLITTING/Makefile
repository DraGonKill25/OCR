CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -Werror -std=c99 $$(pkg-config --cflags sdl SDL_image)
LDFLAGS=
LDLIBS=-lm $$(pkg-config --libs sdl SDL_image)
EXEC=Image_splitting

SRC= Image_Splitting.c
OBJ= ${SRC:.c=.o}

all: ${EXEC}



${EXEC}: ${OBJ}

	${CC} ${LDLIBS} ${LDFLAGS} -o $@ $^ 



clean:
	rm -f ${OBJ} ${EXEC} Image_splitting *.swp *.swo .swn
