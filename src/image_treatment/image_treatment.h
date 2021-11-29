#ifndef IMAGE_TREATMENT_H_
#define IMAGE_TREATMENT_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>


Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void insertionSort(Uint8 arr[], int n);
void grayscale(SDL_Surface* image_surface);
void wait_for_keypressed();
Uint32 MedianValue(SDL_Surface *image, int x, int y);
void MedianFilter(SDL_Surface* image);
void Gamma(SDL_Surface* image_surface);
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format, int value);
void colorTreatment(SDL_Surface *image, int value);
double Convolution(SDL_Surface *image, int kernel[3][3], int row, int col);
void SobelEdgeDetection(SDL_Surface *in, SDL_Surface *out, double threshold);
void DrawLine_v3(SDL_Surface *img, int x0, int y0, int x1, int y1, float wd, Uint32 pixel_color);
void Flip(SDL_Surface *img);
double HoughTransformAngleDetection(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold);//, char *line_color);




#endif
