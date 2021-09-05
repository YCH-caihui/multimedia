#include "xdemux.h"


bool XDemux::open(const char *url)
{
    AVDictionary * opt = nullptr;
    av_dict_set(&opt, "rtsp_transport", "tcp", 0);
    av_dict_set(&opt, "max_delay", "500",0);

    mux.lock();

    int re = avformat_open_input(&ic, url, nullptr, &opt);
    if(re != 0)
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        cout << "Open  " << url << "  failed" << endl;
        return false;
    }
    cout << "open  " << url << " Success !" << endl;

    re = avformat_find_stream_info(ic, nullptr);
    if(re < 0)
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        cout << "find " << url << " stream failed" << endl;
        return false;
    }

    int totalMs = ic->duration / (AV_TIME_BASE / 1000);
    cout << "totalMs = " << totalMs << endl;

    av_dump_format(ic, 0, url, 0);

    videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);

    audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);

    mux.unlock();


    return true;
}


AVPacket * XDemux::read()
{
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        return nullptr ;
    }

    AVPacket * pkt = av_packet_alloc();
    //读取一桢， 并分配空间
    int re = av_read_frame(ic , pkt);
    if(re != 0)
    {
        mux.unlock();
        av_packet_free(&pkt);
        return nullptr;
    }

    pkt->pts = pkt->pts * (1000 *(r2d(ic->streams[pkt->stream_index]->time_base)));
    pkt->dts = pkt->dts * (1000 * (r2d(ic->streams[pkt->stream_index]->time_base)));

    mux.unlock();
    cout << "packet pts:" << pkt->pts << endl;
    return pkt;
}


AVCodecParameters * XDemux::copyVPara()
{
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        return nullptr;
    }
    AVCodecParameters * pa = avcodec_parameters_alloc();
    avcodec_parameters_copy(pa, ic->streams[videoStream]->codecpar);
    mux.unlock();
    return pa;
}

AVCodecParameters * XDemux::copyAPara()
{
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        return nullptr;
    }
    AVCodecParameters * pa = avcodec_parameters_alloc();
    avcodec_parameters_copy(pa, ic->streams[audioStream]->codecpar);
    mux.unlock();
    return pa;
}

bool XDemux::seek(double pos)
{
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        return false;
    }
    //清理缓冲区
    avformat_flush(ic);

    long long seekPos = ic->duration * r2d(ic->streams[videoStream]->time_base) * pos;
    cout << "seek:" <<  seekPos << "duration:" << ic->duration << endl;
    int re = av_seek_frame(ic, videoStream, seekPos, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
    mux.unlock();
    if(re < 0)
    {
        return false;
    }
    return true;

}

XDemux::XDemux()
{

}


XDemux::~XDemux()
{

}
