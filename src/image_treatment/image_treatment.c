//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>
#include <math.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Constants
const double M_PI = 3.14159265358979323846;

//Functions
SDL_Surface * colorTreatment(SDL_Surface *image);
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format);


SDL_Surface * colorTreatment(SDL_Surface *image)
{
	int i, j;
	SDL_LockSurface(image);
	Uint32 *pixels = image->pixels;
	int h = image->h;
	int w = image->w;
	SDL_PixelFormat *Format = image->format;
	for(i = 0; i < h; i++)
	{
    	for(j = 0; j < w; j++)
		{
        	pixels[i * w + j] = blackAndwhite(pixels[i * w + j], Format);
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


    //The window we'll be rendering to
    SDL_Window * window = NULL;

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
        	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,Loaded->w , Loaded->h, SDL_WINDOW_SHOWN );
        	if( window == NULL )
        	{
            	printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        	}
		    else
        	{
            	//Get window surface
            	screenSurface = SDL_GetWindowSurface( window );

	    
	    		//Blit the Loaded image over the window's surface
			    SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);

            	//Update the surface
            	SDL_UpdateWindowSurface( window );
			    SDL_Delay(2000);

			    //Treat the loaded image
			    colorTreatment(Loaded);

			    SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
			    SDL_UpdateWindowSurface( window );

                SDL_Delay(2000);

                Rotate(Loaded, 90);

                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                SDL_UpdateWindowSurface( window );


            	//Wait two seconds
            	SDL_Delay( 2000 );
		    }
	    }
    }
    SDL_FreeSurface( Loaded );
    Loaded = NULL;

    //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}





