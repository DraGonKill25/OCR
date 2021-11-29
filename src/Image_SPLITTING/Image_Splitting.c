#include <stdio.h>
#include <math.h>
#include <err.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>

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





// return 1 if the Pixel of the image is white
// return 0 if the Pixel of the image is black
Uint32 BlackorWhite(Uint32 Pixel,SDL_PixelFormat *Format)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(Pixel,Format,&r, &g, &b);
    if ((r + g + b) / 3 > 240 )
        return 0;
    return 1;

}





// check if there is a Sudoku grid in the x and y position, 
// we return 0 if the position x and y is not the first of a sudoku grid 
// we return 1 if the position x and y is the first position of a sudoku grid



double research_L(SDL_Surface *image, int x,int y,int h)
{
    SDL_PixelFormat *Format = image->format;
    int y1 = y+6;
    int x1 = x+ 6;
    while ( y1 < h && BlackorWhite(get_pixel(image,x1,y1),Format) == 1 ) // search the lenght of a small square
    {
        y1++;
    }
    double l = y1 - y;
    return l;

    
   

}




int Good_research(SDL_Surface *image,int x,int y){

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


SDL_Surface* ZoomGrille(SDL_Surface *img, int x1,int x2, int l, int y1, int y2)
{
    Uint8 r,g,b;
    Uint32 pixel;
    SDL_Surface* result = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);
    for(int y = 0; y < abs(y1-y2); y++)
    {
        for(int x = 0; x < abs(x2-l); x++)
        {
            pixel = get_pixel(img,x+x1,y+y1);
            SDL_GetRGB(pixel,img -> format, &r,&g,&b);
            pixel = SDL_MapRGB(img->format,r,g,b);
            put_pixel(result,x,y,pixel);
        }
    }
    return result;
}






void save_cellsGrille(SDL_Surface* img,int x,int y, int l){
    int xf = x + l * 9;
    int yf = y + l * 9;
    img = ZoomGrille(img, x, xf, l * 9, y, yf);
}




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

            char name[50];
            snprintf(name, 50, "image_segmentation/square_%d%d.bmp", y, x);
            display_image(cell);
            SDL_SaveBMP(cell,name);
        }
    }
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
    int x = 0;
    int y = 0;
    int width = image_surface->w;
    int res = 0;
    int height = image_surface->h;
    while(x < width && res == 0){
        y = 0;
        while( y < height && res == 0){
            res = Good_research(image_surface,x,y);
            y++;
        }
        x++;
    }
    double l = research_L(image_surface,x,y,height);
    printf("%d + %f\n",res,l);
    printf("x=%d and y=%d\n",x,y);
    
    //search_grille(image_surface);
    
    image_surface = Zoom(image_surface, x,x + l*9, l, y,y + l*9);
    save_cells(image_surface);
    wait_for_keypressed();
    SDL_FreeSurface(image_surface);





    return 0;
}
