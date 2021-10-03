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

//define neuron

typedef struct neuron_t
{
	float activ;
	float *out_weights;
	float bias;
	float z;

	float dactv;
	float *dw;
	float dbias;
	float dz;

} neuron;

typedef struct layer_t
{
	int num_neu;
	struct neuron_t *neu;
} layer;

// architecture

int create_architecture()
{
	lay = (layer*) malloc(num_layers * sizeof(layer));

	for (int i = 0, i < num_layers; i++)
	{
		lay[i] = create_layer(num_neurons[i]);
		lay[i].num_neu = num_neurons[i];

		for (int j = 0; j < num_neurons[i]; j++)
		{
			if (i < (num_layers - 1))
			{
				lay[i].neu[j] = create_neuron(num_neurons[i+1]);
			}
		}
	}
	//initialize weight
	
	if (initialize_weights() != SCCESS_INIT_WEIGHTS)
	{
		return ERR_CREATE_ARCHITECTURE;
	}

	return SUCCESS_CREATE_ARCHITECTURE;
}

//training examples

void get_inputs()
{
	for (int i = 0; i < num_training_ex; i++)
	{
		printf("Enter the Inputs for training example[%d]:\n",i);
		for(int j = 0; j<num_neurons[0]; j++)
		{
			scanf("%f",input[i][j]);
		}
	}
	printf("\n"):
}

void get_desired_outputs()
{
	for (int i = 0; i < num_training_ex; i++)
	{
		for (int j = 0; j < num_neurons[num_layers-1]; j++)
		{
			printf("Enter the Desired Outputs (Labels) for training example[%d]...\n",i);
            		scanf("%f",&desired_outputs[i][j]);
            		printf("\n");
		}
	}
}

void forward_prop(void)
{

    for(int i=1;i<num_layers;i++)
    {   
        for(int j=0;j<num_neurons[i];j++)
        {
            lay[i].neu[j].z = lay[i].neu[j].bias;

            for(int k=0;k<num_neurons[i-1];k++)
            {
                lay[i].neu[j].z  = lay[i].neu[j].z + ((lay[i-1].neu[k].out_weights[j])* (lay[i-1].neu[k].actv));
            }

            // Relu Activation Function for Hidden Layers
            if(i < num_layers-1)
            {
                if((lay[i].neu[j].z) < 0)
                {
                    lay[i].neu[j].actv = 0;
                }

                else
                {
                    lay[i].neu[j].actv = lay[i].neu[j].z;
                }
            }
            
            // Sigmoid Activation function for Output Layer
            else
            {
                lay[i].neu[j].actv = 1/(1+exp(-lay[i].neu[j].z));
                printf("OUTPUT: %d\n", (int)round(lay[i].neu[j].actv));
                printf("\n");
            }
        }
    }
}

//Initialize 



double Input[NumInput+1];
double SumH[NumHidden+1];
double Hidden[NumHidden+1];
double WeightIH[NumInput+1][NumHidden+1]


SumH = Weight[0];
for (int i = 1; i<= NumInput; i++)
{
	SumH += Input[i]*Weight[i];
}
Hidden = ;//SoftMax activation function





for (int j = 1; j <= NumHidden; j++)
{
	for (int i = 0; i <= NumInput; i++)
	{
		DeltaWeightIH[i][j] = 0.0;
		WeightIH[i][j] = 2.0 * (rando() - 0.5
	}
}

Error = 0.0;

for (int p = 1; p <= NumPattern; p++)
{
	for (int j = 1; j <= NumHidden; j++)
		{
			SumH[p][j] = WeightIH[0][j];
			for (int i = 1; i ,= NumInput; i++)
			{
				SumH[p][j] += Input[p][j] * WeightIH[i][i];
			}
			Hidden[p][j] += ;	//softmax function
		}
	for( int k = 1 ; k <= NumOutput ; k++ )
	{
		SumO[p][k] = WeightHO[0][k] ;
    	for( int j = 1 ; j <= NumHidden ; j++ ) 
	{
		SumO[p][k] += Hidden[p][j] * WeightHO[j][k];
	}

	Output[p][k] = ;//softmax
	Error += 0.5 * (Target[p][k] - Output[p][k]) * (Target[p][k] - Output[p][k]) ;
	DeltaO[k] = (Target[p][k] - Output[p][k]) * Output[p][k] * (1.0 - Output[p][k]);
	}
    	for( int j = 1 ; j <= NumHidden ; j++ )
	{		/* 'back-propagate' errors to hidden layer */
        	SumDOW[j] = 0.0 ;
        	for(int k = 1 ; k <= NumOutput ; k++ )
		{
            		SumDOW[j] += WeightHO[j][k] * DeltaO[k] ;
        	}
        	DeltaH[j] = SumDOW[j] * Hidden[p][j] * (1.0 - Hidden[p][j]) ;
    	}
    	for( int j = 1 ; j <= NumHidden ; j++ )
	{         /* update weights WeightIH */
        	DeltaWeightIH[0][j] = eta * DeltaH[j] + alpha * DeltaWeightIH[0][j] ;
        	WeightIH[0][j] += DeltaWeightIH[0][j] ;
        	for(int i = 1 ; i <= NumInput ; i++ )
		{
            		DeltaWeightIH[i][j] = eta * Input[p][i] * DeltaH[j] + alpha * DeltaWeightIH[i][j];
            		WeightIH[i][j] += DeltaWeightIH[i][j] ;
        	}
    	}
    	for( int k = 1 ; k <= NumOutput ; k ++ )
	{         /* update weights WeightHO */
        	DeltaWeightHO[0][k] = eta * DeltaO[k] + alpha * DeltaWeightHO[0][k] ;
        	WeightHO[0][k] += DeltaWeightHO[0][k] ;
        	for( int j = 1 ; j <= NumHidden ; j++ )
		{
            		DeltaWeightHO[j][k] = eta * Hidden[p][j] * DeltaO[k] + alpha * DeltaWeightHO[j][k] ;
            		WeightHO[j][k] += DeltaWeightHO[j][k] ;
        	}
    	}
}

//training process

for (epoch = 1; epoch <= 15000; j++)
{
	if (Error < 0.010) break;
}




