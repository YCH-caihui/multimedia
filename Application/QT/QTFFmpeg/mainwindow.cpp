#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "h264decoder.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    H264Decoder * p = new H264Decoder();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
