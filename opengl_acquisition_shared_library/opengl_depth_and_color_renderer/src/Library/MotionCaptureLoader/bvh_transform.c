#include "bvh_transform.h"

#include "../../../../../tools/AmMatrix/matrix4x4Tools.h"

//As http://research.cs.wisc.edu/graphics/Courses/cs-838-1999/Jeff/BVH.html?fbclid=IwAR0Hq96gIhAq-6mvi8OAfMJid2qkv7ZIGNxNMna4vBNngILoceulshvxMfc states
//To calculate the position of a segment you first create a transformation matrix from the local translation and rotation information for that segment. For any joint segment the translation information will simply be the offset as defined in the hierarchy section. The rotation data comes from the motion section. For the root object, the translation data will be the sum of the offset data and the translation data from the motion section. The BVH format doesn't account for scales so it isn't necessary to worry about including a scale factor calculation.
//A straightforward way to create the rotation matrix is to create 3 separate rotation matrices, one for each axis of rotation. Then concatenate the matrices from left to right Y, X and Z.
//
//vR = vYXZ
void create4x4RotationBVH(double * matrix,int rotationType,double degreesX,double degreesY,double degreesZ)
{
  double rX[16]={0};
  double rY[16]={0};
  double rZ[16]={0};

  create4x4RotationX(rX,degreesX);
  create4x4RotationX(rY,degreesY);
  create4x4RotationX(rZ,degreesZ);

  //TODO : Fix correct order..
  //http://www.dcs.shef.ac.uk/intranet/research/public/resmes/CS0111.pdf
  #define FLIP_ROTATION_ORDER 0

  #if FLIP_ROTATION_ORDER
  switch (rotationType)
  {
    case BVH_ROTATION_ORDER_XYZ :
      multiplyThree4x4Matrices( matrix, rZ, rY, rX );
    break;
    case BVH_ROTATION_ORDER_XZY :
      multiplyThree4x4Matrices( matrix, rY, rZ, rX );
    break;
    case BVH_ROTATION_ORDER_YXZ :
      multiplyThree4x4Matrices( matrix, rZ, rX, rY );
    break;
    case BVH_ROTATION_ORDER_YZX :
      multiplyThree4x4Matrices( matrix, rX, rZ, rY );
    break;
    case BVH_ROTATION_ORDER_ZXY :
      multiplyThree4x4Matrices( matrix, rY, rX, rZ );
    break;
    case BVH_ROTATION_ORDER_ZYX :
      multiplyThree4x4Matrices( matrix, rX, rY, rZ );
    break;
  };
  #else
  switch (rotationType)
  {
    case BVH_ROTATION_ORDER_XYZ :
      multiplyThree4x4Matrices( matrix, rX, rY, rZ );
    break;
    case BVH_ROTATION_ORDER_XZY :
      multiplyThree4x4Matrices( matrix, rX, rZ, rY );
    break;
    case BVH_ROTATION_ORDER_YXZ :
      multiplyThree4x4Matrices( matrix, rY, rX, rZ );
    break;
    case BVH_ROTATION_ORDER_YZX :
      multiplyThree4x4Matrices( matrix, rY, rZ, rX );
    break;
    case BVH_ROTATION_ORDER_ZXY :
      multiplyThree4x4Matrices( matrix, rZ, rX, rY );
    break;
    case BVH_ROTATION_ORDER_ZYX :
      multiplyThree4x4Matrices( matrix, rZ, rY, rX );
    break;
  };
  #endif // FLIP_ROTATION_ORDER

}


int bvh_loadTransformForFrame(
                               struct BVH_MotionCapture * bvhMotion ,
                               BVHFrameID fID ,
                               struct BVH_Transform * bvhTransform
                             )
{
  unsigned int jID=0;

  //First of all we need to clean the current transform
  for (jID=0; jID<bvhMotion->jointHierarchySize; jID++)
  {
     create4x4IdentityMatrix(bvhTransform->joint[jID].localTransformation);
     create4x4IdentityMatrix(bvhTransform->joint[jID].finalVertexTransformation);
  }

  //We will now apply all transformations
  double translationM[16]={0};
  double rotationM[16]={0};
  double scalingM[16]={0};
  double centerPoint[4]={0.0,0.0,0.0,1.0};

  create4x4IdentityMatrix(scalingM);
  create4x4ScalingMatrix(scalingM,0.5,0.5,0.5);

  double posX,posY,posZ;
  double rotX,rotY,rotZ;

  float data[8]={0};
  for (jID=0; jID<bvhMotion->jointHierarchySize; jID++)
  {
      bhv_populatePosXYZRotXYZ(bvhMotion,jID,fID,data,sizeof(data));
      float * offset = bvh_getJointOffset(bvhMotion,jID);

      posX = (double) offset[0] + (double) data[0];
      posY = (double) offset[1] + (double) data[1];
      posZ = (double) offset[2] + (double) data[2];

      rotX = (double) data[3];
      rotY = (double) data[4];
      rotZ = (double) data[5];

      create4x4TranslationMatrix(translationM,posX,posY,posZ);
      //create4x4MatrixFromEulerAnglesZYX(rotationM,rotY,rotX,rotZ);
      create4x4RotationBVH(
                            rotationM,
                            bvhMotion->jointHierarchy[jID].channelRotationOrder,
                            rotX,
                            rotY,
                            rotZ
                          );

      /*
      multiplyTwo4x4Matrices(
                               bvhTransform->joint[jID].localTransformation,
                               translationM,
                               rotationM
                            );*/

      multiplyThree4x4Matrices(
                               bvhTransform->joint[jID].localTransformation,
                               translationM,
                               rotationM,
                               scalingM
                              );
  }


  for (jID=0; jID<bvhMotion->jointHierarchySize; jID++)
  {
     if (bhv_jointHasParent(bvhMotion,jID))
      {
        unsigned int parentID = bvhMotion->jointHierarchy[jID].parentJoint;
        multiplyTwo4x4Matrices(
                                //Output
                                bvhTransform->joint[jID].finalVertexTransformation ,
                                //Parent Output
                                bvhTransform->joint[parentID].finalVertexTransformation,
                                //This Local Transform
                                bvhTransform->joint[jID].localTransformation
                              );

        //Also find Center of Joint
        transform3DPointVectorUsing4x4Matrix(
                                              bvhTransform->joint[jID].pos,
                                              bvhTransform->joint[jID].finalVertexTransformation,
                                              centerPoint
                                            );
      } else
      {
       bhv_populatePosXYZRotXYZ(bvhMotion,jID,fID,data,sizeof(data));
       float * offset = bvh_getJointOffset(bvhMotion,jID);

       bvhTransform->joint[jID].pos[0] = (double) offset[0] + (double) data[0];
       bvhTransform->joint[jID].pos[1] = (double) offset[1] + (double) data[1];
       bvhTransform->joint[jID].pos[2] = (double) offset[2] + (double) data[2];

       copy4x4DMatrix(
                       bvhTransform->joint[jID].finalVertexTransformation ,
                       bvhTransform->joint[jID].localTransformation
                      );
      }
  }

  return 1;
}


