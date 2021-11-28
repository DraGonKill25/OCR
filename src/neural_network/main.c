#include "neural_network.h"
#include "neural_network_tools.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "neural_network_XOR.h"

int main(int argc, char** argv)
{
    if (argc <= 3)
    {
        errx(1, "Invalid argument(s)");
    }

    double *input = argv[1];
    double *goal = argv[2];

    Neural_Network* nn = InitializeNetwork();
    Neural_Network_OCR(nn, input, goal);


    return 0;
}
