#pragma once
#include "widgetbase.h"
#include "lcd.h"

namespace SPEEDMETER {

class SpeedMeterDllExport
LCDWidget: public StringWidget {
	size_t lcdSize;
	LCD **lcdList;
public:
	LCDWidget(CImg<unsigned char>&container, size_t width, size_t height,
			size_t offset_x, size_t offset_y);
	virtual ~ LCDWidget();
	virtual bool init(std::map<std::string, std::string> const &parameters);
	virtual CImg<unsigned char>&setIndicator(const char *value);
};

}
