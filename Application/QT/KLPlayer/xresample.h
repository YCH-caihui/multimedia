#ifndef XRESAMPLE_H
#define XRESAMPLE_H

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
#include <mutex>
}

class XResample
{
public:
    XResample();
    //输出参数和输入参数一致除了
    virtual bool open(AVCodecParameters * para);
    ~XResample();
protected:
    std::mutex mutex;
};

#endif // XRESAMPLE_H
