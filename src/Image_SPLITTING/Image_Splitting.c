#include <stdio.h>
#include <math.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include "image_treatment.h"


int *longueur;




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

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}


// return 1 if the Pixel of the image is white
// return 0 if the Pixel of the image is black
Uint32 BlackorWhite(Uint32 Pixel,SDL_PixelFormat *Format)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(Pixel,Format,&r, &g, &b);
    if ((r + g + b) / 3 > 240 )
        return 1;
    return 0;

}




/*
// check if there is a Sudoku grid in the x and y position, 
// we return 0 if the position x and y is not the first of a sudoku grid 
// we return 1 if the position x and y is the first position of a sudoku grid

int differenceLX(SDL_Surface *image,int x1,int y,int w){
    SDL_PixelFormat *Format = image->format;
    int newx = x1 + 1;
    while ( newx < w && BlackorWhite(get_pixel(image,newx,y),Format) == 1){
        newx++;
    }
    return newx - x1;
}
*/

/*

int differenceLY(SDL_Surface *image,int x,int y1,int h){
    SDL_PixelFormat *Format = image->format;
    int newy = y1 + 1;
    while ( newy < h && BlackorWhite(get_pixel(image,x,newy),Format) == 1){
        newy++;
    }
    return newy - y1;
}

*/


//Research the longueur L in the axe x
int research_LX(SDL_Surface *image,int x,int y,int w){
    SDL_PixelFormat *Format = image->format;
    int x1 = x+1;
    while ( x1  < w && BlackorWhite(get_pixel(image,x1,y),Format) == 0 ){ // search the lenght of longueur in X{
        x1++;
    }
    int L = x1 - x;
    return L;
}

//Research the longueur in the axe y
int research_LY(SDL_Surface *image, int x,int y,int h)
{
    SDL_PixelFormat *Format = image->format;
    int y1 = y+1;
    while ( y1  < h && BlackorWhite(get_pixel(image,x,y1),Format) == 0 ){ // search the lenght of longueur Y{
        y1++;
    }
    int L = y1 - y;
    return L;
}


int Good_research(SDL_Surface *image,int x,int y){
    SDL_PixelFormat *Format = image->format;
    int h = image->h;
    int w = image->w;

    if (x > w-1 || y > h-1)
        return 0;
    if(BlackorWhite(get_pixel(image,x,y),Format) == 1)
        return 0;
    else{
        int LX = research_LX(image,x,y,w);
        int LY = research_LY(image,x,y,h);
        if (LX < w / 3 || LY < h / 3)
            return 0;
        if (LX - LY < 10 && LX - LY > -10)
            return 1;
        return 0;

    }
}

/*
int oldGood_research(SDL_Surface *image,int x,int y){

    SDL_PixelFormat *Format = image->format;
    int h = image->h;
    int w = image->w;

    if(x > w-1 || y > h-1)
        return 0;
    if (BlackorWhite(get_pixel(image,x,y),Format) == 1 )
    {
        return 0;
    }
    else
    {
            if(BlackorWhite(get_pixel(image,x+10,y),Format) == 1) // check the right pixel
                return 0;
            if (BlackorWhite(get_pixel(image,x,y+10),Format) == 1) // check the down pixel of y
                return 0;
            if (BlackorWhite(get_pixel(image,x+7,y+7),Format) == 1) // check if the right pixel of the down pixel is white
            {
                int y1 = y+10;
                int x1 = x+ 10;
                while ( y1 < h && BlackorWhite(get_pixel(image,x1,y1),Format) == 1 ) // search the lenght of a small square
                {
                    y1++;
                }
                int l = y1 - y;
                y1 = y1 + 10;
                while ( x1 < w && BlackorWhite(get_pixel(image,x1,y1),Format) == 1 ) // search the lenght of a small square
                {
                    x1++;
                }
                if(l -  (x1 - x) > 6 || l - (x1 - x) < -6)
                    return 0;


                int newx = x + 1;
                int oldx = x;
                int oldl = l;
                while (( newx < w) && ( l < oldl * 9)) //check all the pixel on the width of the grid
                {
                    if(BlackorWhite(get_pixel(image,newx,y),Format) == 1)
                        return 0;
                    if (newx - oldx == oldl)
                    {
                        l += l;
                        oldx = newx;
                    }
                    newx++;
                }
                l = oldl;
                int newy = y + 1;
                int oldy = y;
                while ((newy < h) &&( l < oldl)) //check all the pixel on the lenght of the grid
                {
                    if (BlackorWhite(get_pixel(image,x,newy),Format))
                        return 0;
                    if(newy - oldy == oldl)
                    {
                        l += l;
                        oldy = newy;
                    }
                    newy++;
                }
                int L = oldl * 9;
                if(( x + L/3 < h-1) && ( y + L/3 < w-1) && (2*(x + L/3)< h-1) && (2*( y +
                    L/3) < w-1) && ( x + L < h-1) && ( y + L < w-1)){
                        if ((BlackorWhite(get_pixel(image,x + L/3,y + L/3),Format) == 0) &&
                        ((BlackorWhite(get_pixel(image,2*(x + L/3),2*(y + L/3)),Format) == 0)) &&
                        ((BlackorWhite(get_pixel(image,x + L,y + L),Format))== 0))// check if the pixel of the diagonal of the square are black {
                            return 1;
                            }
            }
        return 0;
    }
}
*/


SDL_Surface *ZoomGrille(SDL_Surface *img, int x1,int x2, int l, int y1, int y2)
{
    Uint8 r,g,b;
    Uint32 pixel;
    SDL_Surface* result = SDL_CreateRGBSurface(0,l,l,32,0,0,0,0);
    for(int y = 0; y < y2 - y1; y++)
    {
        for(int x = 0; x < x2 - x1; x++)
        {
            pixel = get_pixel(img,x+x1,y+y1);
            SDL_GetRGB(pixel,img -> format, &r,&g,&b);
            pixel = SDL_MapRGB(img->format,r,g,b);
            put_pixel(result,x,y,pixel);
        }
    }
    return result;
}



SDL_Surface *save_cellsGrille(SDL_Surface* img,int x,int y, int l){
    int xf = x + l;
    int yf = y + l;
    SDL_Surface *result  = ZoomGrille(img, x, xf, l, y, yf);
    SDL_SaveBMP(result,"testHugo.bmp");

    return result;

}


/*
void search_grille(SDL_Surface *image)
{
    int x= 0;
    int y= 0;
    int h = image->h;
    int w = image->w;
    int x1 = 0;
    int y1 = 0;
    while (y < h-1)
    {
        x = 0;
        while(x < w-1)
        {
            if(Good_research(image,x,y)){
                x1 = x;
                y1 = y;
                break;
            }
            x++;
        }
        y++;
    }
    int l = *longueur;
    save_cellsGrille(image,x1,y1,l);
}
*/




SDL_Surface* Zoom(SDL_Surface *img, int x1,int x2, int x3, int y1, int y4)//, int j, int i)
{
    Uint8 r,g,b;
    Uint32 pixel;
    SDL_Surface* result = SDL_CreateRGBSurface(0,abs(x2-x3),abs(y1-y4),32,0,0,0,0);
    for(int y = 0; y < abs(y1-y4); y++)
    {
        for(int x = 0; x < abs(x2-x3); x++)
        {
            pixel = get_pixel(img,x+x1,y+y1);
            SDL_GetRGB(pixel,img -> format, &r,&g,&b);
            pixel = SDL_MapRGB(img->format,r,g,b);
            put_pixel(result,x,y,pixel);
        }
    }
    return result;
}


SDL_Surface* Resizenumber(SDL_Surface *img)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                        28,
                        28,
                        img->format->BitsPerPixel,0,0,0,0);
  SDL_SoftStretch(img, NULL, dest, NULL);
  //SDL_BlitScaled(img, NULL, dest, NULL);
  return dest;
}


void save_cells(SDL_Surface* img){
    int w = img -> w;
    int h = img -> h;
    int step_h = h / 9;
    int step_w = w / 9;
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            int x1 = x*step_w;
            int x2 = (x+1)*step_w;
            SDL_Surface* cell = Zoom(img, x1, x2, x1, y*step_h, (y+1)*step_h);//, y, x);
            cell = (Resizenumber(cell));
            char name[50];
            snprintf(name, 50, "image_segmentation/square_%d%d.bmp", y, x);
            //display_image(cell);
            SDL_SaveBMP(cell,name);
        }
    }
}

