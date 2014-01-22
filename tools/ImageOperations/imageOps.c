#include "imageOps.h"
#include <stdio.h>
#include <stdlib.h>

#define ABSDIFF(num1,num2) ( (num1-num2) >=0 ? (num1-num2) : (num2 - num1) )

#define MEMPLACE1(x,y,width) ( y * ( width  ) + x )
#define MEMPLACE3(x,y,width) ( y * ( width * 3 ) + x*3 )



unsigned int simplePowInline(unsigned int base,unsigned int exp)
{
    if (exp==0) return 1;
    unsigned int retres=base;
    unsigned int i=0;
    for (i=0; i<exp-1; i++)
    {
        retres*=base;
    }
    return retres;
}



int saveRawImageToFile(char * filename,char *comments ,unsigned char * pixels , unsigned int width , unsigned int height , unsigned int channels , unsigned int bitsperpixel)
{
    //fprintf(stderr,"saveRawImageToFile(%s) called\n",filename);

    if ( (width==0) || (height==0) || (channels==0) || (bitsperpixel==0) ) { fprintf(stderr,"saveRawImageToFile(%s) called with zero dimensions\n",filename); return 0;}
    if(pixels==0) { fprintf(stderr,"saveRawImageToFile(%s) called for an unallocated (empty) frame , will not write any file output\n",filename); return 0; }
    if (bitsperpixel>16) { fprintf(stderr,"PNM does not support more than 2 bytes per pixel..!\n"); return 0; }

    FILE *fd=0;
    fd = fopen(filename,"wb");

    if (fd!=0)
    {
        unsigned int n;
        if (channels==3) fprintf(fd, "P6\n");
        else if (channels==1) fprintf(fd, "P5\n");
        else
        {
            fprintf(stderr,"Invalid channels arg (%u) for SaveRawImageToFile\n",channels);
            fclose(fd);
            return 1;
        }

        if (comments!=0)
        {
          fprintf(fd, "#%s\n",comments);
        } else
        {
          fprintf(fd, "#generated by https://github.com/AmmarkoV/RGBDAcquisition/blob/master/tools/ImageOperations/imageOps.c\n");
        }



        fprintf(fd, "%d %d\n%u\n", width, height , simplePowInline(2 ,bitsperpixel)-1);

        float tmp_n = (float) bitsperpixel/ 8;
        tmp_n = tmp_n * width * height * channels ;
        n = (unsigned int) tmp_n;

        fwrite(pixels, 1 , n , fd);
        fflush(fd);
        fclose(fd);
        return 1;
    }
    else
    {
        fprintf(stderr,"SaveRawImageToFile could not open output file %s\n",filename);
        return 0;
    }
    return 0;
}






int shiftImageRGB(unsigned char * target, unsigned char * source ,  unsigned char transR, unsigned char transG, unsigned char transB , signed int tX,  signed int tY  ,  unsigned int width , unsigned int height)
{
  if ( (target==0)||(source==0) ) { return 0; }
  if ( (width==0)&&(height==0) ) { return 0; }


  unsigned int sourceWidth=width,sourceHeight=height,targetWidth=width,targetHeight=height;
  unsigned int sourceX,sourceY , targetX,targetY;

  if (tX < 0 ) {   sourceX=abs(tX);    targetX=0;   } else
               {   sourceX=0;          targetX=abs(tX); }

  if (tY < 0 ) { sourceY=abs(tY); targetY=0;  } else
               { sourceY=0;       targetY=abs(tY); }

  width=width - abs(tX);
  height=height - abs(tY);

  if (width>sourceWidth) { width=sourceWidth; fprintf(stderr,"Error setting width (?) why did this happen ? :P \n"); }
  if (height>sourceHeight) { height=sourceHeight; fprintf(stderr,"Error setting height (?) why did this happen ? :P \n"); }

  fprintf(stderr,"Doing shift (%d,%d) by bit blitting %u,%u to %u,%u ( size %u,%u) \n",tX,tY,sourceX,sourceY,targetX,targetY,width,height);


  //----------------------------------------------------------------
  unsigned char * maybeCopiedSource = source;
  //In case tx or ty is positive it is impossible to bit blt using the same buffer since lines will be overriden so we use a seperate buffer

  if ( (tX>0) || (tY>0) )
  {
    unsigned int copySize = (width+1)*(height+1)*3*sizeof(unsigned char);
    maybeCopiedSource = (unsigned char * ) malloc(copySize);
    if (maybeCopiedSource==0) { maybeCopiedSource = source; } else
                              { memcpy(maybeCopiedSource,source,copySize); }
  }
  //----------------------------------------------------------------

  bitbltRGB( target ,targetX,targetY, targetWidth,targetHeight,
             maybeCopiedSource ,sourceX,sourceY, sourceWidth,sourceHeight,
             width,height);

  //----------------------------------------------------------------
    if ( (tX>0) || (tY>0) )
   {
     if (maybeCopiedSource!=source) { free(maybeCopiedSource); maybeCopiedSource=0; }
   }
  //----------------------------------------------------------------



   if (tX==0) { } else
   if (tX<0) { bitbltColorRGB(target,targetWidth+tX,0,targetWidth,targetHeight,transR,transG,transB,abs(tX),targetHeight); } else
             { bitbltColorRGB(target,0,0,targetWidth,targetHeight,transR,transG,transB,abs(tX),targetHeight); }

   if (tY==0) { } else
   if (tY<0) { bitbltColorRGB(target,0,targetHeight+tY,targetWidth,targetHeight,transR,transG,transB,targetWidth,abs(tY)); } else
             { bitbltColorRGB(target,0,0,targetWidth,targetHeight,transR,transG,transB,targetWidth,abs(tY)); }

return 1;

}





int shiftImageDepth(unsigned short * target, unsigned short * source , unsigned short depthVal , signed int tX,  signed int tY  ,  unsigned int width , unsigned int height)
{
  if ( (target==0)||(source==0) ) { return 0; }
  if ( (width==0)&&(height==0) ) { return 0; }


  unsigned int sourceWidth=width,sourceHeight=height,targetWidth=width,targetHeight=height;
  unsigned int sourceX,sourceY , targetX,targetY;

  if (tX < 0 ) {   sourceX=abs(tX);    targetX=0;   } else
               {   sourceX=0;          targetX=abs(tX); }

  if (tY < 0 ) { sourceY=abs(tY); targetY=0;  } else
               { sourceY=0;       targetY=abs(tY); }

  width=width - abs(tX)-1;
  height=height - abs(tY)-1;

  if (width>sourceWidth) { width=sourceWidth; fprintf(stderr,"Error setting width (?) why did this happen ? :P \n"); }
  if (height>sourceHeight) { height=sourceHeight; fprintf(stderr,"Error setting height (?) why did this happen ? :P \n"); }

  fprintf(stderr,"Doing shift (%d,%d) by bit blitting %u,%u to %u,%u ( size %u,%u) \n",tX,tY,sourceX,sourceY,targetX,targetY,width,height);

  //----------------------------------------------------------------
  unsigned short * maybeCopiedSource = source;
  //In case tx or ty is positive it is impossible to bit blt using the same buffer since lines will be overriden so we use a seperate buffer
  if ( (tX>0) || (tY>0) )
  {
    unsigned int copySize = width*height*1*sizeof(unsigned short);
    maybeCopiedSource = (unsigned short * ) malloc(copySize);
    if (maybeCopiedSource==0) { maybeCopiedSource = source; } else
                              { memcpy(maybeCopiedSource,source,copySize); }
  }
  //----------------------------------------------------------------

        bitbltDepth( target ,targetX,targetY, targetWidth,targetHeight,
                     maybeCopiedSource ,sourceX,sourceY, sourceWidth,sourceHeight,
                     width,height);


  //----------------------------------------------------------------
   if ( (tX>0) || (tY>0) )
   {
     if (maybeCopiedSource!=source) { free(maybeCopiedSource); maybeCopiedSource=0; }
   }
  //----------------------------------------------------------------

   if (tX==0) { } else
   if (tX<0)  { bitbltDepthValue(target,targetWidth+tX,0,targetWidth,targetHeight,depthVal,abs(tX),targetHeight); } else
              { bitbltDepthValue(target,0,0,targetWidth,targetHeight,depthVal,abs(tX),targetHeight); }

   if (tY==0) { } else
   if (tY<0)  { bitbltDepthValue(target,0,targetHeight+tY,targetWidth,targetHeight,depthVal,targetWidth,abs(tY)); } else
              { bitbltDepthValue(target,0,0,targetWidth,targetHeight,depthVal,targetWidth,abs(tY)); }

  return 1;
}




int mixbltRGB(unsigned char * target,  unsigned int tX,  unsigned int tY , unsigned int targetWidth , unsigned int targetHeight ,
              unsigned char * source , unsigned int sX, unsigned int sY  , unsigned int sourceWidth , unsigned int sourceHeight ,
              unsigned int width , unsigned int height)
{
  if ( (target==0)||(source==0) ) { return 0; }
  if ( (width==0)&&(height==0) ) { return 0; }
  if ( (sourceWidth==0)&&(sourceHeight==0) ) { return 0; }

  //Check for bounds -----------------------------------------
  if (tX+width>=targetWidth) { width=targetWidth-tX-1;  }
  if (tY+height>=targetHeight) { height=targetHeight-tY-1;  }

  if (sX+width>=sourceWidth) { width=sourceWidth-sX-1;  }
  if (sY+height>=sourceHeight) { height=sourceHeight-sY-1;  }
  //----------------------------------------------------------

  unsigned char * sourcePTR; unsigned char * sourceLineLimitPTR; unsigned char * sourceLimitPTR; unsigned int sourceLineSkip;
  unsigned char * targetPTR;  /*unsigned char * targetLimitPTR;*/  unsigned int targetLineSkip;


  sourcePTR      = source+ MEMPLACE3(sX,sY,sourceWidth);
  sourceLimitPTR = source+ MEMPLACE3((sX+width),(sY+height),sourceWidth);
  sourceLineSkip = (sourceWidth-width) * 3;
  sourceLineLimitPTR = sourcePTR + (width*3);
  fprintf(stderr,"SOURCE (RGB %u/%u)  Starts at %u,%u and ends at %u,%u\n",sourceWidth,sourceHeight,sX,sY,sX+width,sY+height);

  targetPTR      = target + MEMPLACE3(tX,tY,targetWidth);
  //targetLimitPTR = target + MEMPLACE3((tX+width),(tY+height),targetWidth);
  targetLineSkip = (targetWidth-width) * 3;
  fprintf(stderr,"TARGET (RGB %u/%u)  Starts at %u,%u and ends at %u,%u\n",targetWidth,targetHeight,tX,tY,tX+width,tY+height);

  unsigned int color = 0;

  while (sourcePTR < sourceLimitPTR)
  {
     while (sourcePTR < sourceLineLimitPTR)
     {
        if (*sourcePTR==0) { color = *targetPTR; } else { color = (unsigned int) ( *targetPTR + *sourcePTR ) / 2; }
        *targetPTR =  (unsigned char ) color;
        ++targetPTR; ++sourcePTR;

        if (*sourcePTR==0) { color = *targetPTR; } else { color = (unsigned int) ( *targetPTR + *sourcePTR ) / 2; }
        *targetPTR =  (unsigned char ) color;
        ++targetPTR; ++sourcePTR;

        if (*sourcePTR==0) { color = *targetPTR; } else { color = (unsigned int) ( *targetPTR + *sourcePTR ) / 2; }
        *targetPTR =  (unsigned char ) color;
        ++targetPTR; ++sourcePTR;
     }


    sourceLineLimitPTR+= sourceWidth*3;//*3;
    targetPTR+=targetLineSkip;
    sourcePTR+=sourceLineSkip;
  }
 return 1;
}






int bitbltRGB(unsigned char * target,  unsigned int tX,  unsigned int tY , unsigned int targetWidth , unsigned int targetHeight ,
              unsigned char * source , unsigned int sX, unsigned int sY  , unsigned int sourceWidth , unsigned int sourceHeight ,
              unsigned int width , unsigned int height)
{
  if ( (target==0)||(source==0) ) { return 0; }
  if ( (width==0)&&(height==0) ) { return 0; }
  if ( (sourceWidth==0)&&(sourceHeight==0) ) { return 0; }

  fprintf(stderr,"BitBlt an area of target image %u,%u  sized %u,%u \n",tX,tY,targetWidth,targetHeight);
  fprintf(stderr,"BitBlt an area of source image %u,%u  sized %u,%u \n",sX,sY,sourceWidth,sourceHeight);
  fprintf(stderr,"BitBlt size was width %u height %u \n",width,height);
  //Check for bounds -----------------------------------------
  if (tX+width>=targetWidth) { width=targetWidth-tX-1;  }
  if (tY+height>=targetHeight) { height=targetHeight-tY-1;  }

  if (sX+width>=sourceWidth) { width=sourceWidth-sX-1;  }
  if (sY+height>=sourceHeight) { height=sourceHeight-sY-1;  }
  //----------------------------------------------------------
  fprintf(stderr,"BitBlt size NOW is width %u height %u \n",width,height);

  unsigned char *  sourcePTR      = source+ MEMPLACE3(sX,sY,sourceWidth);
  unsigned char *  sourceLimitPTR = source+ MEMPLACE3((sX+width),(sY+height),sourceWidth);
  unsigned int     sourceLineSkip = (sourceWidth-width) * 3;
  unsigned char *  sourceLineLimitPTR = sourcePTR + (width*3) -3; /*-3 is required here*/
  //fprintf(stderr,"SOURCE (RGB size %u/%u)  Starts at %u,%u and ends at %u,%u\n",sourceWidth,sourceHeight,sX,sY,sX+width,sY+height);
  //fprintf(stderr,"sourcePTR is %p , limit is %p \n",sourcePTR,sourceLimitPTR);
  //fprintf(stderr,"sourceLineSkip is %u\n",        sourceLineSkip);
  //fprintf(stderr,"sourceLineLimitPTR is %p\n",sourceLineLimitPTR);


  unsigned char * targetPTR      = target + MEMPLACE3(tX,tY,targetWidth);
  unsigned char * targetLimitPTR = target + MEMPLACE3((tX+width),(tY+height),targetWidth);
  unsigned int targetLineSkip = (targetWidth-width) * 3;
  unsigned char * targetLineLimitPTR = targetPTR + (width*3) -3; /*-3 is required here*/
  //fprintf(stderr,"TARGET (RGB size %u/%u)  Starts at %u,%u and ends at %u,%u\n",targetWidth,targetHeight,tX,tY,tX+width,tY+height);
  //fprintf(stderr,"targetPTR is %p , limit is %p \n",targetPTR,targetLimitPTR);
  //fprintf(stderr,"targetLineSkip is %u\n", targetLineSkip);
  //fprintf(stderr,"targetLineLimitPTR is %p\n",targetLineLimitPTR);

  while ( (sourcePTR < sourceLimitPTR) && ( targetPTR+3 < targetLimitPTR ) )
  {
     while ( (sourcePTR < sourceLineLimitPTR) && ((targetPTR+3 < targetLineLimitPTR)) )
     {
        //fprintf(stderr,"Reading Triplet sourcePTR %p targetPTR is %p\n",sourcePTR  ,targetPTR);
        *targetPTR = *sourcePTR; ++targetPTR; ++sourcePTR;
        *targetPTR = *sourcePTR; ++targetPTR; ++sourcePTR;
        *targetPTR = *sourcePTR; ++targetPTR; ++sourcePTR;
     }

    sourceLineLimitPTR += sourceWidth*3;
    targetLineLimitPTR += targetWidth*3;
    sourcePTR+=sourceLineSkip;
    targetPTR+=targetLineSkip;
  }

 return 1;
}





int bitbltColorRGB(unsigned char * target,  unsigned int tX,  unsigned int tY  , unsigned int targetWidth , unsigned int targetHeight ,
                   unsigned char R , unsigned char G , unsigned char B ,
                   unsigned int width , unsigned int height)
{
  //Check for bounds -----------------------------------------
  if (tX+width>=targetWidth) { width=targetWidth-tX-1;  }
  if (tY+height>=targetHeight) { height=targetHeight-tY-1;  }
  //----------------------------------------------------------

  unsigned char * targetPTR; unsigned char * targetLineLimitPTR; unsigned char * targetLimitPTR;   unsigned int targetLineSkip;
  targetPTR      = target + MEMPLACE3(tX,tY,targetWidth);
  targetLimitPTR = target + MEMPLACE3((tX+width),(tY+height),targetWidth);
  targetLineSkip = (targetWidth-width) * 3;
  targetLineLimitPTR = targetPTR + (width*3) -3; /*-3 is required here*/

  fprintf(stderr,"BitBlt Color an area (%u,%u) of target image  starting at %u,%u  sized %u,%u with color RGB(%u,%u,%u)\n",width,height,tX,tY,targetWidth,targetHeight,R,G,B);
  fprintf(stderr,"last Pixels @ %u,%u\n",tX+width,tY+height);
  while ( targetPTR < targetLimitPTR )
  {
     while (targetPTR < targetLineLimitPTR)
     {
        //fprintf(stderr,"Reading Triplet sourcePTR %p targetPTR is %p\n",sourcePTR  ,targetPTR);
        *targetPTR = R; ++targetPTR;
        *targetPTR = G; ++targetPTR;
        *targetPTR = B; ++targetPTR;
     }
    targetLineLimitPTR += targetWidth*3;
    targetPTR+=targetLineSkip;
  }
 return 1;
}



int bitbltDepthValue(unsigned short * target,  unsigned int tX,  unsigned int tY  , unsigned int targetWidth , unsigned int targetHeight ,
                     unsigned short DepthVal ,
                     unsigned int width , unsigned int height)
{
  //Check for bounds -----------------------------------------
  if (tX+width>=targetWidth) { width=targetWidth-tX-1;  }
  if (tY+height>=targetHeight) { height=targetHeight-tY-1;  }
  //----------------------------------------------------------

  unsigned short * targetPTR; unsigned short * targetLineLimitPTR; unsigned short * targetLimitPTR;   unsigned int targetLineSkip;
  targetPTR      = target + MEMPLACE1(tX,tY,targetWidth);
  targetLimitPTR = target + MEMPLACE1((tX+width),(tY+height),targetWidth);
  targetLineSkip = (targetWidth-width);
  targetLineLimitPTR = targetPTR + (width) -1 ;

  fprintf(stderr,"BitBlt Depth an area (%u,%u) of target image  starting at %u,%u  sized %u,%u with Depth(%u)\n",width,height,tX,tY,targetWidth,targetHeight,DepthVal);
  while ( targetPTR < targetLimitPTR )
  {
     while (targetPTR < targetLineLimitPTR)
     {
        //fprintf(stderr,"Reading Triplet sourcePTR %p targetPTR is %p\n",sourcePTR  ,targetPTR);
        *targetPTR = DepthVal; ++targetPTR;
     }
    targetLineLimitPTR += targetWidth;
    targetPTR+=targetLineSkip;
  }
 return 1;
}



int bitbltDepth(unsigned short * target,  unsigned int tX,  unsigned int tY  , unsigned int targetWidth , unsigned int targetHeight ,
                unsigned short * source , unsigned int sX,  unsigned int sY  , unsigned int sourceWidth , unsigned int sourceHeight ,
                unsigned int width , unsigned int height)
{
  if ( (target==0)||(source==0) ) { return 0; }
  if ( (width==0)&&(height==0) ) { return 0; }
  if ( (sourceWidth==0)&&(sourceHeight==0) ) { return 0; }

  //Check for bounds -----------------------------------------
  if (tX+width>=targetWidth) { width=targetWidth-tX-1;  }
  if (tY+height>=targetHeight) { height=targetHeight-tY-1;  }

  if (sX+width>=sourceWidth) { width=sourceWidth-sX-1;  }
  if (sY+height>=sourceHeight) { height=sourceHeight-sY-1;  }
  //----------------------------------------------------------

  unsigned short * sourcePTR;  unsigned short* sourceLineLimitPTR; unsigned short * sourceLimitPTR; unsigned int sourceLineSkip;
  unsigned short * targetPTR;  /*unsigned short * targetLimitPTR;*/    unsigned int targetLineSkip;


  sourcePTR      = source+ MEMPLACE1(sX,sY,sourceWidth);
  sourceLimitPTR = source+ MEMPLACE1((sX+width),(sY+height),sourceWidth);
  sourceLineSkip = (sourceWidth-width)  ;
  sourceLineLimitPTR = sourcePTR + (width) -1;
  //fprintf(stderr,"SOURCE (Depth %u/%u)  Starts at %u,%u and ends at %u,%u\n",sourceWidth,sourceHeight,sX,sY,sX+width,sY+height);

  targetPTR      = target + MEMPLACE1(tX,tY,targetWidth);
  //targetLimitPTR = target + MEMPLACE1((tX+width),(tY+height),targetWidth);
  targetLineSkip = (targetWidth-width)  ;
  //fprintf(stderr,"TARGET (Depth %u/%u)  Starts at %u,%u and ends at %u,%u\n",targetWidth,targetHeight,tX,tY,tX+width,tY+height);

  while (sourcePTR < sourceLimitPTR)
  {
     while (sourcePTR < sourceLineLimitPTR)
     {
        *targetPTR =  *sourcePTR;
        ++targetPTR; ++sourcePTR;
     }

    sourceLineLimitPTR+= sourceWidth;
    targetPTR+=targetLineSkip;
    sourcePTR+=sourceLineSkip;
  }
 return 1;
}






int saveTileRGBToFile(  unsigned int solutionColumn , unsigned int solutionRow ,
                        unsigned char * source , unsigned int sX, unsigned int sY  , unsigned int sourceWidth , unsigned int sourceHeight ,
                        unsigned int width , unsigned int height)
{

 char filename[512];
 sprintf(filename,"tiles/rgb_tile%u_%u.pnm",solutionColumn,solutionRow);


 unsigned char * tile = (unsigned char*) malloc((width+1)*(height+1)*3*sizeof(unsigned char));
 if (tile == 0 ) { return 0; }
 bitbltRGB(tile,0,0,width,height,source,sX,sY,sourceWidth,sourceHeight,width, height);


 saveRawImageToFile(filename ,0,tile , width , height, 3 , 8);
 free(tile);

 return 1;
}

int saveTileDepthToFile(  unsigned int solutionColumn , unsigned int solutionRow ,
                          unsigned short * source , unsigned int sX, unsigned int sY  , unsigned int sourceWidth , unsigned int sourceHeight ,
                          unsigned int width , unsigned int height)
{

 char filename[512];
 sprintf(filename,"tiles/depth_tile%u_%u.pnm",solutionColumn,solutionRow);


 unsigned short * tile = (unsigned short*) malloc((width+1)*(height+1)*1*sizeof(unsigned short));
 if (tile == 0 ) { return 0; }
 bitbltDepth(tile,0,0,width,height,source,sX,sY,sourceWidth,sourceHeight,width, height);


 saveRawImageToFile(filename ,0,(unsigned char*) tile , width , height, 1 , 16);
 free(tile);

 return 1;
}








int bitBltRGBToFile(  char * name  , char * comment ,
                      unsigned char * source , unsigned int sX, unsigned int sY  , unsigned int sourceWidth , unsigned int sourceHeight ,
                      unsigned int width , unsigned int height)
{

 char filename[512];
 sprintf(filename,"%s.pnm",name);


 unsigned char * tile = (unsigned char*) malloc((width+1)*(height+1)*3*sizeof(unsigned char));
 if (tile == 0 ) { return 0; }
 bitbltRGB(tile,0,0,width,height,source,sX,sY,sourceWidth,sourceHeight,width, height);


 if ( ! saveRawImageToFile(filename ,comment,tile , width , height, 3 , 8) )
 {
     fprintf(stderr,"Could not bit blt to File %s\n",name);
 }
 free(tile);

 return 1;
}




int bitBltDepthToFile(  char * name  ,char * comment ,
                        unsigned short * source , unsigned int sX, unsigned int sY  , unsigned int sourceWidth , unsigned int sourceHeight ,
                        unsigned int width , unsigned int height)
{

 char filename[512];
 sprintf(filename,"%s.pnm",name);


 unsigned short * tile = (unsigned short*) malloc((width+1)*(height+1)*1*sizeof(unsigned short));
 if (tile == 0 ) { return 0; }
 bitbltDepth(tile,0,0,width,height,source,sX,sY,sourceWidth,sourceHeight,width, height);


 saveRawImageToFile(filename ,comment,(unsigned char*) tile , width , height, 1  , 16);
 free(tile);

 return 1;
}



unsigned int countOccurancesOfRGBPixel(unsigned char * ptrRGB , unsigned int RGBwidth , unsigned int RGBheight , unsigned char transR ,unsigned char transG , unsigned char transB)
{
 unsigned int cCount = 0;
 unsigned char * sourcePTR =  ptrRGB ;
 unsigned char * sourceLimitPTR =  ptrRGB + (RGBwidth*RGBheight *3);
 unsigned char R,G,B;

  while (sourcePTR < sourceLimitPTR)
  {
    R = *sourcePTR; ++sourcePTR;
    G = *sourcePTR; ++sourcePTR;
    B = *sourcePTR; ++sourcePTR;
    if ( (R==transR) && (G==transG) && (B==transB) ) { ++cCount; }
  }

 return cCount;
}



int getRGBPixel(unsigned char * ptrRGB , unsigned int RGBwidth , unsigned int RGBheight ,  unsigned int x , unsigned int y , unsigned char * R , unsigned char * G , unsigned char * B)
{
 unsigned char * ptr =  ptrRGB  + MEMPLACE3(x,y,RGBwidth);

 *R = *ptr; ++ptr;
 *G = *ptr; ++ptr;
 *B = *ptr; ++ptr;

 return 1;
}



int closeToRGB(unsigned char R , unsigned char G , unsigned char B  ,  unsigned char targetR , unsigned char targetG , unsigned char targetB , unsigned int threshold)
{
 if ( ( ABSDIFF(R,targetR) < threshold ) && ( ABSDIFF(G,targetG) < threshold ) && ( ABSDIFF(B,targetB) < threshold ) )   { return 1; }
 return 0;
}




unsigned int countDepthAverage(unsigned short * source, unsigned int sourceWidth , unsigned int sourceHeight ,
                                unsigned int sX,  unsigned int sY  , unsigned int tileWidth , unsigned int tileHeight)
{
  //Check for bounds -----------------------------------------
  if (sX+tileWidth>=sourceWidth) { tileWidth=sourceWidth-sX-1;  }
  if (sY+tileHeight>=sourceHeight) { tileHeight=sourceHeight-sY-1;  }
  //----------------------------------------------------------

  unsigned short * sourcePTR; unsigned short * sourceLineLimitPTR; unsigned short * sourceLimitPTR;   unsigned int sourceLineSkip;
  sourcePTR      = source + MEMPLACE1(sX,sY,sourceWidth);
  sourceLimitPTR = source + MEMPLACE1((sX+tileWidth),(sY+tileHeight),sourceWidth);
  sourceLineSkip = (sourceWidth-tileWidth);
  sourceLineLimitPTR = sourcePTR + (tileWidth) -1 ;

  fprintf(stderr,"Getting Average Depth at area (%u,%u) of source image  starting at %u,%u  sized %u,%u \n",tileWidth,tileHeight,sX,sY,sourceWidth,sourceHeight);
  unsigned int curDepth = 0;
  unsigned int totalDepth = 0;
  unsigned int totalMeasurements = 0;

  while ( sourcePTR < sourceLimitPTR )
  {
     while (sourcePTR < sourceLineLimitPTR)
     {
        //fprintf(stderr,"Reading Triplet sourcePTR %p targetPTR is %p\n",sourcePTR  ,targetPTR);
        if (*sourcePTR!=0)
             {
               curDepth = (unsigned int) *sourcePTR;
               totalDepth += curDepth;
               ++totalMeasurements;
             }
        ++sourcePTR;
     }
    sourceLineLimitPTR += sourceWidth;
    sourcePTR+=sourceLineSkip;
  }
 fprintf(stderr,"Initial total is %u after %u measurments \n",totalDepth,totalMeasurements);

 if (totalMeasurements==0) { return 0; }
 return (unsigned int) (totalDepth / totalMeasurements);
}


