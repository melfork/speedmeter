#pragma once
#include "widgetbase.h"
#include "gpxprojection.h"
#include "projection.h"
#include "gpx.h"

namespace SPEEDMETER {

class SpeedMeterDllExport
GPXWidget: public TimeWidget {
	ProjectionHelper *helper;
	GPXProjection *proj;
	GPX const &gpx;
	CImg<unsigned char> crop;
	CImg<unsigned char> point;
public:
	GPXWidget(CImg<unsigned char>&container, GPX const &gpx, int width,
			int height, size_t offset_x, size_t offset_y);
	virtual ~ GPXWidget();
	virtual bool init(std::map<std::string, std::string> const &parameters);
	virtual CImg<unsigned char>&setIndicator(time_t value);
};

}
