#include "gpx.h"
#include <iostream>
#include <math.h>
#include "gpxprojection.h"

using namespace SPEEDMETER;

/**
 *
 *
 */
int main(int argc, char *argv[])
{
	if (argc == 2) {
		GPX gpx;
		if (!gpx.load(argv[1]))
			return 0;

		CImg<unsigned char> img(640,480,1,3,1);
		CImgDisplay disp(img, "projTest");
		GPXProjection proj(img,640,480);
		if (proj.draw(gpx,COLORS::Gold)) {
			proj.blur(1.5);
			img.display(disp);
		}
		sleep(5);
	}
}
