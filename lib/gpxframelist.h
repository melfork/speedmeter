#pragma once
#include "gpx.h"
#include "gpxframe.h"

namespace SPEEDMETER {

class GPXFrameList {
	double max_distance, min_distance, max_speed, min_speed, max_incline,
			min_incline;
	size_t listSize;
	GPXFrame *f;
public:
	GPXFrameList(const std::list<GPX::TrkPoint> &l);
	virtual ~GPXFrameList();
	const GPXFrame &operator[](int index) const;

	double getMaxDistance() const {
		return max_distance;
	}
	double getMinDistance() const {
		return min_distance;
	}
	double getMaxSpeed() const {
		return max_speed;
	}
	double getMinSpeed() const {
		return min_speed;
	}
	double getMaxIncline() const {
		return max_incline;
	}
	double getMinIncline() const {
		return min_incline;
	}

};
}

