#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WRKDIR = `pwd`
MAKE = make

all: network_acquisition_shared_library_networkacquisition viewer_viewer grabber_grabber grabber_segment_grabbersegment grabber_mux_grabbermux acquisitionbroadcast_acquisitionbroadcast acquisitionsegment_acquisitionsegment acquisition_mux_acquisitionmux acquisition_acquisition tools_calibration_calibrationlibrary v4l2stereo_acquisition_shared_library_v4l2stereoacquisition v4l2_acquisition_shared_library_v4l2acquisition template_acquisition_shared_library_templateacquisition opengl_acquisition_shared_library_openglacquisition openni1_acquisition_shared_library_openni1acquisition openni2_acquisition_shared_library_openni2aquisition libfreenect_acquisition_shared_library_freenectacquisition

network_acquisition_shared_library_networkacquisition: 
	$(MAKE) -C network_acquisition_shared_library all -f NetworkAcquisition.cbp.mak

viewer_viewer: 
	$(MAKE) -C viewer all -f Viewer.cbp.mak

grabber_grabber: 
	$(MAKE) -C grabber all -f Grabber.cbp.mak

grabber_segment_grabbersegment: 
	$(MAKE) -C grabber_segment all -f GrabberSegment.cbp.mak

grabber_mux_grabbermux: 
	$(MAKE) -C grabber_mux all -f GrabberMux.cbp.mak

acquisitionbroadcast_acquisitionbroadcast: 
	$(MAKE) -C acquisitionBroadcast all -f acquisitionBroadcast.cbp.mak

acquisitionsegment_acquisitionsegment: 
	$(MAKE) -C acquisitionSegment all -f acquisitionSegment.cbp.mak

acquisition_mux_acquisitionmux: 
	$(MAKE) -C acquisition_mux all -f AcquisitionMux.cbp.mak

acquisition_acquisition: 
	$(MAKE) -C acquisition all -f Acquisition.cbp.mak

tools_calibration_calibrationlibrary: 
	$(MAKE) -C tools/Calibration all -f CalibrationLibrary.cbp.mak

v4l2stereo_acquisition_shared_library_v4l2stereoacquisition: 
	$(MAKE) -C v4l2stereo_acquisition_shared_library all -f V4L2StereoAcquisition.cbp.mak

v4l2_acquisition_shared_library_v4l2acquisition: 
	$(MAKE) -C v4l2_acquisition_shared_library all -f V4L2Acquisition.cbp.mak

template_acquisition_shared_library_templateacquisition: 
	$(MAKE) -C template_acquisition_shared_library all -f TemplateAcquisition.cbp.mak

opengl_acquisition_shared_library_openglacquisition: 
	$(MAKE) -C opengl_acquisition_shared_library all -f OpenGLAcquisition.cbp.mak

openni1_acquisition_shared_library_openni1acquisition: 
	$(MAKE) -C openni1_acquisition_shared_library all -f OpenNI1Acquisition.cbp.mak

openni2_acquisition_shared_library_openni2aquisition: 
	$(MAKE) -C openni2_acquisition_shared_library all -f OpenNI2Aquisition.cbp.mak

libfreenect_acquisition_shared_library_freenectacquisition: 
	$(MAKE) -C libfreenect_acquisition_shared_library all -f FreenectAcquisition.cbp.mak

clean: clean_network_acquisition_shared_library_networkacquisition clean_viewer_viewer clean_grabber_grabber clean_grabber_segment_grabbersegment clean_grabber_mux_grabbermux clean_acquisitionbroadcast_acquisitionbroadcast clean_acquisitionsegment_acquisitionsegment clean_acquisition_mux_acquisitionmux clean_acquisition_acquisition clean_tools_calibration_calibrationlibrary clean_v4l2stereo_acquisition_shared_library_v4l2stereoacquisition clean_v4l2_acquisition_shared_library_v4l2acquisition clean_template_acquisition_shared_library_templateacquisition clean_opengl_acquisition_shared_library_openglacquisition clean_openni1_acquisition_shared_library_openni1acquisition clean_openni2_acquisition_shared_library_openni2aquisition clean_libfreenect_acquisition_shared_library_freenectacquisition

clean_network_acquisition_shared_library_networkacquisition: 
	$(MAKE) -C network_acquisition_shared_library clean -f NetworkAcquisition.cbp.mak

clean_viewer_viewer: 
	$(MAKE) -C viewer clean -f Viewer.cbp.mak

clean_grabber_grabber: 
	$(MAKE) -C grabber clean -f Grabber.cbp.mak

clean_grabber_segment_grabbersegment: 
	$(MAKE) -C grabber_segment clean -f GrabberSegment.cbp.mak

clean_grabber_mux_grabbermux: 
	$(MAKE) -C grabber_mux clean -f GrabberMux.cbp.mak

clean_acquisitionbroadcast_acquisitionbroadcast: 
	$(MAKE) -C acquisitionBroadcast clean -f acquisitionBroadcast.cbp.mak

clean_acquisitionsegment_acquisitionsegment: 
	$(MAKE) -C acquisitionSegment clean -f acquisitionSegment.cbp.mak

clean_acquisition_mux_acquisitionmux: 
	$(MAKE) -C acquisition_mux clean -f AcquisitionMux.cbp.mak

clean_acquisition_acquisition: 
	$(MAKE) -C acquisition clean -f Acquisition.cbp.mak

clean_tools_calibration_calibrationlibrary: 
	$(MAKE) -C tools/Calibration clean -f CalibrationLibrary.cbp.mak

clean_v4l2stereo_acquisition_shared_library_v4l2stereoacquisition: 
	$(MAKE) -C v4l2stereo_acquisition_shared_library clean -f V4L2StereoAcquisition.cbp.mak

clean_v4l2_acquisition_shared_library_v4l2acquisition: 
	$(MAKE) -C v4l2_acquisition_shared_library clean -f V4L2Acquisition.cbp.mak

clean_template_acquisition_shared_library_templateacquisition: 
	$(MAKE) -C template_acquisition_shared_library clean -f TemplateAcquisition.cbp.mak

clean_opengl_acquisition_shared_library_openglacquisition: 
	$(MAKE) -C opengl_acquisition_shared_library clean -f OpenGLAcquisition.cbp.mak

clean_openni1_acquisition_shared_library_openni1acquisition: 
	$(MAKE) -C openni1_acquisition_shared_library clean -f OpenNI1Acquisition.cbp.mak

clean_openni2_acquisition_shared_library_openni2aquisition: 
	$(MAKE) -C openni2_acquisition_shared_library clean -f OpenNI2Aquisition.cbp.mak

clean_libfreenect_acquisition_shared_library_freenectacquisition: 
	$(MAKE) -C libfreenect_acquisition_shared_library clean -f FreenectAcquisition.cbp.mak

.PHONY: clean_network_acquisition_shared_library_networkacquisition clean_viewer_viewer clean_grabber_grabber clean_grabber_segment_grabbersegment clean_grabber_mux_grabbermux clean_acquisitionbroadcast_acquisitionbroadcast clean_acquisitionsegment_acquisitionsegment clean_acquisition_mux_acquisitionmux clean_acquisition_acquisition clean_tools_calibration_calibrationlibrary clean_v4l2stereo_acquisition_shared_library_v4l2stereoacquisition clean_v4l2_acquisition_shared_library_v4l2acquisition clean_template_acquisition_shared_library_templateacquisition clean_opengl_acquisition_shared_library_openglacquisition clean_openni1_acquisition_shared_library_openni1acquisition clean_openni2_acquisition_shared_library_openni2aquisition clean_libfreenect_acquisition_shared_library_freenectacquisition

