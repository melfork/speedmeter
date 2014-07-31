#ifdef _WINDOWS
#define _USE_MATH_DEFINES
#include <cmath>
#else
#include <unistd.h>
#include "math.h"
#endif

#include <stdio.h>
#include "gpxframelist.h"
#include <math.h>
#include "basictypes.h"
#include <inttypes.h>

#define EARTHRADIUS (3958.75)
#define MeterConversionFactor (1609.0)
#define DEG2RAD(angdeg) ((angdeg) / 180.0 * M_PI)

#define UPDATE_MAX_MIN(a,M,m) if (a>M) M=a; if (a<m) m=a

namespace SPEEDMETER {

/**
 *
 *
 */
static time_t timeDiff(const GPX::TrkPoint * a, const GPX::TrkPoint * b) {
	return b->time - a->time;
}

/**
 *
 *
 */
static double calculateIncline(double distance, const GPX::TrkPoint *a,
		const GPX::TrkPoint *b) {
	double aux = (fabs(a->ele - b->ele) / distance);
	return aux * 100.0;
}

/*
 * 
 *
 */
static double calculateDistance(const Coordinate &c1, const Coordinate &c2) {
	double dLat = DEG2RAD((c2.lat - c1.lat));
	double dLng = DEG2RAD((c2.lon - c1.lon));
	double a = pow(sin(dLat / 2), 2)
			+ cos(DEG2RAD(c1.lat)) * cos(DEG2RAD(c2.lat))
					* pow(sin(dLng / 2), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double dist = EARTHRADIUS * c;

	return (dist * MeterConversionFactor);
}

/*
 * 
 *
 */
GPXFrameList::GPXFrameList(const std::list<GPX::TrkPoint>&l) {
	listSize = l.size();
	f = new GPXFrame[listSize];
	size_t ind = 0, sindex = 0;
	const GPX::TrkPoint *points[2];

	std::list<GPX::TrkPoint>::const_iterator e = l.end();

	for (std::list<GPX::TrkPoint>::const_iterator i = l.begin(); i != e; i++) {
		const GPX::TrkPoint & point = (*i);
		points[ind] = &point;
		ind++;
		if (ind == 2) {
			double distance = calculateDistance(points[0]->coord,
					points[1]->coord);
			UPDATE_MAX_MIN(distance, max_distance, min_distance);
			f[sindex].distance = distance;

			double speed = f[sindex].distance / timeDiff(points[0], points[1]);
			UPDATE_MAX_MIN(speed, max_speed, min_speed);
			f[sindex].speed = speed;

			double incline = calculateIncline(f[sindex].distance, points[0],
					points[1]);
			UPDATE_MAX_MIN(incline, max_incline, min_incline);
			f[sindex].incline = incline;

			printf("%.2f distance. %.2f speed. %.2f incline\n",distance,speed,incline);
			points[0] = points[1];
			ind = 1;
			sindex++;
		}
	}
}

/*
 * 
 *
 */
GPXFrameList::~GPXFrameList() {
	delete[] f;
}

/*
 * 
 *
 */
const GPXFrame &GPXFrameList::operator[](int index) const {
	return f[index];
}
}

