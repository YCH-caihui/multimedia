#-------------------------------------------------
#
# Project created by QtCreator 2019-07-30T01:05:03
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTFFmpeg
TEMPLATE = app
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14


macx{

#QMAKE_LFLAGS    += -framework OpenGL
#QMAKE_LFLAGS    += -framework GLKit
#QMAKE_LFLAGS    += -framework QuartzCore
#QMAKE_LFLAGS    += -framework CoreVideo
#QMAKE_LFLAGS    += -framework Foundation
#QMAKE_LFLAGS    += -framework AVFoundation
#QMAKE_LFLAGS    += -framework CoreMedia
#QMAKE_LFLAGS    += -framework AVFoundation
#QMAKE_LFLAGS    += -framework VideoToolbox
#QMAKE_LFLAGS    += -framework VideoDecodeAcceleration
#QMAKE_LFLAGS    += -framework QTKit
#QMAKE_LFLAGS    += -framework AudioToolBox
#QMAKE_LFLAGS    += -framework CoreGraphics
#QMAKE_LFLAGS    += -framework CoreMedia
#QMAKE_LFLAGS    += -framework openal
#QMAKE_LFLAGS    += -framework mediaToolbox
#QMAKE_LFLAGS    += -framework accelerate



INCLUDEPATH +=../../../fflib/pc/include
LIBS += -L$$PWD/../../../fflib/pc/lib/
LIBS += -lavcodec \
        -lavfilter \
        -lavformat \
        -lavutil \
        -lswscale
#LIBS += -L /usr/lib -
#LIBS += -lbz2\

}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        cameraclient.cpp \
        h264decoder.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        cameraclient.h \
        cctcpdatadefine.h \
        h264decoder.h \
        mainwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
