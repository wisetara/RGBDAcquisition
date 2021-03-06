
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix4x4Tools.h"

#define PRINT_MATRIX_DEBUGGING 0

enum mat4x4Item
{
    I11 = 0 , I12 , I13 , I14 ,
    I21     , I22 , I23 , I24 ,
    I31     , I32 , I33 , I34 ,
    I41     , I42 , I43 , I44
};



enum mat4x4RTItem
{
    r0 = 0 , r1, r2 , t0 ,
    r3     , r4, r5 , t1 ,
    r6     , r7 ,r8 , t2 ,
    zero0  , zero1 , zero2 , one0
};





enum mat4x4EItem
{
    e0 = 0 , e1  , e2  , e3 ,
    e4     , e5  , e6  , e7 ,
    e8     , e9  , e10 , e11 ,
    e12    , e13 , e14 , e15
};

/* OUR MATRICES STORAGE
    0   1   2   3
    4   5   6   7
    8   9   10  11
    12  13  14  15
*/


double * alloc4x4Matrix()
{
  return (double*) malloc ( sizeof(double) * 16 );
}

void free4x4Matrix(double ** mat)
{
  if (mat==0) { return ; }
  if (*mat==0) { return ; }
  free(*mat);
  *mat=0;
}

void print4x4FMatrix(const char * str , float * matrix4x4,int forcePrint)
{
  #if PRINT_MATRIX_DEBUGGING
   forcePrint=1;
  #endif // PRINT_MATRIX_DEBUGGING

  if (forcePrint)
  {
  fprintf( stderr, " 4x4 float %s \n",str);
  fprintf( stderr, "--------------------------------------\n");
  fprintf(stderr,"%0.4f,%0.4f,%0.4f,%0.4f,\n",matrix4x4[0],matrix4x4[1],matrix4x4[2],matrix4x4[3]);
  fprintf(stderr,"%0.4f,%0.4f,%0.4f,%0.4f,\n",matrix4x4[4],matrix4x4[5],matrix4x4[6],matrix4x4[7]);
  fprintf(stderr,"%0.4f,%0.4f,%0.4f,%0.4f,\n",matrix4x4[8],matrix4x4[9],matrix4x4[10],matrix4x4[11]);
  fprintf(stderr,"%0.4f,%0.4f,%0.4f,%0.4f\n",matrix4x4[12],matrix4x4[13],matrix4x4[14],matrix4x4[15]);
  fprintf( stderr, "--------------------------------------\n");
  }
}

void print4x4DMatrix(const char * str , double * matrix4x4,int forcePrint)
{
  #if PRINT_MATRIX_DEBUGGING
   forcePrint=1;
  #endif // PRINT_MATRIX_DEBUGGING

  if (forcePrint)
  {
  fprintf( stderr, " 4x4 double %s \n",str);
  fprintf( stderr, "--------------------------------------\n");
  fprintf(stderr,"%0.4f,%0.4f,%0.4f,%0.4f,\n",matrix4x4[0],matrix4x4[1],matrix4x4[2],matrix4x4[3]);
  fprintf(stderr,"%0.4f,%0.4f,%0.4f,%0.4f,\n",matrix4x4[4],matrix4x4[5],matrix4x4[6],matrix4x4[7]);
  fprintf(stderr,"%0.4f,%0.4f,%0.4f,%0.4f,\n",matrix4x4[8],matrix4x4[9],matrix4x4[10],matrix4x4[11]);
  fprintf(stderr,"%0.4f,%0.4f,%0.4f,%0.4f\n",matrix4x4[12],matrix4x4[13],matrix4x4[14],matrix4x4[15]);
  fprintf( stderr, "--------------------------------------\n");
  }
}


void print4x4DMathematicaMatrix(const char * str , double * matrix3x3)
{
  #if PRINT_MATRIX_DEBUGGING
  fprintf( stderr, "%s = { { %f , %f , %f ,%f } , { %f , %f , %f , %f } , { %f , %f , %f , %f } , { %f , %f , %f , %f } }\n",str,
           matrix3x3[0],matrix3x3[1],matrix3x3[2],matrix3x3[3],
           matrix3x3[4],matrix3x3[5],matrix3x3[6],matrix3x3[7],
           matrix3x3[8],matrix3x3[9],matrix3x3[10],matrix3x3[11],
           matrix3x3[12],matrix3x3[13],matrix3x3[14],matrix3x3[15]);
  #endif // PRINT_MATRIX_DEBUGGING
}

void copy4x4FMatrix(float * out,float * in)
{
  out[0]=in[0];   out[1]=in[1];   out[2]=in[2];   out[3]=in[3];
  out[4]=in[4];   out[5]=in[5];   out[6]=in[6];   out[7]=in[7];
  out[8]=in[8];   out[9]=in[9];   out[10]=in[10]; out[11]=in[11];
  out[12]=in[12]; out[13]=in[13]; out[14]=in[14]; out[15]=in[15];
}

void copy4x4DMatrix(double * out,double * in)
{
  out[0]=in[0];   out[1]=in[1];   out[2]=in[2];   out[3]=in[3];
  out[4]=in[4];   out[5]=in[5];   out[6]=in[6];   out[7]=in[7];
  out[8]=in[8];   out[9]=in[9];   out[10]=in[10]; out[11]=in[11];
  out[12]=in[12]; out[13]=in[13]; out[14]=in[14]; out[15]=in[15];
}


void copy4x4FMatrixToD(double * out,float * in)
{
  out[0]=in[0];   out[1]=in[1];   out[2]=in[2];   out[3]=in[3];
  out[4]=in[4];   out[5]=in[5];   out[6]=in[6];   out[7]=in[7];
  out[8]=in[8];   out[9]=in[9];   out[10]=in[10]; out[11]=in[11];
  out[12]=in[12]; out[13]=in[13]; out[14]=in[14]; out[15]=in[15];
}



void copy4x4DMatrixToF(float * dest, double * m )
{
    dest[0]=(float)m[0];   dest[1]=(float)m[1];   dest[2]=(float)m[2];    dest[3]=(float)m[3];
    dest[4]=(float)m[4];   dest[5]=(float)m[5];   dest[6]=(float)m[6];    dest[7]=(float)m[7];
    dest[8]=(float)m[8];   dest[9]=(float)m[9];   dest[10]=(float)m[10];  dest[11]=(float)m[11];
    dest[12]=(float)m[12]; dest[13]=(float)m[13]; dest[14]=(float)m[14];  dest[15]=(float)m[15];
}


void create4x4IdentityMatrix(double * m)
{
    m[0] = 1.0;  m[1] = 0.0;  m[2] = 0.0;   m[3] = 0.0;
    m[4] = 0.0;  m[5] = 1.0;  m[6] = 0.0;   m[7] = 0.0;
    m[8] = 0.0;  m[9] = 0.0;  m[10] = 1.0;  m[11] =0.0;
    m[12]= 0.0;  m[13]= 0.0;  m[14] = 0.0;  m[15] = 1.0;
}


void create4x4IdentityMatrixF(float * m)
{
    m[0] = 1.0;  m[1] = 0.0;  m[2] = 0.0;   m[3] = 0.0;
    m[4] = 0.0;  m[5] = 1.0;  m[6] = 0.0;   m[7] = 0.0;
    m[8] = 0.0;  m[9] = 0.0;  m[10] = 1.0;  m[11] =0.0;
    m[12]= 0.0;  m[13]= 0.0;  m[14] = 0.0;  m[15] = 1.0;
}

int doublePEq(double * element , double value )
{
 const double machineFloatPercision= 0.0001;
 if ( *element == value ) { return 1; }

 if ( ( value-machineFloatPercision<*element ) && (( *element<value+machineFloatPercision )) ) { return 1; }
 return 0;
}

int is4x4DIdentityMatrix(double * m)
{
   return (
    (doublePEq(&m[0],1.0)) &&(doublePEq(&m[1],0.0)) &&(doublePEq(&m[2],0.0)) &&(doublePEq(&m[3],0.0)) &&
    (doublePEq(&m[4],0.0)) &&(doublePEq(&m[5],1.0)) &&(doublePEq(&m[6],0.0)) &&(doublePEq(&m[7],0.0)) &&
    (doublePEq(&m[8],0.0)) &&(doublePEq(&m[9],0.0)) &&(doublePEq(&m[10],1.0))&&(doublePEq(&m[11],0.0))&&
    (doublePEq(&m[12],0.0))&&(doublePEq(&m[13],0.0))&&(doublePEq(&m[14],0.0))&&(doublePEq(&m[15],1.0))
           );
}


int is4x4DZeroMatrix(double  * m)
{
   return (
    (doublePEq(&m[0],0.0)) &&(doublePEq(&m[1],0.0)) &&(doublePEq(&m[2],0.0)) &&(doublePEq(&m[3],0.0)) &&
    (doublePEq(&m[4],0.0)) &&(doublePEq(&m[5],0.0)) &&(doublePEq(&m[6],0.0)) &&(doublePEq(&m[7],0.0)) &&
    (doublePEq(&m[8],0.0)) &&(doublePEq(&m[9],0.0)) &&(doublePEq(&m[10],0.0))&&(doublePEq(&m[11],0.0))&&
    (doublePEq(&m[12],0.0))&&(doublePEq(&m[13],0.0))&&(doublePEq(&m[14],0.0))&&(doublePEq(&m[15],0.0))
           );
}


int floatPEq(float * element , float value )
{
 const float machineFloatPercision= 0.0001;
 if ( *element == value ) { return 1; }

 if ( ( value-machineFloatPercision<*element ) && (( *element<value+machineFloatPercision )) ) { return 1; }
 return 0;
}


int is4x4FIdentityMatrix(float  * m)
{
   return (
    (floatPEq(&m[0],1.0)) &&(floatPEq(&m[1],0.0)) &&(floatPEq(&m[2],0.0)) &&(floatPEq(&m[3],0.0)) &&
    (floatPEq(&m[4],0.0)) &&(floatPEq(&m[5],1.0)) &&(floatPEq(&m[6],0.0)) &&(floatPEq(&m[7],0.0)) &&
    (floatPEq(&m[8],0.0)) &&(floatPEq(&m[9],0.0)) &&(floatPEq(&m[10],1.0))&&(floatPEq(&m[11],0.0))&&
    (floatPEq(&m[12],0.0))&&(floatPEq(&m[13],0.0))&&(floatPEq(&m[14],0.0))&&(floatPEq(&m[15],1.0))
           );
}


int is4x4FIdentityMatrixPercisionCompensating(float  * m)
{
   return (
    (floatPEq(&m[0],1.0)) &&(floatPEq(&m[1],0.0)) &&(floatPEq(&m[2],0.0)) &&(floatPEq(&m[3],0.0)) &&
    (floatPEq(&m[4],0.0)) &&(floatPEq(&m[5],1.0)) &&(floatPEq(&m[6],0.0)) &&(floatPEq(&m[7],0.0)) &&
    (floatPEq(&m[8],0.0)) &&(floatPEq(&m[9],0.0)) &&(floatPEq(&m[10],1.0))&&(floatPEq(&m[11],0.0))&&
    (floatPEq(&m[12],0.0))&&(floatPEq(&m[13],0.0))&&(floatPEq(&m[14],0.0))&&(floatPEq(&m[15],1.0))
           );
}


void convert4x4MatrixToRPY(double *m ,double *roll,double *pitch,double *yaw)
{
 if (m[0] == 1.0f)
        {
          *yaw = atan2f( m[2], m[11]);
          *pitch = 0;
          *roll = 0;
        }
        else
 if (m[0] == -1.0f)
        {
          *yaw = atan2f(m[2], m[11]);
          *pitch = 0;
          *roll = 0;
        }
        else
        {
          *yaw = atan2(-m[8],m[0]);
          *pitch = asin(m[4]);
          *roll = atan2(-m[6],m[5]);
        }

}


void create4x4RotationMatrix(double * m , double angle, double x, double y, double z)
{
    double const DEG2RAD=(double) M_PI/180;
    double c = cosf(angle * DEG2RAD);
    double s = sinf(angle * DEG2RAD);
    double xx = x * x;
    double xy = x * y;
    double xz = x * z;
    double yy = y * y;
    double yz = y * z;
    double zz = z * z;
    double one_min_c = (1 - c);
    double x_mul_s = x * s;
    double y_mul_s = y * s;
    double z_mul_s = z * s;

    m[0] = xx * one_min_c + c;
    m[1] = xy * one_min_c - z_mul_s;
    m[2] = xz * one_min_c + y_mul_s;
    m[3] = 0;
    m[4] = xy * one_min_c + z_mul_s;
    m[5] = yy * one_min_c + c;
    m[6] = yz * one_min_c - x_mul_s;
    m[7] = 0;
    m[8] = xz * one_min_c - y_mul_s;
    m[9] = yz * one_min_c + x_mul_s;
    m[10]= zz * one_min_c + c;
    m[11]= 0;
    m[12]= 0;
    m[13]= 0;
    m[14]= 0;
    m[15]= 1;
}



static double degrees_to_rad(double degrees)
{
    return degrees * (M_PI /180.0 );
}


void create4x4MatrixFromEulerAnglesXYZ(double * m ,double eulX, double eulY, double eulZ)
{
    double x = degrees_to_rad(eulX);
    double y = degrees_to_rad(eulY);
    double z = degrees_to_rad(eulZ);

	double cr = cos( x );
	double sr = sin( x );
	double cp = cos( y );
	double sp = sin( y );
	double cy = cos( z );
	double sy = sin( z );

	m[0] = cp*cy ;
	m[1] = cp*sy;
	m[2] = -sp ;
	m[3] = 0; // 4x4


	double srsp = sr*sp;
	double crsp = cr*sp;

	m[4] = srsp*cy-cr*sy ;
	m[5] = srsp*sy+cr*cy ;
	m[6] = sr*cp ;
	m[7] = 0; // 4x4

	m[8] =  crsp*cy+sr*sy ;
	m[9] =  crsp*sy-sr*cy ;
	m[10]= cr*cp ;
    m[11]= 0; // 4x4

     // 4x4 last row
    m[12]= 0;
    m[13]= 0;
    m[14]= 0;
    m[15]= 1.0;
}



void create4x4MatrixFromEulerAnglesZYX(double * m ,double eulX, double eulY, double eulZ)
{
    //roll = X , pitch = Y , yaw = Z
    double x = degrees_to_rad(eulX);
    double y = degrees_to_rad(eulY);
    double z = degrees_to_rad(eulZ);


	double cr = cos(z);
	double sr = sin(z);
	double cp = cos(y);
	double sp = sin(y);
	double cy = cos(x);
	double sy = sin(x);

	double srsp = sr*sp;
	double crsp = cr*sp;

	m[0] = cr*cp;
	m[1] = crsp*sy - sr*cy;
	m[2] = crsp*cy + sr*sy;
	m[3] = 0;  // 4x4

	m[4] = sr*cp;
	m[5] = srsp*sy + cr*cy;
	m[6] = srsp*cy - cr*sy;
	m[7] = 0;  // 4x4

	m[8] = -sp;
	m[9] = cp*sy;
	m[10] = cp*cy;
	m[11] = 0;  // 4x4

     // 4x4 last row
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
}


void create4x4QuaternionMatrix(double * m , double qX,double qY,double qZ,double qW)
{
    double yy2 = 2.0f * qY * qY;
    double xy2 = 2.0f * qX * qY;
    double xz2 = 2.0f * qX * qZ;
    double yz2 = 2.0f * qY * qZ;
    double zz2 = 2.0f * qZ * qZ;
    double wz2 = 2.0f * qW * qZ;
    double wy2 = 2.0f * qW * qY;
    double wx2 = 2.0f * qW * qX;
    double xx2 = 2.0f * qX * qX;
    m[0]  = - yy2 - zz2 + 1.0f;
    m[1]  = xy2 + wz2;
    m[2]  = xz2 - wy2;
    m[3]  = 0;
    m[4]  = xy2 - wz2;
    m[5]  = - xx2 - zz2 + 1.0f;
    m[6]  = yz2 + wx2;
    m[7]  = 0;
    m[8]  = xz2 + wy2;
    m[9]  = yz2 - wx2;
    m[10] = - xx2 - yy2 + 1.0f;
    m[11] = 0.0f;
    m[12] = 0.0;
    m[13] = 0.0;
    m[14] = 0.0;
    m[15] = 1.0f;
}


void create4x4FTranslationMatrix(float * matrix , float x, float y, float z)
{
    create4x4IdentityMatrixF(matrix);
    // Translate slots.
    matrix[3] = x; matrix[7] = y; matrix[11] = z;
}



void create4x4TranslationMatrix(double * matrix , double x, double y, double z)
{
    create4x4IdentityMatrix(matrix);
    // Translate slots.
    matrix[3] = x; matrix[7] = y; matrix[11] = z;
}

void create4x4ScalingMatrix(double * matrix , double scaleX, double scaleY, double scaleZ)
{
    create4x4IdentityMatrix(matrix);
    // Scale slots.
    matrix[0] = scaleX; matrix[5] = scaleY; matrix[10] = scaleZ;
}


void create4x4RotationX(double * matrix,double degrees)
{
    double radians = degrees_to_rad(degrees);

    create4x4IdentityMatrix(matrix);

    // Rotate X formula.
    matrix[5] = cosf(radians);
    matrix[6] = -sinf(radians);
    matrix[9] = -matrix[6];
    matrix[10] = matrix[5];
}

void create4x4RotationY(double * matrix,double degrees)
{
    double radians = degrees_to_rad(degrees);

    create4x4IdentityMatrix(matrix);

    // Rotate Y formula.
    matrix[0] = cosf(radians);
    matrix[2] = sinf(radians);
    matrix[8] = -matrix[2];
    matrix[10] = matrix[0];
}

void create4x4RotationZ(double * matrix,double degrees)
{
    double radians = degrees_to_rad(degrees);

    create4x4IdentityMatrix(matrix);

    // Rotate Z formula.
    matrix[0] = cosf(radians);
    matrix[1] = sinf(radians);
    matrix[4] = -matrix[1];
    matrix[5] = matrix[0];
}

double det4x4Matrix(double * mat)
{
 double * a = mat;

 double  detA  = a[I11] * a[I22] * a[I33]  * a[I44];
         detA += a[I11] * a[I23] * a[I34]  * a[I42];
         detA += a[I11] * a[I24] * a[I32]  * a[I43];

         detA += a[I12] * a[I21] * a[I34]  * a[I43];
         detA += a[I12] * a[I23] * a[I31]  * a[I44];
         detA += a[I12] * a[I24] * a[I33]  * a[I41];

         detA += a[I13] * a[I21] * a[I32]  * a[I44];
         detA += a[I13] * a[I22] * a[I34]  * a[I41];
         detA += a[I13] * a[I24] * a[I31]  * a[I42];

         detA += a[I14] * a[I21] * a[I33]  * a[I42];
         detA += a[I14] * a[I22] * a[I31]  * a[I43];
         detA += a[I14] * a[I23] * a[I32]  * a[I41];

  //FIRST PART DONE
         detA -= a[I11] * a[I22] * a[I34]  * a[I43];
         detA -= a[I11] * a[I23] * a[I32]  * a[I44];
         detA -= a[I11] * a[I24] * a[I33]  * a[I42];

         detA -= a[I12] * a[I21] * a[I33]  * a[I44];
         detA -= a[I12] * a[I23] * a[I34]  * a[I41];
         detA -= a[I12] * a[I24] * a[I31]  * a[I43];

         detA -= a[I13] * a[I21] * a[I34]  * a[I42];
         detA -= a[I13] * a[I22] * a[I31]  * a[I44];
         detA -= a[I13] * a[I24] * a[I32]  * a[I41];

         detA -= a[I14] * a[I21] * a[I32]  * a[I43];
         detA -= a[I14] * a[I22] * a[I33]  * a[I41];
         detA -= a[I14] * a[I23] * a[I31]  * a[I42];

 return detA;
}


int invert4x4MatrixD(double * result,double * mat)
{
 double * a = mat;
 double * b = result;
 double detA = det4x4Matrix(mat);
 if (detA==0.0)
    {
      copy4x4DMatrix(result,mat);
      fprintf(stderr,"Matrix 4x4 cannot be inverted (det = 0)\n");
      return 0;
    }
 double one_div_detA = (double) 1 / detA;

 //FIRST LINE
 b[I11]  = a[I22] * a[I33] * a[I44] +  a[I23] * a[I34] * a[I42]  + a[I24] * a[I32] * a[I43];
 b[I11] -= a[I22] * a[I34] * a[I43] +  a[I23] * a[I32] * a[I44]  + a[I24] * a[I33] * a[I42];
 b[I11] *= one_div_detA;

 b[I12]  = a[I12] * a[I34] * a[I43] +  a[I13] * a[I32] * a[I44]  + a[I14] * a[I33] * a[I42];
 b[I12] -= a[I12] * a[I33] * a[I44] +  a[I13] * a[I34] * a[I42]  + a[I14] * a[I32] * a[I43];
 b[I12] *= one_div_detA;

 b[I13]  = a[I12] * a[I23] * a[I44] +  a[I13] * a[I24] * a[I42]  + a[I14] * a[I22] * a[I43];
 b[I13] -= a[I12] * a[I24] * a[I43] +  a[I13] * a[I22] * a[I44]  + a[I14] * a[I23] * a[I42];
 b[I13] *= one_div_detA;

 b[I14]  = a[I12] * a[I24] * a[I33] +  a[I13] * a[I22] * a[I34]  + a[I14] * a[I23] * a[I32];
 b[I14] -= a[I12] * a[I23] * a[I34] +  a[I13] * a[I24] * a[I32]  + a[I14] * a[I22] * a[I33];
 b[I14] *= one_div_detA;

 //SECOND LINE
 b[I21]  = a[I21] * a[I34] * a[I43] +  a[I23] * a[I31] * a[I44]  + a[I24] * a[I33] * a[I41];
 b[I21] -= a[I21] * a[I33] * a[I44] +  a[I23] * a[I34] * a[I41]  + a[I24] * a[I31] * a[I43];
 b[I21] *= one_div_detA;

 b[I22]  = a[I11] * a[I33] * a[I44] +  a[I13] * a[I34] * a[I41]  + a[I14] * a[I31] * a[I43];
 b[I22] -= a[I11] * a[I34] * a[I43] +  a[I13] * a[I31] * a[I44]  + a[I14] * a[I33] * a[I41];
 b[I22] *= one_div_detA;

 b[I23]  = a[I11] * a[I24] * a[I43] +  a[I13] * a[I21] * a[I44]  + a[I14] * a[I23] * a[I41];
 b[I23] -= a[I11] * a[I23] * a[I44] +  a[I13] * a[I24] * a[I41]  + a[I14] * a[I21] * a[I43];
 b[I23] *= one_div_detA;

 b[I24]  = a[I11] * a[I23] * a[I34] +  a[I13] * a[I24] * a[I31]  + a[I14] * a[I21] * a[I33];
 b[I24] -= a[I11] * a[I24] * a[I33] +  a[I13] * a[I21] * a[I34]  + a[I14] * a[I23] * a[I31];
 b[I24] *= one_div_detA;

 //THIRD LINE
 b[I31]  = a[I21] * a[I32] * a[I44] +  a[I22] * a[I34] * a[I41]  + a[I24] * a[I31] * a[I42];
 b[I31] -= a[I21] * a[I34] * a[I42] +  a[I22] * a[I31] * a[I44]  + a[I24] * a[I32] * a[I41];
 b[I31] *= one_div_detA;

 b[I32]  = a[I11] * a[I34] * a[I42] +  a[I12] * a[I31] * a[I44]  + a[I14] * a[I32] * a[I41];
 b[I32] -= a[I11] * a[I32] * a[I44] +  a[I12] * a[I34] * a[I41]  + a[I14] * a[I31] * a[I42];
 b[I32] *= one_div_detA;

 b[I33]  = a[I11] * a[I22] * a[I44] +  a[I12] * a[I24] * a[I41]  + a[I14] * a[I21] * a[I42];
 b[I33] -= a[I11] * a[I24] * a[I42] +  a[I12] * a[I21] * a[I44]  + a[I14] * a[I22] * a[I41];
 b[I33] *= one_div_detA;

 b[I34]  = a[I11] * a[I24] * a[I32] +  a[I12] * a[I21] * a[I34]  + a[I14] * a[I22] * a[I31];
 b[I34] -= a[I11] * a[I22] * a[I34] +  a[I12] * a[I24] * a[I31]  + a[I14] * a[I21] * a[I32];
 b[I34] *= one_div_detA;

 //FOURTH LINE
 b[I41]  = a[I21] * a[I33] * a[I42] +  a[I22] * a[I31] * a[I43]  + a[I23] * a[I32] * a[I41];
 b[I41] -= a[I21] * a[I32] * a[I43] +  a[I22] * a[I33] * a[I41]  + a[I23] * a[I31] * a[I42];
 b[I41] *= one_div_detA;

 b[I42]  = a[I11] * a[I32] * a[I43] +  a[I12] * a[I33] * a[I41]  + a[I13] * a[I31] * a[I42];
 b[I42] -= a[I11] * a[I33] * a[I42] +  a[I12] * a[I31] * a[I43]  + a[I13] * a[I32] * a[I41];
 b[I42] *= one_div_detA;

 b[I43]  = a[I11] * a[I23] * a[I42] +  a[I12] * a[I21] * a[I43]  + a[I13] * a[I22] * a[I41];
 b[I43] -= a[I11] * a[I22] * a[I43] +  a[I12] * a[I23] * a[I41]  + a[I13] * a[I21] * a[I42];
 b[I43] *= one_div_detA;

 b[I44]  = a[I11] * a[I22] * a[I33] +  a[I12] * a[I23] * a[I31]  + a[I13] * a[I21] * a[I32];
 b[I44] -= a[I11] * a[I23] * a[I32] +  a[I12] * a[I21] * a[I33]  + a[I13] * a[I22] * a[I31];
 b[I44] *= one_div_detA;


 #if PRINT_MATRIX_DEBUGGING
  print4x4DMatrix("Inverted Matrix From Source",a);
  print4x4DMatrix("Inverted Matrix To Target",b);
 #endif // PRINT_MATRIX_DEBUGGING

 return 1;
}


int transpose4x4Matrix(float * mat)
{
  if (mat==0) { return 0; }
  /*       -------  TRANSPOSE ------->
      0   1   2   3           0  4  8   12
      4   5   6   7           1  5  9   13
      8   9   10  11          2  6  10  14
      12  13  14  15          3  7  11  15   */

  float tmp;
  tmp = mat[1]; mat[1]=mat[4];  mat[4]=tmp;
  tmp = mat[2]; mat[2]=mat[8];  mat[8]=tmp;
  tmp = mat[3]; mat[3]=mat[12]; mat[12]=tmp;


  tmp = mat[6]; mat[6]=mat[9]; mat[9]=tmp;
  tmp = mat[13]; mat[13]=mat[7]; mat[7]=tmp;
  tmp = mat[14]; mat[14]=mat[11]; mat[11]=tmp;

  return 1;
}


int transpose4x4MatrixD(double * mat)
{
  if (mat==0) { return 0; }
  /*       -------  TRANSPOSE ------->
      0   1   2   3           0  4  8   12
      4   5   6   7           1  5  9   13
      8   9   10  11          2  6  10  14
      12  13  14  15          3  7  11  15   */

  double tmp;
  tmp = mat[1]; mat[1]=mat[4];  mat[4]=tmp;
  tmp = mat[2]; mat[2]=mat[8];  mat[8]=tmp;
  tmp = mat[3]; mat[3]=mat[12]; mat[12]=tmp;


  tmp = mat[6]; mat[6]=mat[9]; mat[9]=tmp;
  tmp = mat[13]; mat[13]=mat[7]; mat[7]=tmp;
  tmp = mat[14]; mat[14]=mat[11]; mat[11]=tmp;

  return 1;
}


//matrixA x matrixB
int multiplyTwo4x4Matrices(double * result , double * matrixA , double * matrixB)
{
  if ( (matrixA==0) || (matrixB==0) || (result==0) ) { return 0; }

  #if PRINT_MATRIX_DEBUGGING
  fprintf(stderr,"Multiplying 4x4 A and B \n");
  print4x4DMatrix("A", matrixA);
  print4x4DMatrix("B", matrixB);
  #endif // PRINT_MATRIX_DEBUGGING


  //MULTIPLICATION_RESULT FIRST ROW
  result[0]=matrixA[0] * matrixB[0] + matrixA[1] * matrixB[4]  + matrixA[2] * matrixB[8]  + matrixA[3] * matrixB[12];
  result[1]=matrixA[0] * matrixB[1] + matrixA[1] * matrixB[5]  + matrixA[2] * matrixB[9]  + matrixA[3] * matrixB[13];
  result[2]=matrixA[0] * matrixB[2] + matrixA[1] * matrixB[6]  + matrixA[2] * matrixB[10] + matrixA[3] * matrixB[14];
  result[3]=matrixA[0] * matrixB[3] + matrixA[1] * matrixB[7]  + matrixA[2] * matrixB[11] + matrixA[3] * matrixB[15];

  //MULTIPLICATION_RESULT SECOND ROW
  result[4]=matrixA[4] * matrixB[0] + matrixA[5] * matrixB[4]  + matrixA[6] * matrixB[8]  + matrixA[7] * matrixB[12];
  result[5]=matrixA[4] * matrixB[1] + matrixA[5] * matrixB[5]  + matrixA[6] * matrixB[9]  + matrixA[7] * matrixB[13];
  result[6]=matrixA[4] * matrixB[2] + matrixA[5] * matrixB[6]  + matrixA[6] * matrixB[10] + matrixA[7] * matrixB[14];
  result[7]=matrixA[4] * matrixB[3] + matrixA[5] * matrixB[7]  + matrixA[6] * matrixB[11] + matrixA[7] * matrixB[15];

  //MULTIPLICATION_RESULT FOURTH ROW
  result[8] =matrixA[8] * matrixB[0] + matrixA[9] * matrixB[4]  + matrixA[10] * matrixB[8]   + matrixA[11] * matrixB[12];
  result[9] =matrixA[8] * matrixB[1] + matrixA[9] * matrixB[5]  + matrixA[10] * matrixB[9]   + matrixA[11] * matrixB[13];
  result[10]=matrixA[8] * matrixB[2] + matrixA[9] * matrixB[6]  + matrixA[10] * matrixB[10]  + matrixA[11] * matrixB[14];
  result[11]=matrixA[8] * matrixB[3] + matrixA[9] * matrixB[7]  + matrixA[10] * matrixB[11]  + matrixA[11] * matrixB[15];

  result[12]=matrixA[12] * matrixB[0] + matrixA[13] * matrixB[4]  + matrixA[14] * matrixB[8]    + matrixA[15] * matrixB[12];
  result[13]=matrixA[12] * matrixB[1] + matrixA[13] * matrixB[5]  + matrixA[14] * matrixB[9]    + matrixA[15] * matrixB[13];
  result[14]=matrixA[12] * matrixB[2] + matrixA[13] * matrixB[6]  + matrixA[14] * matrixB[10]   + matrixA[15] * matrixB[14];
  result[15]=matrixA[12] * matrixB[3] + matrixA[13] * matrixB[7]  + matrixA[14] * matrixB[11]   + matrixA[15] * matrixB[15];

  #if PRINT_MATRIX_DEBUGGING
   print4x4DMatrix("AxB", result);
  #endif // PRINT_MATRIX_DEBUGGING

  return 1;
}


int multiplyThree4x4Matrices(double * result , double * matrixA , double * matrixB , double * matrixC)
{
  if ( (matrixA==0) || (matrixB==0) || (matrixC==0) || (result==0) ) { return 0; }

  int i=0;
  double tmp[16];
  i+=multiplyTwo4x4Matrices(tmp,matrixB,matrixC);
  i+=multiplyTwo4x4Matrices(result , matrixA , tmp);

  return (i==2);
}


int multiplyTwo4x4FMatrices(float * result , float * matrixA , float * matrixB)
{
  if ( (matrixA==0) || (matrixB==0) || (result==0) ) { return 0; }

  #if PRINT_MATRIX_DEBUGGING
  fprintf(stderr,"Multiplying 4x4 A and B \n");
  print4x4FMatrix("A", matrixA);
  print4x4FMatrix("B", matrixB);
  #endif // PRINT_MATRIX_DEBUGGING


  //MULTIPLICATION_RESULT FIRST ROW
  result[0]=matrixA[0] * matrixB[0] + matrixA[1] * matrixB[4]  + matrixA[2] * matrixB[8]  + matrixA[3] * matrixB[12];
  result[1]=matrixA[0] * matrixB[1] + matrixA[1] * matrixB[5]  + matrixA[2] * matrixB[9]  + matrixA[3] * matrixB[13];
  result[2]=matrixA[0] * matrixB[2] + matrixA[1] * matrixB[6]  + matrixA[2] * matrixB[10] + matrixA[3] * matrixB[14];
  result[3]=matrixA[0] * matrixB[3] + matrixA[1] * matrixB[7]  + matrixA[2] * matrixB[11] + matrixA[3] * matrixB[15];

  //MULTIPLICATION_RESULT SECOND ROW
  result[4]=matrixA[4] * matrixB[0] + matrixA[5] * matrixB[4]  + matrixA[6] * matrixB[8]  + matrixA[7] * matrixB[12];
  result[5]=matrixA[4] * matrixB[1] + matrixA[5] * matrixB[5]  + matrixA[6] * matrixB[9]  + matrixA[7] * matrixB[13];
  result[6]=matrixA[4] * matrixB[2] + matrixA[5] * matrixB[6]  + matrixA[6] * matrixB[10] + matrixA[7] * matrixB[14];
  result[7]=matrixA[4] * matrixB[3] + matrixA[5] * matrixB[7]  + matrixA[6] * matrixB[11] + matrixA[7] * matrixB[15];

  //MULTIPLICATION_RESULT FOURTH ROW
  result[8] =matrixA[8] * matrixB[0] + matrixA[9] * matrixB[4]  + matrixA[10] * matrixB[8]   + matrixA[11] * matrixB[12];
  result[9] =matrixA[8] * matrixB[1] + matrixA[9] * matrixB[5]  + matrixA[10] * matrixB[9]   + matrixA[11] * matrixB[13];
  result[10]=matrixA[8] * matrixB[2] + matrixA[9] * matrixB[6]  + matrixA[10] * matrixB[10]  + matrixA[11] * matrixB[14];
  result[11]=matrixA[8] * matrixB[3] + matrixA[9] * matrixB[7]  + matrixA[10] * matrixB[11]  + matrixA[11] * matrixB[15];

  result[12]=matrixA[12] * matrixB[0] + matrixA[13] * matrixB[4]  + matrixA[14] * matrixB[8]    + matrixA[15] * matrixB[12];
  result[13]=matrixA[12] * matrixB[1] + matrixA[13] * matrixB[5]  + matrixA[14] * matrixB[9]    + matrixA[15] * matrixB[13];
  result[14]=matrixA[12] * matrixB[2] + matrixA[13] * matrixB[6]  + matrixA[14] * matrixB[10]   + matrixA[15] * matrixB[14];
  result[15]=matrixA[12] * matrixB[3] + matrixA[13] * matrixB[7]  + matrixA[14] * matrixB[11]   + matrixA[15] * matrixB[15];

  #if PRINT_MATRIX_DEBUGGING
   print4x4FMatrix("AxB", result);
  #endif // PRINT_MATRIX_DEBUGGING

  return 1;
}


int transform3DNormalVectorUsing3x3PartOf4x4Matrix(double * resultPoint3D, double * transformation4x4, double * point3D)
{
  if ( (resultPoint3D==0) || (transformation4x4==0) || (point3D==0))  { return 0; }


  if (point3D[3]!=0.0)
  {
    fprintf(stderr,"Error with W coordinate transform3DNormalVectorUsing3x3PartOf4x4Matrix , should be zero  \n");
    return 0;
  }

  double * m = transformation4x4;
  register double X=point3D[0],Y=point3D[1],W=point3D[2];
  /*
  What we want to do ( in mathematica )
   { {me0,me1,me2} , {me3,me4,me5} , {me6,me7,me8} } * { { X } , { Y } , { W } }

   This gives us

  {
    {me2 W + me0 X + me1 Y},
    {me5 W + me3 X + me4 Y},
    {me8 W + me6 X + me7 Y}
  }
*/

  double * me0=&m[e0] , * me1=&m[e1] , * me2=&m[e2]  ;  //m[e3]  ignored
  double * me3=&m[e4] , * me4=&m[e5] , * me5=&m[e6]  ;  //m[e7]  ignored
  double * me6=&m[e8] , * me7=&m[e9] , * me8=&m[e10] ;  //m[e11] ignored
  //       last line ignored since we only want 3x3


  resultPoint3D[0] =  (*me2) * W + (*me0) * X + (*me1) * Y;
  resultPoint3D[1] =  (*me5) * W + (*me3) * X + (*me4) * Y;
  resultPoint3D[2] =  (*me8) * W + (*me6) * X + (*me7) * Y;
  resultPoint3D[3] =  0;

 // Ok we have our results but now to normalize our vector
  if(resultPoint3D[2]!=0.0)
  {
   resultPoint3D[0]/=resultPoint3D[2];
   resultPoint3D[1]/=resultPoint3D[2];
   resultPoint3D[2]=1.0; //resultPoint3D[2]/=resultPoint3D[2];
  }

  return 1;
}




int transform3DPointVectorUsing4x4Matrix(double * resultPoint3D, double * transformation4x4, double * point3D)
{
  if ( (resultPoint3D==0) || (transformation4x4==0) || (point3D==0))  { return 0; }

/*
   What we want to do ( in mathematica )
   { {e0,e1,e2,e3} , {e4,e5,e6,e7} , {e8,e9,e10,e11} , {e12,e13,e14,e15} } * { { X } , { Y }  , { Z } , { W } }

   This gives us

  {
    {e3 W + e0 X + e1 Y + e2 Z},
    {e7 W + e4 X + e5 Y + e6 Z},
    {e11 W + e8 X + e9 Y + e10 Z},
    {e15 W + e12 X + e13 Y + e14 Z}
  }
*/
  double * m = transformation4x4;
  register double X=point3D[0],Y=point3D[1],Z=point3D[2],W=point3D[3];

  resultPoint3D[0] =  m[e3] * W + m[e0] * X + m[e1] * Y + m[e2] * Z;
  resultPoint3D[1] =  m[e7] * W + m[e4] * X + m[e5] * Y + m[e6] * Z;
  resultPoint3D[2] =  m[e11] * W + m[e8] * X + m[e9] * Y + m[e10] * Z;
  resultPoint3D[3] =  m[e15] * W + m[e12] * X + m[e13] * Y + m[e14] * Z;

  // Ok we have our results but now to normalize our vector
  if (resultPoint3D[3]!=0.0)
  {
   resultPoint3D[0]/=resultPoint3D[3];
   resultPoint3D[1]/=resultPoint3D[3];
   resultPoint3D[2]/=resultPoint3D[3];
   resultPoint3D[3]=1.0; // resultPoint3D[3]/=resultPoint3D[3];
   return 1;
  } else
  {
     fprintf(stderr,"Error with W coordinate after multiplication of 3D Point with 4x4 Matrix\n");
     print4x4DMatrix("Matrix was",transformation4x4,1);
     fprintf(stderr,"Input Point was %0.2f %0.2f %0.2f %0.2f \n",point3D[0],point3D[1],point3D[2],point3D[3]);
     fprintf(stderr,"Output Point was %0.2f %0.2f %0.2f %0.2f \n",resultPoint3D[0],resultPoint3D[1],resultPoint3D[2],resultPoint3D[3]);
     return 0;
  }

 return 1;
}


int normalize3DPointVector(double * vec)
{
  if ( vec[3]==1.0 ) { return 1; } else
  if ( vec[3]==0.0 )
  {
    fprintf(stderr,"normalize3DPointVector cannot be normalized since element 3 is zero\n");
    return 0;
  }


  vec[0]=vec[0]/vec[3];
  vec[1]=vec[1]/vec[3];
  vec[2]=vec[2]/vec[3];
  vec[3]=1.0; // vec[3]=vec[3]/vec[3];

  return 1;
}


void create4x4ModelTransformation(
                                  double * m ,
                                  //Rotation Component
                                  double rollInDegrees,
                                  double pitchInDegrees,
                                  double yawInDegrees,
                                  //Translation Component
                                  double x, double y, double z ,
                                  double scaleX, double scaleY, double scaleZ
                                 )
{
   if (m==0) {return;}

    //fprintf(stderr,"Asked for a model transformation with RPY(%0.2f,%0.2f,%0.2f)",rollInDegrees,pitchInDegrees,yawInDegrees);
    //fprintf(stderr,"XYZ(%0.2f,%0.2f,%0.2f)",x,y,z);
    //fprintf(stderr,"scaled(%0.2f,%0.2f,%0.2f)\n",scaleX,scaleY,scaleZ);


    double intermediateMatrixTranslation[16];
    create4x4TranslationMatrix(
                               intermediateMatrixTranslation,
                               x,
                               y,
                               z
                              );


    double intermediateMatrixPitch[16];
    double intermediateMatrixHeading[16];
    double intermediateMatrixRoll[16];
    create4x4RotationMatrix(  intermediateMatrixRoll   , rollInDegrees,      0.0,   0.0,   1.0);
    create4x4RotationMatrix(  intermediateMatrixHeading, yawInDegrees,       0.0,   1.0,   0.0);
    create4x4RotationMatrix(  intermediateMatrixPitch  , pitchInDegrees,     1.0,   0.0,   0.0);

    double intermediateMatrixRotation[16];
    multiplyThree4x4Matrices(
                              intermediateMatrixRotation ,
                              intermediateMatrixRoll ,
                              intermediateMatrixHeading ,
                              intermediateMatrixPitch
                            );




  if ( (scaleX!=1.0) || (scaleY!=1.0) || (scaleZ!=1.0) )
      {
        double intermediateScalingMatrix[16];
        create4x4ScalingMatrix(intermediateScalingMatrix,scaleX,scaleY,scaleZ);
        multiplyThree4x4Matrices(m,intermediateMatrixTranslation,intermediateMatrixRotation,intermediateScalingMatrix);
      } else
      {
         multiplyTwo4x4Matrices(m,intermediateMatrixTranslation,intermediateMatrixRotation);
      }
}


void create4x4CameraModelViewMatrixForRendering(
                                                double * m ,
                                                //Rotation Component
                                                double rotationX_angleDegrees,
                                                double rotationY_angleDegrees,
                                                double rotationZ_angleDegrees ,
                                                //Translation Component
                                                double translationX_angleDegrees,
                                                double translationY_angleDegrees,
                                                double translationZ_angleDegrees
                                               )
{
    if (m==0) {return;}

    double intermediateMatrixRX[16];
    double intermediateMatrixRY[16];
    double intermediateMatrixRZ[16];
    create4x4RotationMatrix(  intermediateMatrixRX, rotationX_angleDegrees,  -1.0,   0.0,   0.0);
    create4x4RotationMatrix(  intermediateMatrixRY, rotationY_angleDegrees,   0.0,  -1.0,   0.0);
    create4x4RotationMatrix(  intermediateMatrixRZ, rotationZ_angleDegrees,   0.0,   0.0,  -1.0);

    double intermediateMatrixRotation[16];
    multiplyThree4x4Matrices(
                              intermediateMatrixRotation ,
                              intermediateMatrixRX ,
                              intermediateMatrixRY ,
                              intermediateMatrixRZ
                            );

    double intermediateMatrixTranslation[16];
    create4x4TranslationMatrix(
                               intermediateMatrixTranslation,
                               -translationX_angleDegrees,
                               -translationY_angleDegrees,
                               -translationZ_angleDegrees
                              );

    multiplyTwo4x4Matrices(m,intermediateMatrixRotation,intermediateMatrixTranslation);
}
