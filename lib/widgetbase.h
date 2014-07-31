#pragma once

#include "speedmeter.h"
#ifndef _WINDOWS
#include <unistd.h>
#endif
#include <math.h>
#include <time.h>
#include "CImg.h"
#include <map>
#include <string>
#include "colors.h"
#include "basictypes.h"

using namespace cimg_library;

namespace SPEEDMETER {

/**
 *
 *
 */
class SpeedMeterDllExport
Widget: public CImg<unsigned char> {
public:
	enum Type {
		UNKNOWN, INTEGER, STRING, TIME
	};
protected:
	size_t offsetX, offsetY;
	size_t widgetX, widgetY;
	Type wtype;

	Widget(CImg<unsigned char>&container, int _width, int _height,
			size_t offset_x, size_t offset_y, Type t) :
			CImg<unsigned char>(container, true), offsetX(offset_x), offsetY(
					offset_y), widgetX(_width), widgetY(_height), wtype(t) {
	}

	Widget(int _width, int _height, size_t offset_x, size_t offset_y) :
			CImg<unsigned char>(_width, _height, 1, 3, 1), offsetX(offset_x), offsetY(
					offset_y), widgetX(_width), widgetY(_height) {
	}

public:
	virtual ~ Widget() {
	}

	Type getType() {
		return wtype;
	}

	virtual bool init(std::map<std::string, std::string> const &parameters) = 0;

	const size_t getOffsetX() const {
		return offsetX;
	}

	const size_t getOffsetY() const {
		return offsetY;
	}

	const size_t getWidgetX() const {
		return widgetX;
	}

	const size_t getWidgetY() const {
		return widgetY;
	}
};

/**
 *
 *
 */
class SpeedMeterDllExport
IntegerWidget: public Widget {
public:
	IntegerWidget(CImg<unsigned char>&container, int _width, int _height,
			size_t offset_x, size_t offset_y) :
			Widget(container, _width, _height, offset_x, offset_y,
					Widget::INTEGER) {
	}
	virtual ~IntegerWidget() {
	}
	virtual CImg<unsigned char>&setIndicator(int value) = 0;
};

/**
 *
 *
 */
class SpeedMeterDllExport
StringWidget: public Widget {
public:
	StringWidget(CImg<unsigned char>&container, int _width, int _height,
			size_t offset_x, size_t offset_y) :
			Widget(container, _width, _height, offset_x, offset_y,
					Widget::STRING) {
	}
	virtual ~StringWidget() {
	}
	virtual CImg<unsigned char>&setIndicator(const char *value) = 0;
};

/**
 *
 *
 */
class SpeedMeterDllExport
TimeWidget: public Widget {
public:
	TimeWidget(CImg<unsigned char>&container, int _width, int _height,
			size_t offset_x, size_t offset_y) :
			Widget(container, _width, _height, offset_x, offset_y, Widget::TIME) {
	}
	virtual ~TimeWidget() {
	}
	virtual CImg<unsigned char>&setIndicator(time_t value) = 0;
};
}
