#include "neural_network_tools.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <err.h>
#include "neural_network.h"

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

//A random that returns a double in [-1; 1]
double Random()
{
    return ((double)rand()) / ((double)RAND_MAX / 2) - 1;
}

//The classical sigmoid function
double Sigmoid(double x)
{
  return(1.0 / (1.0 + exp(-x)));
}

//The classical derivate of the sigmoid function
double Derivate_Sigmoid(double x)
{
  return x * (1.0 - x);
}

//Calculates the Squared error
void SquaredError(struct Neural_Network *net)
{
  double max = 0;
  double sum = 0;

  for (int o = 0; o < net -> nbOutput; o++)
  {
    if (max < net -> OutputO[o])
    {
      max = net -> OutputO[o];
    }
    sum += (net -> Goal[o] - net -> OutputO[o]) *
                      (net -> Goal[o] - net -> OutputO[o]);
  }
  net -> ErrorRate += 0.5 * sum;
}


//Save data of the NN in 4 files:
//WeightIH - WeightHO - BiasH - BiasO
void SaveData(struct Neural_Network *net)
{
  FILE* weightIH = fopen("weightIH.w", "w");
  for(int i = 0; i < net -> nbInput; ++i)
  {
    for(int h = 0; h < net -> nbHidden; ++h)
    {
      fprintf(weightIH, "%f\n", net -> WeightIH[i][h]);
    }
  }
  fclose(weightIH);

  FILE* weightHO = fopen("weightHO.w", "w");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    for(int o = 0; o < net -> nbOutput; ++o)
    {
        fprintf(weightHO, "%f\n", net -> WeightHO[h][o]);
    }
  }
  fclose(weightHO);

  FILE* biasH = fopen("biasH.b", "w");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    fprintf(biasH, "%f\n", net -> BiasH[h]);
  }
  fclose(biasH);

  FILE* biasO = fopen("biasO.b", "w");
  for (int o = 0; o < net -> nbOutput; ++o)
  {
    fprintf(biasO, "%f\n", net -> BiasO[o]);
  }
  fclose(biasO);
}


//Extract data previously saved in 4 files:
//WeightIH - WeightHO - BiasH - BiasO
struct Neural_Network* ExtractData()
{
  //CREATE NN
  struct Neural_Network *net = malloc(sizeof(struct Neural_Network));
  net -> nbInput = 28*28; //size of imgs
  net -> nbHidden = 20;
  net -> nbOutput = 9; //10 digits

  int sizeMax = 15;
  char *line = calloc(15, sizeof(char));

  //WeightIH
  FILE* weightIH = fopen("weightIH.w", "r");
  for(int i = 0; i < net -> nbInput; ++i)
  {
    for(int h = 0; h < net -> nbHidden; ++h)
    {
      fgets(line, sizeMax, weightIH);
      strtok(line, "\n");
      net -> WeightIH[i][h] = atof(line);
    }
  }
  fclose(weightIH);

  //Weight HO
  FILE* weightHO = fopen("weightHO.w", "r");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    for(int o = 0; o < net -> nbOutput; ++o)
    {
        fgets(line, sizeMax, weightHO);
        strtok(line, "\n");
        net -> WeightHO[h][o] = atof(line);
    }
  }
  fclose(weightHO);

  //BiasH
  FILE* biasH = fopen("biasH.b", "r");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    fgets(line, sizeMax, biasH);
    strtok(line, "\n");
    net -> BiasH[h] = atof(line);
  }
  fclose(biasH);

  //BiasO
  FILE* biasO = fopen("biasO.b", "r");
  for (int o = 0; o < net -> nbOutput; ++o)
  {
    fgets(line, sizeMax, biasO);
    strtok(line, "\n");
    net -> BiasO[o] = atof(line);
  }
  fclose(biasO);

  return net;
}


//softmax to determine the output layer
/*int soft_max(struct Neural_Network* net)
{
    double tablo[10];
    double denominator = 0.0;

    //denominator
    for (size_t i = 1; net->OutputO; i++)
    {
        denominator += exp(net->OutputO[i]);
    }

    //applies softmax and stack it in "tablo"
    for (size_t i = 1; net->OutputO; i++)
    {
        float nominator = net->OutputO[i];
        tablo[i] = nominator / denominator;
    }

    double max = 0;
    
    //position of the max value in array, 0 -> blank, j otherwise
    int j;
    //determine the highest proba for each output
    for (j = 0; j < 11; j++)
    {
        if (tablo[j] > max)
        {
            max = tablo[j];
        }
    }
    
    return j;
}*/

int RetrievePos(struct Neural_Network *net)
{
  double max = 0;
  int posMax = 0;

  for (int o = 0; o < net -> nbOutput; o++)
  {
    if (max < net -> OutputO[o])
    {
      posMax = o;
      max = net -> OutputO[o];
    }
  }
  return posMax;
}

int PosGoal(double *goal)
{
  //printf("toto\n");
  int count = 0;

  while(goal[count] != 1.0)
    count++;

  return count;
}

void PrintState(struct Neural_Network *net)
{
    //Squared error function
    SquaredError(net);
    int output = RetrievePos(net) + 1;

    //Retrive the chars : wanted & found
    char goalDigit = PosGoal(net -> Goal) + 1;
    char recognizedDigit = output;

    if(net -> ErrorRate > net -> MaxErrorRate)
        net -> MaxErrorRate = net -> ErrorRate;

    //Print the progress
    if(output == PosGoal(net -> Goal) + 1)
        printf("Position Found = %d Expected %d %sOK \n",
                output, PosGoal(net -> Goal)+1,KGRN);
    else
        printf("Position Found = %d Expected %d %sKO \n",
                output, PosGoal(net -> Goal)+1,KRED);

    printf("%s",KWHT);

    printf("Char entered: %d | Char recoginized: %d | ErrorRate: %f\n",
            goalDigit,
            recognizedDigit,
            net -> ErrorRate);
}

static void ForwardPass(struct Neural_Network *net)
{
    double sum, weight, output, bias;

  //Calculate Output for Hidden neurons
  for (int h = 0; h < net -> nbHidden; h++)
  {
    sum = 0.0;
    for (int i = 0; i < net -> nbInput; i++)
    {
      weight = net -> WeightIH[i][h];
      output = net -> OutputI[i];

      sum += weight * output;
    }
    bias = net -> BiasH[h];
    net -> OutputH[h] = Sigmoid(sum + bias);
  }

  //Calculate Output for Output neurons
  for (int o = 0; o < net -> nbOutput; o++)
  {
    sum = 0.0;
    for (int h = 0; h < net -> nbHidden; h++)
    {
      weight = net -> WeightHO[h][o];
      output = net -> OutputH[h];

      sum += weight * output;
    }
    bias = net -> BiasO[o];
    net -> OutputO[o] = Sigmoid(sum + bias);
  }
}

char DetectText(struct Neural_Network *net, double* letter)
{
    for(int i = 0; i < net -> nbInput; i++)
    {
        net -> OutputI[i] = letter[i];
    }

    ForwardPass(net);

    //Retrieve the character detected
    int pos = RetrievePos(net);
    char c;

    switch(pos)
    {
        case 0:
            c = '1';
            break;
        case 1:
            c = '2';
            break;
        case 2:
            c = '3';
            break;
        case 3:
            c = '4';
            break;
        case 4:
            c = '5';
            break;
        case 5:
            c = '6';
            break;
        case 6:
            c = '7';
            break;
        case 7:
            c = '8';
            break;
        case 8:
            c = '9';
            break;
    }

    return c;
}


int image_a_traiter(SDL_Surface* image)
{
    SDL_Surface *img = image;
    Uint8 r, g, b;
    int moyenne = 0;
    for (int i = 0; i < img->h; i++)
    {
        for(int j = 0; j < img->w; j++)
        {
            Uint32 pix = get_pixel(image, j, i);
            SDL_GetRGB(pix, img->format, &r, &g, &b);
            moyenne += r + g + b;
        }
    }
    /*if (moyenne > (250*784))
    {
        return 0;
    }*/


    return 1;
}

char Convert(struct Neural_Network* net, SDL_Surface* img)
{
    char res;
    if (image_a_traiter(img) == 1)
    {
        double* letter = create_matrix(img);
        res = DetectText(net, letter);
    }
    else
    {
        res = '.';
    }
    return res;
}
