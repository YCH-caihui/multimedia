#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "h264decoder.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    H264Decoder * p = new H264Decoder();
    ui->setupUi(this);

    m_camClient = new cameraclient();
    m_camClient->setUpdateGUICallback(updateVideo2GUI, (unsigned long)this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void  MainWindow::updateVideo2GUI(RGBData_Define *rgbData, unsigned long userData){
    MainWindow * window = (MainWindow *)userData;
    if(window){
        window->onRecvFrame(rgbData->dataBuffer, rgbData->width, rgbData->height);
    }
}


void MainWindow::onRecvFrame(unsigned char * data, int width, int height)
{
    QMutexLocker locker(&m_mutex);

    //QImage:format_RGB888等于24
    const int imageSize =(((width * 24 + 31) >> 5) << 2) * height;

    uint8_t *dataTmp = new uint8_t(imageSize);
    //将数据拷贝到一个临时内存中，因为onRecvFrame是异步执行的，data可能已经无效
    try {
        memcpy(dataTmp, data ,imageSize);
    } catch (...) {
        delete [] dataTmp;
    }

    //转化为rgb数据为可现实的图形对象
    QImage image = QImage(dataTmp, width, height, QImage::Format_RGB888);
    if(image.isNull())
    {
       // qDebug <<"Receive frame error, width:%d, height:%d."<<width<<height;
       return;
    }
    m_image = image.copy(0,0, width, height);
    delete []  dataTmp;
}
