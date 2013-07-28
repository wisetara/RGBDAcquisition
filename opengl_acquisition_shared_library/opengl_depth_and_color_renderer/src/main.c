/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c shader_loader.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c shader_loader.c -lGL -lX11
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#include "glx.h"
#include "model_loader_obj.h"
#include "scene.h"

#include "shader_loader.h"
#include "AmMatrix/matrixCalculations.h"

#include "OGLRendererSandbox.h"

#define FLIP_OPEN_GL_IMAGES 1


int getOpenGLDepth(short * depth , unsigned int x,unsigned int y,unsigned int width,unsigned int height)
{
    double depth_bias=0.0; double depth_scale=1.0;
    glGetDoublev(GL_DEPTH_BIAS,  &depth_bias);  // Returns 0.0
    glGetDoublev(GL_DEPTH_SCALE, &depth_scale); // Returns 1.0

    float * zbuffer = (float *) malloc((width-x)*(height-y)*sizeof(float));
    glReadPixels(x, y, width, height, GL_DEPTH_COMPONENT, GL_FLOAT,zbuffer);

    float max_distance = farPlane-nearPlane;
    float multiplier = (float) 65535 / max_distance;

    memset(depth,0 , (width-x)*(height-y)*2 );


    #if FLIP_OPEN_GL_IMAGES
     unsigned int yp = 0;
     int i=0;
     unsigned int stride = (width-x)*1;

     for (yp=0; yp<height; yp++)
       {
         for ( i =0 ; i < (width-x); i ++ )
            {
               if (zbuffer[(height-1-yp)*stride+i]>=max_distance)  { depth[yp*stride+i]=  (short) 0;  } else
                                                                         {
                                                                           float tmpF  = (1.0f - zbuffer[(height-1-yp)*stride+i]) * multiplier;
                                                                           unsigned short tmp = (unsigned short) tmpF;
                                                                           depth[yp*stride+i]= tmp ;
                                                                         }

            }
       }
    #else
    int i=0;
    for ( i =0 ; i < (width-x)*(height-y); i ++ )
      {
        if (zbuffer[i]>=max_distance)  { depth[i]=  (short) 0; } else
                                       {
                                         float tmpF  = (1.0f - zbuffer[i]) * multiplier;
                                         unsigned short tmp = tmpF;
                                         depth[i]= tmp;
                                       }
      }
    #endif

    if (zbuffer!=0) { free(zbuffer); zbuffer=0; }
    return 1;
}



int getOpenGLColor(char * depth , unsigned int x,unsigned int y,unsigned int width,unsigned int height)
{
    #if FLIP_OPEN_GL_IMAGES
       char * inverter = (char *) malloc(3*(width-x)*(height-y)*sizeof(char));
       glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE,inverter);

      //SLOW INVERSION CODE :P
       unsigned int yp = 0;
       unsigned int stride = (width-x)*3;

       for (yp=0; yp<height; yp++)
       {
         char * where_to = &depth[yp*stride];
         char * where_from = &inverter[(height-1-yp)*stride];
         memcpy(where_to , where_from , stride * sizeof(char));
       }
      free(inverter);
    #else
       glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE,depth);
    #endif

   return 1;
}


void writeOpenGLColor(char * depthfile,unsigned int x,unsigned int y,unsigned int width,unsigned int height)
{

    char * zbuffer = (char *) malloc((width-x)*(height-y)*sizeof(char)*3);

    getOpenGLColor(zbuffer, x, y, width,  height);
    saveRawImageToFile(depthfile,zbuffer,(width-x),(height-y),3,8);

    if (zbuffer!=0) { free(zbuffer); zbuffer=0; }

    return ;
}



void writeOpenGLDepth(char * depthfile,unsigned int x,unsigned int y,unsigned int width,unsigned int height)
{
    short * zshortbuffer = (short *) malloc((width-x)*(height-y)*sizeof(short));

    getOpenGLDepth(zshortbuffer,x,y,width,height);

    saveRawImageToFile(depthfile,zshortbuffer,(width-x),(height-y),1,16);

    if (zshortbuffer!=0) { free(zshortbuffer); zshortbuffer=0; }

    return ;
}




void redraw(void)
{
    renderScene();
    glx_endRedraw();
}


int setOpenGLNearFarPlanes(double near , double far)
{
 farPlane=far;
 nearPlane=near;
 return 1;
}

int setOpenGLIntrinsicCalibration(double * camera)
{
  useIntrinsicMatrix=1;
  cameraMatrix[0]=camera[0];
  cameraMatrix[1]=camera[1];
  cameraMatrix[2]=camera[2];
  cameraMatrix[3]=camera[3];
  cameraMatrix[4]=camera[4];
  cameraMatrix[5]=camera[5];
  cameraMatrix[6]=camera[6];
  cameraMatrix[7]=camera[7];
  cameraMatrix[8]=camera[8];
  return 1;
}


int setOpenGLExtrinsicCalibration(double * rodriguez,double * translation)
{
  useCustomMatrix=1;
  convertRodriguezAndTranslationToOpenGL4x4DMatrix(customMatrix , rodriguez , translation );

  customTranslation[0] = translation[0];
  customTranslation[1] = translation[1];
  customTranslation[2] = translation[2];

  customRotation[0] = rodriguez[0];
  customRotation[1] = rodriguez[1];
  customRotation[2] = rodriguez[2];
  return 1;
}


double getOpenGLFocalLength()
{
 return nearPlane;
}

double getOpenGLPixelSize()
{
 return 2/WIDTH;
}


int startOGLRendererSandbox()
{

  testMatrices();

  char test[12]={0};
  char * testP = test;
  start_glx_stuff(WIDTH,HEIGHT,0,&testP);


  #if FLIP_OPEN_GL_IMAGES
    fprintf(stderr,"This version of OGLRendererSandbox is compiled to flip OpenGL frames to their correct orientation\n");
  #endif

  initScene();
  return 1;
}


int snapOGLRendererSandbox()
{
    if (glx_checkEvents())
    {
      tickScene();
      redraw();
      return 1;
    }
   return 0;
}

int stopOGLRendererSandbox()
{
  closeScene();
  return 1;
}
