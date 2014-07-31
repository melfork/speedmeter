DEPDIR=.deps

#
# compilation flags
#
FFMPEG_CFLAGS=$(shell pkg-config --cflags libavformat libavcodec libswscale libavutil)
CIMG_CFLAGS=-I../../CImg -I/usr/X11R6/include -Dcimg_verbosity=3 -Dcimg_strict_warnings -g -Dcimg_use_vt100
DEBUG_FLAGS=-O0 -g -Wall
RELEASE_FLAGS=-O3 -ffast-math -msse2 -mmmx

#GST_INC=-I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/libxml2 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng15

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

#
# Binary Information
#
EXENAME=test

all: $(EXENAME)

#
#
$(EXENAME): $(COBJECTS) $(CPPOBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

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

clean:
	rm -rf $(COBJECTS) $(CPPOBJECTS) $(EXENAME) $(DEPDIR)/ 
