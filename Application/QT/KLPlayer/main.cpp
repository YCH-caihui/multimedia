#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "xdemux.h"
#include "xdecode.h"


int main(int argc, char *argv[])
{
    XDemux demux;
    demux.open("/Users/ych-caihui/Movies/a.flv");

    XDecode decode;
    std::cout << "vdecode.open() =" << decode.open(demux.copyVPara()) << std::endl;
    decode.clear();
    decode.close();
    std::cout << "adecode.open() =" << decode.open(demux.copyAPara()) << std::endl;

//    for(;;)
//    {
//        AVPacket * pkt = dumux.read();
//        if(!pkt)
//        {
//            cout << "结束了" << endl;
//            break;
//        }
//    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
