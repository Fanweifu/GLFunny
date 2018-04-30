#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <opencv.hpp>
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


    void initWidget();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event) ;
    void mouseReleaseEvent(QMouseEvent *event) ;
    void mouseMoveEvent(QMouseEvent *event) ;
    void wheelEvent(QWheelEvent * event);
    void keyPressEvent(QKeyEvent *event);

private :
    bool isTrans = true;

    bool onDrag = false;
    float dragX =0;
    float dragY =0;
    int lastX = 0;
    int lastY = 0;

    //move
    double cameraPosZ = 100;
    double cameraPosX = 0;
    double cameraPosY = 0;
    //timer
    void initTimer();
    void uninitTimer();
    void updateCamera();
    Image3DEx inputImg;

    Mat iImg;
    Mat rImg;

};

#endif // OPENGLWIDGET_H


