#include "videoreader.h"
#include "libavutil/pixfmt.h"

namespace SPEEDMETER {

        /**
         *
         *
         */
        VideoReader::VideoReader():
            videoStream(-1),audioStream(-1),
            fileFormatCtx(NULL), videoCodecCtx(NULL),
            videoCodec(NULL),audioCodecCtx(NULL),audioCodec(NULL) {
        }

        /**
         *
         *
         */
        VideoReader::~VideoReader() {
                if (videoCodecCtx)
                        avcodec_close(videoCodecCtx);
                if (audioCodecCtx)
                        avcodec_close(audioCodecCtx);

                if (fileFormatCtx)
                        avformat_close_input(&fileFormatCtx);
        }

        /**
         *
         *
         */
        bool VideoReader::openVideoCodec() {
                bool ret=true;
                AVDictionary *optionsDict = NULL;

                // Get a pointer to the codec context for the video stream
                videoCodecCtx = fileFormatCtx->streams[videoStream]->codec;

                // Find the decoder for the video stream
                videoCodec = avcodec_find_decoder(videoCodecCtx->codec_id);
                if (videoCodec == NULL)
                        ret=false;      // Codec not found
                // Open codec
                else if (avcodec_open2(videoCodecCtx, videoCodec, &optionsDict) < 0)
                        ret=false;      // Could not open codec

                return ret;
        }

        /**
         *
         *
         */
        bool VideoReader::openAudioCodec() {
                bool ret=true;
                AVDictionary *optionsDict = NULL;

                // Get a pointer to the codec context for the video stream
                audioCodecCtx = fileFormatCtx->streams[audioStream]->codec;

                // Find the decoder for the video stream
                audioCodec = avcodec_find_decoder(audioCodecCtx->codec_id);
                if (audioCodec == NULL)
                        ret=false;      // Codec not found
                // Open codec
                else if (avcodec_open2(audioCodecCtx, audioCodec, &optionsDict) < 0)
                        ret=false;      // Could not open codec

                return ret;
        }


        /**
         *
         *
         */
        bool VideoReader::open(const char *filename)
        {
                // Open video file
                if (avformat_open_input(&fileFormatCtx, filename, NULL, NULL) != 0)
                        return false;   // Couldn't open file

                // Retrieve stream information
                if (avformat_find_stream_info(fileFormatCtx, NULL) < 0)
                        return false;   // Couldn't find stream information

                // Find the first video stream
                videoStream = -1;
                audioStream = -1;
                for (unsigned i = 0; i < fileFormatCtx->nb_streams; i++) {
                        switch (fileFormatCtx->streams[i]->codec->codec_type) {
                        case AVMEDIA_TYPE_VIDEO:
                                videoStream = i;
                                if (!openVideoCodec())
                                        return false;
                                break;
                        case AVMEDIA_TYPE_AUDIO:
                                audioStream = i;
                                if (!openAudioCodec())
                                        return false;
                                break;
                        }
                }
                return (videoStream!=-1);
        }

        /**
         *
         *
         */
        int VideoReader::width() {
                return (videoCodecCtx ? videoCodecCtx->width : -1);
        }

        /**
         *
         *
         */
        int VideoReader::height() {
                return (videoCodecCtx ? videoCodecCtx->height : -1);
        }

        /**
         *
         *
         */
        PixelFormat VideoReader::pixFormat() {
                return (videoCodecCtx ? videoCodecCtx->pix_fmt : PIX_FMT_NONE);
        }

        /**
         *
         *
         */
        VideoReader::FrameIterator::FrameIterator() {
                // Allocate video frame
                videoFrame = avcodec_alloc_frame();
                // Allocate audio frame
                videoFrame = avcodec_alloc_frame();
        }

        /**
         *
         *
         */
        VideoReader::FrameIterator::~FrameIterator() {
                if (videoFrame)
                        av_free(videoFrame);
                if (audioFrame)
                        av_free(audioFrame);
        }

        /**
         *
         *
         */
        VideoReader::FrameIterator VideoReader::getFirstFrame() {
                FrameIterator it;
                it.fileFormatCtx = fileFormatCtx;
                it.videoCodecCtx = videoCodecCtx;
                it.audioCodecCtx = audioCodecCtx;
                it.videoStream = videoStream;
                it.audioStream = audioStream;
                return it;
        }

        /**
         *
         *
         */
        int VideoReader::FrameIterator::getNextFrame(AVFrame **frame) {
		AVPacket packet;
		AVFrame *f=*frame;
		int ret=0;
                int frameFinished=0;


                // Read frames
                while (av_read_frame(fileFormatCtx, &packet) >= 0) {
                        // Is this a packet from the video stream?
                        if (packet.stream_index == videoStream) {
                                // Decode video frame
                                ret = videoStream;
				f = videoFrame;
                                avcodec_decode_video2(videoCodecCtx, f, &frameFinished, &packet);
                        } else if (packet.stream_index == audioStream) {
                                // Decode audio frame
                                ret = audioStream;
				f = audioFrame;
                                avcodec_get_frame_defaults(f);
                                if (avcodec_decode_audio4(audioCodecCtx, f, &frameFinished, &packet)<0)
                                        break;
                        }
                        av_free_packet(&packet);
                        if (frameFinished)
                                return ret;
                }
                return 0;
        }
}
