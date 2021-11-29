#include "neural_network.h"
#include "neural_network_tools.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "neural_network_XOR.h"

int main()
{
    struct Neural_Network* nn = InitializeNetwork();


    //trainning
    Neural_Network_OCR(nn, input, goal);


    //tableau save
    char input; 
    char goal;

    //appeler forward
    //recup les weights
    //appliquer softmax
    //ecrire dans le fichier



    return 0;
}
