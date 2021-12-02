#include "neural_network_tools.h"
#include "neural_network.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

struct Neural_Network* InitializeNetwork()
{
  struct Neural_Network *net = NULL;
  net = malloc(sizeof(struct Neural_Network));
  net -> nbInput = 28*28;
  net -> nbHidden = 20;
  net -> nbOutput = 9;
  net -> ErrorRate = 0.0;
  net -> MaxErrorRate = 0.0;
  net -> eta = 0.5;
  net -> alpha = 0.9;

  //Set Values
  for (int i = 0; i < net -> nbInput; i++)
  {
    for (int h = 0; h < net -> nbHidden; h++)
    {
      net -> WeightIH[i][h] = Random();
      net -> dWeightIH[i][h] = 0.0;
    }
  }

  for (int h = 0; h < net -> nbHidden; h++)
  {
    for(int o = 0; o < net -> nbOutput; o++)
    {
      net -> WeightHO[h][o] = Random();
      printf("%f \n", net -> WeightHO[h][o]);
      net -> dWeightHO[h][o] = 0.0;
    }
    net -> BiasH[h] = Random();
  }

  for (int o = 0; o < net -> nbOutput; o++)
  {
    net -> BiasO[o] = Random();
    net -> dOutputO[o] = 0.0;
  }
  return net;
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

static void BackwardPass(struct Neural_Network *net)
{
  //Calulate Cost function for output neurons
  double output, derivate, goal, error;
  for (int o = 0; o < net -> nbOutput; o++)
  {
    output = net -> OutputO[o];
    derivate = Derivate_Sigmoid(output);
    goal = net -> Goal[o];
    error = (goal - output) * derivate;

    net -> dOutputO[o] = error;
  }

  ///Calculate Cost function for hidden neurons
  double sum, weight, delta;
  for (int h = 0; h < net -> nbHidden; h++)
  {
    sum = 0.0;
    for (int o = 0; o < net -> nbOutput; o++)
    {
      weight = net -> WeightHO[h][o];
      delta = net -> dOutputO[o];

      sum += weight * delta;
    }
    output = net -> OutputH[h];
    derivate = Derivate_Sigmoid(output);
    net -> dHidden[h] = sum * derivate;
  }
}

static void UpdateWeights(struct Neural_Network *net)
{
  double eta, alpha, error, output, dWeight;
  eta = net -> eta;
  alpha = net -> alpha;

   //Weights between Input and Hidden layers
  for (int h = 0; h < net -> nbHidden; h++)
  {
    for(int i = 0; i < net -> nbInput; i++)
    {
      output = net -> OutputI[i];
      error = net -> dHidden[h];
      dWeight = net -> dWeightIH[i][h];

      net -> WeightIH[i][h] += eta * error * output +
                                alpha * dWeight;
      net -> dWeightIH[i][h] = eta * error * output;
    }
  }

  //Weights between Hidden and Ouput layers
  for (int o = 0; o < net -> nbOutput; o++)
  {
    for (int h = 0; h < net -> nbHidden; h++)
    {
      output = net -> OutputH[h];
      error = net -> dOutputO[o];
      dWeight = net -> dWeightHO[h][o];

      net -> WeightHO[h][o] += eta * error * output +
                                alpha * dWeight;
      net -> dWeightHO[h][o] = eta * error * output;
    }
  }
}

static void UpdateBiases(struct Neural_Network *net)
{
  double eta, delta, dBias;
  eta = net -> eta;
  //Update BiasH
  for (int h = 0; h < net -> nbHidden; h++)
  {
    delta = net -> dHidden[h];
    dBias = eta * delta;

    net -> BiasH[h] += dBias;
  }

  //Update BiasO
  for (int o = 0; o < net -> nbOutput; o++)
  {
    delta = net -> dOutputO[o];
    dBias = eta * delta;
    //printf("%f \n", dBias);

    net -> BiasO[o] += dBias;
  }
}

//Colors for print
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"

//training
void Neural_Network_OCR(struct Neural_Network *net, double *input, double *goal)
{

  //Initialise Goals & InputValues for this digit
  for (int g = 0; g < 9; g++)
  {
    net -> Goal[g] = goal[g];
  }
  for(int i = 0; i < net -> nbInput; i++)
  {
    net -> OutputI[i] = input[i];
  }

  //Run the learning
  net -> ErrorRate = 0;
  ForwardPass(net);
  BackwardPass(net);
  UpdateWeights(net);
  UpdateBiases(net);
}

