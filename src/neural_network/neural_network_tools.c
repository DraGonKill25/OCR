#include "neural_network_tools.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <err.h>

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}


void wait_for_keypressed()
{
    SDL_Event event;

//A random that returns a double in [-1; 1]
double Random()
{
    return ((double)rand()) / ((double)RAND_MAX / 2) - 1;
}

//The classical sigmoid function
double Sigmoid(double x)
{
  return(1.0 / (1.0 + exp(-x)));
}

//The classical derivate of the sigmoid function
double Derivate_Sigmoid(double x)
{
  return x * (1.0 - x);
}


void image_to_list(SDL_Surface* image_surface, int* input)
{
    SDL_Surface* image = image_surface;

    size_t w = image->w;
    size_t h = image->h;

    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = 0; j < h; j++)
        {
            Uint32 pixel = get_pixel(image, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if (r == 255)
            {
                input[j * w + i] = 1;
            }
            else
            {
                input[j * w + i] = 0;
            }
        }
    }
}




//Return the position of the output with the greatest sigmoid result
int RetrievePos(struct Neural_Network *net)
{
  double max = 0;
  int posMax = 0;

  for (int o = 0; o < net -> nbOutput; o++)
  {
    if (max < net -> OutputO[o])
    {
      posMax = o;
      max = net -> OutputO[o];
    }
  }
  return posMax;
}

//Calculates the Squared error
void SquaredError(struct Neural_Network *net)
{
  double max = 0;
  double sum = 0;

  for (int o = 0; o < net -> nbOutput; o++)
  {
    if (max < net -> OutputO[o])
    {
      max = net -> OutputO[o];
    }
    sum += (net -> Goal[o] - net -> OutputO[o]) *
                      (net -> Goal[o] - net -> OutputO[o]);
  }
  net -> ErrorRate += 0.5 * sum;
}


//Retrive the position of the char in the goal tab
int PosGoal(double *goal)
{
  int count = 0;

  while(goal[count] != 1.0)
    count++;

  return count;
}

//Retrive char from value val
char RetrieveChar(int val)
{
  char c;

  if(val <= 25)
  {
    c = val + 65;
  }
  else if(val > 25 && val <= 51)
  {
    c = (val + 97 - 26);
  }
  else if(val > 51 && val <= 61)
  {
    c = val + 48 - 52;
  }
  else
  {
    switch(val)
    {
      case 62:
        c = ';';
        break;
      case 63:
        c = '\'';
        break;
      case 64:
        c = ':';
        break;
      case 65:
        c = '-';
        break;
      case 66:
        c = '.';
        break;
      case 67:
        c = '!';
        break;
      case 68:
        c = '?';
        break;
      case 69:
        c = '(';
        break;
      case 70:
        c = '\"';
        break;
      case 71:
        c = ')';
        break;
      default:
        exit(1);
        break;
    }
  }
  return c;
}

//Save data of the NN in 4 files:
//WeightIH - WeightHO - BiasH - BiasO
void SaveData(struct Neural_Network *net)
{
  FILE* weightIH = fopen("weightIH.w", "w");
  for(int i = 0; i < net -> nbInput; ++i)
  {
    for(int h = 0; h < net -> nbHidden; ++h)
    {
      fprintf(weightIH, "%f\n", net -> WeightIH[i][h]);
    }
  }
  fclose(weightIH);

  FILE* weightHO = fopen("weightHO.w", "w");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    for(int o = 0; o < net -> nbOutput; ++o)
    {
        fprintf(weightHO, "%f\n", net -> WeightHO[h][o]);
    }
  }
  fclose(weightHO);

  FILE* biasH = fopen("biasH.b", "w");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    fprintf(biasH, "%f\n", net -> BiasH[h]);
  }
  fclose(biasH);

  FILE* biasO = fopen("biasO.b", "w");
  for (int o = 0; o < net -> nbOutput; ++o)
  {
    fprintf(biasO, "%f\n", net -> BiasO[o]);
  }
  fclose(biasO);
}

//Extract data previously saved in 4 files:
//WeightIH - WeightHO - BiasH - BiasO
struct Neural_Network* ExtractData()
{
  //CREATE NN
  struct Neural_Network *net = malloc(sizeof(struct Neural_Network));
  net -> nbInput = 28*28; //size of imgs
  net -> nbHidden = 20;
  net -> nbOutput = 52; //26*2 letters
  net -> str = malloc(sizeof(char)*1200);
  net -> str = "\0";

  int sizeMax = 15;
  char *line = calloc(15, sizeof(char));

  //WeightIH
  FILE* weightIH = fopen("weightIH.w", "r");
  for(int i = 0; i < net -> nbInput; ++i)
  {
    for(int h = 0; h < net -> nbHidden; ++h)
    {
      fgets(line, sizeMax, weightIH);
      strtok(line, "\n");
      net -> WeightIH[i][h] = atof(line);
    }
  }
  fclose(weightIH);

  //Weight HO
  FILE* weightHO = fopen("weightHO.w", "r");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    for(int o = 0; o < net -> nbOutput; ++o)
    {
        fgets(line, sizeMax, weightHO);
        strtok(line, "\n");
        net -> WeightHO[h][o] = atof(line);
    }
  }
  fclose(weightHO);

  //BiasH
  FILE* biasH = fopen("biasH.b", "r");
  for(int h = 0; h < net -> nbHidden; ++h)
  {
    fgets(line, sizeMax, biasH);
    strtok(line, "\n");
    net -> BiasH[h] = atof(line);
  }
  fclose(biasH);

  //BiasO
  FILE* biasO = fopen("biasO.b", "r");
  for (int o = 0; o < net -> nbOutput; ++o)
  {
    fgets(line, sizeMax, biasO);
    strtok(line, "\n");
    net -> BiasO[o] = atof(line);
  }
  fclose(biasO);

  return net;
}


//softmax to determine the output layer
int soft_max(struct Neural_Network* net)
{
    double tablo[10];
    double denominator = 0.0;

    //denominator
    for (size_t i = 1; net->OutputO; i++)
    {
        denominator += exp(net->OutputO[i]);
    }

    //applies softmax and stack it in "tablo"
    for (size_t i = 1; net->OutputO; i++)
    {
        float nominator = net->OutputO[i];
        tablo[i] = nominator / denominator;
    }

    double max = 0;
    
    //position of the max value in array, 0 -> blank, j otherwise
    int j;
    //determine the highest proba for each output
    for (j = 0; j < 11; j++)
    {
        if (tablo[j] > max)
        {
            max = tablo[j];
        }
    }
    
    return j;
}


