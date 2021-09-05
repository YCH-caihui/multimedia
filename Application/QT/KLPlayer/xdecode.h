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

    //发送到解码线程，不管成功与否都释放pkt空间（对象和媒体内容）
    virtual bool send(AVPacket * pkt);

    //获取解码后数据，一次send可能需要多次recv,获取缓冲的数据send, null在recv多次
    //空间复制一份，由调用者释放
    virtual AVFrame* recv();

    virtual void close();
    virtual void clear();
    XDecode();
    virtual ~XDecode();
};

#endif // XDECODE_H
