#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>



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


SDL_Surface* resizenumber(SDL_Surface *img)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                        60,
                        60,
                        img->format->BitsPerPixel,0,0,0,0);
  SDL_SoftStretch(img, NULL, dest, NULL);
  //SDL_BlitScaled(img, NULL, dest, NULL);
  return dest;
}


void print_grid(SDL_Surface* img, SDL_Surface* grid, int x, int y)
{
    for(int i=0; i<img->h; i++)
    {
        for(int j=0; j<img->w; j++)
        {
            Uint32 pixel= get_pixel(img, j, i);
            put_pixel(grid, x+j, y+i, pixel);
        }
    }
}
// Blackscale function
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(Pixel, Format, &r, &g, &b);
    if ((r + g + b) / 3 < 20)
    {
        return SDL_MapRGB(Format, 10, 200, 10);
    }
    else
    {
        return SDL_MapRGB(Format, 255, 255, 255);
    }
}

int main()
{
    init_sdl();

    SDL_Surface *grid = NULL;
    grid = /*SDL_CreateRGBSurface(0, (60*10), (60*10), 32, 0,0,0,0);*/ load_image("grid.jpg");
    //SDL_Surface *num1 = load_image("digit_on_grid/1.png");
    SDL_Surface *num2 = load_image("digit_on_grid/2.png");
    //SDL_Surface *num3 = load_image("digit_on_grid/3.png");
    //SDL_Surface *num4 = load_image("digit_on_grid/4.png");
    //SDL_Surface *num5 = load_image("digit_on_grid/5.png");
    //SDL_Surface *num6 = load_image("digit_on_grid/6.png");
    //SDL_Surface *num7 = load_image("digit_on_grid/7.png");
    //SDL_Surface *num8 = load_image("digit_on_grid/8.png");
    //SDL_Surface *num9 = load_image("digit_on_grid/9.png");
    /*SDL_Rect position;
    position.x = 22;
    position.y = 30;
    position.w = 51;
    position.h = 51;*/
    num2 = resizenumber(num2);
    /*
    SDL_Rect end;
    end.x = 79;
    end.y = 80;
    end.w = 51;
    end.h = 51;
    //SDL_BlitSurface(num1, &position, grid, &end);
    position.x = 22+51;
    end.x = 79+51;*/
    int x=2;
    int y=2;


    for(int i=0; i<num2->h; i++)
    {
        //x=2;
        for(int j=0; j<num2->w; j++)
        {
            print_grid(num3, grid, x, y);
            printf("%d||%d\n", x,y);
            x+=66;
            if(j%3==0)
                x+=3;
        }
        if(i%3==0)
            y+=3;
        y+=66;
    }
    //SDL_BlitSurface(num2, &position, grid , &position);
    SDL_SaveBMP(num2, "solve_digit/2.png");


    return 0;
}
