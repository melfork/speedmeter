DEPDIR=.deps

#
# compilation flags
#
FFMPEG_CFLAGS=$(shell pkg-config --cflags libavformat libavcodec libswscale libavutil)
CIMG_CFLAGS=-I../../CImg -I/usr/X11R6/include -Dcimg_verbosity=3 -Dcimg_strict_warnings -g -Dcimg_use_vt100 -Dcimg_use_png
DEBUG_FLAGS=-O0 -g -Wall
RELEASE_FLAGS=-O3 -ffast-math -msse2 -mmmx

INCLUDES=-I. -I../lib $(FFMPEG_CFLAGS) $(CIMG_CFLAGS)  
CFLAGS+= $(INCLUDES) -D__STDC_CONSTANT_MACROS -pthread $(DEBUG_FLAGS)

#
# Linker flags
#
FFMPEG_LDFLAGS=$(shell pkg-config --libs libavformat libavcodec libswscale libavutil)
CIMG_LDFLAGS=-L/usr/X11R6/lib -lX11 
#GST_LIBS= -lgstapp-0.10 -lgstvideo-0.10 -lgstbase-0.10 -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lxml2 -lglib-2.0 -lgdk_pixbuf-2.0
LDFLAGS+= -L/usr/lib -L../lib -lspeedmeter  $(FFMPEG_LDFLAGS) $(CIMG_LDFLAGS) -lexpat -ldl -lpthread -lm -lbz2 -lz -lrt

#
# Objects files
#
COBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))
CPPOBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))


all: testproj testgpx testlcd testgauge testgmaps

#
# use dependencies
#
-include ./$(DEPDIR)/*.d


testproj: testproj.o
	$(CXX) -o $@ $^ $(LDFLAGS)

testgpx: testgpx.o
	$(CXX) -o $@ $^ $(LDFLAGS)


testlcd: testlcd.o
	$(CXX) -o $@ $^ $(LDFLAGS)

testgauge: testgauge.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	
testgmaps: testgmaps.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	@mkdir -p $(DEPDIR)
	$(CXX) -MM $(CFLAGS) $*.c > $(DEPDIR)/$*.d

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	@mkdir -p $(DEPDIR) 
	$(CXX) -MM $(CFLAGS) $*.cpp > $(DEPDIR)/$*.d

clean:
	rm -rf $(COBJECTS) $(CPPOBJECTS) $(EXENAME) $(DEPDIR)/ 
