# ifndef NEURAL_NETWORK_TOOLS_H_
# define NEURAL_NETWORK_TOOLS_H_

# include "neural_network.h"
# include <stdlib.h>
# include <stdio.h>
# include <err.h>

//A random that returns a double in [-1; 1]
double Random();

//The classical sigmoid function
double Sigmoid(double x);

//The classical derivate of the sigmoid function
double Derivate_Sigmoid(double x);

//Return the position of the output with the greatest sigmoid result
int RetrievePos(struct Neural_Network *net);

//Calculates the Squared error
void SquaredError(struct Neural_Network *net);

//Retrive the position of the char in the goal tab
int PosGoal(double *goal);

//Retrive char from value val
char RetrieveChar(int val);

//Save data of the NN in 4 files:
//WeightIH - WeightHO - BiasH - BiasO
void SaveData(struct Neural_Network *net);

//Extract data previously saved in 4 files:
//WeightIH - WeightHO - BiasH - BiasO
struct Neural_Network* ExtractData ();

# endif
