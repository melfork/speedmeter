#pragma once
#include "gpx.h"

namespace SPEEDMETER {

class ProjectionHelper {
	size_t width;
	size_t height;
	double scale;
	double x_mid;
	double y_mid;
public:
	ProjectionHelper(GPX const & gpx, size_t _width, size_t _height) {
		width = _width;
		height = _height;

		// x== lon y==lat
		double x_min = gpx.getMinLongitude();
		double x_max = gpx.getMaxLongitude();
		double y_min = gpx.getMinLatitude();
		double y_max = gpx.getMaxLatitude();

		// Calculate the middle
		x_mid = (x_min + x_max) / 2;
		y_mid = (y_min + y_max) / 2;

		// First get the scale factors needed to fit inside the plot panel.
		double scale_x = width / (x_max - x_min);
		double scale_y = height / (y_max - y_min);

		// choose the smallest scale
		scale = (scale_x > scale_y ? scale_y : scale_x);
	}

	virtual ~ProjectionHelper() {
	}

	inline void CoordinateToPoint(Coordinate const &coord, Point &point) const {
		point.X = (int) (width / 2 + scale * (coord.lon - x_mid));
		point.Y = (int) (height / 2 - scale * (coord.lat - y_mid));
	}
};

}

