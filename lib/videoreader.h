#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

namespace SPEEDMETER {

        class VideoReader {
                int videoStream;
                int audioStream;
                AVFormatContext *fileFormatCtx;
                AVCodecContext *videoCodecCtx;
                AVCodec *videoCodec;
                AVCodecContext *audioCodecCtx;
                AVCodec *audioCodec;
                bool openVideoCodec();
                bool openAudioCodec();
         public:
                VideoReader();
                virtual ~ VideoReader();
                bool open(const char *filename);
                int width();
                int height();
                PixelFormat pixFormat();

                class FrameIterator {
                        AVFormatContext *fileFormatCtx;
                        AVCodecContext *videoCodecCtx;
                        AVCodecContext *audioCodecCtx;
			AVFrame *videoFrame;
			AVFrame *audioFrame;
                        int videoStream;
                        int audioStream;
                public:
                        friend class VideoReader;
                        FrameIterator();
                        virtual ~ FrameIterator();
			// return the frame stream, 0 if ended
                        int getNextFrame(AVFrame **frame);
                        bool isEnded();
                };
         VideoReader::FrameIterator getFirstFrame();
        };
}
