QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += $$PWD/../../../FFmpeg/fflib/pc/include
DEPENDPATH += $$PWD/../../../FFmpeg/fflib/pc/lib


macx{
 message("=====build  macx======")
 LIBS += -L$$PWD/../../../FFmpeg/fflib/pc/lib/ -lavformat.58.20.100
 LIBS += -lavutil.56.22.100
 LIBS += -lavcodec.58.35.100
 LIBS += -lswresample.3.3.100
 LIBS += -lswscale.5.3.100

}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    xdecode.cpp \
    xdemux.cpp

HEADERS += \
    mainwindow.h \
    xdecode.h \
    xdemux.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
