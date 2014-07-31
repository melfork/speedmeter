#pragma once

#include "speedmeter.h"
#include "CImg.h"
#include <map>
#include "colors.h"
#include "gpx.h"

using namespace cimg_library;

namespace SPEEDMETER {

/**
 * 
 * 
 */
class SpeedMeterDllExport
GPXProjection: public CImg<unsigned char> {
	size_t width;
	size_t height;
public:
	GPXProjection(size_t width, size_t height);
	GPXProjection(CImg<unsigned char>&container, size_t width, size_t height);
	virtual ~GPXProjection() {
	}
	bool draw(GPX const & gpx, COLORS::ColorName color);
};
}

