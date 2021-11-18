#ifndef SOLVER_H_
#define SOLVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

//-------------------------------------------
//           solver relative tool
//------------------------------------------

//check if the given grill have the right parameter and fit with the rules of
//a sudoku
int Grid_Check(int grid[9][9],int row, int col, int num);
//Function that solve the sudoku
int Solve_Sudoku(int grid[9][9], int row, int col);



//-----------------------------------------
//              file management tool
//-----------------------------------------

//simple function that will change the dot  of the file we read into 0 in the
//grid or the int value of the caractere read
int change_dot(char toto);

//this function will first create a file with the expected name and then will
//write the resolve grid into it
void write_file(char *file_name, int grid[9][9]);