#ifndef MAIN_NN_H_
#define MAIN_NN_H


#include "neural_network/neural_network.h"
#include "neural_network/neural_network_tools.h"
#include "neural_network/sdl_tools.h"

int trainNN();

char OCR(struct Neural_Network* net, SDL_Surface* img);

#endif
