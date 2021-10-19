#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>
#include <math.h>

SDL_Surface * colorTreatment(SDL_Surface *image);
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format);

int *_xgrille = NULL;
int *_ygrille = NULL;
int *_xfinal = NULL;
int *_yfinal = NULL;
int *_L = NULL;

//Functions
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

// return 1 if the Pixel of the image is white
// return 0 if the Pixel of the image is black
Uint32 BlackorWhite(Uint32 Pixel,SDL_PixelFormat *Format)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(Pixel,Format,&r, &g, &b);
    if ((r + g + b) / 3 == 255 )
        return 1;
    return 0;

}

// create a new Image with the width and height of L/9
// the Image is a square of a Sudoku that takes place in x and y
// Splitting not the Image but the Image of the square

SDL_Rect dstrect;

SDL_Surface* IMAGE_SPLITTING(SDL_Surface* src,int x,int y)
{
    int L = *_L;
    SDL_Surface* dst = SDL_CreateRGBSurface(0, L /  9,L / 9,32,0,0,0,0);
    // sdl lock surface
    SDL_Rect srcrect;
    srcrect.x = x;
    srcrect.y = y;
    srcrect.w = L / 9;
    srcrect.h = L / 9;

    SDL_BlitSurface(src, &srcrect, dst, NULL);
    // sdl unlock surface
    return dst;
}


// Return the list of all Images Splitting
// This list is a matrix[9][9], the images are organised like in The Array initial 
// x0 = the most left x, x1 = the most right x, y0 = the most top y, y1 = the most low y 
 SDL_Surface* list_Splitting(SDL_Surface *image)
{
    SDL_Surface* new_List[9][9];
    int L = *_L;
    int y = *_ygrille;
    while (y < *_yfinal)
    {
        int x = *_xgrille;
        while (x < *_xfinal)
            {
                new_List[x][y] = (IMAGE_SPLITTING(image,x,y));
                x += L/9;
            }
        y += L/9;
    }
    return new_List; // retourne bonne image
}


// check if there is a Sudoku grid in the x and y position, 
// we return 0 if the position x and y is not the first of a sudoku grid 
// we return 1 if the position x and y is the first position of a sudoku grid
//
int Good_research(SDL_Surface *image,int x,int y){

    SDL_PixelFormat *Format = image->format;
    if (BlackorWhite(get_pixel(image,x,y),Format))
    {
        return 0;
    }
    else
    {
        if(BlackorWhite(get_pixel(image,x+1,y),Format)) // check the right pixel
            return 0;
        if (BlackorWhite(get_pixel(image,x,y+1),Format)) // check the down pixel of y
            return 0;
        if (BlackorWhite(get_pixel(image,x+1,y+1),Format)) // check if the right pixel of the down pixel is white
        {
            int y1 = y+1;
            int x1 = x+ 1;
            while (BlackorWhite(get_pixel(image,x,y),Format)) // search the lenght of a small square
            {
                x1++;
                y1++;
            }
            int l = x1 - x;
            int newx = x+1;
            int oldx = x;
            while ( l < l * 9) //check all the pixel on the width of the grid
            {
                if(BlackorWhite(get_pixel(image,newx,y),Format))
                    return 0;
                if (newx - oldx == l/9)
                {
                    l += l;
                    oldx = newx;
                }
                newx++;
            }
            int  L = l*9;
            int newy = y + 1;
            while ( newy < y + L) //check all the pixel on the lenght of the grid
            {
                if (BlackorWhite(get_pixel(image,x,newy),Format))
                    return 0;
                newy++;
            }
            if ((!BlackorWhite(get_pixel(image,x + L/3,y + L/3),Format)) &&
                ((!BlackorWhite(get_pixel(image,2*(x + L/3),2*(y + L/3)),Format))) &&
                ((!BlackorWhite(get_pixel(image,x + L,y + L),Format))))// check if the pixel of the diagonal of the square are black 
                {
                    *_xgrille = x;
                    *_ygrille = y;
                    *_xfinal = x + L;
                    *_yfinal  = y + L;
                    *_L = L;
                    return 1;
                }
            
        }
        return 0;
    }
}


int search_grille(SDL_Surface *image)
{
    int x= 0;
    int y= 0;
    int h = image->h;
    int w = image->w;
    while (y < h)
    {
        x = 0;
        while(x < w)
        {
            if(Good_research(image,x,y))
                return 1;
            x++;

        }
        y++;
    }
    return 0;
}



