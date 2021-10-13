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
		input[i] = expf(input[i] - m - log(sum));
	}
}

//Number of inputs
	#define numInput 2;

	//Number of hidden layers
	#define numHidden 1;

	//Number of output layers
	#define numOutput 1;

	//number of training pattern
	#define numPattern 4;

int main() {
	
	//training pattern
	double target[4][1] = {
		{1},
		{1},
		{0},
		{0}};


	double input[numInput];

	double sumH[numHidden];

	double hidden[numHidden];

	double weightIH[numInput][numHidden];

	double weightHO[numHidden][1];
	
	double deltaweightHO[][];

	double output[1];

	double sumH[numHidden];
	double sumO[numO];

	double deltaweightIH[numInput][numHidden];
	double deltaweightHO[numHidden][numOutput];



	//xor learn and training


	for(size_t epoch = 1; epoch < 5000; epoch++)
	{
		float Error = 0.0;

		for (size_t p = 1; p <= numPattern; p++)
		{
			for(size_t j = 1; j <= numHidden; j++)
			{
				sumH[p][j] = weightIH[0][j];
				
				for(size_t i = 1; i <= numInput; i++)
				{
					sumH[p][j] += input[p][i] * weightIH[i][j];
				}
				hidden[p][j] = 1.0/(1.0 + exp(-sumH[p][j]));
			}

			for(size_t k = 1; k <= numOutput; k++)
			{
				sumO[p][k] = weightHO[0][k];

				for(size_t j = 1; j <= numHidden; j++)
				{
					sumO[p][k] += hidden[p][j] * weightHO[j][k];
				}
				output[p][k] = 1.0/(1.0 + exp(-sumO[p][k]));
				Error += 0.5 * (target[p][k] - output[p][k]) * (target[p][k] - output[p][k]);
				DeltaO[k] = (target[p][k] - output[p][k]) * ouput[p][k] * (1.0 - output[p][k]);

			}
			
			for(size_t j = 1; j <= numHidden; j++)
			{
				sumDOW[j] = 0.0;

				for(size_t k = 1; k <= numOutput; k++)
				{
					sumDOW[j] += weightHO[j][k] * DeltaO[k];
				}
				deltaH[j] = sumDOW[j] * hidden[p][j] * (1.0 - hidden[p][j]);
				weightIH[0][j] += deltaweightIH[0][j];

				for(size_t i = 1; i <= numInput; i++)
				{
					deltaweightIH[i][j] = eta * deltaH[j] + alpha * deltaweightIH[0][j];
					weightIH[i][j] += deltaweightIH[i][j];
				}
			}

			for (size_t k = 1; k <= numOutput; k++)
			{
				deltaweightHO[0][k] = eta * deltaO[k] + alpha * deltaweightHO[0][k];
				weightHO[0][k] += deltaweightHO[0][k];

				for(size_t j = 1; j <= numHidden; j++)
				{
					deltaweightHO[j][k] = eta * hidden[p][j] * deltaO[k] + alpha * deltaweightHO[j][k];
					weightHO[j][k] += deltaweightHO[j][k];
				}
			}
		}

		if(Error < 0.10) break; //when the error is minimum, the xor learnt
	}
	
	return 0;
}
