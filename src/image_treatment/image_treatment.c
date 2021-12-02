//Using SDL and standard IO
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <float.h>

//Constants
const double PI = 3.14159265358979323846;

//Functions
void colorTreatment(SDL_Surface *image, int value);
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format, int value);


Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

// get the pixel data depending on the format used
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


// Put the given pixel in the surface at the coordinates (x,y)
// Adapting to the format of the image
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

// Simple function applying a treatment to every pixel in an image
// in this case it is a Blackscale treatment and directly modify the image
void colorTreatment(SDL_Surface *image, int value)
{
    int i, j;
    SDL_LockSurface(image);
    int h = image->h;
    int w = image->w;
    SDL_PixelFormat* Format = image->format;
    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            put_pixel(image,j,i,blackAndwhite(get_pixel(image,j,i), Format, value));
        }
    }
    SDL_UnlockSurface(image);
}

// Blackscale function
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format, int value)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(Pixel, Format, &r, &g, &b);
    if ((r + g + b) / 3 > value)
    {
        return SDL_MapRGB(Format, 255, 255, 255);
    }
    else
    {
        return SDL_MapRGB(Format, 0, 0, 0);
    }
}

void insertionSort(Uint8 arr[], int n)
{
    int i, j;
    Uint8 key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
 
        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

Uint32 MedianValue(SDL_Surface *image, int x, int y)
{
    Uint32 pix[9];
    Uint8 tab[9];
    Uint8 r;
    Uint8 g;
    Uint8 b;

    pix[0] = get_pixel(image, x-1, y+1);
    pix[1] = get_pixel(image, x, y+1);
    pix[2] = get_pixel(image, x+1, y+1);
    pix[3] = get_pixel(image, x-1, y);
    pix[4] = get_pixel(image, x, y);
    pix[5] = get_pixel(image, x+1, y);
    pix[6] = get_pixel(image, x-1, y-1);
    pix[7] = get_pixel(image, x, y-1);
    pix[8] = get_pixel(image, x+1, y-1);

    for (int i = 0; i < 9; i++)
    {
        SDL_GetRGB(pix[i], image->format, &r,&g,&b);
        tab[i] = r;
    }
    
    insertionSort(tab, 9);
    int mid = 0;
    for (int j = 2; j <= 6; j++)
        mid += (int) tab[j];
    mid = mid/5;
    return SDL_MapRGB(image->format, tab[4],tab[4],tab[4]);
    //return SDL_MapRGB(image->format, mid,mid,mid);
}

void MedianFilter(SDL_Surface* image)
{
    for (int y = 1; y < image->h-1; y++)
    {
        for (int x = 1; x < image->w-1; x++)
            put_pixel(image, x, y, MedianValue(image, x, y));
    }
}


void Gamma(SDL_Surface* image_surface)
{
    Uint32 pixel;
    Uint8 r, g, b;
    Uint8 averager, averageg, averageb;;

    for(int i=0; i<image_surface->h; i++){
        for(int j=0; j<image_surface->w; j++){
            pixel = get_pixel(image_surface, j, i);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            averager = 255*pow((r/255.),.1);
            averageg = 255*pow((g/255.),.1);
            averageb = 255*pow((b/255.),.1);
            pixel = SDL_MapRGB(image_surface->format, averager, averageg, averageb);
            put_pixel(image_surface, j, i, pixel);
        }
    }
}




void grayscale(SDL_Surface* image_surface)
{
    Uint32 pixel;
    Uint8 r, g, b;
    Uint8 average;

    for(int i=0; i<image_surface->h; i++){
        for(int j=0; j<image_surface->w; j++){
            pixel = get_pixel(image_surface, j, i);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            average = 0.3*r + 0.59 *g + 0.11*b;
            pixel = SDL_MapRGB(image_surface->format, average, average, average);
            put_pixel(image_surface, j, i, pixel);
        }
    }
}




void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}


double Convolution(SDL_Surface *image, int kernel[3][3], int row, int col)
{
    double sum = 0;

    Uint32 pixel;
    Uint8 r, g, b;
    double px_value;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            pixel = get_pixel(image, j + col, i + row);
            SDL_GetRGB(pixel, image -> format, &r, &g, &b);

            if (r == 0 && g == 0 && b == 0)
                px_value = 1;
            else
                px_value = 0;

            sum += px_value * kernel[i][j];
        }

    }

    return sum;
}

void SobelEdgeDetection(SDL_Surface *in, SDL_Surface *out, double threshold)
{
    double gx, gy;

    double g_px;
    //double theta;
    Uint32 pixel;
    //Uint8 r, g, b;

    int kernel_x[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};

    int kernel_y[3][3] = {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}};


    for (int i = 1; i < in -> h - 2; i++)
    {
        for (int j = 1; j < in -> w - 2; j++)
        {
            gx = Convolution(in, kernel_x, i, j);
            gy = Convolution(in, kernel_y, i, j);

            g_px = sqrt(gx * gx + gy * gy);


            if ((g_px > threshold))
            {
                pixel = SDL_MapRGB(in -> format, 255, 255, 255);
                put_pixel(out, j, i, pixel);
            }
            else
            {
                pixel = SDL_MapRGB(in -> format, 0, 0, 0);
                put_pixel(out, j, i, pixel);
            }
        }
    }

    //SDL_SaveBMP(out, "sobel.bmp");
}


void DrawLine_v3(SDL_Surface *img, int x0, int y0, int x1, int y1, float wd, Uint32 pixel_color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx - dy, e2, x2, y2;
    float ed = dx + dy == 0 ? 1 : sqrt((float) dx * dx + (float) dy * dy);


    Uint32 pixel = pixel_color;

    for (wd = (wd + 1) / 2 ; ; )
    {
        if (x0 >= 0 && y0 >= 0 && x0 < img -> h && y0 < img -> w)
        {
            put_pixel(img, y0, x0, pixel);
        }

        e2 = err;
        x2 = x0;

        if (2 * e2 >= -dx)
        {
            for (e2 += dy, y2 = y0; e2 < ed * wd && (y1 != y2 || dx > dy); e2 += dx)
            {
                if (x0 >= 0 && x0 < img -> h && (y2 + sy) >= 0 && (y2 + sy) < img -> w)
                {
                    put_pixel(img, (y2 += sy), x0, pixel);
                }
            }

            if (x0 == x1)
            {
                break;
            }

            e2 = err;
            err -= dy;
            x0 += sx;
        }

        if (2 * e2 <= dy)
        {
            for (e2 = dx - e2; e2 < ed * wd && (x1 != x2 || dx < dy); e2 += dy)
            {
                if ((x2 + sx >= 0 && x2 + sx < img -> h) && (y0 >= 0 && y0 < img -> w))
                {
                    put_pixel(img, y0, x2 += sx, pixel);
                }
            }

            if (y0 == y1)
            {
                break;
            }

            err += dx;
            y0 += sy;
        }


    }
}

void Flip(SDL_Surface *img)
{
    SDL_Surface *flipped = SDL_CreateRGBSurface(0, img -> h, img -> w, 32, 0, 0, 0, 0);

    Uint32 pixel;

    for (int i = 0; i < img -> h; i++)
    {
        for (int j = 0; j < img -> w; j++)
        {
            pixel = get_pixel(img, j, i);
            put_pixel(flipped, i, j, pixel);
        }
    }

    *img = *flipped;

    free(flipped);
}

double HoughTransformAngleDetection(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold)//, char *line_color)
{
    if (num_rhos == 0 || num_thetas == 0)
    {
        errx(1, "Errror Hough : incorrect parameters\n");
    }

    int d = (int) (sqrt((edge_image -> h) * (edge_image -> h) + (edge_image -> w) * (edge_image -> w)));

    double d_theta =  181 / num_thetas;
//  double d_rho = (int) (2 * d + 1) / num_rhos;

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

    //printf("%f    %f\n", rhos[0], rhos[2 * d]);

    // init accumulator
    int accumulator[2 * d + 1][181];

    for (int i = 0; i < 2 * d + 1; i++)
        for (int j = 0; j < 181; j++)
            accumulator[i][j] = 0;


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



    //printf("%s", line_color);


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

    printf("Angle : %f\n", max_peak_theta);

    if (max_peak_theta > 90 && max_peak_theta < 135){
        return max_peak_theta + 180 + 360;}

    if (max_peak_theta > 135){
        return max_peak_theta + 180 + 360;
    }
    return max_peak_theta-90+360; //- 90 + 360;
}


SDL_Surface* resizenumber(SDL_Surface *img)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                        28,
                        28,
                        img->format->BitsPerPixel,0,0,0,0);
  SDL_SoftStretch(img, NULL, dest, NULL);
  //SDL_BlitScaled(img, NULL, dest, NULL);
  return dest;
}

Uint32 Toblack(Uint32 Pixel, SDL_PixelFormat *Format, int value)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(Pixel, Format, &r, &g, &b);
    if ((r + g + b) / 3 > value)
    {
        return SDL_MapRGB(Format, 0, 0, 0);
    }
    else
    {
        return SDL_MapRGB(Format, 255, 255, 255);
    }
}
void colorTreatment2(SDL_Surface *image, int value)
{
    int i, j;
    SDL_LockSurface(image);
    int h = image->h;
    int w = image->w;
    SDL_PixelFormat* Format = image->format;
    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            put_pixel(image,j,i,Toblack(get_pixel(image,j,i), Format, value));
        }
    }
    SDL_UnlockSurface(image);
}

