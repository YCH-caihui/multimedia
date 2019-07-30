#ifndef H264DECODER_H
#define H264DECODER_H
extern "C"{
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libswscale/swscale.h"
#include "libavformat/avformat.h"
}

typedef struct rgbDataDefine
{
    int width;
    int height;
    int bufferSize;
    char * dateBuffer;
}RGBData_Define;

class H264Decoder
{
public:
    H264Decoder();
    ~H264Decoder();

    int DecodeH264Frames(unsigned char * inputBuffer, int aLength, RGBData_Define * outputRGBData);

private:
    int pictureWidth;
    int setRecordREsolveState;
    AVCodec *  pCodec;
    AVCodecContext * pCodecCtx;
    AVFrame *        pVideoFrame;
    AVPacket         pAvPacket;
    AVPicture        outPicture;
    struct SwsContext * img_conver_ctx;

    void copyDecodedFrame(unsigned char * src);

};

#endif // H264DECODER_H
