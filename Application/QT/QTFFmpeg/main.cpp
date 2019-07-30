#include "mainwindow.h"
#include <QApplication> //应用程序抽象类
// 窗口类
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //窗口对象
    MainWindow w;
    QPushButton button;
    button.setText("本宝宝的");
    //窗口对象的父子关系，影响显示位置
    button.setParent(&w);
    button.show();
    w.setWindowTitle("Hellow");
    //显示窗口
    w.show();

   //在exe中有消息循环
    return a.exec();
}
