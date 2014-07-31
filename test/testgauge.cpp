#include "gpx.h"
#include <iostream>
#include <math.h>
#include "gaugewidget.h"

using namespace SPEEDMETER;

/**
 *
 *
 */
int main(int argc, char *argv[])
{
	std::map < std::string, std::string > conf;
	CImg<unsigned char> img(320,200,1,3,1);
	CImgDisplay disp(img, "GaugeTest");

	GaugeWidget lcd(img,140,140,1,1);
	lcd.init(conf);
	
	for(int i=0;i<90;i++) {
		lcd.setIndicator(i).display(disp);
		usleep(250000);
	}
	for(int i=90;i<0;i++) {
		lcd.setIndicator(i).display(disp);
		usleep(250000);
	}
	return 0;
}
