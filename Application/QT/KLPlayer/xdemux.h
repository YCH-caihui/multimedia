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
    XDemux();
    virtual ~XDemux();
};

#endif // XDEMUX_H
