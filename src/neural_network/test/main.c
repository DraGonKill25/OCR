#include "main.h"


int main(int argc, char *argv[])
{
    //Argument gesture
    if(argc>1)
    {
        if(strcmp(argv[1], "--OCR") == 0)
        {
            trainNN();
        }
    }
}
