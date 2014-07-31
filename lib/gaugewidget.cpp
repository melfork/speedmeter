#ifdef _WINDOWS
#define _USE_MATH_DEFINES
#include <cmath>
#else
#include "math.h"
#endif

#include "gaugewidget.h"
#include <iostream>
#include "ctype.h"

#define DEGREE2RADIANS(A) (A*M_PI/180)

namespace SPEEDMETER {

/**
 * 
 * 
 */
GaugeWidget::GaugeWidget(CImg<unsigned char>&container, size_t _width,
		size_t _height, size_t offset_x, size_t offset_y) :
		IntegerWidget(container, _width, _height, offset_x, offset_y), gauge(
				_width, _height, 1, 3, 0), degreeDiff(0), preValue(-1) {
}
/**
 * 
 * 
 */
GaugeWidget::~GaugeWidget() {
}

/**
 * 
 * 
 */
bool GaugeWidget::init(std::map<std::string, std::string> const &parameters) {
	radius = (std::min(getWidgetX(), getWidgetY()) / 2) - 4;
	center.X = getWidgetX() / 2;
	center.Y = getWidgetY() / 2;
	minDegree = 135;
	maxDegree = 270;
	degreeDiff = (maxDegree - minDegree);
	minValue = 0;
	maxValue = 90;

	setIndicator(minValue);
	return true;
}

/**
 * 
 * 
 */
static inline double COS(double t) {
	return cos(t);
}

/**
 * 
 * 
 */
static inline double SIN(double t) {
	return sin(t);
}

/**
 * 
 * 
 */
CImg<unsigned char>&GaugeWidget::setIndicator(int value) {
	if (value < minValue)
		value = minValue;
	else if (value > maxValue)
		value = maxValue;

	if (preValue != value) {
		preValue = value;
		double rad = minDegree + ((degreeDiff * value) / maxValue);
		int radiusLine = radius - 10;

		int x = center.X
				+ radiusLine * static_cast<int>(COS(DEGREE2RADIANS(rad)));
		int y = center.Y
				+ radiusLine * static_cast<int>(SIN(DEGREE2RADIANS(rad)));
		gauge.fill(0);
		//gauge.draw_ellipse(center.X, center.Y, radius, radius, 90.0, COLORS::RGB[COLORS::White], 1, 1);
		gauge.draw_line(center.X, center.Y, x, y, COLORS::RGB[COLORS::White],
				1.0, 0xFFFFFFFF);
	}
	draw_image(getOffsetX(), getOffsetY(), gauge);
	return *this;
}
}
