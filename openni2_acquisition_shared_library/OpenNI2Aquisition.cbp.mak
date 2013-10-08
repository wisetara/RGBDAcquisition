#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -I../3dparty/OpenNI2/Include/
CFLAGS = -Wall -fexceptions -fPIC
RESINC = 
LIBDIR = 
LIB = ../3dparty/OpenNI2/Bin/x64-Release/libOpenNI2.so
LDFLAGS = 

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = libOpenNI2Acquisition.so

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = libOpenNI2Acquisition.so

OBJ_DEBUG = $(OBJDIR_DEBUG)/OpenNI2Acquisition.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/OpenNI2Acquisition.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) -shared $(LIBDIR_DEBUG) $(OBJ_DEBUG)  -o $(OUT_DEBUG) $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/OpenNI2Acquisition.o: OpenNI2Acquisition.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c OpenNI2Acquisition.cpp -o $(OBJDIR_DEBUG)/OpenNI2Acquisition.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) -shared $(LIBDIR_RELEASE) $(OBJ_RELEASE)  -o $(OUT_RELEASE) $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/OpenNI2Acquisition.o: OpenNI2Acquisition.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c OpenNI2Acquisition.cpp -o $(OBJDIR_RELEASE)/OpenNI2Acquisition.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

