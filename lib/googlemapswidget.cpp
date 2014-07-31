#include "googlemapswidget.h"
#include "mercatorprojection.h"
#include <stdlib.h>
#include <stdio.h>

int getGoogleMapsImage(int zoom, SPEEDMETER::Coordinate const &center,
		int width, int heigth, char **image);

static char *generateTmpFile(const char *img, size_t size) {
	char *tempname=strdup("/tmp/tmpFileXXXXXX");
	int f=mkstemp(tempname);
	if (f>0) {
		for(const char *end=img+size; img<end; img+=write(f,img,1024));
		close(f);
	}
	return tempname;
}

namespace SPEEDMETER {

GPXGoogleWidget::GPXGoogleWidget(CImg<unsigned char>&container, GPX const &_gpx, int _width,int _height, size_t _offset_x, size_t _offset_y):
		TimeWidget(container, _width, _height, _offset_x, _offset_y),
		gpx(_gpx),zoomlevel(0),point(6, 6, 1, 3, 1),
		crop(6, 6, 1, 3, 1) {

}

GPXGoogleWidget::~GPXGoogleWidget() {
}

bool GPXGoogleWidget::init(std::map<std::string, std::string> const &parameters) {
	MercatorProjection proj;
	point.draw_circle(3, 3, 2, COLORS::RGB[COLORS::Blue]).blur(1.5);

	Bounds bounds=gpx.getBounds();
	zoomlevel=proj.calculateBoundsZoomLevel(bounds,getWidgetX(),getWidgetY());
	if (zoomlevel>0) {
		char *image;
		Coordinate center=proj.CalcCenterFromBounds(bounds);
		int imageSize=getGoogleMapsImage(zoomlevel, center, getWidgetX(),getWidgetY(),&image);

		if (imageSize>0) {
			char *pngfile=generateTmpFile(image,imageSize);
			map.load_png(pngfile);
			free(pngfile);
			return true;
		}
	}
	return false;
}

CImg<unsigned char>& GPXGoogleWidget::setIndicator(time_t value) {
	draw_image(getOffsetX(), getOffsetY(), map);
	return *this;
}

}
