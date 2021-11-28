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

    SDL_SaveBMP(out, "sobel.bmp");
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

double HoughTransformAngleDetection(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold, char *line_color)
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

    printf("%f    %f\n", rhos[0], rhos[2 * d]);

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
   
    if (max_peak_theta > 90 && max_peak_theta < 135){
        return max_peak_theta + 180 + 360;}
       
    if (max_peak_theta > 135){
        return max_peak_theta + 180 + 360;
    }
    return max_peak_theta-90+360; //- 90 + 360;
}


int crop_vertical_blob(SDL_Surface* img, int *border_1, int *border_2)
{
    Uint32 pixel;
    Uint8 r, g, b;
    int nb_inter = 0;
    int i = 0;
    int j = 0;

    /*-----------------------------------draw empty lines------------------------------------*/
    SDL_Surface *copy = SDL_CreateRGBSurface(0, img->w, img->h, 32,0,0,0,0);
    SDL_BlitSurface(img, NULL, copy, NULL);
    while (j < (img -> w))
    {
        while (i < (img -> h))
        {
            pixel = get_pixel(img, j, i);
            SDL_GetRGB(pixel, img -> format, &r, &g, &b);

            if (r == 0)
                nb_inter += 1;
            i += 1;
        }

        if (nb_inter > 10)
        {
            int a = 0;
            pixel = SDL_MapRGB(img -> format, 0, 255, 0);
            while(a < (img -> h))
            {
                put_pixel(copy, j, a, pixel);
                a += 1;
            }          
        }

        nb_inter = 0;
        i = 0;
        j += 1;
    }


    /*------------------------------------crop image----------------------------------------*/


    // detect green lines

    int border1 = 0;
    int border2 = 0;

    int max = 0;
    int max_border1 = 0;
    int max_border2 = 0;

    j = 0;
    i = (img -> h) / 2;


    while (j < (img -> w))
    {
        pixel = get_pixel(copy, j, i);
        SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
       
        while (r == 0 && g == 0 && b == 0 && (j < (img -> w)))
        {
            pixel = get_pixel(copy, j, i);
            SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
            j += 1;
        }

        if (r == 0 && g == 255 && b == 0)
        {
            border1 = j;

            while (r == 0 && g == 255 && b == 0 && (j < (img -> w)))
            {
                pixel = get_pixel(copy, j, i);
                SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
                j += 1;
            }

            border2 = j;
        }
       
        if (border2 - border1 > max)
        {
            max_border1 = border1;
            max_border2 = border2;
            max = border2 - border1;
        }
        j += 1;
    }


    *border_1 = max_border1;
    *border_2 = max_border2;


    // crop

    SDL_Surface* cropped_image = SDL_CreateRGBSurface(0, max_border2 - max_border1, (copy -> h), 32, 0, 0, 0, 0);

    for (int i = 0; i < (copy -> h); i++)
    {
        for (int j = max_border1; j < max_border2; j++)
        {
            pixel = get_pixel(img, j, i);
            put_pixel(cropped_image, j - max_border1, i, pixel);
        }
    }

    *img = *cropped_image;


    free(cropped_image);
    free(copy);

    return 1;


}

int crop_horizontal_blob(SDL_Surface* img, int *border_1, int *border_2)
{
    Uint32 pixel;
    Uint8 r, g, b;
    int nb_inter = 0;
    int i = 0;
    int j = 0;

    /*-----------------------------------draw empty lines------------------------------------*/
    SDL_Surface *copy = SDL_CreateRGBSurface(0, img->w, img->h, 32,0,0,0,0);
    SDL_BlitSurface(img, NULL, copy, NULL);

    while (i < (img -> h))
    {
        while (j < (img -> w))
        {
            pixel = get_pixel(img, j, i);
            SDL_GetRGB(pixel, img -> format, &r, &g, &b);

            if (r == 0)
            {
                nb_inter += 1;
            }

            j += 1;
        }

        if (nb_inter > 10)
        {
            int a = 0;
            pixel = SDL_MapRGB(img -> format, 0, 255, 0);
            while(a < (img -> w))
            {
                put_pixel(copy, a, i, pixel);
                a += 1;
            }
        }

        nb_inter = 0;
        j = 0;
        i += 1;
    }


    /*------------------------------------crop image----------------------------------------*/


    // detect green lines

    int border1 = 0;
    int border2 = 0;

    int max = 0;
    int max_border1 = 0;
    int max_border2 = 0;

    i = 0;
    j = (img -> w) / 2;


    while (i < (img -> h))
    {
        pixel = get_pixel(copy, j, i);
        SDL_GetRGB(pixel, copy -> format, &r, &g, &b);

        while (r == 0 && g == 0 && b == 0 && (i < (img -> h)))
        {
            pixel = get_pixel(copy, j, i);
            SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
            i += 1;
        }

        if (r == 0 && g == 255 && b == 0)
        {
            border1 = i;
            while (r == 0 && g == 255 && b == 0 && (i < (img -> h)))
            {
                pixel = get_pixel(copy, j, i);
                SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
                i += 1;
            }
            border2 = i;
        }

        if (border2 - border1 > max)
        {
            max_border1 = border1;
            max_border2 = border2;
            max = border2 - border1;
        }
        i += 1;
   }

    *border_1 = max_border1;
    *border_2 = max_border2;


    SDL_Surface* cropped_image = SDL_CreateRGBSurface(0, (copy -> w), max_border2 - max_border1, 32, 0, 0, 0, 0);

    for (int i = max_border1; i < max_border2; i++)
    {
        for (int j = 0; j < (img -> w); j++)
        {
            pixel = get_pixel(img, j, i);
            put_pixel(cropped_image, j, i - max_border1, pixel);
        }
    }

    *img = *cropped_image;

    free(cropped_image);
    free(copy);
    // crop successful return TRUE (1)

    return 1;
}

void DetectBiggestBlob(SDL_Surface *image_surface, int *pos_x, int *pos_y)
{
    int b1, b2;
    int b3, b4;
    int b5, b6;
    int b7, b8;
    crop_vertical_blob(image_surface, &b1, &b2);
    *pos_y = b1;
    crop_horizontal_blob(image_surface, &b3, &b4);
    *pos_x = b3;

    crop_vertical_blob(image_surface, &b5, &b6);
    if (b5 != b1)
        *pos_y += b5;

    crop_horizontal_blob(image_surface, &b7, &b8);
    if (b7 != b3)
        *pos_x += b7;

    while ((b1 != b5) || (b2 != b6) || (b3 != b7) || (b4 != b8))
    {
        printf("toto");
        crop_vertical_blob(image_surface, &b1, &b2);
        *pos_y += b1;
        crop_horizontal_blob(image_surface, &b3, &b4);
        *pos_x += b3;

        crop_vertical_blob(image_surface, &b5, &b6);
        if (b5 != b1)
            *pos_y += b5;
        crop_horizontal_blob(image_surface, &b7, &b8);
        if (b7 != b3)
        *pos_x += b7;
    }

    SDL_SaveBMP(image_surface, "blob.bmp");
}










int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "Number of argument exceed or is less than 2");
    }

    //SDL_Surface *screenSurface = NULL;

    init_sdl();
    SDL_Surface* image_de_base;
    image_de_base = load_image(argv[1]);

    //load image_surface
    //create int x,y;
    //appelle fonction avec &x et &y
    SDL_Surface *sobel_surface= SDL_CreateRGBSurface(0,image_de_base->w, image_de_base->h, 32, 0,0,0,0);
    SobelEdgeDetection(image_de_base, sobel_surface, 0.02);
    //Hough(houg_surface, image_surface, 180, 180, 3, "blue");
    SDL_Surface *final_surface= SDL_CreateRGBSurface(0,houg_surface->w, houg_surface->h, 32, 0,0,0,0);
    double angle = HoughTransformAngleDetection(sobel_surface, image_de_base, 180, 180, 3, "blue");
    printf("\n%f\n", angle);
    /*int pos_x=0;
    int pos_y=0;
    DetectBiggestBlob(image_de_base, &pos_x, &pos_y);
    printf("%d|%d\n", pos_x, pos_y);*/

    //SDL_BlitSurface(image_surface,NULL,screenSurface,NULL);
    SDL_FreeSurface(image_de_base);
    SDL_FreeSurface(sobel_surface);

    return 0;
}
