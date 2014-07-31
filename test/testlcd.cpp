#include "gpx.h"
#include <iostream>
#include <math.h>
#include "gpxprojection.h"
#include "lcdwidget.h"

using namespace SPEEDMETER;

/**
 *
 *
 */
int main(int argc, char *argv[])
{
	std::map < std::string, std::string > conf;
	CImg<unsigned char> img(320,200,1,3,1);
	CImgDisplay disp(img, "LCDTest");

	LCDWidget lcd(img,80,40,1,1);
	lcd.init(conf);
	
	char text[4];
	for(int i=0;i<100;i++) {
		sprintf(text,"%d",i);
		lcd.setIndicator(text).display(disp);
		usleep(250000);
	}
	return 0;
}
