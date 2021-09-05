#ifndef XDECODE_H
#define XDECODE_H

#include <iostream>
#include <mutex>
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

class XDecode
{
private:
    AVCodecContext * codecContext = 0;
    std::mutex mux;
public:
    bool isAudio = false;
    //打开解码器
    virtual  bool open(AVCodecParameters * para);

    virtual void close();
    virtual void clear();
    XDecode();
    virtual ~XDecode();
};

#endif // XDECODE_H
