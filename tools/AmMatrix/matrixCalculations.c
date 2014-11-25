#include "matrixCalculations.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrixTools.h"
#include "matrix3x3Tools.h"
#include "matrix4x4Tools.h"
#include "solveLinearSystemGJ.h"
#include "solveHomography.h"

enum mat3x3MItem
{
    m0 = 0 , m1  , m2  ,
    m3     , m4  , m5  ,
    m6     , m7 ,  m8
};


enum mat4x4EItem
{
    e0 = 0 , e1  , e2  , e3 ,
    e4     , e5  , e6  , e7 ,
    e8     , e9  , e10 , e11 ,
    e12    , e13 , e14 , e15
};


enum quatOrder
{
  qWqXqYqZ=0,
  qXqYqZqW
};


int convertRodriguezTo3x3(double * result,double * matrix)
{
  if ( (matrix==0) ||  (result==0) ) { return 0; }


  double x = matrix[0] , y = matrix[1] , z = matrix[2];
  double th = sqrt( x*x + y*y + z*z );
  double cosTh = cos(th);
  x = x / th; y = y / th; z = z / th;

  if ( th < 0.00001 )
    {
       create3x3IdentityMatrix(result);
       return 1;
    }

   //NORMAL RESULT
   result[0]=x*x * (1 - cosTh) + cosTh;          result[1]=x*y*(1 - cosTh) - z*sin(th);      result[2]=x*z*(1 - cosTh) + y*sin(th);
   result[3]=x*y*(1 - cosTh) + z*sin(th);        result[4]=y*y*(1 - cosTh) + cosTh;          result[5]=y*z*(1 - cosTh) - x*sin(th);
   result[6]=x*z*(1 - cosTh) - y*sin(th);        result[7]=y*z*(1 - cosTh) + x*sin(th);      result[8]=z*z*(1 - cosTh) + cosTh;

  #if PRINT_MATRIX_DEBUGGING
   fprintf(stderr,"rodriguez %f %f %f\n ",matrix[0],matrix[1],matrix[2]);
   print3x3DMatrix("Rodriguez Initial", result);
  #endif // PRINT_MATRIX_DEBUGGING

  return 1;
}


void changeYandZAxisOpenGL4x4Matrix(double * result,double * matrix)
{
  #if PRINT_MATRIX_DEBUGGING
   fprintf(stderr,"Invert Y and Z axis\n");
  #endif // PRINT_MATRIX_DEBUGGING

  double * invertOp = (double * ) malloc ( sizeof(double) * 16 );
  if (invertOp==0) { return; }

  create4x4IdentityMatrix(invertOp);
  invertOp[5]=-1;   invertOp[10]=-1;
  multiplyTwo4x4Matrices(result, matrix, invertOp);
  free(invertOp);
}


int normalizeQuaternions(double *qX,double *qY,double *qZ,double *qW)
{
#if USE_FAST_NORMALIZATION
      // Works best when quat is already almost-normalized
      double f = (double) (3.0 - (((*qX) * (*qX)) + ( (*qY) * (*qY) ) + ( (*qZ) * (*qZ)) + ((*qW) * (*qW)))) / 2.0;
      *qX *= f;
      *qY *= f;
      *qZ *= f;
      *qW *= f;
#else
      double sqrtDown = (double) sqrt(((*qX) * (*qX)) + ( (*qY) * (*qY) ) + ( (*qZ) * (*qZ)) + ((*qW) * (*qW)));
      double f = (double) 1 / sqrtDown;
       *qX *= f;
       *qY *= f;
       *qZ *= f;
       *qW *= f;
#endif // USE_FAST_NORMALIZATION
  return 1;
}


void quaternion2Matrix3x3(double * matrix3x3,double * quaternions,int quaternionConvention)
{
    //http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
    double qX,qY,qZ,qW;

    switch (quaternionConvention)
     {
       case qWqXqYqZ  :
        qW = quaternions[0]; qX = quaternions[1]; qY = quaternions[2]; qZ = quaternions[3];
       break;
       case qXqYqZqW :
        qX = quaternions[0]; qY = quaternions[1]; qZ = quaternions[2]; qW = quaternions[3];
       break;

       default :
       fprintf(stderr,"Unhandled quaternion order given (%u) \n",quaternionConvention);
       break;
     }


     double * m = matrix3x3;

     m[m0]=1 -(2*qY*qY) - (2*qZ*qZ); /*|*/  m[m1]=(2*qX*qY) - (2*qZ*qW);     /*|*/ m[m2]=(2*qX*qZ) + (2*qY*qW);
     m[m3]=(2*qX*qY) + (2*qZ*qW);    /*|*/  m[m4]=1 - (2*qX*qX) - (2*qZ*qZ); /*|*/ m[m5]=(2*qY*qZ) - (2*qX*qW);
     m[m6]=(2*qX*qZ) - (2*qY*qW);    /*|*/  m[m7]=(2*qY*qZ) + (2*qX*qW);     /*|*/ m[m8]=1 - (2*qX*qX) - (2*qY*qY);

 return ;
}


void  rotationXYZ_2_Matrix3x3(double * matrix3x3,double * rotationsXYZ)
{
 double heading = degreesToRadians(rotationsXYZ[0]);
 double attitude= degreesToRadians(rotationsXYZ[1]);
 double bank    = degreesToRadians(rotationsXYZ[2]);

 double ch=cos(heading);
 double ca=cos(attitude);
 double cb=cos(bank);
 double sh=sin(heading);
 double sa=sin(attitude);
 double sb=sin(bank);


  matrix3x3[0]=ch*ca;      /*|*/    matrix3x3[1]=((-1)*ch*sa*cb) + (sh*sb);       /*|*/    matrix3x3[2]=(ch*sa*sb) + (sh*cb);
  matrix3x3[3]=sa;         /*|*/    matrix3x3[4]=ca*cb;                           /*|*/    matrix3x3[5]=(-1)*ca*sb;
  matrix3x3[6]=(-1)*sh*ca; /*|*/    matrix3x3[7]=(sh*sa*cb) + (ch*sb);            /*|*/    matrix3x3[8]=((-1)*sh*sa*sb) + (ch*cb);
}



int projectPointsFrom3Dto2D(double * x2D, double * y2D , double * x3D, double *y3D , double * z3D , double * intrinsics , double * rotation3x3 , double * translation)
{
  double fx = intrinsics[0];
  double fy = intrinsics[4];
  double cx = intrinsics[2];
  double cy = intrinsics[5];

  double * t = translation;
  double * r = rotation3x3;

  //Result
  //fx * t0 + cx * t2 + (x3D) * ( fx * r0 + cx * r6 )  + (y3D) * ( fx * r1 + cx * r7 ) + (z3D) * (fx * r2 +cx * r8) / t3 + r7 x3D + r8 * y3D + r9 * z3D
  //fy * t1 + cy * t2 + x3D * ( fy * r3 + cy * r6 )  + y3D * ( fy * r4 + cy * r7 ) + z3D * (fy * r5 +cy * r8) / t3 + r7 x3D + r8 * y3D + r9 * z3D
  //1

  double x2DBuf =  fx * t[0] + cx * t[2] + (*x3D) * ( fx * r[0] + cx * r[6] )  + (*y3D) * ( fx * r[1] + cx * r[7] ) + (*z3D) * (fx * r[2] +cx * r[8]);
  double y2DBuf =  fy * t[1] + cy * t[2] + (*x3D) * ( fy * r[3] + cy * r[6] )  + (*y3D) * ( fy * r[4] + cy * r[7] ) + (*z3D) * (fy * r[5] +cy * r[8]);
  double scale =   t[2] + r[6] * (*x3D) + r[7] * (*y3D) + r[8] * (*z3D);

  if ( scale == 0.0 ) { fprintf(stderr,"could not projectPointsFrom3Dto2D"); return 0; }
  *x2D = x2DBuf / scale;
  *y2D = y2DBuf / scale;

 return 1;
}


int convertRodriguezAndTranslationTo4x4DUnprojectionMatrix(double * result4x4, double * rodriguez , double * translation , double scaleToDepthUnit)
{
  double * matrix3x3Rotation = alloc4x4Matrix();    if (matrix3x3Rotation==0) { return 0; }

  //Our translation vector is ready to be used!
  #if PRINT_MATRIX_DEBUGGING
  fprintf(stderr,"translation %f %f %f\n ",translation[0],translation[1],translation[2]);
  #endif // PRINT_MATRIX_DEBUGGING

  //Our rodriguez vector should be first converted to a 3x3 Rotation matrix
  convertRodriguezTo3x3((double*) matrix3x3Rotation , rodriguez);

  //Shorthand variables for readable code :P
  double * m  = result4x4;
  double * rm = matrix3x3Rotation;
  double * tm = translation;


  //double scaleToDepthUnit = 1000.0; //Convert Unit to milimeters
  double Tx = tm[0]*scaleToDepthUnit;
  double Ty = tm[1]*scaleToDepthUnit;
  double Tz = tm[2]*scaleToDepthUnit;


  /*
      Here what we want to do is generate a 4x4 matrix that does the inverse transformation that our
      rodriguez and translation vector define

      In order to do that we should have the following be true

                                      (note the minus under)
      (   R  |  T  )       (   R trans |  - R trans * T  )         (   I  |  0   )
      (  --------- )    .  (  -------------------------- )     =   ( ----------- )
      (   0  |  1  )       (   0       |        1        )         (   0  |  I   )

      Using matlab to do the calculations we get the following matrix
  */

   m[0]=  rm[0];        m[1]= rm[3];        m[2]=  rm[6];       m[3]= -1.0 * ( rm[0]*Tx + rm[3]*Ty + rm[6]*Tz );
   m[4]=  rm[1];        m[5]= rm[4];        m[6]=  rm[7];       m[7]= -1.0 * ( rm[1]*Tx + rm[4]*Ty + rm[7]*Tz );
   m[8]=  rm[2];        m[9]= rm[5];        m[10]= rm[8];       m[11]=-1.0 * ( rm[2]*Tx + rm[5]*Ty + rm[8]*Tz );
   m[12]= 0.0;          m[13]= 0.0;         m[14]=0.0;          m[15]=1.0;


  print4x4DMatrix("ModelView", result4x4);
  free4x4Matrix(&matrix3x3Rotation);
  return 1;
}


int convertRodriguezAndTranslationToOpenGL4x4DProjectionMatrix(double * result4x4, double * rodriguez , double * translation , double scaleToDepthUnit )
{
  double * matrix3x3Rotation = alloc4x4Matrix();    if (matrix3x3Rotation==0) { return 0; }

  //Our translation vector is ready to be used!
  #if PRINT_MATRIX_DEBUGGING
  fprintf(stderr,"translation %f %f %f\n ",translation[0],translation[1],translation[2]);
  #endif // PRINT_MATRIX_DEBUGGING


  //Our rodriguez vector should be first converted to a 3x3 Rotation matrix
  convertRodriguezTo3x3((double*) matrix3x3Rotation , rodriguez);

  //Shorthand variables for readable code :P
  double * m  = result4x4;
  double * rm = matrix3x3Rotation;
  double * tm = translation;


  //double scaleToDepthUnit = 1000.0; //Convert Unit to milimeters
  double Tx = tm[0]*scaleToDepthUnit;
  double Ty = tm[1]*scaleToDepthUnit;
  double Tz = tm[2]*scaleToDepthUnit;

  /*
      Here what we want to do is generate a 4x4 matrix that does the normal transformation that our
      rodriguez and translation vector define
  */
   m[0]=  rm[0];        m[1]= rm[1];        m[2]=  rm[2];       m[3]= -Tx;
   m[4]=  rm[3];        m[5]= rm[4];        m[6]=  rm[5];       m[7]= -Ty;
   m[8]=  rm[6];        m[9]= rm[7];        m[10]= rm[8];       m[11]=-Tz;
   m[12]= 0.0;          m[13]= 0.0;         m[14]=0.0;          m[15]=1.0;


  #if PRINT_MATRIX_DEBUGGING
   print4x4DMatrix("ModelView", result4x4);
   fprintf(stderr,"Matrix will be transposed to become OpenGL format ( i.e. column major )\n");
  #endif // PRINT_MATRIX_DEBUGGING

  transpose4x4MatrixD(result4x4);

  free4x4Matrix(&matrix3x3Rotation);
  return 1;
}


int move3DPoint(double * resultPoint3D, double * transformation4x4, double * point3D  )
{
  return transform3DPointVectorUsing4x4Matrix(resultPoint3D,transformation4x4,point3D);
}



void buildOpenGLProjectionForIntrinsics   (
                                             double * frustum,
                                             int * viewport ,
                                             double fx,
                                             double fy,
                                             double skew,
                                             double cx, double cy,
                                             unsigned int imageWidth, unsigned int imageHeight,
                                             double nearPlane,
                                             double farPlane
                                           )
{
   fprintf(stderr,"buildOpenGLProjectionForIntrinsics according to old Ammar code Image ( %u x %u )\n",imageWidth,imageHeight);
   fprintf(stderr,"fx %0.2f fy %0.2f , cx %0.2f , cy %0.2f , skew %0.2f \n",fx,fy,cx,cy,skew);
   fprintf(stderr,"Near %0.2f Far %0.2f \n",nearPlane,farPlane);


    // These parameters define the final viewport that is rendered into by
    // the camera.
    //     Left    Bottom   Right       Top
    double L = 0.0 , B = 0.0  , R = imageWidth , T = imageHeight;

    // near and far clipping planes, these only matter for the mapping from
    // world-space z-coordinate into the depth coordinate for OpenGL
    double N = nearPlane , F = farPlane;
    double R_sub_L = R-L , T_sub_B = T-B , F_sub_N = F-N , F_plus_N = F+N , F_mul_N = F*N;

    if  ( (R_sub_L==0) || (R_sub_L-1.0f==0) ||
          (T_sub_B==0) || (T_sub_B-1.0f==0) ||
          (F_sub_N==0) ) { fprintf(stderr,"Problem with image limigs R-L=%f , T-B=%f , F-N=%f\n",R_sub_L,T_sub_B,F_sub_N); }


   // set the viewport parameters
   viewport[0] = L; viewport[1] = B; viewport[2] = R_sub_L; viewport[3] = T_sub_B;

   //OpenGL Projection Matrix ready for loading ( column-major ) , also axis compensated
   frustum[0] = -2.0f*fx/R_sub_L;     frustum[1] = 0.0f;                 frustum[2] = 0.0f;                              frustum[3] = 0.0f;
   frustum[4] = 0.0f;                 frustum[5] = 2.0f*fy/T_sub_B;      frustum[6] = 0.0f;                              frustum[7] = 0.0f;
   frustum[8] = 2.0f*cx/R_sub_L-1.0f; frustum[9] = 2.0f*cy/T_sub_B-1.0f; frustum[10]=-1.0*(F_plus_N/F_sub_N);            frustum[11] = -1.0f;
   frustum[12]= 0.0f;                 frustum[13]= 0.0f;                 frustum[14]=-2.0f*F_mul_N/(F_sub_N);            frustum[15] = 0.0f;
   //Matrix already in OpenGL column major format
}























int pointFromRelationWithObjectToAbsolute(double * absoluteOutPoint3DRotated, double * objectPosition , double * objectRotation3x3 ,  double * relativeInPoint3DUnrotated)
{
  //  What we want to do ( in mathematica )
  // (  { {r0,r1,r2,0} , {r3,r4,r5,0} , {r6,r7,r8,0} , {0,0,0,1} } * { { X }  , { Y }  , { Z } , { 1.0 } } ) + { {ObjX} , {ObjY} , {ObjZ} , { 0 }  }

  //We have a coordinate space in Relation to our object so we want to first rotate our point and then translate it
  //back to absolute coordinate space

  double objectRotation4x4[4*4]={0};
  //We make the 3x3 matrix onto a 4x4 by adding zeros and 1 as the diagonal element
  upscale3x3to4x4(objectRotation4x4,objectRotation3x3);

  double relativePoint3DRotated[4]={0};
  transform3DPointVectorUsing4x4Matrix(relativePoint3DRotated,objectRotation4x4,relativeInPoint3DUnrotated);

  //Normalization is done automatically
  //normalize3DPointVector(relativePoint3DRotated);

  absoluteOutPoint3DRotated[0]=relativePoint3DRotated[0]+objectPosition[0];
  absoluteOutPoint3DRotated[1]=relativePoint3DRotated[1]+objectPosition[1];
  absoluteOutPoint3DRotated[2]=relativePoint3DRotated[2]+objectPosition[2];
  absoluteOutPoint3DRotated[3]=1.0;  //We know we are talking about 2 normalized 3d points

  return 1;
}

/*
    We have an object with an absolute Position X,Y,Z (objectPosition[]) and Rotation (objectRotation3x3[])
    We also have an absolute position of a 3D point , and we want to calculate the relative position
    of the 3D point in relation to the object ( unrotated relative position )
*/
int pointFromAbsoluteToInRelationWithObject(double * relativeOutPoint3DUnrotated, double * objectPosition , double * objectRotation3x3 , double * absoluteInPoint3DRotated )
{
  //  What we want to do ( in mathematica )
  // { {r0,r3,r6,0} , {r1,r4,r7,0} , {r2,r5,r8,0} , {0,0,0,1} } * { { X-ObjX } , { Y-ObjY }  , { Z-ObjZ } , { 1.0 } }

  //printf("pointFromAbsoluteToInRelationWithObject Using Simple Code\n");
  double relativeInPoint3DRotated[4]={0};

  relativeInPoint3DRotated[0]=absoluteInPoint3DRotated[0]-objectPosition[0];
  relativeInPoint3DRotated[1]=absoluteInPoint3DRotated[1]-objectPosition[1];
  relativeInPoint3DRotated[2]=absoluteInPoint3DRotated[2]-objectPosition[2];
  relativeInPoint3DRotated[3]=1.0;  //We know we are talking about 2 3d points

  double objectTransposedRotation3x3[3*3]={0};
  //We transpose our 3x3 rotation matrix because we want the inverse transformation
  transpose3x3MatrixDFromSource(objectTransposedRotation3x3,objectRotation3x3);

  double objectTransposedRotation4x4[4*4]={0};
  //We make the 3x3 matrix onto a 4x4 by adding zeros and 1 as the diagonal element
  upscale3x3to4x4(objectTransposedRotation4x4,objectTransposedRotation3x3);


  transform3DPointVectorUsing4x4Matrix(relativeOutPoint3DUnrotated,objectTransposedRotation4x4,relativeInPoint3DRotated);
  return 1;
}



/*
    We have an object with an absolute Position X,Y,Z (objectPosition[]) and Rotation (objectRotation3x3[])
    We also have an absolute position of a 3D point , and we want to calculate the relative position
    of the 3D point in relation to the object ( unrotated relative position )
*/
int pointFromAbsoluteToInRelationWithObject_UsingInversion(double * relativeOutPoint3DUnrotated, double * objectPosition , double * objectRotation3x3 , double * absoluteInPoint3DRotated )
{
  //printf("pointFromAbsoluteToInRelationWithObject Using Inversion Code\n");
  double objectRotation4x4[4*4]={0};
  //We make the 3x3 matrix onto a 4x4 by adding zeros and 1 as the diagonal element
  upscale3x3to4x4(objectRotation4x4,objectRotation3x3);

  objectRotation4x4[e3]=objectPosition[0];
  objectRotation4x4[e7]=objectPosition[1];
  objectRotation4x4[e11]=objectPosition[2];
  objectRotation4x4[e15]=1.0;


  double objectInvRotation4x4[4*4]={0};
  invert4x4MatrixD(objectInvRotation4x4,objectRotation4x4);

  transform3DPointVectorUsing4x4Matrix(relativeOutPoint3DUnrotated,objectInvRotation4x4,absoluteInPoint3DRotated);
  return 1;
}







/*
    We have an object with an absolute Position X,Y,Z (objectPosition[]) and Rotation (objectRotation3x3[])
    We also have an absolute position of a 3D point , and we want to calculate the relative position
    of the 3D point in relation to the object ( unrotated relative position )
*/
int pointFromAbsoluteToRelationWithObject_PosXYZRotationXYZ(unsigned int method, double * relativeOutPoint3DUnrotated, double * objectPosition , double * objectRotation , double * absoluteInPoint3DRotated )
{
    double objectRotation3x3[9];

    rotationXYZ_2_Matrix3x3(objectRotation3x3,objectRotation);

    //print3x3DMatrix("Quaternion to 3x3",objectRotation3x3);
    //print3x3DMathematicaMatrix("Quat3x3",objectRotation3x3);

    if (method==0) { pointFromAbsoluteToInRelationWithObject(relativeOutPoint3DUnrotated,objectPosition,objectRotation3x3,absoluteInPoint3DRotated); } else
                   { pointFromAbsoluteToInRelationWithObject_UsingInversion(relativeOutPoint3DUnrotated,objectPosition,objectRotation3x3,absoluteInPoint3DRotated); }

    //We have to try to normalize the output point , although it should already be normalized..
    normalize3DPointVector(relativeOutPoint3DUnrotated);

    return 1;
}




/*
    We have an object with an absolute Position X,Y,Z (objectPosition[]) and Rotation (objectRotation3x3[])
    We also have an absolute position of a 3D point , and we want to calculate the relative position
    of the 3D point in relation to the object ( unrotated relative position )
*/
int pointFromAbsoluteToRelationWithObject_PosXYZQuaternionXYZW(unsigned int method, double * relativeOutPoint3DUnrotated, double * objectPosition , double * objectQuaternion , double * absoluteInPoint3DRotated )
{
    double objectRotation3x3[9];

    //printf("Object Position is %f,%f,%f  \n", objectPosition[0], objectPosition[1], objectPosition[2] );
    //printf("Quaternion %f,%f,%f,%f \n",objectQuaternion[0],objectQuaternion[1],objectQuaternion[2],objectQuaternion[3]);
    normalizeQuaternions(&objectQuaternion[0],&objectQuaternion[1],&objectQuaternion[2],&objectQuaternion[3]);
    //printf("Normalized Quaternion %f,%f,%f,%f \n",objectQuaternion[0],objectQuaternion[1],objectQuaternion[2],objectQuaternion[3]);

    quaternion2Matrix3x3(objectRotation3x3,objectQuaternion,qXqYqZqW);

    //print3x3DMatrix("Quaternion to 3x3",objectRotation3x3);
    //print3x3DMathematicaMatrix("Quat3x3",objectRotation3x3);

    if (method==0) { pointFromAbsoluteToInRelationWithObject(relativeOutPoint3DUnrotated,objectPosition,objectRotation3x3,absoluteInPoint3DRotated); } else
                   { pointFromAbsoluteToInRelationWithObject_UsingInversion(relativeOutPoint3DUnrotated,objectPosition,objectRotation3x3,absoluteInPoint3DRotated); }

    //We have to try to normalize the output point , although it should already be normalized..
    normalize3DPointVector(relativeOutPoint3DUnrotated);

    return 1;
}


/*
    We have an object with a relative Position X,Y,Z to an Object (objectPosition[])
*/
int pointFromRelationWithObjectToAbsolute_PosXYZRotationXYZ(double * absoluteOutPoint3DRotated , double * objectPosition , double * objectRotation ,double * relativeInPoint3DUnrotated)
{
    double objectRotation3x3[9];

    rotationXYZ_2_Matrix3x3(objectRotation3x3,objectRotation);

    //print3x3DMatrix("Quaternion to 3x3",objectRotation3x3);
    //print3x3DMathematicaMatrix("Quat3x3",objectRotation3x3);

    pointFromRelationWithObjectToAbsolute(absoluteOutPoint3DRotated,objectPosition,objectRotation3x3,relativeInPoint3DUnrotated);

    //We have to try to normalize the output point , although it should already be normalized..
    normalize3DPointVector(absoluteOutPoint3DRotated);

    return 1;
}



/*
    We have an object with a relative Position X,Y,Z to an Object (objectPosition[])
*/
int pointFromRelationWithObjectToAbsolute_PosXYZQuaternionXYZW(double * absoluteOutPoint3DRotated , double * objectPosition , double * objectQuaternion ,double * relativeInPoint3DUnrotated)
{
    double objectRotation3x3[9];

    //printf("Object Position is %f,%f,%f  \n", objectPosition[0], objectPosition[1], objectPosition[2] );
    //printf("Quaternion %f,%f,%f,%f \n",objectQuaternion[0],objectQuaternion[1],objectQuaternion[2],objectQuaternion[3]);
    normalizeQuaternions(&objectQuaternion[0],&objectQuaternion[1],&objectQuaternion[2],&objectQuaternion[3]);
    //printf("Normalized Quaternion %f,%f,%f,%f \n",objectQuaternion[0],objectQuaternion[1],objectQuaternion[2],objectQuaternion[3]);

    quaternion2Matrix3x3(objectRotation3x3,objectQuaternion,qXqYqZqW);

    //print3x3DMatrix("Quaternion to 3x3",objectRotation3x3);
    //print3x3DMathematicaMatrix("Quat3x3",objectRotation3x3);

    pointFromRelationWithObjectToAbsolute(absoluteOutPoint3DRotated,objectPosition,objectRotation3x3,relativeInPoint3DUnrotated);

    //We have to try to normalize the output point , although it should already be normalized..
    normalize3DPointVector(absoluteOutPoint3DRotated);

    return 1;
}





void testMatrices()
{
   //testHomographySolver();
   testGJSolver();
  return ;


  double A[16]={ 1 ,2 ,3 ,4,
                 5 ,6 ,7 ,8,
                 9 ,10,11,12,
                 13,14,15,16
                };


  double B[16]={ 1 ,2 ,3 ,4,
                 4 ,3 ,2 ,1,
                 1 ,2 ,3 ,4,
                 4 ,3 ,2 ,1
                };

  double Res[16]={0};

  multiplyTwo4x4Matrices(Res,A,B);
/*
  28.000000 26.000000 24.000000 22.000000
  68.000000 66.000000 64.000000 62.000000
  108.000000 106.000000 104.000000 102.000000
  148.000000 146.000000 144.000000 142.000000*/

}
