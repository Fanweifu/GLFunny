#ifndef OPENGLTEST_H
#define OPENGLTEST_H


#include"qevent.h"
#include"shape\camera.h"
#include"shape\layer.h"
#include"Shape\Shader\SeaShader.h"

#include <QTCORE>
#include <QWIDGET>
#include <QOPENGLWIDGET>
#include <QSURFACEFORMAT>

class ShaderPlay: public QOpenGLWidget
{
public:
    ShaderPlay(QWidget* parent = 0,bool fs = false);

    void BindShader(SeaShader& shd);


protected:

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
    
    //void render();

    bool onDrag = false;

    Camera camera;
    Shape testShape;
    
    QTimer  timer;
    SeaShader *shd;
};

#endif // OPENGLTEST_H
