#pragma once

#include "speedmeter.h"
#include <time.h>
#include <expat.h>
#include <list>
#include <string>
#include "basictypes.h"

#define MAX_GPX_BUFFER (1024*32)

namespace SPEEDMETER {

class SpeedMeterDllExport
GPX {
public:
	struct TrkPoint {
		Coordinate coord;
		double ele;
		time_t time;

		bool isEmpty() const {
			return time <= 0;
		}
	};
	GPX();
	virtual ~ GPX();
	const std::list<TrkPoint> &getTrackList() const;
	bool load(const char *filename);
	size_t getTotalPoints() const {
		return totalTrks;
	}
	double getMaxElevation() const {
		return maxEle;
	}
	double getMinElevation() const {
		return minEle;
	}
	double getMaxLatitude() const {
		return maxLat;
	}
	double getMinLatitude() const {
		return minLat;
	}
	double getMaxLongitude() const {
		return maxLon;
	}
	double getMinLongitude() const {
		return minLon;
	}
	Bounds getBounds() const {
		Bounds ret;
		ret.northEast.lat=maxLat;
		ret.northEast.lon=maxLon;
		ret.southWest.lat=minLat;
		ret.southWest.lon=minLon;
		return ret;
	}
private:
	static void XMLStartElementHandler(void *userData, const XML_Char * name,
			const XML_Char ** atts);
	static void XMLEndElementHandler(void *userData, const XML_Char * name);
	static void XMLCharactersHandler(void *userData, const XML_Char * s,
			int len);

	time_t trkTime;
	//std::string trkName;

	std::list<TrkPoint> trks;
	size_t trks_index;
	size_t trks_pos;
	double minEle;
	double maxEle;
	double maxLat;
	double maxLon;
	double minLat;
	double minLon;
	size_t totalTrks;
};

}
