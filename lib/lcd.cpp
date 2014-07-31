#include "lcd.h"
#include <algorithm>

namespace SPEEDMETER {

/**
 * 
 * 
 */
LCD::LCD(CImg<unsigned char>&container, size_t _width, size_t _height,
		size_t offset_x, size_t offset_y) :
		CImg<unsigned char>(container, true), lcdwidth(_width), lcdheight(
				_height), offsetX(offset_x), offsetY(offset_y) {
	initialize();
}

/**
 * 
 * 
 */
LCD::LCD(size_t _width, size_t _height) :
		CImg<unsigned char>(_width, _height, 1, 3, 0), lcdwidth(_width), lcdheight(
				_height), offsetX(0), offsetY(0) {
	initialize();
}

/**
 *
 *
 */
void LCD::initialize() {
	if ((elementWidth = std::min(lcdwidth, lcdheight) / 8) > 10)
		elementWidth = 10;
	if (elementWidth < 3)
		elementWidth = 1;

	int halfHeight = lcdheight / 2, halfWidth = elementWidth / 2;

	int p = 0;
	segPoints[p][0].X = elementWidth + 1;
	segPoints[p][0].Y = 0;
	segPoints[p][1].X = lcdwidth - elementWidth - 1;
	segPoints[p][1].Y = 0;
	segPoints[p][2].X = lcdwidth - halfWidth - 1;
	segPoints[p][2].Y = halfWidth;
	segPoints[p][3].X = lcdwidth - elementWidth - 1;
	segPoints[p][3].Y = elementWidth;
	segPoints[p][4].X = elementWidth + 1;
	segPoints[p][4].Y = elementWidth;
	segPoints[p][5].X = halfWidth + 1;
	segPoints[p][5].Y = halfWidth;

	p++;
	segPoints[p][0].X = 0;
	segPoints[p][0].Y = elementWidth + 1;
	segPoints[p][1].X = halfWidth;
	segPoints[p][1].Y = halfWidth + 1;
	segPoints[p][2].X = elementWidth;
	segPoints[p][2].Y = elementWidth + 1;
	segPoints[p][3].X = elementWidth;
	segPoints[p][3].Y = halfHeight - halfWidth - 1;
	segPoints[p][4].X = 4;
	segPoints[p][4].Y = halfHeight - 1;
	segPoints[p][5].X = 0;
	segPoints[p][5].Y = halfHeight - 1;

	p++;
	segPoints[p][0].X = lcdwidth - elementWidth;
	segPoints[p][0].Y = elementWidth + 1;
	segPoints[p][1].X = lcdwidth - halfWidth;
	segPoints[p][1].Y = halfWidth + 1;
	segPoints[p][2].X = lcdwidth;
	segPoints[p][2].Y = elementWidth + 1;
	segPoints[p][3].X = lcdwidth;
	segPoints[p][3].Y = halfHeight - 1;
	segPoints[p][4].X = lcdwidth - 4;
	segPoints[p][4].Y = halfHeight - 1;
	segPoints[p][5].X = lcdwidth - elementWidth;
	segPoints[p][5].Y = halfHeight - halfWidth - 1;

	p++;
	segPoints[p][0].X = elementWidth + 1;
	segPoints[p][0].Y = halfHeight - halfWidth;
	segPoints[p][1].X = lcdwidth - elementWidth - 1;
	segPoints[p][1].Y = halfHeight - halfWidth;
	segPoints[p][2].X = lcdwidth - 5;
	segPoints[p][2].Y = halfHeight;
	segPoints[p][3].X = lcdwidth - elementWidth - 1;
	segPoints[p][3].Y = halfHeight + halfWidth;
	segPoints[p][4].X = elementWidth + 1;
	segPoints[p][4].Y = halfHeight + halfWidth;
	segPoints[p][5].X = 5;
	segPoints[p][5].Y = halfHeight;

	p++;
	segPoints[p][0].X = 0;
	segPoints[p][0].Y = halfHeight + 1;
	segPoints[p][1].X = 4;
	segPoints[p][1].Y = halfHeight + 1;
	segPoints[p][2].X = elementWidth;
	segPoints[p][2].Y = halfHeight + halfWidth + 1;
	segPoints[p][3].X = elementWidth;
	segPoints[p][3].Y = lcdheight - elementWidth - 1;
	segPoints[p][4].X = halfWidth;
	segPoints[p][4].Y = lcdheight - halfWidth - 1;
	segPoints[p][5].X = 0;
	segPoints[p][5].Y = lcdheight - elementWidth - 1;

	p++;
	segPoints[p][0].X = lcdwidth - elementWidth;
	segPoints[p][0].Y = halfHeight + halfWidth + 1;
	segPoints[p][1].X = lcdwidth - 4;
	segPoints[p][1].Y = halfHeight + 1;
	segPoints[p][2].X = lcdwidth;
	segPoints[p][2].Y = halfHeight + 1;
	segPoints[p][3].X = lcdwidth;
	segPoints[p][3].Y = lcdheight - elementWidth - 1;
	segPoints[p][4].X = lcdwidth - halfWidth;
	segPoints[p][4].Y = lcdheight - halfWidth - 1;
	segPoints[p][5].X = lcdwidth - elementWidth;
	segPoints[p][5].Y = lcdheight - elementWidth - 1;

	p++;
	segPoints[p][0].X = elementWidth + 1;
	segPoints[p][0].Y = lcdheight - elementWidth;
	segPoints[p][1].X = lcdwidth - elementWidth - 1;
	segPoints[p][1].Y = lcdheight - elementWidth;
	segPoints[p][2].X = lcdwidth - halfWidth - 1;
	segPoints[p][2].Y = lcdheight - halfWidth;
	segPoints[p][3].X = lcdwidth - elementWidth - 1;
	segPoints[p][3].Y = lcdheight;
	segPoints[p][4].X = elementWidth + 1;
	segPoints[p][4].Y = lcdheight;
	segPoints[p][5].X = halfWidth + 1;
	segPoints[p][5].Y = lcdheight - halfWidth;

	set(LCD_None);
}

/**
 * 
 * 
 */
LCD::~LCD() {
}

/**
 * 
 * 
 */
CImg<unsigned char>&LCD::set(ValuePattern pattern) {
	CImg<int> coords(6, 2);
	for (int seg = 0, it = 1; seg < 7; seg++, it <<= 1) {
		for (int i = 0; i < 6; i++) {
			coords(i, 0) = segPoints[seg][i].X + offsetX;
			coords(i, 1) = segPoints[seg][i].Y + offsetY;
		}
		draw_polygon(coords,
				(pattern & it ?
						COLORS::RGB[COLORS::Yellow] : COLORS::RGB[COLORS::Gray]));
	}
	return *this;
}
}
