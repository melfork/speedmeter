#include "textwidget.h"
#include <ctype.h>

namespace SPEEDMETER {

/**
 * 
 * 
 */
TextWidget::TextWidget(CImg<unsigned char>&container, int _width, int _height,
		size_t offset_x, size_t offset_y) :
		StringWidget(container, _width, _height, offset_x, offset_y), text(
				_width, _height, 1, 3, 0) {
}

/**
 * 
 * 
 */
TextWidget::~TextWidget() {
}

/**
 * 
 * 
 */
bool TextWidget::init(std::map<std::string, std::string> const &parameters) {
	text.fill(0);
	return true;
}

/**
 * 
 * 
 */
CImg<unsigned char>&TextWidget::setIndicator(int value) {
	char aux[32];
	sprintf(aux, "%d", value);
	return setIndicator(aux);
}

/**
 * 
 * 
 */
CImg<unsigned char>&TextWidget::setIndicator(const char *value) {
	text.fill(0);
	text.draw_text(2, 2, value, COLORS::RGB[COLORS::White], 0, 1, 10);
	draw_image(getOffsetX(), getOffsetY(), text);
	return *this;
}
}
