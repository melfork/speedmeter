#pragma once
#include <math.h>
#include "videoreader.h"

namespace SPEEDMETER {

class FrameConverter {
	SwsContext *context;
	AVFrame *pConvertedFrame;
	int inHeight;
	PixelFormat outPixFormat;
	unsigned char *buffer;
public:
	FrameConverter(int inWidth, int inHeight, PixelFormat inPixFormat,
			int outWidth, int outHeight, PixelFormat outPixFormat);
	virtual ~FrameConverter();
	const AVFrame *convert(AVFrame *f);
};
}

