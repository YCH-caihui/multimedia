#ifndef XDEMUX_H
#define XDEMUX_H

#include <iostream>
#include <mutex>
extern "C" {
#include "libavformat/avformat.h"
}

using namespace std;

static double r2d(AVRational r)
{
    return r.den == 0 ? 0 : (double) r.num / (double)r.den;
}

class XDemux
{
protected:
    mutex mux;
    AVFormatContext * ic;
    int videoStream = 0;
    int audioStream = 1;
public:
    //打开媒体文件，或者流媒体rtmp http rstp
    virtual bool open(const char * url);

    //空间需要调用者释放，释放AVPacket 对象空间和数据空间
    virtual AVPacket * read();

    //获取视频参数
    AVCodecParameters * copyVPara();

    //获取音频参数
    AVCodecParameters * copyAPara();

    //seek 位置 pos 0.0~1.0
    virtual bool seek(double pos);

    virtual void clear();
    virtual void close();

    XDemux();
    virtual ~XDemux();
};

#endif // XDEMUX_H
