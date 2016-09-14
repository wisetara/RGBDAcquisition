#include "model_loader.h"
#include "model_loader_tri.h"
#include "model_loader_transform_joints.h"

#include <stdio.h>
#include <stdlib.h>


/*



void readNodeHeirarchyOLD(const aiMesh * mesh , const aiNode* pNode,  struct boneState * bones , struct skeletonHuman * sk, aiMatrix4x4 & ParentTransform , unsigned int recursionLevel)
{

    if (recursionLevel==0)    { fprintf(stderr,"readNodeHeirarchy : \n"); } else
                              {  fprintf(stderr,"   "); }
    fprintf(stderr,"%s\n" , pNode->mName.data);

    aiMatrix4x4 NodeTransformation=pNode->mTransformation;


    unsigned int foundBone;
    unsigned int boneNumber=findBoneFromString(mesh,pNode->mName.data,&foundBone);


    unsigned int i=0;
    if (foundBone)
    {
    for (i=0; i<HUMAN_SKELETON_PARTS; i++)
        {
            if (strcmp(pNode->mName.data , smartBodyNames[i])==0)
              {
               if ( sk->active[i] )
               {
               fprintf(stderr,GREEN "hooked with %s ( r.x=%0.2f r.y=%0.2f r.z=%0.2f ) !\n" NORMAL,jointNames[i] , sk->relativeJointAngle[i].x, sk->relativeJointAngle[i].y, sk->relativeJointAngle[i].z);
               bones->bone[boneNumber].ScalingVec.x=1.0;
               bones->bone[boneNumber].ScalingVec.y=1.0;
               bones->bone[boneNumber].ScalingVec.z=1.0;

               bones->bone[boneNumber].TranslationVec.x=pNode->mTransformation.a4;
               bones->bone[boneNumber].TranslationVec.y=pNode->mTransformation.b4;
               bones->bone[boneNumber].TranslationVec.z=pNode->mTransformation.c4;

              aiMatrix4x4::Scaling(bones->bone[boneNumber].ScalingVec,bones->bone[boneNumber].scalingMat);
              aiMatrix4x4::Translation (bones->bone[boneNumber].TranslationVec,bones->bone[boneNumber].translationMat);
              //aiMakeQuaternion( &bones.bone[k].rotationMat , &bones.bone[k].RotationQua );
              //aiPrintMatrix(&bones->bone[boneNumber].rotationMat );


               //zxy 120 - xyz 012

               bones->bone[boneNumber].rotationMat.FromEulerAnglesXYZ(
                                                                      degrees_to_rad ( sk->relativeJointAngle[i].z + defaultJointsOffsetZXY[i*3+2] ),
                                                                      degrees_to_rad ( sk->relativeJointAngle[i].x + defaultJointsOffsetZXY[i*3+0] ),
                                                                      degrees_to_rad ( sk->relativeJointAngle[i].y + defaultJointsOffsetZXY[i*3+1] )
                                                                      );


               NodeTransformation =  bones->bone[boneNumber].translationMat  * bones->bone[boneNumber].rotationMat * bones->bone[boneNumber].scalingMat;
              } else
              {
               fprintf(stderr, RED " inactive %s ( r.x=%0.2f r.y=%0.2f r.z=%0.2f ) !\n" NORMAL ,jointNames[i] ,
                       sk->relativeJointAngle[i].x,
                       sk->relativeJointAngle[i].y,
                       sk->relativeJointAngle[i].z);
              }
            }
        }

    aiMatrix4x4 GlobalTransformation = ParentTransform  * NodeTransformation;
    bones->bone[boneNumber].finalTransform = m_GlobalInverseTransform * GlobalTransformation * bones->bone[boneNumber].boneInverseBindTransform;
    for ( i = 0 ; i < pNode->mNumChildren ; i++)
    {
        readNodeHeirarchyOLD(mesh,pNode->mChildren[i],bones,sk,GlobalTransformation,recursionLevel+1);
    }
    } else
    {
      aiMatrix4x4 GlobalTransformation = ParentTransform  * pNode->mTransformation;
      fprintf(stderr,"        <!%s!>\n",pNode->mName.data);
       for ( i = 0 ; i < pNode->mNumChildren ; i++)
       {
         readNodeHeirarchyOLD(mesh,pNode->mChildren[i],bones,sk,GlobalTransformation,recursionLevel+1);
       }
    }
}


*/








void recursiveJointHeirarchyTransformer( struct TRI_Model * in  , int curBone ,   struct TRI_Transform * finalTransforms , double * parentTransform , unsigned int recursionLevel)
{

    if (recursionLevel==0)    { fprintf(stderr,"readNodeHeirarchy : \n"); } else
                              { fprintf(stderr,"   "); }
    fprintf(stderr,"%s\n" , in->bones[curBone].boneName );


    double NodeTransformation[16]; create4x4IdentityMatrix(&parentTransform) ;


    unsigned int foundBone;

    /*

    unsigned int i=0;
    if (foundBone)
    {
    for (i=0; i<HUMAN_SKELETON_PARTS; i++)
        {
            if (strcmp(pNode->mName.data , smartBodyNames[i])==0)
              {
               if ( sk->active[i] )
               {
               fprintf(stderr,GREEN "hooked with %s ( r.x=%0.2f r.y=%0.2f r.z=%0.2f ) !\n" NORMAL,jointNames[i] , sk->relativeJointAngle[i].x, sk->relativeJointAngle[i].y, sk->relativeJointAngle[i].z);
               bones->bone[boneNumber].ScalingVec.x=1.0;
               bones->bone[boneNumber].ScalingVec.y=1.0;
               bones->bone[boneNumber].ScalingVec.z=1.0;

               bones->bone[boneNumber].TranslationVec.x=pNode->mTransformation.a4;
               bones->bone[boneNumber].TranslationVec.y=pNode->mTransformation.b4;
               bones->bone[boneNumber].TranslationVec.z=pNode->mTransformation.c4;

              aiMatrix4x4::Scaling(bones->bone[boneNumber].ScalingVec,bones->bone[boneNumber].scalingMat);
              aiMatrix4x4::Translation (bones->bone[boneNumber].TranslationVec,bones->bone[boneNumber].translationMat);
              //aiMakeQuaternion( &bones.bone[k].rotationMat , &bones.bone[k].RotationQua );
              //aiPrintMatrix(&bones->bone[boneNumber].rotationMat );


               //zxy 120 - xyz 012

               bones->bone[boneNumber].rotationMat.FromEulerAnglesXYZ(
                                                                      degrees_to_rad ( sk->relativeJointAngle[i].z + defaultJointsOffsetZXY[i*3+2] ),
                                                                      degrees_to_rad ( sk->relativeJointAngle[i].x + defaultJointsOffsetZXY[i*3+0] ),
                                                                      degrees_to_rad ( sk->relativeJointAngle[i].y + defaultJointsOffsetZXY[i*3+1] )
                                                                      );


               NodeTransformation =  bones->bone[boneNumber].translationMat  * bones->bone[boneNumber].rotationMat * bones->bone[boneNumber].scalingMat;
              } else
              {
               fprintf(stderr, RED " inactive %s ( r.x=%0.2f r.y=%0.2f r.z=%0.2f ) !\n" NORMAL ,jointNames[i] ,
                       sk->relativeJointAngle[i].x,
                       sk->relativeJointAngle[i].y,
                       sk->relativeJointAngle[i].z);
              }
            }
        }

    aiMatrix4x4 GlobalTransformation = ParentTransform  * NodeTransformation;
    bones->bone[boneNumber].finalTransform = m_GlobalInverseTransform * GlobalTransformation * bones->bone[boneNumber].boneInverseBindTransform;
    for ( i = 0 ; i < pNode->mNumChildren ; i++)
    {
        readNodeHeirarchyOLD(mesh,pNode->mChildren[i],bones,sk,GlobalTransformation,recursionLevel+1);
    }
    } else
    {
      aiMatrix4x4 GlobalTransformation = ParentTransform  * pNode->mTransformation;
      fprintf(stderr,"        <!%s!>\n",pNode->mName.data);
       for ( i = 0 ; i < pNode->mNumChildren ; i++)
       {
         readNodeHeirarchyOLD(mesh,pNode->mChildren[i],bones,sk,GlobalTransformation,recursionLevel+1);
       }
    }

    */
}








int doModelTransform( struct TRI_Model * triModelOut , struct TRI_Model * triModelIn , float * jointData , unsigned int jointDataSize)
{
  fprintf(stderr,"doModelTransform deactivated.. \n");
  return 0;

  if (triModelIn==0) { fprintf(stderr,"doModelTransform called without input TRI Model \n"); return 0; }

  copyModelTri( triModelOut , triModelIn );



  double transPosition[4]={0};
  double position[4]={0};

  double normal[4]={0};
  double transNormal[4]={0};


   struct TRI_Transform * finalTransforms = (struct TRI_Transform * ) malloc( triModelIn->header.numberOfBones * sizeof(struct TRI_Transform) );
   if (finalTransforms==0) { return 0; }


  double parentTransform[16]={0};
  create4x4IdentityMatrix(&parentTransform) ;
  recursiveJointHeirarchyTransformer( triModelIn , 0 , finalTransforms , parentTransform , 0 );


   fprintf(stderr,"Clearing vertices & normals \n");
   //We NEED to clear the vertices and normals since they are added uppon , not having
   //the next two lines results in really weird and undebuggable visual behaviour
   memset(triModelOut->vertices, 0, triModelOut->header.numberOfVertices  * sizeof(float));
   memset(triModelOut->normal  , 0, triModelOut->header.numberOfNormals   * sizeof(float));



   unsigned int k=0,i=0;
   for (k=0; k<triModelIn->header.numberOfBones; k++ )
   {
     fprintf(stderr,"Transforming %u bone\n",k);
     for (i=0; i<triModelIn->bones[i].info->boneWeightsNumber; i++ )
     {
       //V is the vertice we will be working in this loop
       unsigned int v = triModelIn->bones[i].weightIndex[k];
       //W is the weight that we have for the specific bone
       float w = triModelIn->bones[i].weightValue[k];

       //We load our input into position/normal
       position[0] = triModelIn->vertices[v*3+0];
       position[1] = triModelIn->vertices[v*3+1];
       position[2] = triModelIn->vertices[v*3+2];
       position[3] = 1.0;

       normal[0]   = triModelIn->normal[v*3+0];
       normal[1]   = triModelIn->normal[v*3+1];
       normal[2]   = triModelIn->normal[v*3+2];
       normal[3]   = 1.0;

       //We transform input (initial) position with the transform we computed to get transPosition
       transform3DPointVectorUsing4x4Matrix(transPosition, finalTransforms[k].finalTransform ,position);
      // aiTransformVecByMatrix4(&position, &modifiedSkeleton.bone[k].finalTransform );
	   triModelOut->vertices[v*3+0] += (float) transPosition[0] * w;
	   triModelOut->vertices[v*3+1] += (float) transPosition[1] * w;
	   triModelOut->vertices[v*3+2] += (float) transPosition[2] * w;

       //We transform input (initial) normal with the transform we computed to get transNormal
       transform3DPointVectorUsing4x4Matrix(transNormal, finalTransforms[k].finalTransform ,normal);
    //   aiTransformVecByMatrix3(&normal, &finalTransform3x3);
	   triModelOut->normal[v*3+0] += (float) transNormal[0] * w;
	   triModelOut->normal[v*3+1] += (float) transNormal[1] * w;
	   triModelOut->normal[v*3+2] += (float) transNormal[2] * w;
     }
   }


  free(finalTransforms);

}
