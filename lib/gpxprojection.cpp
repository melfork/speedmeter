#include "gpxprojection.h"
#include "projection.h"
#include <math.h>

namespace SPEEDMETER {

/**
 * 
 * 
 */
GPXProjection::GPXProjection(size_t _width, size_t _height) :
		CImg<unsigned char>(_width, _height, 1, 3, 1), width(_width), height(
				_height) {
}

/**
 * 
 * 
 */
GPXProjection::GPXProjection(CImg<unsigned char>&container, size_t _width,
		size_t _height) :
		CImg<unsigned char>(container, true), width(_width), height(_height) {
}

/**
 * 
 * 
 */
bool GPXProjection::draw(GPX const & gpx, COLORS::ColorName color) {
	ProjectionHelper helper(gpx, width, height);

	std::list<GPX::TrkPoint> const &l = gpx.getTrackList();
	Point p1, p2;

	std::list<GPX::TrkPoint>::const_iterator i = l.begin();
	std::list<GPX::TrkPoint>::const_iterator e = l.end();

	while (i != e) {
		GPX::TrkPoint const &trk = (*i);
		i++;
		if (i != e) {
			GPX::TrkPoint const &next_trk = (*i);
			helper.CoordinateToPoint(trk.coord, p1);
			helper.CoordinateToPoint(next_trk.coord, p2);

			draw_line(p1.X, p1.Y, p2.X, p2.Y, COLORS::RGB[color], 1.0,
					0xFFFFFFFF);
		} else
			break;
		i++;
	}
	return true;

}
}
