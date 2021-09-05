#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "xdemux.h"


int main(int argc, char *argv[])
{
    XDemux dumux;
    dumux.open("/Users/ych-caihui/Movies/a.flv");

   cout <<  dumux.seek(0.99f) << endl;
    for(;;)
    {
        AVPacket * pkt = dumux.read();
        if(!pkt)
        {
            cout << "结束了" << endl;
            break;
        }
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
