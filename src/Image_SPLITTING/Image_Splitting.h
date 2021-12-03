#ifndef IMAGE_SPLITTING_H
#define IMAGE_SPLITTING_H

#include <stdio.h>                                                                                   
#include <math.h>                                                               
#include <err.h>                                                                
#include <SDL/SDL.h>                                                            
#include <SDL/SDL_image.h>                                                      
#include <stdlib.h>

void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
Uint32 BlackorWhite(Uint32 Pixel,SDL_PixelFormat *Format);
int research_LX(SDL_Surface *image,int x,int y,int w);
int research_LY(SDL_Surface *image, int x,int y,int h);
int Good_research(SDL_Surface *image,int x,int y);
SDL_Surface *ZoomGrille(SDL_Surface *img, int x1,int x2, int l, int y1, int y2);
SDL_Surface *save_cellsGrille(SDL_Surface* img,int x,int y, int l);
SDL_Surface* Zoom(SDL_Surface *img, int x1,int x2, int x3, int y1, int y4);
SDL_Surface* resizenumber(SDL_Surface *img);
void save_cells(SDL_Surface* img);

#endif
