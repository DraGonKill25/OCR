#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


void Interchange_Row(int n1,int n2);
void Interchange_Column(int n1,int n2);
void Interchange_GroupRows(int n1,int n2);
void Interchange_GroupColumns(int n1,int n2);
void Clear_Cell(int lvl);
void push(int a, int b, int c);
struct Stack *pop();
void solver(int arr[9][9]);
int Is_It_Safe(int arr[9][9],int row, int col, int n);
void display(int arr[9][9]);
void copy_arr();
int arr[9][9];
int arr1[9][9];
int c=0;
int main()
{
    int ext=0;
    int i,j,a,b,level;
    
    return 0;
}

    //Eliminate the cells
    void Clear_Cell(int lvl)
    {
        int x,y;
    while(lvl>0)
    {
        int cell_number = rand() % 82;
        int i = cell_number / 9;
        int j = cell_number % 9;

        if(j!=0)
            j = j-1;

        if(arr[i][j]!=0)
        {
                arr[i][j]=0;
                lvl--;
        }
    }
    }


    //SOLVER
int num=1;
//CREATING THE STACK
struct Stack
{
    int row;
    int column;
    int number;
    struct Stack *prev;
    struct Stack *next;
};

//TO PUSH INTO THE STACK
struct Stack *top=NULL;
void push(int a, int b, int c)
{
    struct Stack temp *= (struct Stack *)malloc(sizeof(struct Stack));
    temp->next = NULL;
    temp->prev = top;
    top = temp;
    temp->row = a;
    temp->column = b;
    temp->number = c;
}

//TO POP FROM STACK
struct Stack *pop()
{
    if(top==NULL)
        return NULL;
    else
        {
            struct Stack *temp = top;
            top = top->prev;
            return temp;
        }
}

//FUNCTION TO SOLVE THE SUDOKU USING STACK WHICH IS IMPLIMENTED BY DOUBLY LINKED LIST
void solver(int arr[9][9])
{
    int i,j,q,var;
    do
    {
        q=0;
        var=0;
    for(i=0;i<9;i++)
    {
        for(j=0;j<9;j++)
        {
            if(arr[i][j]==0)
            {
                q=202;
                int k;
                for(k=num;k<=9;k++)
                {
                   // printf("%d",k);
                    if(Is_It_Safe(arr, i, j, k)==1)
                    {
                        q=10;
                        arr[i][j]=k;
                        push(i,j,k);
                        num=1;
                        break;
                    }
                }
               // printf("\n");
                if(q!=10)
                {
                    struct Stack *tem = pop();
                    if(tem==NULL){
                        c=100;
                        printf("no");
                    }
                    else
                    {
                        arr[tem->row][tem->column] = 0;
                        num = tem->number + 1;
                    }
                    var=1;break;
                }
            }
            if(i==8 && j==8 && q==0){
                c=200;
            }
        }
        if(var==1){
            break;}
    }
    }while(c==0);
}

//TO CHECK IF IT IS FINE TO PUT THE NUMBER IN A PARTICULAR PLACE
int Is_It_Safe(int arr[9][9],int row, int col, int n)
{
    int i,j;
    //TO CHECK IN THE ROW
    for (i = 0; i < 9; i++)
    {
        if (arr[row][i] == n)
        {
            //printf("row");
            return 0;
        }
    }

    //TO CHECK IN THE COLUMN
    for (i = 0; i < 9; i++)
    {
        if (arr[i][col] == n)
        {
           // printf("col");
            return 0;
        }
    }

    //TO CHECK IN SQUARE
    int p = row - row % 3;
    int q = col - col % 3;
  //  printf("|%d %d|",row,col);
    for (i = p; i < p + 3; i++)
    {
        for (j = q; j < q + 3; j++)
        {
            if (arr[i][j] == n)
            {
               // printf("%d %d box", i,j);
                return 0;
            }
        }
    }

    return 1;
}

//DISPLAY THE SUDOKU
void display(int arr[9][9])
{
    int i,j;
    for(i=0;i<9;i++)
    {
        printf("-------------------------------------\n");
        for(j=0;j<9;j++)
        {
            if(j==8)
                printf("| %d |",arr[i][j]);
            else
                printf("| %d ",arr[i][j]);
        }
        printf("\n");
        if(i==8)
            printf("---------------------------------------\n\n");
    }
}

//COPY SUDOKU FROM ARR TO ARR1
void copy_arr()
{
    int i,j;
    for(i=0;i<9;i++)
    {
        for(j=0;j<9;j++)
        {
            arr1[i][j]=arr[i][j];
        }
    }
}

//CHECK WHETER THE ANSWE ENTERED BY THE USER IS CORRECT OR NOT
int check()
{
    int i,j;
    for(i=0;i<9;i++)
    {
        for(j=0;j<9;j++)
        {
            if(arr1[i][j]!=arr[i][j])
                return 0;
        }
    }
    return 1;
}
