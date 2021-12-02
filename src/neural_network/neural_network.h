// neural_network.h

# ifndef NEURAL_NETWORK_H_
# define NEURAL_NETWORK_H_

# include <stdlib.h>
# include <stdio.h>
# include <err.h>
# include <time.h>
# include <math.h>

struct Neural_Network
{
    int nbInput;
    int nbHidden;
    int nbOutput;

    //Arrays
    double OutputI[28*28];
    double Goal[11];
    //Weight Arrays
    double WeightIH[28*28][20];
    double WeightHO[20][11];
    //Bias Array
    double BiasH[20];
    //Bias output
    double BiasO[11];
    //Output Hidden
    double OutputH[20];
    //Output of output
    double OutputO[11];

    //delta weight
    double dWeightIH[28*28][20];
    double dWeightHO[20][11];
    //delta of output
    double dOutputO[11];
    //delta of hidden
    double dHidden[20];

    double MaxErrorRate;
    double ErrorRate;
    double eta;
    double alpha;

    char *str[81];
};


struct Neural_Network* InitializeNetwork();

void PrintState(struct Neural_Network *net);

void Neural_Network_OCR(struct Neural_Network *net, double *input,double *goal);

# endif
