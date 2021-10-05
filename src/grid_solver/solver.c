#include <stdio.h>
#include <stdlib.h>
#include <err.h>




//all the function in this file are going to be use for solve the sudoku
//however the main will be in an other file name main.c
//i will add a Makefile excpecialy for this part later
//for just try to make all the function work corretly

//-------------------------------------------
//           solver relative tool
//------------------------------------------
//Function that solve the sudoku
int Solve_Sudoku(int grid[][9], int row, int col);

//check if the given grill have the right parameter and fit with the rules of
//a sudoku
int Grid_Check(int grid[][9],int row, int col, int num);



//-----------------------------------------
//              file management tool
//-----------------------------------------


//void change_dot(char *file_name, int grid[9][9]);
size_t my_str_len(char str[]);
void write_file(char *file_name, int grid[9][9]);

//------------------------------------------------------------------
//------------------------------------------------------------------


int Grid_Check(int grid[][9], int row, int col, int num){

    int rowStart = (row/3) * 3;
    int colStart = (col/3) * 3;
    int i;

    for(i=0; i<9; ++i){

        if (grid[row][i] == num) return 0;
        if (grid[i][col] == num) return 0;
        if (grid[rowStart + (i%3)][colStart + (i/3)] == num) return 0;
    }
    return 1;
}

int Solve_Sudoku(int grid[][9], int row, int col){

    if(row<9 && col<9){

        if(grid[row][col]){

            if((col+1)<9) return Solve_Sudoku(grid, row, col+1);
            else if((row+1)<9) return Solve_Sudoku(grid, row+1, 0);
            else return 1;
        }
        else{

            for(int i=1; i<10; i++){

                if(Grid_Check(grid, row, col, i)){

                    grid[row][col] = i;
                    if((col+1)<9){

                        if(Solve_Sudoku(grid, row, col +1))
                            return 1;
                        else grid[row][col] = 0;
                    }

                    else if((row+1)<9){

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


int change_dot(char *file_name/*, int grid[9][9]*/){

    FILE *file = NULL;
    file = fopen(file_name, "r");
    int c, i = 0, j = 0;
    int grid[9][9];

    while ((c = fgetc(file)) != EOF){

        if (c == ' ' || c == '\n' || c == '\r' || c == '\0' || c == '\f')
            continue;

        if (c == '.'){
            grid[i][j] = 0;
            j += 1;
        }

        else{
            grid[i][j] = (int) c - 48;
            j += 1;
        }

        if (j == 9){
            i += 1;
            j = 0;
        }
    }

    fclose(file);
    return grid[i][j];
}

size_t my_str_len(char str[]){

    size_t len = 0;
    while (str[len] != '\0')
        len += 1;

    return len;
}

void write_file(char *file_name, int grid[9][9]){
//create le the file name in a good format

    char result_name[my_str_len(file_name) + 7 + 1];

    for (size_t i = 0; i < my_str_len(file_name); i++)
        result_name[i] = file_name[i];


    char extension[] = ".result";

    for (size_t i = 0; i < my_str_len(extension); i++)
        result_name[my_str_len(file_name) + i] = extension[i];


    result_name[my_str_len(file_name) + 7] = '\0'; //last caractere 0: end of the string


//end of the writing file name


    FILE *file;
    file = fopen(result_name, "w");


    for (size_t i = 0; i < 9; i++){

        for (size_t j = 0; j < 9; j++){

            fputc(48 + grid[i][j], file);

            if ((j + 1) % 3 == 0 && (j != 8))
                fputc(32,file);

        }

        if ((i + 1) % 3 == 0)
            fputc(10, file);

        fputc(10, file);
    }

    fclose(file);
}



int main(int argc,  char *argv[]){

    if(strtoul(argv[1],NULL,10)==0)
        errx(1,"Error");

    if (argc!=2)
        errx(1,"The number of arguments is not valid");

    unsigned long test = strtoul(argv[1],NULL,10);

    if(test==0)
        errx(1,"The parameter is not valid");



    /*int i, j;
    int grid [9][9]= {{0, 2, 0, 0, 0, 0, 6, 0, 9},
                      {8, 5, 7, 0, 6, 4, 2, 0, 0},
                      {0, 9, 0, 0, 0, 1, 0, 0, 0},
                      {0, 1, 0, 6, 5, 0, 3, 0, 0},
                      {0, 0, 8, 1, 0, 3, 5, 0, 0},
                      {0, 0, 3, 0, 2, 9, 0, 8, 0},
                      {0, 0, 0, 4, 0, 0, 0, 6, 0},
                      {0, 0, 2, 8, 7, 0, 1, 3, 5},
                      {1, 0, 6, 0, 0, 0, 0, 2, 0}
                    };

    if(Solve_Sudoku(grid, 0, 0)){

        printf("\n+-----+-----+-----+\n");

        for(i=1; i<10; ++i){

            for(j=1; j<10; ++j) printf("|%d", grid[i-1][j-1]);
            printf("|\n");

            if (i%3 == 0) printf("+-----+-----+-----+\n");
        }
    }
    else
        printf("\n\nNO SOLUTION\n\n");*/

    return 0;
}
