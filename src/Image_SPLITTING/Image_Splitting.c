#include <stdio.h>
#include <SDL.h>

SDL_Surface * colorTreatment(SDL_Surface *image);
Uint32 blackAndwhite(Uint32 Pixel, SDL_PixelFormat *Format);

int *_xgrille = NULL;
int *_ygrille = NULL;
int *_final = NULL;
int *_yfinal = NULL;

//Functions
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





int distance_L(int x0,int x1)
{
    return x1-x0;
}

// Return the list of all Images Splitting
// This list is a matrix[9][9], the images are organised like in The Array initial 
// x0 = the most left x, x1 = the most right x, y0 = the most top y, y1 = the most low y 
array[][] list_Splitting(SDL_Surface *image)
{
    array[9][9] new_List;
    int L = distance_L(int x0,int x1);
    int i = 0;
    int x = x0;
    int y = y0;
    while (y < y1)
    {
		int x = x0;
        while (x < x1)
            {
                
                new_List.add(IMAGE_SPLITTING(x,y))
                x += L/9;
            }
        y += L/9		
    }
    return 
}


// create a new Image with the width and height of L/9
// the Image is a square of a Sudoku that takes place in x and y
// Splitting not the Image but the Image of the square

SDL_Rect dstrect;

SDL_BlitSurface IMAGE_SPLITTING(SDL_Rect srcrect,int x,int y)
{
    srcrect.x = x0; //premiere element 
    srcrect.y = y0; //première element 
    srcrect.w = distance_L // taille de la longueur
    srcrect.h = distance_L // taille de la hauteur
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = distance_L(int x0,int x1)/9;
    dstrect.h = distance_L(int x0,int x1)/9;

    return SDL_BlitSurface(src, &srcrect, dst, &dstrect); 
}

int search_grille(SDL_Surface *image)
{
    int xgrille = &_xgrille;
    int ygrille = &_ygrille;

    while (ygrille < height)
    {
        xgrille = _xgrille;
        while (xgrille < width):
        {
            if Good_research(*image,xgrille,ygrille)
                return (xgrille,ygrille)
            xgrille++;

        }
        ygrille++;
    }
}

int Good_research(SDL_Surface *image,int x,int y)
{
    Uint32 *pixels = image->pixels;
    int w = image->w;
    SDL_PixelFormat *Format = image->format;
    if (BlackOrWhite(pixels[y * w + x], Format))
        {
            return 0;
        }
    else
        {
        }

}

int BlackOrWhite(Uint32 Pixel, SDL_PixelFormat *Format)
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	SDL_GetRGB(Pixel, Format, &r, &g, &b);
	if ((r + g + b) / 3 == 255)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}