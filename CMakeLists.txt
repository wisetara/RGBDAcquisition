project( RGBDAcquisitionProject ) 
cmake_minimum_required( VERSION 2.8.7 )

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3dparty ${CMAKE_MODULE_PATH})

      
OPTION (COMPILE_FOR_PERFORMANCE OFF)
OPTION (ENABLE_AMMARSERVER OFF)
OPTION (ENABLE_FACEDETECTION_DEV OFF)
OPTION (ENABLE_FREENECT ON)
OPTION (ENABLE_FREENECT2 ON)
OPTION (ENABLE_REALSENSE ON)
OPTION (ENABLE_TEMPLATE ON)
OPTION (ENABLE_DESKTOP ON)
OPTION (ENABLE_V4L2 ON)
OPTION (ENABLE_CUDA OFF)
OPTION (ENABLE_OPENNI1 OFF)
OPTION (ENABLE_OPENNI2 ON)
OPTION (ENABLE_DEPTHSENSE OFF)
OPTION (ENABLE_NITE2 OFF)
OPTION (ENABLE_OPENGL ON)
OPTION (ENABLE_NETWORK ON)
OPTION (ENABLE_EDITOR ON)
OPTION (ENABLE_LOCATION_SERVICE OFF)
OPTION (ENABLE_TESTS OFF)

OPTION (ENABLE_AUTOSENSING ON)
OPTION (ENABLE_OPENCV OFF)
OPTION (ENABLE_OPENCV_BASED_TOOLS OFF)
OPTION (ENABLE_OPENCV_BINDINGS OFF)
  
OPTION (ENABLE_PROCESSOR_BODYTRACKER OFF)
OPTION (ENABLE_PROCESSOR_DARKNET OFF)
OPTION (ENABLE_PROCESSOR_MOVIDIUS OFF)


set(CMAKE_CXX_FLAGS "-fPIC -s") 
set(CMAKE_C_FLAGS "-fPIC -s") 
IF( COMPILE_FOR_PERFORMANCE )
 #-march=native -mtune=native
 set(CMAKE_CXX_FLAGS "-fPIC  -O3 -fexpensive-optimizations -s") 
 set(CMAKE_C_FLAGS "-fPIC  -O3 -fexpensive-optimizations -s") 
ENDIF( COMPILE_FOR_PERFORMANCE )



IF( ENABLE_AUTOSENSING ) 
#----
MESSAGE("Trying to auto-sense OpenCV presence .. ")
find_package(OpenCV)
if (OpenCV_FOUND)
 OPTION (ENABLE_OPENCV ON) 
 OPTION (ENABLE_OPENCV_BASED_TOOLS ON)
ELSE (OpenCV_FOUND)
 OPTION (ENABLE_OPENCV OFF)
 OPTION (ENABLE_OPENCV_BASED_TOOLS OFF)
 MESSAGE("OpenCV Not found , some parts like viewer etc will not be compiled")
ENDIF(OpenCV_FOUND)
#----
ENDIF( ENABLE_AUTOSENSING ) 




OPTION (ENABLE_JPG OFF)
OPTION (ENABLE_PNG OFF)

add_subdirectory (3dparty/fast)



IF( ENABLE_CUDA ) 
  add_subdirectory (gpuCompare/)  
ENDIF( ENABLE_CUDA ) 


IF( ENABLE_AMMARSERVER )
  #add_subdirectory (3dparty/AmmarServer/src/AmmServerlib) 
  add_subdirectory (3dparty/AmmarServer/) 
  add_definitions(-DUSE_AMMARSERVER)
ENDIF( ENABLE_AMMARSERVER )


if (ENABLE_FREENECT2)
  add_subdirectory (3dparty/librealsense)
ELSE (ENABLE_FREENECT2)
 #OPTION (ENABLE_OPENCV OFF)
 MESSAGE("Freenect2 not found , some parts will not be compiled")
ENDIF(ENABLE_FREENECT2)


if (ENABLE_REALSENSE)
  add_definitions(-DBUILD_REALSENSE)
  #add_subdirectory (3dparty/libfreenect2/examples/protonect)
  add_subdirectory (librealsense_acquisition_shared_library)
ELSE (ENABLE_REALSENSE) 
 MESSAGE("Realsense support will not be compiled")
ENDIF(ENABLE_REALSENSE)


#First to make our handy tools
add_subdirectory (tools/)
  
#Then make our handy processor plugins 
add_subdirectory (processors/) 

#This is the core library
add_subdirectory (acquisition/)
add_subdirectory (acquisitionSegment/)
add_subdirectory (acquisition_mux/)
#This is the core application  
add_subdirectory (grabber/)
add_subdirectory (grabber_mux/)
add_subdirectory (grabber_segment/)
  

#Acquisition Plugins 
IF( ENABLE_TEMPLATE )
add_subdirectory (template_acquisition_shared_library/)
ENDIF( ENABLE_TEMPLATE )

IF( ENABLE_OPENGL )
  add_subdirectory (opengl_acquisition_shared_library/)
  #add_subdirectory (opengl_acquisition_shared_library/opengl_depth_and_color_renderer)
  add_definitions(-DUSE_GLEW)
  add_subdirectory (viewerOGL/)
ENDIF( ENABLE_OPENGL )

IF( ENABLE_V4L2 )
add_subdirectory (v4l2_acquisition_shared_library/)
add_subdirectory (v4l2stereo_acquisition_shared_library/)
ENDIF( ENABLE_V4L2 )

IF( ENABLE_DESKTOP )
add_subdirectory (3dparty/xwd-1.0.5)
add_subdirectory (desktop_acquisition_shared_library/)
ENDIF( ENABLE_DESKTOP )

 
IF( ENABLE_NETWORK )
IF( ENABLE_AMMARSERVER )
  add_subdirectory (network_acquisition_shared_library/)
ENDIF( ENABLE_AMMARSERVER )
ENDIF( ENABLE_NETWORK )
 
IF( ENABLE_FREENECT )
  add_subdirectory (libfreenect_acquisition_shared_library/)
ENDIF( ENABLE_FREENECT )

IF( ENABLE_FREENECT2 )
  add_subdirectory (libfreenect2_acquisition_shared_library/)
ENDIF( ENABLE_FREENECT2 )

IF( ENABLE_OPENNI1 )
  add_subdirectory (openni1_acquisition_shared_library/)
ENDIF( ENABLE_OPENNI1 )

IF( ENABLE_OPENNI2 )
  add_subdirectory (openni2_acquisition_shared_library/)
ENDIF( ENABLE_OPENNI2 )


IF( ENABLE_DEPTHSENSE )
add_subdirectory (depthsense_acquisition_shared_library/)
ENDIF( ENABLE_DEPTHSENSE )

 
 
#Composite ( bloated ;p ) parts 
IF( ENABLE_EDITOR )

 FIND_PACKAGE(wxWidgets)
 IF(wxWidgets_FOUND)
   add_subdirectory (editor/)
 ELSE(wxWidgets_FOUND)
   MESSAGE("wxWidgets not found , cannot build editor!")
 ENDIF(wxWidgets_FOUND)

ENDIF( ENABLE_EDITOR )



IF( ENABLE_TESTS ) 
   add_subdirectory (tests/) 
ENDIF( ENABLE_TESTS )







IF( ENABLE_OPENCV ) 
 add_subdirectory (viewer/)
 IF( ENABLE_OPENCV_BINDINGS )
  add_subdirectory (3dparty/OpenCVBindings)
 ENDIF( ENABLE_OPENCV_BINDINGS )
ELSE (ENABLE_OPENCV)
 MESSAGE("Viewer will not be compiled , OpenCV stuff missing? ( check ENABLE_OPENCV flag )..")
ENDIF( ENABLE_OPENCV )










# TODO FIX INSTALLATION DIRECTORIES 

#    install(TARGETS RGBDAcquisitionProject
#                    LIBRARY DESTINATION lib
#                    ARCHIVE DESTINATION lib
#                    RUNTIME DESTINATION bin)
  
