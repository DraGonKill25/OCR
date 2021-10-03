#include <stdlib.h> //rando() fct

//Initialize 



double Input[NumInput+1];
double SumH[NumHidden+1];
double Hidden[NumHidden+1];
double WeightIH[NumInput+1][NumHidden+1]


SumH = Weight[0];
for (i = 1; i<= NumInput; i++)
{
	SumH += Input[i]*Weight[i];
}
Hidden = ;//SoftMax activation function





for (j = 1; j <= NumHidden; j++)
{
	for (i = 0; i <= NumInput; i++)
	{
		DeltaWeightIH[i][j] = 0.0;
		WeightIH[i][j] = 2.0 * (rando() - 0.5
	}
}

Error = 0.0;

for ( p = 1; p <= NumPattern; p++)
{
	for ( j = 1; j <= NumHidden; j++)
		{
			SumH[p][j] = WeightIH[0][j];
			for (int i = 1; i ,= NumInput; i++)
			{
				SumH[p][j] += Input[p][j] * WeightIH[i][i];
			}
			Hidden[p][j] += ;	//softmax function
		}
	for( k = 1 ; k <= NumOutput ; k++ )
	{
		SumO[p][k] = WeightHO[0][k] ;
    	for( j = 1 ; j <= NumHidden ; j++ ) 
	{
		SumO[p][k] += Hidden[p][j] * WeightHO[j][k];
	}

	Output[p][k] = ;//softmax
	Error += 0.5 * (Target[p][k] - Output[p][k]) * (Target[p][k] - Output[p][k]) ;
	DeltaO[k] = (Target[p][k] - Output[p][k]) * Output[p][k] * (1.0 - Output[p][k]);
	}
    	for( j = 1 ; j <= NumHidden ; j++ )
	{		/* 'back-propagate' errors to hidden layer */
        	SumDOW[j] = 0.0 ;
        	for( k = 1 ; k <= NumOutput ; k++ )
		{
            		SumDOW[j] += WeightHO[j][k] * DeltaO[k] ;
        	}
        	DeltaH[j] = SumDOW[j] * Hidden[p][j] * (1.0 - Hidden[p][j]) ;
    	}
    	for( j = 1 ; j <= NumHidden ; j++ )
	{         /* update weights WeightIH */
        	DeltaWeightIH[0][j] = eta * DeltaH[j] + alpha * DeltaWeightIH[0][j] ;
        	WeightIH[0][j] += DeltaWeightIH[0][j] ;
        	for(int i = 1 ; i <= NumInput ; i++ )
		{
            		DeltaWeightIH[i][j] = eta * Input[p][i] * DeltaH[j] + alpha * DeltaWeightIH[i][j];
            		WeightIH[i][j] += DeltaWeightIH[i][j] ;
        	}
    	}
    	for( k = 1 ; k <= NumOutput ; k ++ )
	{         /* update weights WeightHO */
        	DeltaWeightHO[0][k] = eta * DeltaO[k] + alpha * DeltaWeightHO[0][k] ;
        	WeightHO[0][k] += DeltaWeightHO[0][k] ;
        	for( j = 1 ; j <= NumHidden ; j++ )
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




