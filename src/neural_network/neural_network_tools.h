# ifndef NEURAL_NETWORK_TOOLS_H_
# define NEURAL_NETWORK_TOOLS_H_

# include "neural_network.h"
# include <stdlib.h>
# include <stdio.h>
# include <err.h>
#include <SDL.h>


void init_sdl();

SDL_Surface* load_image(char *path);

SDL_Surface* display_image(SDL_Surface *img);

void wait_for_keypressed();

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

void update_surface(SDL_Surface* screen, SDL_Surface* image);

void image_to_list(SDL_Surface* surf, int* input);

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

int soft_max(struct Neural_Network* net);


# endif
