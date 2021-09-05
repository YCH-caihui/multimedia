#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "xdemux.h"


int main(int argc, char *argv[])
{
    XDemux dumux;
    dumux.open("/Users/ych-caihui/Movies/a.flv");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
