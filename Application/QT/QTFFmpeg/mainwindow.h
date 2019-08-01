#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cameraclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onRecvFrame(unsigned char * data, int width, int height);

private:
    Ui::MainWindow *ui;
    cameraclient * m_camClient;
    QImage m_image;
    QMutex m_mutex;

    void static updateVideo2GUI(RGBData_Define *rgbData, unsigned long userData);
};

#endif // MAINWINDOW_H
