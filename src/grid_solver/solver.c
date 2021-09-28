#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

void ABOUT();
void RULES();
void Create();
void Generate_Random(int Q);
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

    do
    {
    printf("!!!---WELOME TO THE GAME OF SUDOKU---!!!\n");
    printf("1. WHAT IS SUDOKU ?\n");
    printf("2. RULES TO SOLVE A SUDOKU.\n");
    printf("3. GENERATE A SUDOKU.\n");
    printf("4. SOLVE SUDOKU.\n");
    printf("5. EXIT THE GAME.\n");
    printf("ENTER YOUR CHOICE\n");
    int choice;
    scanf("%d",&choice);
    switch(choice)
    {
    case 1:
        ABOUT();
        break;

    case 2:
        RULES();
        break;

    case 3:

    printf("ENTER THE LEVEL :-\n1. EASY\n2. MEDIUM\n3. HARD\n");
    int lvl;
    scanf("%d",&lvl);

    //Create the Sudoku
    Create();
    srand(time(0));

    //Interchange Rows within the group
    Generate_Random(1);

    //Interchange Columns within the group
    Generate_Random(0);

    //Interchange groups of Rows and Columns
    int limit[] = {0, 3 ,6};
    for(i=1; i<=2; i++)
    {
        a = limit[rand() % 3];
        b = limit[rand()%3];
        
        while(a==b){
            a = limit[rand() % 3];
        }

        if(i==1)
            Interchange_GroupRows(a, b);
        else
            Interchange_GroupColumns(a, b);
    }
    copy_arr();
    if(lvl==3)
    {
        level = rand()%6;
        level = level + 20;
    }
    else if(lvl==2)
    {
        level = rand()%6;
        level = level +30;
    }
    else
    {
        level = rand()%6;
        level = level +40;
    }
    Clear_Cell(level);
    display(arr);
    printf("\n  1. CHECK YOUR ANSWER\n  2. SEE THE SOLUTION\n 3. EXIT");
    int s;
    scanf("%d",&s);
    if(s==1)
    {
        int i,j;
        for(i=0;i<9;i++)
        {
            for(j=0;j<9;j++)
            {
                scanf("%d",&arr[i][j]);
            }
        }
        if(check()==1)
            printf("WOW! YOUR ANSWER IT CORRECT ");
        else
            printf("SORRY! WRONG ANSWER\n ");
    }
    else if(s==2)
    {
        display(arr1);
    }
        break;

    case 4:
        printf("ENTER A 9*9 SUDOKU IN MATRIX(Each line having 9 single space seperated numbers)\n NOTE: FOR BLANK SPACE ENTER '0'");
        int i,j;
        for(i=0;i<9;i++)
        {
            for(j=0;j<9;j++)
            {
                scanf("%d",arr1[i][j]);
            }
        }
        solver(arr1);
        if(c==200)
            display(arr1);
        else
            printf("SUDOKU CANNOT BE SOLVED!!\n");
        break;

    case 5:
        ext=10;
        break;

    default:
        printf("INVALID INPUT!!\n");
    }
    }while(ext==0);

return 0;
    }


    // GENERATOR
    void Create()
    {
    int A=1; int B=1;
    int i, j;
    for(i=0 ; i<9 ; i++)
    {
        A = B;
        for(j=0 ; j<9 ; j++)
        {
            if(A<=9)
            {
                arr[i][j]=A;
                A++;
            }
            else
            {
                A=1;
                arr[i][j]=A;
                A++;
            }
        }
        B = B + 3;
        if(B>9)
            B = B%9 + 1;
    }
    }

    //Function to generate random numbers to interchange rows and columns within same group
    void Generate_Random(int Q)
    {
    int i,j;
    int lower=0, upper=2;
    int num1, num2 ; //To store random numbers
    for(i=1; i<=3; i++)  //Loop is used to generate three sets of number
    {
        num1 = (rand() % (upper - lower + 1)) + lower;
        do
        {
            num2 = (rand() % (upper - lower + 1)) + lower;
        }while(num1==num2); //To ensure both the numbers aren't equal

        lower = lower + 3;
        upper = upper + 3;

        if(Q == 1)
            Interchange_Row(num1, num2);
        else
            Interchange_Column(num1, num2);
        }
    }

    //Function to interchange rows within the same group
    void Interchange_Row(int n1,int n2)
    {
   int i;
   int temp; //n1 and n2 are two rows that we are selecting to interchange.
   for(i=0;i<9;i++)
   {
      temp=arr[n1][i];
      arr[n1][i]=arr[n2][i];
      arr[n2][i]=temp;
    }
    }

    //Function to interchange columns within the same group
    void Interchange_Column(int n1,int n2)
    {
    int temp; int i;
    for(i=0; i<9; i++)
    {
      temp=arr[i][n1];
      arr[i][n1]=arr[i][n2];
      arr[i][n2]=temp;
    }
    }

    //Function to Interchange two group of rows
    void Interchange_GroupRows(int n1,int n2)
    {
    int i,j;
   int temp;
   for(i=1; i<=3; i++)
   {
      for(j=0; j<9; j++)
      {
         temp=arr[n1][j];
         arr[n1][j]=arr[n2][j];
         arr[n2][j]=temp;
      }
      n1++;
      n2++;
   }
    }

    //Function to Interchange two group of Columns
    void Interchange_GroupColumns(int n1,int n2)
    {
    int i,j;
   int temp;
   for(i=1; i<=3; i++)
   {
      for(j=0; j<9; j++)
      {
         temp=arr[j][n1];
         arr[j][n1]=arr[j][n2];
         arr[j][n2]=temp;
      }
      n1++;
      n2++;
   }
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
