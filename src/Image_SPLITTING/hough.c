#include <stdio.h>
#include <math.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdlib.h>

#define M_PI 3.14159265358979323846

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

void drawLine(SDL_Surface *image, int x0, int y0, int x1, int y1, Uint32 pixel)
{
    int w = image->w;
    int h = image->h;
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while (1)
    {
        
        if (0 <= x0 && x0 < w && 0 <= y0 && y0 < h)
        {
            put_pixel(image,x0,y0,pixel);
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}


void Hough(SDL_Surface *image)
{
    double width = image->w;
    double height = image->h;
    double half_width = width / 2;
    double half_heigth = height / 2;
    double count = 200;

    double rho,theta;

    double d = sqrt(width * width + height * height);

    double num_rhos = 2 * d + 1;
    double num_thetas = num_rhos;
    double dtheta = 180 / num_thetas;
    double drhos = num_rhos / 180;

    int i;
    double *thetas = malloc(sizeof(double)*num_thetas+1);
    double *rhos = malloc(sizeof(double)*num_rhos+1);

    for (i = 0; i <= num_thetas; i++) thetas[i] = i*dtheta;
    for (i = 0; i <= num_rhos; i++) {rhos[i] = i*drhos-d; printf("%lf\n",rhos[i]);}

    double *cos_thetas = malloc(sizeof(double) * (num_thetas + 1));
    double *sin_thetas = malloc(sizeof(double) * (num_thetas + 1));
    for (int i = 0; i <= num_thetas; i++)
    {
        thetas[i] = degToRad(thetas[i]);
        cos_thetas[i] = cos(thetas[i]);
        sin_thetas[i] = sin(thetas[i]);
    }

    int **accu = malloc(sizeof(int*)*num_rhos);
    for (i = 0; i < num_rhos; i++) 
    {
        accu[i] = malloc(sizeof(int)*num_thetas);
        for (int j = 0; j < num_thetas; j++)
        {
            accu[i][j] = 0;
        }
    }

    Uint32 pixel;
    Uint8 r, g, b;
    int x,y;

    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            pixel = get_pixel(image, x, y);
            r = pixel >> 16 & 0xFF;
            g = pixel >> 8 & 0xFF;
            b = pixel & 0xFF;
            int pixel_value = (r+g+b)/3;
                  
            if (pixel_value == 255)
            {
                for (int t = 0; t < num_thetas; t++)
                {
                    rho = (x * cos_thetas[t]) + (y * sin_thetas[t]);
                    int p = rho + d;
                    accu[p][t]++;
                }
            }
        }
    }

    pixel = SDL_MapRGB(image->format,0,0,255);

    drawLine(image,-600,0,-933,1000,pixel);

    int prev = accu[0][0];
    int t = 0, rr = 0;
    int inc = 1;

    for (y = 0; y < num_rhos; y++)
    {
        for (x = 0; x < num_rhos; x++)
        {
            int val = accu[y][x];
            if (val >= prev)
            {
                prev = val;
                r = y;
                t = x;
                inc = 1;
                continue;
            }
            else if (val < prev && inc)
                inc = 0;
            if (val < count)
                continue;
            
            rho = rhos[r];
            theta = thetas[t];
            double a = cos(theta);
            double b = sin(theta);
            int x0 = a * rho;
            int y0 = b * rho;
            int x1 = x0 + d * (-b);
            int y1 = y0 + d * (a);
            int x2 = x0 - d * (-b);
            int y2 = y0 - d * (a);
            drawLine(image,x1,y1,x2,y2,pixel);
        }
    }

    for (y = 0; y < num_rhos; y++)
    {
        free(accu[y]);
    }
    free(accu);
    free(rhos);
    free(thetas);
    free(cos_thetas);
    free(sin_thetas);
}




int main(int argc, char* argv[])
{
        if (argc != 2)
        {
            errx(1, "Number of argument exceed or is less than 2");
        }

        SDL_Surface* image_surface;
        init_sdl();

        image_surface = load_image(argv[1]);

        Hough(image_surface);

        SDL_FreeSurface(image_surface);

        return 0;
}


