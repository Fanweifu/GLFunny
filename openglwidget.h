#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGL>
#include <opencv.hpp>
#include"modelgl.h"
using namespace cv;
using namespace std;


class OpenglWidget : public QGLWidget
{
public:
    float stepLength = 0.1;
    ModelGL control;
    OpenglWidget(QWidget* parent = 0,bool fs = false);

    bool loadImg(string path);

protected:
    bool fullScreen;
    GLfloat Rtri;
    GLfloat Rquad;


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
    double cameraPosZ = 0;
    double cameraPosX = 0;
    double cameraPosY = 0;
    //timer
    void initTimer();
    void uninitTimer();
    void updateCamera();
    Mat inputImg;
    Mat resultImg;

};

#endif // OPENGLWIDGET_H


