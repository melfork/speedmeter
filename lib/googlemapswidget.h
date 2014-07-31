#pragma once
#include "widgetbase.h"
#include "gpx.h"

namespace SPEEDMETER {

class SpeedMeterDllExport
GPXGoogleWidget: public TimeWidget {
	GPX const &gpx;
	int zoomlevel;
	CImg<unsigned char> map;
	CImg<unsigned char> crop;
	CImg<unsigned char> point;
public:
	GPXGoogleWidget(CImg<unsigned char>&container, GPX const &gpx, int width,
			int height, size_t offset_x, size_t offset_y);
	virtual ~ GPXGoogleWidget();
	virtual bool init(std::map<std::string, std::string> const &parameters);
	virtual CImg<unsigned char>&setIndicator(time_t value);
};

}
