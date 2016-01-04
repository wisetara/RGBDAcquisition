#include <stdio.h>
#include <stdlib.h>
#include "../Codecs/codecs.h"



int runFilter(char * filenameInput,char * filenameOutput)
{
 unsigned int inputType = guessFilenameTypeStupid(filenameInput);
 struct Image * inputImage = readImage(filenameInput,inputType,0);
 if (inputImage!=0)
 {
    unsigned int outputType = guessFilenameTypeStupid(filenameOutput);
    writeImageFile(inputImage,outputType ,filenameOutput);

    destroyImage(inputImage);
    return 1;
 }
 return 0;
}



int main(int argc, char *argv[])
{
    if (argc<3)
    {
      fprintf(stderr,"Not enough arguments\n");
      return 1;
    } else
    if (argc>3)
    {
      fprintf(stderr,"Too many arguments\n");
      return 1;
    }

    fprintf(stderr,"Converting %s to %s !\n",argv[1],argv[2]);
      runFilter(argv[1],argv[2]);
    return 0;
}
