#pragma once

namespace SPEEDMETER {

struct Point {
	int X, Y;
};

struct Point3d: Point {
	int Z;
};

struct Line {
	Point A, B;
};

struct Coordinate {
	double lat;
	double lon;
};

struct Bounds {
    Coordinate southWest, northEast;
};

}
