#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include <fcntl.h>

//Xor initilization

//number of training pattern
#define NUMPAT 4

//number of input cells
#define NUMIN  2

//number of hidden layer cells
#define NUMHID 2

//number of output cells
#define NUMOUT 1

//double numbers generator function
#define rando() ((double)rand()/((double)RAND_MAX+1))

int main()
{
    int    p, np, op, ranpat[NUMPAT+1], epoch;
    int    NumPattern = NUMPAT, NumInput = NUMIN, NumHidden = NUMHID, NumOutput = NUMOUT;
    
    //array that contains patters 
    double Input[NUMPAT+1][NUMIN+1] = {{0, 0, 0},{0, 0, 0},{0, 1, 0},{0, 0, 1},{0, 1, 1} };

    //array with 
    double Target[NUMPAT+1][NUMOUT+1] = { {0, 0},{0, 0},{0, 1},{0, 1},{0, 0} };
   
    double SumH[NUMPAT+1][NUMHID+1], WeightIH[NUMIN+1][NUMHID+1], Hidden[NUMPAT+1][NUMHID+1];
    double SumO[NUMPAT+1][NUMOUT+1], WeightHO[NUMHID+1][NUMOUT+1], Output[NUMPAT+1][NUMOUT+1];
    
    //Back propagation weights
    double DeltaO[NUMOUT+1], SumDOW[NUMHID+1], DeltaH[NUMHID+1];
    double DeltaWeightIH[NUMIN+1][NUMHID+1], DeltaWeightHO[NUMHID+1][NUMOUT+1];
    
    double Error, eta = 0.5, alpha = 0.9, smallwt = 0.5;

    //eta is gradient descent contribution -> learning rate
    //alpha maintain and smooth weight changes
    //smallwt -> maximum abs size of initial weights

    //deltaweight are a little shift to reduce the error when update weights
    
  	// initialize WeightIH randomly and DeltaWeightIH with zeros
    for(int j = 1; j <= NumHidden ; j++)
    {    
        for(int i = 0 ; i <= NumInput; i++) 
        { 
            DeltaWeightIH[i][j] = 0.0;
            WeightIH[i][j] = 2.0 * (rando() - 0.5) * smallwt;
        }
    }
    
    // initialize WeightHO randomly and DeltaWeightHO with zeros
    for(int k = 1 ; k <= NumOutput ; k ++)
    {    
        for(int j = 0; j <= NumHidden; j++) 
        {
            DeltaWeightHO[j][k] = 0.0;              
            WeightHO[j][k] = 2.0 * (rando() - 0.5) * smallwt;
        }
    }
    
    //setting epoch to a high number to get a reduced value of error
    for(epoch = 0; epoch < 100000; epoch++)
    {   
        //iterating for all different traininig patterns
        for(p = 1; p <= NumPattern ; p++ )
        {    
            ranpat[p] = p;
        }
        //randomizing cells order in training pattern
        for(p = 1 ; p <= NumPattern; p++)
        {
            np = p + rando() * (NumPattern + 1 - p);
            op = ranpat[p]; ranpat[p] = ranpat[np]; ranpat[np] = op;
        }
        
        //setting error to 0 at the begining of every epoch to make an evolution
        //accross time
        Error = 0.0 ;
        
        // repeat for all the training patterns
        for(np = 1; np <= NumPattern; np++)
        {    
            p = ranpat[np];
            
            //activation of every cells in hidden layer
            for(int j = 1; j <= NumHidden; j++)
            {    
                SumH[p][j] = WeightIH[0][j];
                for(int i = 1 ; i <= NumInput; i++)
                {
                    SumH[p][j] += Input[p][i] * WeightIH[i][j];
                }
                Hidden[p][j] = 1.0/(1.0 + exp(-SumH[p][j]));
            }
            
            //activation of every cells in output layer
            //updating the error
            for(int k = 1; k <= NumOutput; k++)
            {    
                SumO[p][k] = WeightHO[0][k];
                for(int j = 1; j <= NumHidden; j++) 
                {
                    SumO[p][k] += Hidden[p][j] * WeightHO[j][k];
                }
                Output[p][k] = 1.0/(1.0 + exp(-SumO[p][k])) ;   // Sigmoidal Outputs
                Error += 0.5 * (Target[p][k] - Output[p][k]) * (Target[p][k] - Output[p][k]) ;
                DeltaO[k] = (Target[p][k] - Output[p][k]) * Output[p][k] * (1.0 - Output[p][k]) ;

            }
            
            // 'back-propagate' errors to hidden layer
            for(int j = 1; j <= NumHidden; j++) 
            {    
                SumDOW[j] = 0.0;
                
                for(int k = 1; k <= NumOutput ; k++)
                {
                    SumDOW[j] += WeightHO[j][k] * DeltaO[k];
                }
                
                DeltaH[j] = SumDOW[j] * Hidden[p][j] * (1.0 - Hidden[p][j]);
            }
            
            // update weights WeightIH
            for(int j = 1; j <= NumHidden; j++)
            {     
                DeltaWeightIH[0][j] = eta * DeltaH[j] + alpha * DeltaWeightIH[0][j];
                WeightIH[0][j] += DeltaWeightIH[0][j];
                
                for(int i = 1; i <= NumInput; i++ )
                { 
                    DeltaWeightIH[i][j] = eta * Input[p][i] * DeltaH[j] + alpha * DeltaWeightIH[i][j];
                    WeightIH[i][j] += DeltaWeightIH[i][j];
                }
            }
            
            // update weights WeightHO
            for(int k = 1; k <= NumOutput; k ++)
            {    
                DeltaWeightHO[0][k] = eta * DeltaO[k] + alpha * DeltaWeightHO[0][k];
                WeightHO[0][k] += DeltaWeightHO[0][k] ;
                for(int j = 1; j <= NumHidden; j++ )
                {
                    DeltaWeightHO[j][k] = eta * Hidden[p][j] * DeltaO[k] + alpha * DeltaWeightHO[j][k];
                    WeightHO[j][k] += DeltaWeightHO[j][k];
                }
            }
        }
        //print every 500 epoch
        if( epoch%500 == 0 ) fprintf(stdout, "\nEpoch %-5d :   Error = %f", epoch, Error) ;
        if( Error < 0.0001 ) break ;  // stop learning when 'near enough'
    }

    //some print to show evolution
    fprintf(stdout, "\n\nNETWORK DATA - EPOCH %d\n\nPat\t", epoch);   
    
    for(int i = 1; i <= NumInput; i++ )
    {
        fprintf(stdout, "Input%-4d\t", i);
    }
    
    for(int k = 1; k <= NumOutput; k++)
    {
        fprintf(stdout, "Target%-4d\tOutput%-4d\t", k, k);
    }
    
    for(p = 1; p <= NumPattern; p++)
    {        
    	fprintf(stdout, "\n%d\t", p);
    	
        for(int i = 1; i <= NumInput; i++)
        {
            fprintf(stdout, "%f\t", Input[p][i]);
        }
        
        for(int k = 1; k <= NumOutput ; k++)
        {
            fprintf(stdout, "%f\t%f\t", Target[p][k], Output[p][k]);
        }
    }

    printf("\n");
    return 1 ;
}
