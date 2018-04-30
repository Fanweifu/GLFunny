#ifndef OPENGLTEST_H
#define OPENGLTEST_H


#include"qevent.h"
#include"shape\camera.h"
#include"shape\layer.h"

#include <QtCore>
#include <QWidget>
#include <QOpenGLWidget>

class OpenglTest: public QOpenGLWidget
{
public:
    OpenglTest(QWidget* parent = 0,bool fs = false);

    float stepLength = 0.4;

    Camera camera;
    Camera cammodel;

    Shape testShape;
    Layer testlayer;

protected:

    void initWidget();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event) ;
    void mouseReleaseEvent(QMouseEvent *event) ;
    void mouseMoveEvent(QMouseEvent *event) ;
    void wheelEvent(QWheelEvent * event);
    void keyPressEvent(QKeyEvent *event);


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
};

#endif // OPENGLTEST_H
