#include "image_treatment.h"

int str_bool(char* arg)
{
    if (arg[1] != '\0')
        errx(2, "Not 1 or 0");
    if (arg[0] == '0')
        return 0;
    return 1;
}


int main( int argc, char* args[] )
{
    if (argc!=4)
    {
        errx(1, "The number of argument(s) is wrong! Please make sure that there is only one argument\n");
    }

    int value=0;
    char *p=args[3];

    while(*p!='\0'){
        value*=10;
        value+=(int)(*p-'0');
        p++;
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

                if (str_bool(args[2]))
                {
                    //Median filter
                    MedianFilter(Loaded);
                    SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                    SDL_Flip(screenSurface);
                    wait_for_keypressed();
                }

                //Gamma
                Gamma(Loaded);
                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                SDL_Flip(screenSurface);
                wait_for_keypressed();

                //Black and White
                colorTreatment(Loaded, value);
                SDL_SaveBMP(Loaded, "BlackAndWhite.bmp");
                SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                SDL_Flip(screenSurface);
                wait_for_keypressed();

                if (str_bool(args[2]))
                {
                    //Median filter
                    MedianFilter(Loaded);
                    SDL_BlitSurface(Loaded, NULL, screenSurface, NULL);
                    SDL_Flip(screenSurface);
                    wait_for_keypressed();   
                }

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
