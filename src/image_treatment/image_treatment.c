//Using SDL and standard IO
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdio.h>
#include <err.h>
#include <math.h>

//Constants
const double PI = 3.14159265358979323846;

//Functions
void colorTreatment(SDL_Surface *image);
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format);

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
void colorTreatment(SDL_Surface *image)
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
            put_pixel(image,j,i,blackAndwhite(get_pixel(image,j,i), Format));
        }
    }
    SDL_UnlockSurface(image);
}

// Blackscale function
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(Pixel, Format, &r, &g, &b);
    if ((r + g + b) / 3 > 245)
    {
        return SDL_MapRGB(Format, 255, 255, 255);
    }
    else
    {
        return SDL_MapRGB(Format, 0, 0, 0);
    }
}




void Gamma(SDL_Surface* image_surface){
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
    SDL_SaveBMP(image_surface, "Gamma.bmp");
}




void grayscale(SDL_Surface* image_surface){
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

int main( int argc, char* args[] )
{
    if (argc!=2)
    {
        errx(1, "The number of argument(s) is wrong! Please make sure that there is only one argument");
    }


    //The surface displayed on the window
    SDL_Surface * screenSurface = NULL;

    //The surface of the image we are gonna load
    SDL_Surface * Loaded = NULL;

    //Initialize SDL video module
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Load image using SDL_image
        Loaded=IMG_Load(args[1]);
        if(!Loaded) 
        {
            printf("IMG_Load: %s\n", IMG_GetError());
        }
        else
        {
            //Create a window that is the same size as our image
            screenSurface = SDL_SetVideoMode( Loaded->w, Loaded->h, 32, SDL_SWSURFACE );
            if(!screenSurface)
            {
                printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            }
            else
            {
                //Blit the Loaded image over the window's surface
                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                //Update the surface
                SDL_Flip(screenSurface);
                wait_for_keypressed();

                //Treat the loaded image
                //
                //Grayscale
                grayscale(Loaded);
                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                SDL_Flip(screenSurface);
                wait_for_keypressed();

                //Gamma
                Gamma(Loaded);
                Loaded = IMG_Load("Gamma.bmp");
                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                SDL_Flip(screenSurface);
                wait_for_keypressed();


                //Black and White
                colorTreatment(Loaded);
                SDL_SaveBMP(Loaded, "BlackAndWhite.bmp");
                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                SDL_Flip(screenSurface);
                wait_for_keypressed();


                //Rotation and update
                Loaded = rotozoomSurface(screenSurface, -90, 1, 1);
                screenSurface = SDL_SetVideoMode( Loaded->w, Loaded->h, 32,SDL_SWSURFACE);
                SDL_BlitSurface(Loaded,NULL,screenSurface,NULL);
                SDL_Flip(screenSurface);

                //Wait for a key to be pressed to end the program
                wait_for_keypressed();
            }
        }
    }
    SDL_FreeSurface( screenSurface );
    SDL_FreeSurface( Loaded );

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}





