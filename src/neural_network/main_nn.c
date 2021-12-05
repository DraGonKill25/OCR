#include "neural_network.h"
#include "neural_network_tools.h"
#include "sdl_tools.h"


#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"




int trainNN()
{
    printf("test\n");
    //Variables
    int nbEpoch = 5000;
    int nbDigits = 9;
    int currentDigit = 0;
    //int count = 0;
    srand(time(NULL));

    //Intialize network
    struct Neural_Network *net = InitializeNetwork();

    //Initialize all goals & letters
    double **digits = digitsMatrix();
    double **goal = goalMatrix();

    /*for (int i = 0; i < 10; i++)
    {
        printf("\n ");
        for (int j = 0; j< 784;j++)
        {
            if (j % 28 == 0)printf("\n");
            if (digits[i][j] == 1.0)
            {
                printf("%s",KRED);
                printf("1 ");
            }
            else
            {
                printf("%s",KWHT);
                printf("0 ");
            }
        }
    }
    printf("\n");


    for (int i = 0; i < 10; i++)
    {
        printf("\n ");
        for (int j = 0; j< 10;j++)
        {
            printf("%f ", goal[i][j]);
        }
    }
    printf("\n");
    */

    for (int epoch = 0; epoch < nbEpoch; epoch++)
    {
        currentDigit = 0; //+1
        for (int l = 0; l < nbDigits; l++)
        {

            Neural_Network_OCR(net, digits[l], goal[currentDigit]);
            currentDigit++;

            if (epoch % 100 == 0)
            {
                PrintState(net);
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
        if(net->MaxErrorRate<0.0001 && net->MaxErrorRate != 0.0)
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


//traitement bla bla bla qui renvoie un tablo[81] SDL_Surface des 81 cases
//

char OCR(struct Neural_Network* net, SDL_Surface* img)
{
    return Convert(net, img);
}

/*
int main_nn(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "erreur argument");
    }
    init_sdl();
    
    printf("toto\n");
    if (strcmp(argv[1], "1") == 0)
    {
        trainNN();
    }
    else
    {
        struct Neural_Network *net = ExtractData();
        SDL_Surface* img = NULL;
        img = IMG_Load(argv[1]);

        char toto = OCR(net, img);
        printf("%c\n", toto);
    }
    return 0;
}
*/
