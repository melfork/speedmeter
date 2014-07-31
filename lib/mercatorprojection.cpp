#include "mercatorprojection.h"
#include <stdlib.h>


static inline double BOUND(double value, double opt_min, double opt_max) {
	double a1=(value > opt_min ? value : opt_min);
	return (a1 < opt_max ? a1 : opt_max);
}

#define DEGREE2RADIANS(A) (A*M_PI/180)

namespace SPEEDMETER {

MercatorProjection::MercatorProjection() {
    int pixels = 256;
    const double _2PI_=(2.0*M_PI);
   
    for (int z=0; z<ZOOM_LEVELS; z++) {
        pixels_per_lon_degree[z]=pixels / 360.0;
        pixels_per_lon_radian[z]=(pixels / _2PI_);
        pixel_origo[z].X=pixel_origo[z].Y=(pixels / 2);
        pixel_range[z]=pixels;
        pixels = pixels * 2;
    }
}

   /*Given lat/lng and a zoom level, returns a Point instance.

    This method takes in a lat/lng and a _test_ zoom level and based on that it
    calculates at what pixel this lat/lng would be on the map given the zoom
    level.  This method is used by CalculateBoundsZoomLevel to see if this
    _test_ zoom level will allow us to fit these bounds in our given map size.

    Args:
      lat_lng: A list of a lat/lng point [lat, lng]
      zoom: A list containing the width/height in pixels of the map.

    Returns:
      A Point instance in pixels.
    */
Point MercatorProjection::fromLatLngToPixel(Coordinate const &lat_lng, int zoom) const {
	Point const &o = pixel_origo[zoom];
	Point ret;
	ret.X = round(o.X + lat_lng.lon * pixels_per_lon_degree[zoom]);
	double sinl=sin(DEGREE2RADIANS(lat_lng.lat));
	double siny = BOUND(sinl, -0.9999, 0.9999);
	ret.Y = round(o.Y + 0.5 * log((1.0 + siny) /
		(1.0 - siny)) * -pixels_per_lon_radian[zoom]);
	return ret;
}
 
 /*Given lat/lng bounds, returns map zoom level.

    This method is used to take in a bounding box (southwest and northeast
    bounds of the map view we want) and a map size and it will return us a zoom
    level for our map.  We use this because if we take the bottom left and
    upper right on the map we want to show, and calculate what pixels they
    would be on the map for a given zoom level, then we can see how many pixels
    it will take to display the map at this zoom level.  If our map size is
    within this many pixels, then we have the right zoom level.

    Args:
      bounds: A list of length 2, each holding a list of length 2. It holds
        the southwest and northeast lat/lng bounds of a map.  It should look
        like this: [[southwestLat, southwestLat], [northeastLat, northeastLng]]
      view_size: A list containing the width/height in pixels of the map.

    Returns:
      An int zoom level.
    */
 int MercatorProjection::calculateBoundsZoomLevel(Bounds const &bounds, int width, int height) const {
	 Coordinate top_right;
	 Coordinate bottom_left;
	 top_right.lat=bounds.northEast.lat;
	 top_right.lon=bounds.northEast.lon;

	 bottom_left.lat=bounds.southWest.lat;
	 bottom_left.lon=bounds.southWest.lon;
	 return calculateBoundsZoomLevel(top_right,bottom_left,width,height);
 }

 int MercatorProjection::calculateBoundsZoomLevel(Coordinate const &top_right, Coordinate const &bottom_left, int width, int height) const {
   
    for (int z=ZOOM_LEVELS-1; z>=0; z--) {
        Point bottom_left_pixel = fromLatLngToPixel(bottom_left, z);
        Point top_right_pixel = fromLatLngToPixel(top_right, z);
        if (bottom_left_pixel.X > top_right_pixel.X)
            bottom_left_pixel.X -= pixel_range[z];
       
        if (abs(top_right_pixel.X - bottom_left_pixel.X) <= width &&
            abs(top_right_pixel.Y - bottom_left_pixel.Y) <= height)
                return z;
    }
    return 0;
}

 /*Calculates the max/min lat/lng in the lists.

  This method takes in a list of lats and a list of lngs, and outputs the
  southwest and northeast bounds for these points.  We use this method when we
  have done a search for points on the map, and we get multiple results.  In
  the results we don't get a bounding box so this method calculates it for us.

  Args:
    lats: List of latitudes
    lngs: List of longitudes

  Returns:
    A list of length 2, each holding a list of length 2.  It holds
    the southwest and northeast lat/lng bounds of a map.  It should look
    like this: [[southwestLat, southwestLat], [northeastLat, northeastLng]]
  */
 Bounds MercatorProjection::CalcBoundsFromPoints(const Coordinate *coords) const {
    double west=180.0,
	east=-180.0,
	north=-90.0,
	south=90.0;       
    for (const Coordinate *coord; coord!=NULL; coord++) {
	if (coord->lon < west)
		west=coord->lon;
	if (coord->lon > east)
		east=coord->lon;
	if (coord->lat > north)
		north=coord->lat;
	if (coord->lat < south)
		south=coord->lat;
    }
 
    Bounds ret;
    ret.southWest.lat=south;
    ret.southWest.lon=west;
    ret.northEast.lat=north;
    ret.northEast.lon=east;
    return ret;
}

 /*Calculates the center point given southwest/northeast lat/lng pairs.

  Given southwest and northeast bounds, this method will return the center 
  point.  We use this method when we have done a search for points on the map,
  and we get multiple results.  In the results we don't get anything to
  calculate the center point of the map so this method calculates it for us.

  Args:
    bounds: A list of length 2, each holding a list of length 2. It holds
      the southwest and northeast lat/lng bounds of a map.  It should look
      like this: [[southwestLat, southwestLat], [northeastLat, northeastLng]]

  Returns:
    An dict containing keys lat and lng for the center point.
  */
Coordinate MercatorProjection::CalcCenterFromBounds(Bounds const &bounds) {
    double north = bounds.northEast.lat;
    double south = bounds.southWest.lat;
    double east = bounds.northEast.lon;
    double west = bounds.southWest.lon;
   
    Coordinate center;
    center.lat = north - ((north - south) / 2.0);
    center.lon = east - ((east - west) / 2.0);
    return center;
}

}
