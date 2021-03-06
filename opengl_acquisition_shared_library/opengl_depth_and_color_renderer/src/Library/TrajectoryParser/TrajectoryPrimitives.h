
/** @file TrajectoryPrimitives.h
* @brief A tool that scans for a string in a very fast and robust way
* @author Ammar Qammaz (AmmarkoV)
*/

#ifndef TRAJECTORYPRIMITIVES_H_INCLUDED
#define TRAJECTORYPRIMITIVES_H_INCLUDED


/** @brief Enumerator for the IDs of TrajectoryPrimitives so we can know what the result was*/
enum { 
 TRAJECTORYPRIMITIVES_EMPTY=0,
 TRAJECTORYPRIMITIVES_SAVED_FILE_DEPTH_SCALE, // 1 
 TRAJECTORYPRIMITIVES_COMMENT, // 2 
 TRAJECTORYPRIMITIVES_ARROW, // 3 
 TRAJECTORYPRIMITIVES_SILENT, // 4 
 TRAJECTORYPRIMITIVES_SHADER, // 5 
 TRAJECTORYPRIMITIVES_POSERAW, // 6 
 TRAJECTORYPRIMITIVES_POSE4X4, // 7 
 TRAJECTORYPRIMITIVES_POSEQ, // 8 
 TRAJECTORYPRIMITIVES_POSE, // 9 
 TRAJECTORYPRIMITIVES_POS, // 10 
 TRAJECTORYPRIMITIVES_FOG, // 11 
 TRAJECTORYPRIMITIVES_DONE_DECLARING_OBJECTS, // 12 
 TRAJECTORYPRIMITIVES_MOVE_VIEW, // 13 
 TRAJECTORYPRIMITIVES_MOVE, // 14 
 TRAJECTORYPRIMITIVES_DEBUG, // 15 
 TRAJECTORYPRIMITIVES_TIMESTAMP, // 16 
 TRAJECTORYPRIMITIVES_AUTOREFRESH, // 17 
 TRAJECTORYPRIMITIVES_INTERPOLATE_TIME, // 18 
 TRAJECTORYPRIMITIVES_ALWAYS_SHOW_LAST_FRAME, // 19 
 TRAJECTORYPRIMITIVES_BACKGROUND, // 20 
 TRAJECTORYPRIMITIVES_COMPOSITE_OBJECT, // 21 
 TRAJECTORYPRIMITIVES_RIGID_OBJECT, // 22 
 TRAJECTORYPRIMITIVES_OBJECT_TYPE, // 23 
 TRAJECTORYPRIMITIVES_OBJECTTYPE, // 24 
 TRAJECTORYPRIMITIVES_OBJECT, // 25 
 TRAJECTORYPRIMITIVES_OBJ_OFFSET, // 26 
 TRAJECTORYPRIMITIVES_OBJ, // 27 
 TRAJECTORYPRIMITIVES_EVENT, // 28 
 TRAJECTORYPRIMITIVES_PROJECTION_MATRIX, // 29 
 TRAJECTORYPRIMITIVES_EMULATE_PROJECTION_MATRIX, // 30 
 TRAJECTORYPRIMITIVES_MODELVIEW_MATRIX, // 31 
 TRAJECTORYPRIMITIVES_SCALE_WORLD, // 32 
 TRAJECTORYPRIMITIVES_GENERATE_ANGLE_OBJECTS, // 33 
 TRAJECTORYPRIMITIVES_OFFSET_ROTATIONS, // 34 
 TRAJECTORYPRIMITIVES_MAP_ROTATIONS, // 35 
 TRAJECTORYPRIMITIVES_FRAME_RESET, // 36 
 TRAJECTORYPRIMITIVES_FRAME, // 37 
 TRAJECTORYPRIMITIVES_RATE, // 38 
 TRAJECTORYPRIMITIVES_AFFIX_OBJ_TO_OBJ_FOR_NEXT_FRAMES, // 39 
 TRAJECTORYPRIMITIVES_INCLUDE, // 40 
 TRAJECTORYPRIMITIVES_SMOOTH, // 41 
 TRAJECTORYPRIMITIVES_CONNECTOR, // 42 
 TRAJECTORYPRIMITIVES_HAND_POINTS, // 43 
 TRAJECTORYPRIMITIVES_PQ, // 44 
 TRAJECTORYPRIMITIVES_P, // 45 
 TRAJECTORYPRIMITIVES_END_OF_ITEMS
};



/** @brief Scan a string for one of the words of the TrajectoryPrimitives word set
* @ingroup stringParsing
* @param Input String , to be scanned
* @param Length of Input String
* @retval See above enumerator*/
 int scanFor_TrajectoryPrimitives(const char * str,unsigned int strLength); 

#endif
