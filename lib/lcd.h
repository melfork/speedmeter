#pragma once
#include "widgetbase.h"

namespace SPEEDMETER {

class LCD: public CImg<unsigned char> {
	size_t lcdheight;
	size_t lcdwidth;
	size_t elementWidth;
	size_t offsetX, offsetY;
	Point segPoints[7][6];
	void initialize();
public:
	enum ValuePattern {
		LCD_None = 0,
		LCD_Zero = 0x77,
		LCD_One = 0x24,
		LCD_Two = 0x5D,
		LCD_Three = 0x6D,
		LCD_Four = 0x2E,
		LCD_Five = 0x6B,
		LCD_Six = 0x7B,
		LCD_Seven = 0x25,
		LCD_Eight = 0x7F,
		LCD_Nine = 0x6F,
		LCD_A = 0x3F,
		LCD_B = 0x7A,
		LCD_C = 0x53,
		LCD_D = 0x7C,
		LCD_E = 0x5B,
		LCD_F = 0x1B,
		LCD_G = 0x73,
		LCD_H = 0x3E,
		LCD_J = 0x74,
		LCD_L = 0x52,
		LCD_N = 0x38,
		LCD_O = 0x78,
		LCD_P = 0x1F,
		LCD_Q = 0x2F,
		LCD_R = 0x18,
		LCD_T = 0x5A,
		LCD_U = 0x76,
		LCD_Y = 0x6E,
		LCD_Dash = 0x8,
		LCD_Equals = 0x48
	};

	LCD(CImg<unsigned char>&container, size_t _width, size_t _height,
			size_t offset_x, size_t offset_y);
	LCD(size_t width, size_t height);
	virtual ~ LCD();

	CImg<unsigned char>&set(ValuePattern pattern);
};

}
