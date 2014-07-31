#include "frameconverter.h"

namespace SPEEDMETER {

/**
 *
 *
 */
FrameConverter::FrameConverter(int _inWidth, int _inHeight,
		PixelFormat _inPixFormat, int _outWidth, int _outHeight,
		PixelFormat _outPixFormat) {

	context = sws_getContext(_inWidth, _inHeight, _inPixFormat, _outWidth,
			_outHeight, _outPixFormat, SWS_BICUBIC, NULL, NULL, NULL);

	// Allocate an AVFrame structure
	pConvertedFrame = avcodec_alloc_frame();

	// Determine required buffer size and allocate buffer
	int numBytes = avpicture_get_size(PIX_FMT_RGB24, _inWidth, _inHeight);

	buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

	avpicture_fill((AVPicture *) pConvertedFrame, buffer, PIX_FMT_RGB24,
			_inWidth, _inHeight);

	inHeight = _inHeight;
	outPixFormat = _outPixFormat;
}

/**
 *
 *
 */
FrameConverter::~FrameConverter() {
	av_free(buffer);
	av_free(pConvertedFrame);
	sws_freeContext(context);
}

/**
 *
 *
 */
const AVFrame *FrameConverter::convert(AVFrame *f) {
	sws_scale(context, f->data, f->linesize, 0, inHeight, pConvertedFrame->data,
			pConvertedFrame->linesize);
	return pConvertedFrame;
}
}

