#include "xdecode.h"


bool XDecode::open(AVCodecParameters *para)
{
    if(!para) return false;
    close();
    //解码器打开
    //找到解码器
    AVCodec * vcodec = avcodec_find_decoder(para->codec_id);
    if(!vcodec)
    {
        avcodec_parameters_free(&para);
        std::cout << "can't find the codex id" << para->codec_id << std::endl;
        return false;
    }
    std::cout << "find the AVCodec " << para->codec_id << std::endl;

    mux.lock();
    codecContext = avcodec_alloc_context3(vcodec);

    //配置解码上下文参数
    avcodec_parameters_to_context(codecContext, para);
    avcodec_parameters_free(&para);

    codecContext->thread_count = 8;

    //打开解码上下文
    int re = avcodec_open2(codecContext, vcodec, nullptr);
    if(re != 0)
    {
        avcodec_free_context(&codecContext);
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        std::cout << "avcodec_open2 failed:" << buf << std::endl;
        return false;

    }
    mux.unlock();
    std::cout << "avcodec_open2 succes" << std::endl;
    return true;
}


bool XDecode::send(AVPacket *pkt)
{
    //容错处理
    if(!pkt || pkt->size <= 0  || !pkt->data) return false;
    mux.lock();
    if(!codecContext)
    {
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(codecContext, pkt);
    mux.unlock();
    av_packet_free(&pkt);
    if(re != 0) return false;
    return true;
}

AVFrame* XDecode::recv()
{
    mux.lock();
    if(!codecContext)
    {
        mux.unlock();
        return 0;
    }
    AVFrame * frame = av_frame_alloc();
    int re = avcodec_receive_frame(codecContext, frame);
     mux.unlock();
    if(re != 0)
    {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        std::cout << "decode failed:" << buf << std::endl;
        av_frame_free(&frame);
        return nullptr;
    }

    return frame;

}

void XDecode::close()
{
    mux.lock();
    if(codecContext)
    {
        avcodec_close(codecContext);
        avcodec_free_context(&codecContext);
    }
    mux.unlock();
}

void XDecode::clear()
{
    mux.lock();
    if(codecContext)
    {
        avcodec_flush_buffers(codecContext);
    }
    mux.unlock();
}

XDecode::XDecode()
{

}


XDecode::~XDecode()
{

}
