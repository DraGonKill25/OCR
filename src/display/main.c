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





int main()
{
    init_sdl();

    SDL_Surface *grid = NULL;
    grid = load_image("blank_grid.png");
    SDL_Surface *num1 = load_image("digit_on_grid/1.png");
    //SDL_Surface *num2 = load_image("digit_on_grid/2.png");
    //SDL_Surface *num3 = load_image("digit_on_grid/3.png");
    //SDL_Surface *num4 = load_image("digit_on_grid/4.png");
    //SDL_Surface *num5 = load_image("digit_on_grid/5.png");
    //SDL_Surface *num6 = load_image("digit_on_grid/6.png");
    //SDL_Surface *num7 = load_image("digit_on_grid/7.png");
    //SDL_Surface *num8 = load_image("digit_on_grid/8.png");
    //SDL_Surface *num9 = load_image("digit_on_grid/9.png");
    SDL_Rect position;
    position.x = 22;
    position.y = 28;
    position.w = 51;
    position.h = 51;
    SDL_Rect end;
    end.x = 79;
    end.y = 80;
    end.w = 51;
    end.h = 51;
    SDL_BlitSurface(num1, &end, grid, &position);
    SDL_SaveBMP(grid, "grid_solve.bmp");



    return 0;
}
