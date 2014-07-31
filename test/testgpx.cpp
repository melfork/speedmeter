#include "gpx.h"
#include <iostream>
#include <math.h>
#include "gpxframelist.h"

using namespace SPEEDMETER;

/**
 *
 *
 */
int main(int argc, char *argv[])
{
	if (argc == 2) {
		GPX gpx;
		if (!gpx.load(argv[1]))
			return 0;

		std::cout << "Total points "  << gpx.getTotalPoints()  << std::endl;
		std::cout << "Max Elevation " << gpx.getMaxElevation() << std::endl;
		std::cout << "Min Elevation " << gpx.getMinElevation() << std::endl;
		std::cout << "Max Latitude "  << gpx.getMaxLatitude()  << std::endl;
		std::cout << "Min Latitude "  << gpx.getMinLatitude()  << std::endl;
		std::cout << "Max Longitude " << gpx.getMaxLongitude() << std::endl;
		std::cout << "Min Longitude " << gpx.getMinLongitude() << std::endl;
		
		GPXFrameList l(gpx.getTrackList());

		std::cout << "Max Trk Distance "  << l.getMaxDistance()  << std::endl;
		std::cout << "Min Trk Distance "  << l.getMinDistance()  << std::endl;
		std::cout << "Max Speed "         << l.getMaxSpeed()     << std::endl;
		std::cout << "Min Speed "         << l.getMinSpeed()     << std::endl;
		std::cout << "Max Incline "       << l.getMaxIncline()   << std::endl;
		std::cout << "Min Incline "       << l.getMinIncline()   << std::endl;
	}
}
