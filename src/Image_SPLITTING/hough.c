#include <stdio.h>
#include <math.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)


void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}



Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}





void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}




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
            
            rho = rhos[rr];
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

double HoughTransformAngleDetection(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold, char *line_color)
{
    if (num_rhos == 0 || num_thetas == 0)
    {
        errx(1, "Errror Hough : incorrect parameters\n");
    }

    int d = (int) (sqrt((edge_image -> h) * (edge_image -> h) + (edge_image -> w) * (edge_image -> w)));

    double d_theta =  181 / num_thetas;
//    double d_rho = (int) (2 * d + 1) / num_rhos;

    double thetas[181];
    double sin_thetas[181];
    double cos_thetas[181];
    double rhos[2 * d + 1];

    int step = 0;
    for (int i = 0; i < 181; i++)
    {
        thetas[i] = step;

        sin_thetas[i] = sin(thetas[i] * (M_PI / 180));
        cos_thetas[i] = cos(thetas[i] * (M_PI / 180));
        step += d_theta;
    }

    step = -d;


    for (int i = 0; i < 2 * d + 1; i++)
    {
        rhos[i] = step + i;
    }

    printf("%f    %f\n", rhos[0], rhos[2 * d]);

    // init accumulator
    int accumulator[2 * d + 1][181];

    for (int i = 0; i < 2 * d + 1; i++)
    {
        for (int j = 0; j < 181; j++)
        {
            accumulator[i][j] = 0;
        }
    }


    // begin hough

    Uint32 pixel;
    Uint8 r, g, _b;

    int theta;
    double rho;

    int half_w = edge_image -> w / 2;
    int half_h = edge_image -> h / 2;
    int rho_index = 0;


    for (int y = 0; y < edge_image -> h; y++)
    {
        for (int x = 0; x < edge_image -> w; x++)
        {
            pixel = get_pixel(edge_image, x, y);
            SDL_GetRGB(pixel, src -> format, &r, &g, &_b);

            double edge_point_1 = x - half_w;
            double edge_point_2 = y - half_h;

            if (r == 255 && g == 255 && _b == 255) // white pixel => edge pixel
            {
                for (int k = 0; k < 181; k += d_theta)
                {
                    rho = (edge_point_1 * cos_thetas[k]) + (edge_point_2 * sin_thetas[k]);

                    theta = thetas[k];

                    double min_rho_abs = DBL_MAX;

                    for (int l = 0; l <  2 * d + 1; l++)
                    {
                        // get rho index
                        if (fabs(rho - rhos[l]) < min_rho_abs)
                        {
                            rho_index = l;
                            min_rho_abs = fabs(rho - rhos[l]);
                        }
                        if (rhos[l] > rho) // reduce hough run time: rhos array in increasing order => break if current val > rho
                            break;
                    }

                    accumulator[rho_index][theta]++;
                }
            }
        }
    }

    double max_peak = 0;
    double max_peak_theta = 0;


    printf("%s", line_color);


    for (int y = 0; y < 2 * d + 1; y++)
    {
        for (int x = 0; x < 181; x++)
        {
            if (accumulator[y][x] > threshold)
            {
                rho = rhos[y];
                theta = thetas[x];

                if (accumulator[y][x] > max_peak)
                {
                    max_peak = accumulator[y][x];
                    max_peak_theta = theta;
                }
            }
        }
    }

    printf("Angle : %f", max_peak_theta);

    if (max_peak_theta > 90 && max_peak_theta < 135)
        return max_peak_theta + 180 + 360;

    if (max_peak_theta > 135)
        return max_peak_theta + 180 + 360 ;

    return max_peak_theta + 360;
}

int main(int argc, char* argv[])
{
        if (argc != 2)
        {
            errx(1, "Number of argument exceed or is less than 2");
        }

        SDL_Surface * screenSurface = NULL;

        SDL_Surface* image_surface;
        init_sdl();

        image_surface = load_image(argv[1]);

        Hough(image_surface);
        HoughTransformAngleDetection(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold, char *line_color)


        SDL_BlitSurface(image_surface,NULL,screenSurface,NULL);
        SDL_FreeSurface(image_surface);

        return 0;
}


