#include "opengltest.h"

OpenglTest::OpenglTest(QWidget* parent,bool fs )
    :QOpenGLWidget(parent)
{
    setGeometry( 0, 0, 640, 480 );

    initWidget();

    if(fs){
        showFullScreen();
    }
}

void OpenglTest::initTimer(){
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(30);
}

void OpenglTest::initializeGL()
{
    camera.init();
    cammodel.init();
}

void drawGrid(float size, float step)
{
    // disable lighting
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);

    glColor3f(1.0f, 1.0f, 0.0f);
    for(float i=step; i <= size; i+= step)
    {
        glVertex3f(-size, 0,  i);   // lines parallel to X-axis
        glVertex3f( size, 0,  i);
        glVertex3f(-size, 0, -i);   // lines parallel to X-axis
        glVertex3f( size, 0, -i);

        glVertex3f( i, 0, -size);   // lines parallel to Z-axis
        glVertex3f( i, 0,  size);
        glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
        glVertex3f(-i, 0,  size);
    }

    // x-axis
    glColor3f(0.0f,1.0f, 1.0f);
    glVertex3f(-size, 0, 0);
    glVertex3f( size, 0, 0);

    // z-axis
    glColor3f(1.0f,0,1.0f);
    glVertex3f(0, 0, -size);
    glVertex3f(0, 0,  size);

    glEnd();
    // enable lighting back
    glEnable(GL_LIGHTING);
}

void drawGrid(){
    drawGrid(10,1);
}

void OpenglTest::initWidget()
{
    this->setMouseTracking(true);
    this->setGeometry(100,100,300,300);
    setWindowTitle(tr("opengl demo"));
    initTimer();

    cammodel.isdrawAxis = true;
    cammodel.setFar(100);
    testShape.testDrawFunc = &drawGrid;

    testlayer.add(&cammodel);
    testlayer.add(&testShape);

    camera.scene = &testlayer;
    cammodel.scene = &testShape;

    camera.setPosition(0,10,100);
    Light& main = camera.getLight();
    main.setPostion(0,1,1,0);
    main.setAmbient(0.5,0.5,0.5,1);
}

void OpenglTest::updateCamera() {

    cammodel.setPosition(cameraPosX,cameraPosY,cameraPosZ);
    cammodel.setRotation(-dragY*0.1,-dragX*0.1,0);
}

void OpenglTest::paintGL()
{

    camera.drawView();
    cammodel.drawView();
}
void OpenglTest::resizeGL(int width, int height)
{
    camera.setViewPort(0,0,width/2,height);
    cammodel.setViewPort(width/2,0,width/2,height);
}

void OpenglTest::mouseMoveEvent(QMouseEvent *event){
    int x = event->x(),y = event->y();

    if(onDrag){
        dragX +=(x-lastX);
        dragY +=(y-lastY);
        updateCamera();
    }

    lastX = x;
    lastY = y;
}

void OpenglTest::mousePressEvent(QMouseEvent *event){

    onDrag = true;

}

void OpenglTest::mouseReleaseEvent(QMouseEvent *event){
    onDrag = false;
}

void OpenglTest::wheelEvent(QWheelEvent *event){
    int val= event->delta();
    cameraPosZ += (float)val/100;
    updateCamera();

}

void OpenglTest::keyPressEvent(QKeyEvent *event){
    int key = event->key();
    if(key==Qt::Key_W||key==Qt::Key_Up){
        cameraPosY +=stepLength;
    }else if(key==Qt::Key_S||key==Qt::Key_Down){
        cameraPosY -=stepLength;
    }else if(key==Qt::Key_A||key==Qt::Key_Left){
        cameraPosX -=stepLength;
    }else if(key==Qt::Key_D||key==Qt::Key_Right){
        cameraPosX += stepLength;
    }else if(key==Qt::Key_Space){
        camera.isOrtho = !camera.isOrtho;
    }

    updateCamera();
}
