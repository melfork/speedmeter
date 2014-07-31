#include "basictypes.h"

#ifdef _WINDOWS
#define _USE_MATH_DEFINES
#include <cmath>
#else
#include "math.h"
#endif

#define ZOOM_LEVELS (18)

namespace SPEEDMETER {

class MercatorProjection {
	double pixels_per_lon_degree[ZOOM_LEVELS];
	double pixels_per_lon_radian[ZOOM_LEVELS];
	Point pixel_origo[ZOOM_LEVELS];
	int pixel_range[ZOOM_LEVELS];
public:
	MercatorProjection();
	Point fromLatLngToPixel(Coordinate const &lat_lng, int zoom) const;
	int calculateBoundsZoomLevel(Bounds const &bounds, int width, int height) const;
	int calculateBoundsZoomLevel(
		Coordinate const &top_right,
		Coordinate const &bottom_left, int width, int height) const;
	Bounds CalcBoundsFromPoints(const Coordinate *coords) const;
	Coordinate CalcCenterFromBounds(Bounds const &bounds);
};
}
