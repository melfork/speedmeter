#include "gpx.h"
#include <iostream>
#include <math.h>
#include "lcdwidget.h"
#include "textwidget.h"
#include "gaugewidget.h"
#include "videoreader.h"
#include "frameconverter.h"
#include "gpxprojection.h"
#include "gpxframelist.h"

using namespace SPEEDMETER;

static void applyWidgetsList(CImg < uint8_t > &next_image) {
//	next_image.display(disp);
}

/**
 *
 *
 */
static int mainloop(GPXFrameList const &f, const char *videoInName, const char *videoOutName)
{
	size_t count=0;
	// Register all formats and codecs
	av_register_all();

	VideoReader *vreader = new VideoReader();
	if (vreader->open(videoInName)) {
		VideoReader::FrameIterator it = vreader->getFirstFrame();
		AVFrame *frame;//, *pConvertedFrame;

		CImg < uint8_t > next_image;
		CImgDisplay disp(next_image, "lcdTest");

		FrameConverter converter(vreader->width(), vreader->height(), vreader->pixFormat(),
			vreader->width(), vreader->height(), PIX_FMT_RGB24);
		while ((frame = ++it)) {
			const AVFrame *pConvertedFrame=converter.convert(frame);
			next_image.assign(*pConvertedFrame->data, 3, vreader->width(), vreader->height(), 1, true);
			next_image.permute_axes("yzcx");

			applyWidgetsList(next_image);
		}
	}

	delete vreader;
	return 0;
}

static void showGPXInfo(GPX const &gpx) {
	std::cout << "GPX Information: " << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << "Total points "  << gpx.getTotalPoints()  << std::endl;
	std::cout << "Max Elevation " << gpx.getMaxElevation() << std::endl;
	std::cout << "Min Elevation " << gpx.getMinElevation() << std::endl;
	std::cout << "Max Latitude "  << gpx.getMaxLatitude()  << std::endl;
	std::cout << "Min Latitude "  << gpx.getMinLatitude()  << std::endl;
	std::cout << "Max Longitude " << gpx.getMaxLongitude() << std::endl;
	std::cout << "Min Longitude " << gpx.getMinLongitude() << std::endl;
}

static vois showGPXFrameInfo(GPXFrameList const &l) {
	std::cout << "GPXFrame Information:" << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << "Max Trk Distance "  << l.getMaxDistance()  << std::endl;
	std::cout << "Min Trk Distance "  << l.getMinDistance()  << std::endl;
	std::cout << "Max Speed "         << l.getMaxSpeed()     << std::endl;
	std::cout << "Min Speed "         << l.getMinSpeed()     << std::endl;
	std::cout << "Max Incline "       << l.getMaxIncline()   << std::endl;
	std::cout << "Min Incline "       << l.getMinIncline()   << std::endl;
}

/**
 *
 *
 */
int main(int argc, char *argv[])
{
	if (argc == 4) {
		GPX gpx;
		if (!gpx.load(argv[1]))
			return 0;

		showGPXInfo(gpx);

		const std::list < GPX::TrkPoint >&l = gpx.getTrackList();


		// calculate frames matrix...
		const GPXFrameList f(l);
		
		showGPXInfo(f);

		CImg<unsigned char> img(640,480,1,3,1);
		CImgDisplay disp(img, "lcdTest");
		Point p;
		GPXProjection proj(img,640,480);
		if (proj.draw(gpx,COLORS::Gold)) {
			proj.blur(1.5);
			img.display(disp);
		}
		sleep(100);
		

		mainloop(f, argv[2], argv[3]);

/*		CImg<unsigned char> img(1024,768,1,3,1);

		LCDWidget lcd(img,300,80,100,10);
		LCDWidget lcd2(img,30,12,200,100);
		TextWidget text(img,60,30,200,200);
		GaugeWidget gauge(img,90,90,200,300);
		std::map<std::string,std::string> conf;
		lcd.init(conf);
		lcd2.init(conf);
		text.init(conf);
		gauge.init(conf);
				CImgDisplay disp(img,"lcdTest");
				char t[32];
				for(int i=0, gi=0; i<1000 && !disp.is_closed(); i++, gi++) {
					sprintf(t,"Numero: %d",i);
					text.setIndicator(t);
					lcd2.setIndicator(999-i);
					gauge.setIndicator(gi);
					if (gi>90)
						gi=0;
					lcd.setIndicator(i).display(disp);
					usleep(100000);
				}
						*//*
		   LCD lcd(70,40);
		   CImgDisplay disp(lcd,"lcdTest");
		   lcd.set(LCD::LCD_None);
		   while(!disp.is_closed()) {
		   lcd.set(LCD::LCD_One).display(disp); sleep(1);
		   lcd.set(LCD::LCD_Two).display(disp); sleep(1);
		   lcd.set(LCD::LCD_Three).display(disp); sleep(1);
		   lcd.set(LCD::LCD_Four).display(disp); sleep(1);
		   lcd.set(LCD::LCD_Five).display(disp); sleep(1);
		   lcd.set(LCD::LCD_Six).display(disp); sleep(1);
		   }
		 */
	}
}
