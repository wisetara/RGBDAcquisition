#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = -Wall -fexceptions `pkg-config --cflags opencv`
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = `pkg-config --libs opencv` -ljpeg -lpng

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = /DepthImagesConverter

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = /DepthImagesConverter

OBJ_DEBUG = $(OBJDIR_DEBUG)/__/Codecs/codecs.o $(OBJDIR_DEBUG)/__/Codecs/jpgExifexternal.o $(OBJDIR_DEBUG)/__/Codecs/jpgExiforient_embed.o $(OBJDIR_DEBUG)/__/Codecs/jpgInput.o $(OBJDIR_DEBUG)/__/Codecs/pngInput.o $(OBJDIR_DEBUG)/__/Codecs/ppmInput.o $(OBJDIR_DEBUG)/main.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/__/Codecs/codecs.o $(OBJDIR_RELEASE)/__/Codecs/jpgExifexternal.o $(OBJDIR_RELEASE)/__/Codecs/jpgExiforient_embed.o $(OBJDIR_RELEASE)/__/Codecs/jpgInput.o $(OBJDIR_RELEASE)/__/Codecs/pngInput.o $(OBJDIR_RELEASE)/__/Codecs/ppmInput.o $(OBJDIR_RELEASE)/main.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d $(OBJDIR_DEBUG)/__/Codecs || mkdir -p $(OBJDIR_DEBUG)/__/Codecs
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/__/Codecs/codecs.o: ../Codecs/codecs.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../Codecs/codecs.c -o $(OBJDIR_DEBUG)/__/Codecs/codecs.o

$(OBJDIR_DEBUG)/__/Codecs/jpgExifexternal.o: ../Codecs/jpgExifexternal.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../Codecs/jpgExifexternal.c -o $(OBJDIR_DEBUG)/__/Codecs/jpgExifexternal.o

$(OBJDIR_DEBUG)/__/Codecs/jpgExiforient_embed.o: ../Codecs/jpgExiforient_embed.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../Codecs/jpgExiforient_embed.c -o $(OBJDIR_DEBUG)/__/Codecs/jpgExiforient_embed.o

$(OBJDIR_DEBUG)/__/Codecs/jpgInput.o: ../Codecs/jpgInput.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../Codecs/jpgInput.c -o $(OBJDIR_DEBUG)/__/Codecs/jpgInput.o

$(OBJDIR_DEBUG)/__/Codecs/pngInput.o: ../Codecs/pngInput.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../Codecs/pngInput.c -o $(OBJDIR_DEBUG)/__/Codecs/pngInput.o

$(OBJDIR_DEBUG)/__/Codecs/ppmInput.o: ../Codecs/ppmInput.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../Codecs/ppmInput.c -o $(OBJDIR_DEBUG)/__/Codecs/ppmInput.o

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf $(OBJDIR_DEBUG)/__/Codecs
	rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d $(OBJDIR_RELEASE)/__/Codecs || mkdir -p $(OBJDIR_RELEASE)/__/Codecs
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/__/Codecs/codecs.o: ../Codecs/codecs.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../Codecs/codecs.c -o $(OBJDIR_RELEASE)/__/Codecs/codecs.o

$(OBJDIR_RELEASE)/__/Codecs/jpgExifexternal.o: ../Codecs/jpgExifexternal.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../Codecs/jpgExifexternal.c -o $(OBJDIR_RELEASE)/__/Codecs/jpgExifexternal.o

$(OBJDIR_RELEASE)/__/Codecs/jpgExiforient_embed.o: ../Codecs/jpgExiforient_embed.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../Codecs/jpgExiforient_embed.c -o $(OBJDIR_RELEASE)/__/Codecs/jpgExiforient_embed.o

$(OBJDIR_RELEASE)/__/Codecs/jpgInput.o: ../Codecs/jpgInput.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../Codecs/jpgInput.c -o $(OBJDIR_RELEASE)/__/Codecs/jpgInput.o

$(OBJDIR_RELEASE)/__/Codecs/pngInput.o: ../Codecs/pngInput.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../Codecs/pngInput.c -o $(OBJDIR_RELEASE)/__/Codecs/pngInput.o

$(OBJDIR_RELEASE)/__/Codecs/ppmInput.o: ../Codecs/ppmInput.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../Codecs/ppmInput.c -o $(OBJDIR_RELEASE)/__/Codecs/ppmInput.o

$(OBJDIR_RELEASE)/main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf $(OBJDIR_RELEASE)/__/Codecs
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

