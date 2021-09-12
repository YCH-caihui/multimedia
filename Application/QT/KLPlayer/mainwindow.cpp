#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

XVideoWidget * MainWindow::getVideo()
{
    return ui->video;
}

MainWindow::~MainWindow()
{
    delete ui;
}

