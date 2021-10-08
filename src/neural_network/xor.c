#include <stdlib.h> //rando() fct
#include <stdio.h>
#include <math.h>
#include <assert.h>

//softmax function

static void softmax(double *input, int input_len)
{
	assert (input != NULL);
	assert (input_len != 0);
	double m;
	//max value from input array
	m = input[0];
	for (int i = 1; i < input_len; i++)
	{
		if (input[i] > m)
		{	
		m = input[i];
		}
	}

	double sum = 0;
	for (int i = 0; i < input_len; i++)
	{
		sum += expf(input[i]-m);
	}

	for(int i = 0; i < input_len; i++)
	{
		input[i] = expf(input[i] - m log(sum));
	}
}


//Initialize 

//Out => Softmax Function

//Number of inputs
int numInput = 2;

//Number of hidden layers
int numHidden = 1;



double input[numInput+1];
double sumH[numHidden+1];
double hidden[numHidden+1];
double weightIH[numInput+1][numHidden+1];
double output[numO+1];
double sumO[numO];



//xor learn and training


for (int epoch = 1; epoch < 5000; epoch++)
{
	float Error = 0.0;

	for (p = 1; p <= numPattern; p++)
	{

		for(int j = 1; j <= numHidden; j++)
		{
			
			sumH[p][j] = weightIH[0][j];

			for(int i = 1; i <= numInput; i++)
			{
				sumH[p][j] += input[p][i] * weightIH[i][j];
			}
			hidden[p][j] = 1.0/(1.0 + exp(-sumH[p][j]));
		}

		for(int k = 1; k <= numOutput; k++)
		{
			sumO[p][k] = weightHO[0][k];
			for(int j = 1; j <= numHidden; j++)
			{
				sumO[p][k] += hidden[p][j] * weightHO[j][k];
			}
			output[p][k] = 1.0/(1.0 + exp(-sumO[p][k]));
			Error += 0.5 * (target[p][k] - output[p][k]) * (target[p][k] - output[p][k]);
			DeltaO[k] = (target[p][k] - output[p][k]) * ouput[p][k] * (1.0 - output[p][k]);
		}

		for(int j = 1; j <= numHidden; j++)
		{
			sumDOW[j] = 0.0;
			for(int k = 1; k <= numOuput; k++)
			{
				sumDOW[j] += weightHO[j][k] * DeltaO[k];
			}
			deltaH[j] = sumDOW[j] * hidden[p][j] * (1.0 - hidden[p][j]);
			weightIH[0][j] += deltaWeightIH[0][j];
			
			for(int i = 1; i <= numInput; i++)
			{
				deltaWeightIH[i][j] = eta * deltaH[j] + alpha * deltaWeightIH[0][j];
				weightIH[i][j] += deltaWeightIH[i][j];
			}
		}
		for (int k = 1; k <= numOuput; k++)
		{
			deltaWeightHO[0][k] = eta * deltaO[k] + alpha * deltaweightHO[0][k];
			wightHO[0][k] += deltaweightHO[0][k];

			for(int j = 1; j <= numHidden; j++)
			{
				deltaWeightHO[j][k] = eta * hidden[p][j] * deltaO[k] + alpha * deltaWeightHO[j][k];
				wightHO[j][k] += deltaWeight[j][k];
			}
		}

	}
	if(Error < 0.10) break; //when the error is minimum, the xor learnt
}
