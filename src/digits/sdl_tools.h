//sdl_tools.h
#ifndef SDL_TOOLS_H_
#define SDL_TOOLS_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <err.h>

void init_sdl();
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
SDL_Surface* load_image(char* path);
SDL_Surface* display_image(SDL_Surface *img);
void image_to_list(SDL_Surface* image_surface, int* input);
double *create_matrix(SDL_Surface *img);
double* matrixFromFile(char *filemane);
double **digitsMatrix();
double **goalMatrix();
SDL_Surface* resizenumber(SDL_Surface *img);

#endif
