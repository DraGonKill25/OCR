#include "solver.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


/*
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
*/


//all the function in this file are going to be use for solve the sudoku
//however the main will be in an other file name main.c
//i will add a Makefile excpecialy for this part later
//for just try to make all the function work corretly

int Grid_Check(int grid[9][9], int row, int col, int num)
{

    int rowStart = (row/3) * 3;   //it will start a the upper left of the square
    int colStart = (col/3) * 3;   //and at the right hight
    int i;

   for(i=0; i<9; ++i)
   {

        if (grid[row][i] == num) return 0;//check if the number exist in the col
        if (grid[i][col] == num) return 0;//check if the number exist in the row

        //regerde si le number is not in the square
        if (grid[rowStart + (i%3)][colStart + (i/3)] == num) return 0;
    }
    return 1;
}

int Solve_Sudoku(int grid[9][9], int row, int col)
{

    if(row<9 && col<9){//check if we are still in the grid

        if(grid[row][col])
        {
            //ckeck if we can call the function on the next col
            if((col+1)<9) return Solve_Sudoku(grid, row, col+1);
            //else check if we can call the function on the next raw
            else if((row+1)<9) return Solve_Sudoku(grid, row+1, 0);
            //else all the grid are full and all the number are at a good
            //position so we return true
            else return 1;
        }
        else
        {

            for(int i=1; i<10; i++)
            {//value of the case

                if(Grid_Check(grid, row, col, i))
                {//check each cell

                    grid[row][col] = i;
                    if((col+1)<9)
                    {

                        if(Solve_Sudoku(grid, row, col +1))
                            return 1;
                        else grid[row][col] = 0;
                    }

                    else if((row+1)<9)
                    {

                        if(Solve_Sudoku(grid, row+1, 0))
                            return 1;
                        else grid[row][col] = 0;
                    }

                    else
                        return 1;
                }
            }
        }
        return 0;
    }
    else
        return 1;
}


int change_dot(char toto)
{

    if(toto =='.')
        return 0;
    return (int)(toto - '0');//give each int value of each char of the source file
}

void write_file(char *file_name, int grid[9][9])
{

    //create the file name in a good format
    int l = strlen(file_name);
    char result_name[l+8];

    for (int i = 0; i < l; i++)
        result_name[i] = file_name[i];


    char extension[] = ".result";

    for (size_t i = 0; i < 7; i++)
        result_name[l + i] = extension[i];


    result_name[l + 7] = '\0'; //last caractere 0: end of the string


//end of the writing file name


    FILE *file;
    file = fopen(result_name, "w");


    for (size_t i = 0; i < 9; i++)
    {
        //put each caractere at the good place and
        //check when go to an other line

        for (size_t j = 0; j < 9; j++)
        {

            fputc(48 + grid[i][j], file);

            if ((j + 1) % 3 == 0 && (j != 8))
            {
                fputc(32,file);
                fputc(32,file);
           }
        }

        if ((i + 1) % 3 == 0)
            fputc(10, file);

        fputc(10, file);
    }

    fclose(file);
}

/*
SDL_Surface* resizenumber(SDL_Surface *img)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                        60,
                        60,
                        img->format->BitsPerPixel,0,0,0,0);
  SDL_SoftStretch(img, NULL, dest, NULL);
  //SDL_BlitScaled(img, NULL, dest, NULL);
  return dest;
}*/


void print_grid(SDL_Surface* img, SDL_Surface* grid, int x, int y)
{
    if(img ==NULL || grid == NULL || x==0 || y==0)
        errx(1, "something is wrong i can fell it");

    for(int i=0; i<img->h; i++)
    {
        for(int j=0; j<img->w; j++)
        {
            Uint32 pixel= get_pixel(img, j, i);
            put_pixel(grid, x+j, y+i, pixel);
        }
    }
}

void solve_grid(SDL_Surface* img, int imgtochoose, int x, int y)
{
    init_sdl();
    SDL_Surface* chiffre = NULL;

    switch (imgtochoose)
    {
        case 1:
            chiffre = load_image("solve_digit/1.png");
            break;

        case 2:
            chiffre = load_image("solve_digit/2.png");
            break;

        case 3:
            chiffre = load_image("solve_digit/3.png");
            break;

        case 4:
            chiffre = load_image("solve_digit/4.png");
            break;

        case 5:
            chiffre = load_image("solve_digit/5.png");
            break;

        case 6:
            chiffre = load_image("solve_digit/6.png");
            break;

        case 7:
            chiffre = load_image("solve_digit/7.png");
            break;

        case 8:
            chiffre = load_image("solve_digit/8.png");
            break;

        case 9:
            chiffre = load_image("solve_digit/9.png");
            break;

        case 10:
            chiffre = load_image("digit_on_grid/1.png");
            break;

        case 11:
            chiffre = load_image("digit_on_grid/2.png");
            break;

        case 12:
            chiffre = load_image("digit_on_grid/3.png");
            break;

        case 13:
            chiffre = load_image("digit_on_grid/4.png");
            break;

        case 14:
            chiffre = load_image("digit_on_grid/5.png");
            break;

        case 15:
            chiffre = load_image("digit_on_grid/6.png");
            break;

        case 16:
            chiffre = load_image("digit_on_grid/7.png");
            break;

        case 17:
            chiffre = load_image("digit_on_grid/8.png");
            break;

        case 18:
            chiffre = load_image("digit_on_grid/9.png");
            break;
    }



    print_grid(resizenumber(chiffre), img, x, y);
    printf("%d||%d\n", x,y);
    //SDL_BlitSurface(num2, &position, grid , &position);
    //SDL_SaveBMP(num2, "solve_digit/2.png");
}












