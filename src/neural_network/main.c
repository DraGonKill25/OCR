#include "neural_network.h"
#include "neural_network_tools.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "neural_network_XOR.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

int main(int argc, char* argv)
{
    struct Neural_Network* nn = InitializeNetwork();


    //recuparation data
    //bias & weight
    //
    //training
    //Neural_Network_ORC(nn, input, goal);
    SDL_Surface* img = NULL;

    img = IMG_Load(argv[1]);

    size_t size = img->w * img->h;

    int* input = calloc(size, sizeof(int));

    image_to_list(img, input);




    //tableau save
     


    //appeler forward
    //recup les weights
    //appliquer softmax
    //ecrire dans le fichier



    return 0;
}
