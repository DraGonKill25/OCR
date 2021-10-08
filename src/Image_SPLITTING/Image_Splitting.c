#include <stdio.h>
#include <SDL.h>


int x0 = x0;
int distance_L(int x0,int x1)
{
    return x1-x0;
}

// Return the list of all Images Splitting
// This list is a matrix[9][9], the images are organised like in The Array initial 
// x0 = the most left x, x1 = the most right x, y0 = the most top y, y1 = the most low y 
array[][] list_Splitting(SDL_Rect picture)
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
