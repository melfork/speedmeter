#include "gpxwidget.h"

namespace SPEEDMETER {

/**
 *
 *
 */
GPXWidget::GPXWidget(CImg<unsigned char>&container, GPX const &_gpx, int _width,
		int _height, size_t _offset_x, size_t _offset_y) :
		TimeWidget(container, _width, _height, _offset_x, _offset_y),
		proj(NULL), helper(NULL), gpx(_gpx), point(6, 6, 1, 3, 1), crop(6, 6, 1, 3, 1) {
}

/**
 *
 *
 */
GPXWidget::~GPXWidget() {
	if (proj)
		delete proj;
	if (helper)
		delete helper;
}

/**
 *
 *
 */
bool GPXWidget::init(std::map<std::string, std::string> const &parameters) {
	if (proj)
		return false;
	proj = new GPXProjection(getWidgetX() - 2, getWidgetY() - 2);

	helper = new ProjectionHelper(gpx, getWidgetX() - 2, getWidgetY() - 2);

	point.draw_circle(3, 3, 2, COLORS::RGB[COLORS::Blue]).blur(1.5);
	return proj->draw(gpx, COLORS::White);
}

/**
 *
 *
 */
CImg<unsigned char>& GPXWidget::setIndicator(time_t value) {
	return *this;
}
}
