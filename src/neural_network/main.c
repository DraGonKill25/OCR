#include "neural_network.h"
#include "neural_network_tools.h"
#include "training.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "sdl_tools.h"

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

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "Wrong arguments !");
    }
    struct Neural_Network* nn = InitializeNetwork();

    //training
    for (size_t i = 1; i < 11; i++)
    {
        Neural_Network_OCR(nn, Input[i], goal[i]);
    } 
    SDL_Surface* img;

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
