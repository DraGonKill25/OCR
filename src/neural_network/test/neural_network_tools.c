# include "neural_network_tools.h"

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

//Return the position of the output with the greatest sigmoid result
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


//Retrive the position of the digit in the goal tab
int PosGoal(double *goal)
{
  int count = 0;

  while(goal[count] != 1.0)
    count++;

  return count;
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
struct Neural_Network* ExtractData ()
{
  //CREATE NN
  struct Neural_Network *net = malloc(sizeof(struct Neural_Network));
  net -> nbInput = 28*28; //size of imgs
  net -> nbHidden = 20;
  net -> nbOutput = 10; //10 digits
  net -> dbl = malloc(sizeof(double)*1200);
  net -> dbl = 0;

  int sizeMax = 15;
  char *line = calloc(15, sizeof(double));

  //WeightIH
  FILE* weightIH = fopen("weightIH.w", "r");
  for(int i = 0; i < net -> nbInput; ++i)
  {
    for(int h = 0; h < net -> nbHidden; ++h)
    {
      net -> WeightIH[i][h] = line;
    }
  }
  fclose(weightIH);

  //Weight HO
  FILE* weightHO = fopen("weightHO.w", "r");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    for(int o = 0; o < net -> nbOutput; ++o)
    {
        net -> WeightHO[h][o] = line;
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
