#ifndef XVIDEOWIDGET_H
#define XVIDEOWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <mutex>
#include <QDebug>
#include <QTimer>
extern "C" {
#include <libavutil/frame.h>
}

#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4



class XVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
private:
    std::mutex mux;
    //shader程序
    QOpenGLShaderProgram program;
    //shader中的yuv变量地址
    GLuint unis[3] = {0};
    //opengl的 texture地址
    GLuint texs[3] = {0};
    //材质内存空间
    unsigned char * datas[3] = {0};
    int width = 0;
    int height = 0;

public:
    XVideoWidget(QWidget * parent);
    void init(int width, int height);
    //不管成功与否都释放frame空间
    virtual void repaint(AVFrame * frame);
    ~XVideoWidget();

protected:
    //刷新显示
    void painGL();

    //初始化gl
    void initializeGL();

    //窗口尺寸变化
    void resizeGL(int width, int height);


};

#endif // XVIDEOWIDGET_H
