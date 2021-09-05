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

    int totalMs = ic->duration ;
    cout << "totalMs = " << totalMs << endl;

    av_dump_format(ic, 0, url, 0);

    videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);

    mux.unlock();


    return true;
}

XDemux::XDemux()
{

}


XDemux::~XDemux()
{

}
