#include "gpx.h"
#include <iostream>
#include <math.h>
#include "gpxframelist.h"
#include "googlemapswidget.h"
#include <stdio.h>

using namespace SPEEDMETER;

/**
 *
 *
 */
int main(int argc, char *argv[]) {
	GPX gpx;
	if (!gpx.load(argv[1]))
				return 0;

	std::map < std::string, std::string > conf;
	CImg<unsigned char> img(640,480,1,3,1);
	CImgDisplay disp(img, "GMapTest");

	GPXGoogleWidget googleWidget(img, gpx,320,240,0,0);
	googleWidget.init(conf);
	googleWidget.setIndicator(0).display(disp);


	getchar();
	return 0;
}
