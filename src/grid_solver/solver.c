#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>




//all the function in this file are going to be use for solve the sudoku
//however the main will be in an other file name main.c
//i will add a Makefile excpecialy for this part later
//for just try to make all the function work corretly

//-------------------------------------------
//           solver relative tool
//------------------------------------------
//Function that solve the sudoku
int Solve_Sudoku(int grid[9][9], int row, int col);

//check if the given grill have the right parameter and fit with the rules of
//a sudoku
int Grid_Check(int grid[9][9],int row, int col, int num);



//-----------------------------------------
//              file management tool
//-----------------------------------------


int change_dot(char toto);
size_t my_str_len(char str);
void write_file(char file_name, int grid[9][9]);

//------------------------------------------------------------------
//------------------------------------------------------------------


int Grid_Check(int grid[9][9], int row, int col, int num){

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

int Solve_Sudoku(int grid[9][9], int row, int col){

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


int change_dot(char toto){

    if(toto =='.')
        return 0;
    return (int)(toto - '0');
}

size_t my_str_len(char str){

    //size_t len = 0;
    //char string[] = str;
    //while (string[len] != '\0')
        //len += 1;

    return 1;
}

void write_file(char file_name, int grid[9][9]){

    //create the file name in a good format
    int l = strlen(file_name)/*my_str_len(file_name);*/
    char result_name[l+8];

    for (size_t i = 0; i < l; i++)
        result_name[i] = file_name[i];
    
    result_name += ".result";

    //char extension[] = ".result";

    /*for (size_t i = 0; i < my_str_len(extension); i++)
        result_name[l + i] = extension[i];*/


    result_name[l + 7] = '\0'; //last caractere 0: end of the string


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

    if (argc>2||argc<2)
        errx(1,"The number of arguments is not valid");

    char toto[9][9];
    int good_one[9][9];
    int i=0,j=0,c;

    FILE *file = NULL;
    file = fopen(argv[1], "r");
    if(file == NULL)
        exit(1);

    //IT WORKS PERFECTLY
    while((c = (fgetc(file))) != EOF){

        if(i==9){
            i=0;
            j+=1;
        }

        if(c == ' ' || c == '\r' || c=='\n' || c=='\f' || c=='\0')
            continue;
        else{
            toto[j][i]= c;
            i++;
        }
    }

    for(int x=0; x<9; x++){
        for(int y=0; y<9; y++){
            good_one[x][y] = (change_dot(toto[x][y]));
            //printf("%i", good_one[x][y]);
        }
        //printf("\n");
    }


    /*if(!Solve_Sudoku(good_one, row, col))
        errx(1,"The grid is impossible to solve");
    else
        for(int x=0; x<9; x++){
            for(int y=0; y<9; y++){
                //good_one[x][y] = (change_dot(toto[x][y]));
                printf("%i", good_one[x][y]);
            }
            printf("\n");
        }*/

    if(Solve_Sudoku(good_one, 0, 0))
    {
        /*printf("\n+-----+-----+-----+\n");
        for(int h=0; h<9; h++)
        {
            for(int w=0; w<9; w++) printf("|%d", good_one[h][w]);
            printf("|\n");
            if ((h+1)%3 == 0) printf("+-----+-----+-----+\n");
        }*/
        write_file(argv[1], good_one);
    }
    else printf("\n\nNO SOLUTION\n\n");

    return 0;
}
