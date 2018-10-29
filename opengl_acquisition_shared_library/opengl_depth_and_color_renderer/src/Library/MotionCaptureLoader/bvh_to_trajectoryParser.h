#ifndef BVH_TO_TRAJECTORYPARSER_H_INCLUDED
#define BVH_TO_TRAJECTORYPARSER_H_INCLUDED

#include "bvh_loader.h"
#include "bvh_to_tri_pose.h"


int dumpBVHToTrajectoryParserTRI(
                                  const char * filename ,
                                  struct BVH_MotionCapture * mc,
                                  struct bvhToTRI * bvhtri
                                );


int dumpBVHToTrajectoryParser(const char * filename , struct BVH_MotionCapture * mc);

#endif // BVH_TO_TRAJECTORYPARSER_H_INCLUDED
