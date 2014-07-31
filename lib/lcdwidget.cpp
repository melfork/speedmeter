#include "lcdwidget.h"
#include <ctype.h>

#define LCD_SEPARATOR	(4)

namespace SPEEDMETER {

/**
 * 
 * 
 */
static LCD::ValuePattern getPattern(int value) {
	LCD::ValuePattern pat = LCD::LCD_None;
	if (isascii(value))
		switch (value) {
		case '0':
			pat = LCD::LCD_Zero;
			break;
		case '1':
			pat = LCD::LCD_One;
			break;
		case '2':
			pat = LCD::LCD_Two;
			break;
		case '3':
			pat = LCD::LCD_Three;
			break;
		case '4':
			pat = LCD::LCD_Four;
			break;
		case '5':
			pat = LCD::LCD_Five;
			break;
		case '6':
			pat = LCD::LCD_Six;
			break;
		case '7':
			pat = LCD::LCD_Seven;
			break;
		case '8':
			pat = LCD::LCD_Eight;
			break;
		case '9':
			pat = LCD::LCD_Nine;
			break;
		case 'a':
			pat = LCD::LCD_A;
			break;
		case 'b':
			pat = LCD::LCD_B;
			break;
		case 'c':
			pat = LCD::LCD_C;
			break;
		case 'd':
			pat = LCD::LCD_D;
			break;
		case 'e':
			pat = LCD::LCD_E;
			break;
		case 'f':
			pat = LCD::LCD_F;
			break;
		case 'g':
			pat = LCD::LCD_G;
			break;
		case 'h':
			pat = LCD::LCD_H;
			break;
		case 'j':
			pat = LCD::LCD_J;
			break;
		case 'l':
			pat = LCD::LCD_L;
			break;
		case 'n':
			pat = LCD::LCD_N;
			break;
		case 'o':
			pat = LCD::LCD_O;
			break;
		case 'p':
			pat = LCD::LCD_P;
			break;
		case 'q':
			pat = LCD::LCD_Q;
			break;
		case 'r':
			pat = LCD::LCD_R;
			break;
		case 't':
			pat = LCD::LCD_T;
			break;
		case 'u':
			pat = LCD::LCD_U;
			break;
		case 'y':
			pat = LCD::LCD_Y;
			break;
		case '-':
			pat = LCD::LCD_Dash;
			break;
		case '=':
			pat = LCD::LCD_Equals;
			break;
		}
	return pat;
}

/**
 * 
 * 
 */
LCDWidget::LCDWidget(CImg<unsigned char>&container, size_t _width,
		size_t _height, size_t offset_x, size_t offset_y) :
		StringWidget(container, _width, _height, offset_x, offset_y), lcdSize(
				3), lcdList(NULL) {
}

/**
 * 
 * 
 */
LCDWidget::~LCDWidget() {
	if (lcdList) {
		for (size_t i = 0; i < lcdSize; i++)
			delete lcdList[i];
		delete[] lcdList;
	}
}

/**
 * 
 * 
 */
bool LCDWidget::init(std::map<std::string, std::string> const &parameters) {
	size_t lcdw = (getWidgetX() - (LCD_SEPARATOR * 2)
			- (LCD_SEPARATOR * (lcdSize - 1))) / lcdSize;
	size_t lcdh = getWidgetY() - (LCD_SEPARATOR * 2);
	size_t offsetX = getOffsetX() + LCD_SEPARATOR;
	size_t offsetY = getOffsetY() + LCD_SEPARATOR;

	lcdList = new LCD *[lcdSize];
	for (size_t i = 0; i < lcdSize; i++) {
		lcdList[i] = new LCD(*this, lcdw, lcdh, offsetX, offsetY);
		offsetX += (lcdw + LCD_SEPARATOR);
	}
	return true;
}

/**
 * 
 * 
 */
CImg<unsigned char>&LCDWidget::setIndicator(const char *value) {
	if (lcdList) {
		size_t len = 0;
		if (value) {
			len = strlen(value);
			if (len > lcdSize)
				len = lcdSize;
		}

		for (size_t i = 0; i < len; i++)
			lcdList[i]->set(getPattern(value[i]));

		for (size_t i = len; i < lcdSize; i++)
			lcdList[i]->set(LCD::LCD_None);
	}
	return *this;
}
}
