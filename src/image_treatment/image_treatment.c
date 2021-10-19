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
SDL_Surface * colorTreatment(SDL_Surface *image);
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format);

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


SDL_Surface * colorTreatment(SDL_Surface *image)
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
	return image;
}

/* SDL_Surface * surfaceRotation(SDL_Surface *image, double angle)
{
    int i, j;
   // SDL_PixelFormat* Imgformat = image->format;
    SDL_Surface *Returned = 
    SDL_CreateRGBSurfaceFrom(image->pixels,image->w,image->h,32,image->pitch,0,0,0,0);
    SDL_LockSurface(Returned);
    Uint32 *Dstpixels = Returned->pixels;
    Uint32 *Srcpixels = image->pixels;
    int w = image->w;
    int h = image->h;

    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {

            Dstpixels[i * w + j] = Srcpixels[(i*(int)cos(angle)-j*(int)sin(angle)) * w +
            (j*(int)cos(angle) + i*(int)sin(angle))];
        }
    }
    SDL_UnlockSurface(Returned);
    return Returned;
}
*/
/*
void Rotate(SDL_Surface* img, double angle)
{
    //int n_w, n_h;

    //GetNewSize(img, angle, &n_h, &n_w);

    SDL_Surface* new_img = SDL_CreateRGBSurface(0, img -> w, img -> h, 32, 0, 0, 0, 0);
    int center_x = img -> w / 2;
    int center_y = img -> h / 2;
    Uint32 *Dstpixels = new_img->pixels;                                       
    Uint32 *Srcpixels = img->pixels;   
    SDL_LockSurface(new_img);
    angle = angle * M_PI / 180;

    for (int i = 0; i < img -> w; i++)
    {
        for (int j = 0; j < img -> h; j++)
        {
            int x = (int) ((i - center_x) * cos(angle) - (j - center_y) * sin(angle) + center_x);
            int y = (int) ((i - center_x) * sin(angle) + (j - center_y) * cos(angle) + center_y);

            if (x >= 0 && y >= 0 && x < img -> w && y < img -> h)
            {
                Uint32 pixel = Srcpixels[j * (img->w) + i]; //get_pixel(img, x, y);
                Dstpixels[y * (img->w) + x] = pixel;        //put_pixel(new_img, i, j, pixel);
            }
            else
            {
                Uint32 pixel = SDL_MapRGB(img -> format, 255, 255, 255);
                Dstpixels[j * (img->w) + i] = pixel;        //put_pixel(new_img, i, j, pixel);
            }
        }
    }   
    SDL_UnlockSurface(new_img);
    *img = *new_img;
    SDL_FreeSurface(new_img);
}
*/
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format)
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	SDL_GetRGB(Pixel, Format, &r, &g, &b);
	if ((r + g + b) / 3 > 200)
    {
        return SDL_MapRGB(Format, 255, 255, 255);
    }
    else
    {
        return SDL_MapRGB(Format, 0, 0, 0);
    }
}


int main( int argc, char* args[] )
{
    if (argc)
    {
        
    }
    if (args)
    {
        
    }


    //The surface contained by the window
    SDL_Surface * screenSurface = NULL;

    //The surface loaded
    SDL_Surface * Loaded = NULL;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Load image
	    Loaded=IMG_Load("image_03.jpeg");
	    if(!Loaded) 
	    {
    		printf("IMG_Load: %s\n", IMG_GetError());
	    }	
	    else
	    {
		    //Create window
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
			    SDL_Delay(2000);

			    //Treat the loaded image
			    colorTreatment(Loaded);

			    SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
			    SDL_Flip(screenSurface);

                SDL_Delay(2000);

                Loaded = rotozoomSurface(screenSurface, 30, 1, 1);
                screenSurface = SDL_SetVideoMode( Loaded->w, Loaded->h, 32, SDL_SWSURFACE);
                                
                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                SDL_Flip(screenSurface);


            	//Wait two seconds
            	SDL_Delay( 2000 );
		    }
	    }
    }
    SDL_FreeSurface( screenSurface );
    SDL_FreeSurface( Loaded );

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}





