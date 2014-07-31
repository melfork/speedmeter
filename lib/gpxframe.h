#pragma once

namespace SPEEDMETER {

struct GPXFrame {
	double distance;	// meters
	double speed;		// meters / seconds
	size_t timestamp;	// seconds
	double elevation;
	double incline;		// degrees
};
}
