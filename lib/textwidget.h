#pragma once
#include "widgetbase.h"

namespace SPEEDMETER {

class SpeedMeterDllExport
TextWidget: public StringWidget {
	CImg<unsigned char> text;
public:
	TextWidget(CImg<unsigned char>&container, int width, int height,
			size_t offset_x, size_t offset_y);
	virtual ~ TextWidget();
	virtual bool init(std::map<std::string, std::string> const &parameters);
	virtual CImg<unsigned char>&setIndicator(int value);
	virtual CImg<unsigned char>&setIndicator(const char *value);
};

}
