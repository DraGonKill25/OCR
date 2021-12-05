#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "solver.h"
//#include "../Image_Splitting.h"
//#include "../image_treatment.h"



int main(int argc, char* argv[])
{


    char toto[9][9];//create a temp array of the char in the origin file
    int good_one[9][9];//array that will contain the grid
    int toprint[9][9];//array for print original number
    int i=0,j=0,c;

    FILE *file = NULL;
    file = fopen(argv[1], "r");
    if(file == NULL)
        exit(1);

    //read the file and take each caractere in a grid at the good place
    while((c = (fgetc(file))) != EOF)
    {

        if(i==9)
        {
            i=0;
            j+=1;
        }

        if(c == ' ' || c == '\r' || c=='\n' || c=='\f' || c=='\0')
            continue;
        else
        {
            toto[j][i]= c;
            i++;
        }
    }

    //put the char into int number
    for(int x=0; x<9; x++)
    {
        for(int y=0; y<9; y++)
        {
            int test = change_dot(toto[x][y]);
            if(test==0)
                toprint[x][y]=1;

            else
                toprint[x][y]=0;

            good_one[x][y] = test;
        }
    }

    //resolve the grid if posible else return an error
    if(Solve_Sudoku(good_one, 0, 0))
    {
        printf("toto");
        //write into a new file the solve grid
        write_file(argv[1],good_one);
    }
    else errx(1,"NO SOLUTION! The given grid is wrong please change it !");

    int x=2, y=2;

    init_sdl();
    SDL_Surface *grid = NULL;
    grid = load_image("grid.jpg");

    for(int t=1; t<10; t++)
    {
        x=2;
        for(int h=1; h<10; h++)
        {
            if(toprint[t-1][h-1]==1)
                solve_grid(grid, good_one[t-1][h-1], x, y);

            else
                solve_grid(grid, good_one[t-1][h-1]+9, x, y);

            if(h%3==0)
                x+=3;
            x+=66;
        }
        if(t%3==0)
            y+=3;
        y+=66;
    }

    SDL_SaveBMP(grid, "solve_grid1.jpg");


    return 0;
}
