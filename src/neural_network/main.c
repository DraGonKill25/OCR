#include "neural_network.h"
#include "neural_network_tools.h"
#include "training.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "sdl_tools.h"


#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"


double goal[10][10] = {
    {1,0,0,0,0,0,0,0,0,0},//0 // Blank
    {0,1,0,0,0,0,0,0,0,0},//1
    {0,0,1,0,0,0,0,0,0,0},//2
    {0,0,0,1,0,0,0,0,0,0},//3
    {0,0,0,0,1,0,0,0,0,0},//4
    {0,0,0,0,0,1,0,0,0,0},//5
    {0,0,0,0,0,0,1,0,0,0},//6
    {0,0,0,0,0,0,0,1,0,0},//7
    {0,0,0,0,0,0,0,0,1,0},//8
    {0,0,0,0,0,0,0,0,0,1}//9
};

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"


int trainNN()
{
    //Variables
    int nbEpoch = 5000;
    int nbDigits = 10;
    int currentDigit = 0;
    //int count = 0;
    srand(time(NULL));

    //Intialize network
    struct Neural_Network *net = InitializeNetwork();

    //Initialize all goals & letters
    double *digits = Input;

    for (int epoch = 0; epoch < nbEpoch; epoch++)
    {
        currentDigit = 0;
        for (int l = 0; l < nbDigits; l++)
        {

            Neural_Network_OCR(net, digits[l], goal[currentDigit]);
            currentDigit++;

            if (epoch % 100 == 0)
            {
                //PrintState(net);
            }
            //count++;
        }
        //== PRINT ERROR EVERY 100 EPOCHs ==//
        if (epoch % 100 == 0)
        {
            if(net -> MaxErrorRate > 0.005)
                printf("Epoch %-5d | MaxErrorRate = %s %f \n",
                        epoch,KRED,net->MaxErrorRate);
            else
            {
                printf("Epoch %-5d | MaxErrorRate = %s %f \n",
                        epoch,KGRN,net->MaxErrorRate);
                // canSave = 1;
            }
            printf("%s",KWHT);
        }
        if(net->MaxErrorRate<0.0005 && net->MaxErrorRate != 0.0)
        {
            break;
        }
        net -> MaxErrorRate = 0.0;
    }
    printf("Save data...\n");
    SaveData(net);
    printf("Learn finish\n");
    return EXIT_SUCCESS;
}


int main()
{
    trainNN();
    return 0;
}
