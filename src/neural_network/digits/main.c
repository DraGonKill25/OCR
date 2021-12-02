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



//Create the .txt file with the values of pixels
void create_matrix_file(SDL_Surface *img, char *filename)
{
    //Variables
    Uint8 r;
    Uint8 g;
    Uint8 b;

    strtok(filename,".");
    strcat(filename,".txt");
    FILE *file = fopen(filename,"w");
    printf("%s \n",filename);

    for(int i = 0; i < img -> h; i++)
    {
        for(int j = 0;j < img -> w; j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
            SDL_GetRGB(pixel, img -> format, &r, &g, &b);
            if((r + g + b) / 3 < 30)
                fputs("1",file);
            else
                fputs("0",file);
        }
        fputs("\n",file);
    }
    fclose(file);
}


int main(int argc, char* argv[])
{
    if(argc!=3)
        errx(1, "nique ta race");
    init_sdl();
    SDL_Surface* toto = NULL;
    toto = IMG_Load(argv[1]);
    char* filename = argv[2];
    toto = resizenumber(toto);
    create_matrix_file(toto, filename);

    return 0;
}
