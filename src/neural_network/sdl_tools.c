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

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
    {
        errx(3, "can't load %s: %s", path, IMG_GetError());
    }

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

//Create the .txt file with the values of pixels
void create_matrix_file(SDL_Surface *img, char *filename)
{
    //Variables
    Uint8 r;
    Uint8 g;
    Uint8 b;

    strtok(filename,".");
    strcat(filename,".txt");
    FILE *file = fopen(filename,"w");
    printf("%s \n",filename);

    for(int i = 0; i < img -> h; i++)
    {
        for(int j = 0;j < img -> w; j++)
        {
            Uint32 pixel = get_pixel(img, j, i);
            SDL_GetRGB(pixel, img -> format, &r, &g, &b);
            if(r == 0 && g == 0 && b == 0)
                fputs("1",file);
            else
                fputs("0",file);
        }
        fputs("\n",file);
    }
    fclose(file);
}


double *goalArray(char digit)
{
  double *goal = calloc(10, sizeof(double));
  if(digit >= '0' && digit <= '9')
  {
      goal[(int)digit - 48] = 1;
  }
  return goal;
}

//Create & return all the goals matrixes (for all letters)


double **goalMatrix()
{
  double **goalMatrix = malloc(sizeof(double*) * 9);
  char dig = '0';
  for(int i = 0; i < 9; i++)
  {
      goalMatrix[i] = goalArray(dig);
      dig++;
  }
  return goalMatrix;
}

double *matrixFromFile(char *filename)
{
  double *matrix = malloc(sizeof(double) * 28 * 28);
  FILE *file = fopen(filename,"r");

  if(file == NULL)
    printf("File is NULL \n");
  for(int i = 0; i <= 28; i++)
  {
    for(int j = 0; j <= 28; j++)
    {
      int c = fgetc(file);
      if(c == 49)
        matrix[j+i*28] = 1;
      if(c == 48)
        matrix[j+i*28] = 0;
    }
  }
  fclose(file);
  return matrix;
}

double **digitsMatrix()
{
    //Variables
    char digit_path[15] = "digits/1/1.txt\0";
    double **digitsMatrix = malloc(sizeof(double *) * 9);
    char digit = '1';

    for(int i = 0; i < 9; i++)
    {
        //printf("%c\n", digit);
        digit_path[7] = digit;
        digit_path[9] = digit;
        //printf("%s\n",digit_path);
        digitsMatrix[i] = matrixFromFile(digit_path);
        digit++;


    }
    return digitsMatrix;
}
SDL_Surface* resizenumber(SDL_Surface *img);

double *create_matrix(SDL_Surface *img)
{

  SDL_Surface *imgnew = resizenumber(img);
  //Variables
  double *digitMatrix = malloc(sizeof(double) * 28 * 28);
  Uint8 r;
  Uint8 g;
  Uint8 b;

  for(int i = 0; i < imgnew -> h; i++)
  {
      for(int j = 0; j < imgnew -> w; j++)
      {
          Uint32 pixel = get_pixel(imgnew, j, i);
          SDL_GetRGB(pixel, imgnew -> format, &r, &g, &b);
          if(r == 0 && g == 0 && b == 0)
          {
              digitMatrix[j + i * imgnew -> w] = 1;
              //printf("%f", digitMatrix[j + i * imgnew->w]);
          }
          else
          {
              digitMatrix[j + i * imgnew -> w] = 0;
              //printf("%f", digitMatrix[j + i * imgnew->w]);
          }
      }
  }
  return digitMatrix;
}


SDL_Surface* resizenumber(SDL_Surface *img)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                        28,
                        28,
                        img->format->BitsPerPixel,0,0,0,0);
  SDL_SoftStretch(img, NULL, dest, NULL);
  //SDL_BlitScaled(img, NULL, dest, NULL);
  return dest;
}

