#include <stdio.h>
#include "bvh_loader.h"
#include "../TrajectoryParser/InputParser_C.h"

//http://research.cs.wisc.edu/graphics/Courses/cs-838-1999/Jeff/BVH.html?fbclid=IwAR0BopXj4Kft_RAEE41VLblkkPGHVF8-mon3xSCBMZueRtyb9LCSZDZhXPA


int pushNewBVHMotionState(struct BVH_MotionCapture * bvhMotion , char * parameters)
{
   if (
         (bvhMotion->motionValues==0) ||
         (bvhMotion->motionValuesSize==0)
      )
   {
     fprintf(stderr,"cannot pushNewBVHMotionState without space to store new information\n");
     return 0;
   }

   struct InputParserC * ipc = InputParser_Create(1024,5);
   if (ipc==0) { return 0; }

   InputParser_SetDelimeter(ipc,0,' ');
   InputParser_SetDelimeter(ipc,1,'\t');
   InputParser_SetDelimeter(ipc,2,' ');
   InputParser_SetDelimeter(ipc,3,' ');
   InputParser_SetDelimeter(ipc,3,'\n');

   unsigned int i=0;
   int numberOfParameters = InputParser_SeperateWords(ipc,parameters,1);
   fprintf(stderr,"MOTION command has %u parameters\n",numberOfParameters);

   if (numberOfParameters>0)
   {
     if (numberOfParameters + bvhMotion->numberOfFramesEncountered  * bvhMotion->numberOfValuesPerFrame < bvhMotion->motionValuesSize+1)
     {
      fprintf(stderr,
              "Filling from %u to %u \n",
              bvhMotion->numberOfFramesEncountered  * bvhMotion->numberOfValuesPerFrame,
              numberOfParameters+bvhMotion->numberOfFramesEncountered  * bvhMotion->numberOfValuesPerFrame
             );

      for (i=0; i<numberOfParameters; i++)
      {
        //fprintf(stderr,"P%u=%0.2f ",i,InputParser_GetWordFloat(ipc,i));
        bvhMotion->motionValues[i+bvhMotion->numberOfFramesEncountered  * bvhMotion->numberOfValuesPerFrame] = InputParser_GetWordFloat(ipc,i);
      }
     }
     bvhMotion->numberOfFramesEncountered++;
   }

   InputParser_Destroy(ipc);
   return 1;
}






int readBVHMotion(struct BVH_MotionCapture * bvhMotion , FILE * fd )
{
  int atMotionSection=0;
  ssize_t read;
  unsigned int frameNumber =0;

  if (fd!=0)
  {
   struct InputParserC * ipc = InputParser_Create(1024,5);

   InputParser_SetDelimeter(ipc,0,':');
   InputParser_SetDelimeter(ipc,1,'[');
   InputParser_SetDelimeter(ipc,2,',');
   InputParser_SetDelimeter(ipc,3,']');
   InputParser_SetDelimeter(ipc,4,'\n');

    char str[512];
    char * line = NULL;
    size_t len = 0;

    while ((read = getline(&line, &len, fd)) != -1)
    {
       printf("Retrieved line of length %zu :\n", read);
       printf("%s", line);
       int num = InputParser_SeperateWords(ipc,line,1);

       //InputParser_GetWord(ipc,0,str,512);
       //fprintf(stderr,"Word0=`%s`",str);
       //InputParser_GetWord(ipc,1,str,512);
       //fprintf(stderr,"Word1=`%s`",str);
       //InputParser_GetWord(ipc,2,str,512);
       //fprintf(stderr,"Word2=`%s`",str);

       if (!atMotionSection)
       {
          if (InputParser_WordCompareAuto(ipc,0,"MOTION"))      { atMotionSection=1; }
       } else
       {
         if (InputParser_WordCompareAuto(ipc,0,"Frames"))      { bvhMotion->numberOfFrames = InputParser_GetWordInt(ipc,1); } else
         if (InputParser_WordCompareAuto(ipc,0,"Frame Time"))  { bvhMotion->frameTime = InputParser_GetWordFloat(ipc,1); }      else
         {
           if (bvhMotion->motionValues==0)
           {
             //If we haven't yet allocated a motionValues array we need to do so now..!
             bvhMotion->motionValuesSize = bvhMotion->numberOfFrames * bvhMotion->numberOfValuesPerFrame;
             bvhMotion->motionValues = (float*)  malloc(sizeof(float) * (1+bvhMotion->motionValuesSize));
           }

           //This is motion input
           InputParser_GetWord(ipc,0,str,512);
           pushNewBVHMotionState(bvhMotion,str);
           str[0]=0;//Clean up str
         }
       }
    }

   InputParser_Destroy(ipc);
  }

  fprintf(
           stderr,
           "Frames: %u(%u) / Frame Time : %0.4f\n",
           bvhMotion->numberOfFrames,
           bvhMotion->numberOfFramesEncountered,
           bvhMotion->frameTime
          );

  return (atMotionSection);
}


int readBVHHeader(struct BVH_MotionCapture * bvhMotion , FILE * fd )
{
  bvhMotion->numberOfValuesPerFrame = 57;



  int atHeaderSection=0;
  ssize_t read;
  unsigned int frameNumber =0;

  if (fd!=0)
  {
   struct InputParserC * ipc = InputParser_Create(1024,5);

   InputParser_SetDelimeter(ipc,0,':');
   InputParser_SetDelimeter(ipc,1,'[');
   InputParser_SetDelimeter(ipc,2,',');
   InputParser_SetDelimeter(ipc,3,']');
   InputParser_SetDelimeter(ipc,4,'\n');

    char str[512];
    char * line = NULL;
    size_t len = 0;

    while ((read = getline(&line, &len, fd)) != -1)
    {
       printf("Retrieved line of length %zu :\n", read);
       printf("%s", line);
       int num = InputParser_SeperateWords(ipc,line,1);
       if (!atHeaderSection)
       {
          if (InputParser_WordCompareAuto(ipc,0,"HIERARCHY"))      { atHeaderSection=1; }
       } else
       {
         //TODO :

       }
    }

   InputParser_Destroy(ipc);
  }


 return atHeaderSection;
}


int loadBVH(const char * filename , struct BVH_MotionCapture * bvhMotion)
{
  int successfullRead=0;
  FILE *fd=0;
  fd = fopen(filename,"r");
  if (fd!=0)
    {
      if (readBVHHeader(bvhMotion,fd))
      {
       if (readBVHMotion(bvhMotion,fd))
       {
         successfullRead=1;
       }
      }
      fclose(fd);
    }
 return successfullRead;
}
