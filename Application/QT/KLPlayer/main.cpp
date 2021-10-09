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
    XDecode adecode;
    XVideoWidget * video;
public:
    void init()
    {
        demux.open("/Users/ych-caihui/Movies/a.flv");
        vdecode.open(demux.copyVPara());
        adecode.open(demux.copyAPara());

    }

    void run()
    {
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
                adecode.send(pkt);
                AVFrame * aFrame = adecode.recv();
                cout << "Audio:" << aFrame << endl;

            }
            else
            {
                vdecode.send(pkt);
                AVFrame * vFrame = vdecode.recv();
                video->repaint(vFrame);
                msleep(40);
                cout << "Video:" << vFrame << endl;

            }
        }
    }
};


int main(int argc, char *argv[])
{

    TestThread tt;
    tt.init();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

   w.getVideo()->init(tt.demux.width, tt.demux.height);
    tt.video = w.getVideo();
    tt.start();
    return a.exec();
}
