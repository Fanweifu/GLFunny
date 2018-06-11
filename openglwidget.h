#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <opencv2/opencv.hpp>
#include"shape\image3dex.h"
#include"shape\camera.h"
#include"shape\layer.h"
#include"shape\light.h"

#include <qevent.h>
#include <QWidget>
#include <QtCore>
#include <QOpenGLWidget>

using namespace cv;
using namespace std;


class OpenglWidget : public QOpenGLWidget
{
public:
    float stepLength = 3;
    Camera camera;

    OpenglWidget(QWidget* parent = 0,bool fs = false);

    bool loadImg(string path);

protected:

    bool fullScreen;
    string imgPath;

    void initWidget();
    void uninitWidget();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event) ;
    void mouseReleaseEvent(QMouseEvent *event) ;
    void mouseMoveEvent(QMouseEvent *event) ;
    void wheelEvent(QWheelEvent * event);
    void keyPressEvent(QKeyEvent *event);

private :
    
    bool onDrag = false;
    Image3DEx inputImg;
    cv::Mat srcimg;
    cv::Mat dstimg;
    
    QTimer  timer;
};

#endif // OPENGLWIDGET_H


