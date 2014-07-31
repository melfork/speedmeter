DEPDIR=.deps

#
# compilation flags
#
FFMPEG_CFLAGS=$(shell pkg-config --cflags libavformat libavcodec libswscale libavutil)
CIMG_CFLAGS=-I../../CImg -I/usr/X11R6/include -Dcimg_verbosity=3 -Dcimg_strict_warnings -g -Dcimg_use_vt100 -Dcimg_use_png
DEBUG_FLAGS=-O0 -g -Wall
RELEASE_FLAGS=-O3 -ffast-math -msse2 -mmmx

INCLUDES=-I. $(FFMPEG_CFLAGS) $(CIMG_CFLAGS)  
CFLAGS+= $(INCLUDES) -D__STDC_CONSTANT_MACROS -fPIC -pthread $(DEBUG_FLAGS)

#
# Linker flags
#
FFMPEG_LDFLAGS=$(shell pkg-config --libs libavformat libavcodec libswscale libavutil)
CIMG_LDFLAGS=-L/usr/X11R6/lib -lX11

LDFLAGS+= -L/usr/lib $(FFMPEG_LDFLAGS) $(CIMG_LDFLAGS) -lexpat -lcurl -lpng15 -ldl -lpthread -lm -lbz2 -lz -lrt

#
# Objects files
#
COBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))
CPPOBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
NASAGRIDCPPOBJETS :=$(patsubst nasagrid/%.cc,%.o,$(wildcard nasagrid/*.cc))

#
# Binary Information
#
LIBNAME=libspeedmeter.so

all: $(LIBNAME)

#
#
$(LIBNAME): $(COBJECTS) $(CPPOBJECTS) $(NASAGRIDCPPOBJETS)
	$(CXX) -o $@ -shared $^ $(LDFLAGS)

#
# use dependencies
#
-include ./$(DEPDIR)/*.d

%.o: %.c
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	@mkdir -p $(DEPDIR)
	$(CXX) -MM $(CFLAGS) $*.c > $(DEPDIR)/$*.d

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	@mkdir -p $(DEPDIR) 
	$(CXX) -MM $(CFLAGS) $*.cpp > $(DEPDIR)/$*.d
	
gridsquare.o: nasagrid/gridsquare.cc
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	@mkdir -p $(DEPDIR) 
	$(CXX) -MM $(CFLAGS) $< > $(DEPDIR)/$*.d

nasagridsquare.o: nasagrid/nasagridsquare.cc
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	@mkdir -p $(DEPDIR) 
	$(CXX) -MM $(CFLAGS) $< > $(DEPDIR)/$*.d

utils.o: nasagrid/utils.cc
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	@mkdir -p $(DEPDIR) 
	$(CXX) -MM $(CFLAGS) $< > $(DEPDIR)/$*.d

	
clean:
	rm -rf $(COBJECTS) $(CPPOBJECTS) $(EXENAME) $(DEPDIR)/ 
