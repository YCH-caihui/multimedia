#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "xdemux.h"
#include "xdecode.h"
#include "xvideowidget.h"
#include <QThread>


class TestThread : public QThread
{
public:
    XDemux demux;
    XDecode vdecode;
    XDecode aDecode;
    XVideoWidget * video;
public:
    void init()
    {
        demux.open("/Users/ych-caihui/Movies/a.flv");
        vdecode.open(demux.copyVPara());
        vdecode.open(demux.copyAPara());

    }

    void run()
    {
        for(;;)
        {
            AVPacket *pkt = demux.read();
            if(!demux.isAudio(pkt))
            {
                vdecode.send(pkt);
                AVFrame * frame = vdecode.recv();
                video->repaint(frame);
                msleep(40);
            }
        }
    }
};


int main(int argc, char *argv[])
{
    /*

    XDemux demux;
    demux.open("/Users/ych-caihui/Movies/a.flv");

    XDecode vDecode;
    std::cout << "vdecode.open() =" << vDecode.open(demux.copyVPara()) << std::endl;

    XDecode aDecode;
    std::cout << "adecode.open() =" << aDecode.open(demux.copyAPara()) << std::endl;

    for(;;)
    {
        AVPacket * pkt = demux.read();

        if(!pkt)
        {
            cout << "结束了" << endl;
            break;
        }
        if(demux.isAudio(pkt))
        {
            aDecode.send(pkt);
            AVFrame * aFrame = aDecode.recv();
            cout << "Audio:" << aFrame << endl;

        }
        else
        {
            vDecode.send(pkt);
            AVFrame * vFrame = vDecode.recv();
            cout << "Video:" << vFrame << endl;

        }
    }
    */


    TestThread tt;
    tt.init();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//   w.getVideo()->init(tt.demux.width, tt.demux.height);
//    tt.video = w.getVideo();
//    tt.start();
    return a.exec();
}
