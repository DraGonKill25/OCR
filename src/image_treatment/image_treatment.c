//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Functions
SDL_Surface * colorTreatment(SDL_Surface *image);
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format);


SDL_Surface * colorTreatment(SDL_Surface *image)
{
	size_t i, j;
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
	return image;
}

Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format)
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	SDL_GetRGB(Pixel, Format, &r, &g, &b);
	if ((r + g + b) / 3 > 127)
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
    //The window we'll be rendering to
    SDL_Window * window = NULL;

    //The surface contained by the window
    SDL_Surface * screenSurface = NULL;

    //The surface loaded
    SDL_Surface * Loaded = NULL;

    //Treated surface
    SDL_Surface * NewLoaded = NULL;

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
			SDL_Delay(5000);

			//Treat the loaded image
			NewLoaded = colorTreatment(Loaded);

			SDL_BlitSurface(NewLoaded, NULL, screenSurface, NULL);
			SDL_UpdateWindowSurface( window );


            		//Wait two seconds
            		SDL_Delay( 5000 );
		}
	}
    }
    SDL_FreeSurface( Loaded );
    SDL_FreeSurface( NewLoaded );
    NewLoaded = NULL;
    Loaded = NULL;

    //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}





