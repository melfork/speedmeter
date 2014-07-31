#pragma once
#include "widgetbase.h"

namespace SPEEDMETER {

class SpeedMeterDllExport
GaugeWidget: public IntegerWidget {
	CImg<unsigned char> gauge;
	size_t radius;
	Point center;
	size_t minDegree;
	size_t maxDegree;
	size_t degreeDiff;
	int minValue;
	int maxValue;
	int preValue;
public:
	GaugeWidget(CImg<unsigned char>&container, size_t width, size_t height,
			size_t offset_x, size_t offset_y);
	virtual ~ GaugeWidget();
	virtual bool init(std::map<std::string, std::string> const &parameters);
	virtual CImg<unsigned char>&setIndicator(int value);
};

}
