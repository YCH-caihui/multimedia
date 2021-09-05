#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "xdemux.h"
#include "xdecode.h"


int main(int argc, char *argv[])
{
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
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
