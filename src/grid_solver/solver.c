#include "solver.h"


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

int Solve_Sudoku(int g0rid[9][9], int row, int col)
{

    if(row<9 && col<9)0{//check if we are still in the grid

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



