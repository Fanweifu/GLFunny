#include "openglwidget.h"
#include<opencv.hpp>
using namespace cv;
using namespace std;

void OpenglWidget::initTimer(){
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(5);
}


OpenglWidget::OpenglWidget(QWidget* parent,bool fs )
    :QGLWidget(parent)
{
    initWidget();
    initializeGL();

    fullScreen = fs;
    setGeometry( 0, 0, 640, 480 );

    initTimer();

    if(fs){
        showFullScreen();
    }
}

bool OpenglWidget::loadImg(string path){
    try{

        inputImg = imread(path,IMREAD_COLOR);
        if(inputImg.empty()){
            qDebug()<<"input is empty!";
            return false;
        }


        cv::resize(inputImg,inputImg,Size(100,100),0,0);
        medianBlur(inputImg,resultImg,5);
        return true;
    }catch(exception e){
        qDebug()<<e.what();
        return false;
    }

}



void OpenglWidget::initializeGL()
{
    control.init();
}

void OpenglWidget::initWidget()
{
    setGeometry( 100, 100, 640, 480 );
    setWindowTitle(tr("opengl demo"));
}


uchar getuchar(int rowidx, int colidx,int chidx, Mat &img){
    return img.data[rowidx *img.step+ colidx*img.channels()+chidx];
}

void glrendPoint(int rowidx, int colidx ,int chidx, Mat &img){
    GLfloat b = getuchar(rowidx,colidx,chidx,img)/255;//b range:(0,1.0)

    GLfloat x = rowidx*10;
    GLfloat y = colidx *10;
    GLfloat z = b*10;

    switch(chidx){
    case 0:
        glColor3f(0,0,b);
        break;
    case 1:
        glColor3f( 0,b,0);
        break;
    case 2:
        glColor3f( b,0,0);
        break;
    }
    glVertex3f(x,y,0);

}


void renderQuad(){
    glColor3f(1,1,0);
    glBegin( GL_QUADS );
    glVertex3f( -0.1,  0.1,  -1.0 );
    glVertex3f(  0.1,  0.1,  -1.0 );
    glVertex3f(  0.1, -0.1,  -1.0 );
    glVertex3f( -0.1, -0.1,  -1.0 );
    glEnd();
}


void OpenglWidget::updateCamera() {

    control.setCameraX(cameraPosX);
    control.setCameraZ(cameraPosZ);
    control.setCameraY(cameraPosY);


    control.setCameraAngleZ(dragX);
    control.setCameraAngleY(dragY);

}

void OpenglWidget::paintGL()
{
//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//    cameraControl();
//    renderQuad();

    //    if(resultImg.empty()||inputImg.empty()) return;
    //    int width = resultImg.cols,height = resultImg.rows;
    //    for(int chidx =0;chidx<=2;chidx++){
    //        for(int ridx=0 ;ridx<=height-2;ridx++){
    //            for(int cidx=0;cidx<=width-2;cidx++){

    //                glBegin(GL_TRIANGLES);
    //                glrendPoint(ridx,cidx,chidx,inputImg);
    //                glrendPoint(ridx,cidx+1,chidx,inputImg);
    //                glrendPoint(ridx+1,cidx,chidx,inputImg);
    //                glEnd();

    //                glBegin(GL_TRIANGLES);
    //                glrendPoint(ridx,cidx+1,chidx,inputImg);
    //                glrendPoint(ridx+1,cidx+1,chidx,inputImg);
    //                glrendPoint(ridx+1,cidx,chidx,inputImg);
    //                glEnd();
    //            }
    //        }
    //   }

    control.draw();
}






void OpenglWidget::resizeGL(int width, int height)
{
//    if(0 == height) {
//        height = 1;
//    }

//    glViewport(0, 0, (GLint)width, (GLint)height);

//    glMatrixMode(GL_PROJECTION);

//    glLoadIdentity();



//    GLdouble aspectRatio = (GLfloat)width/(GLfloat)height;
//    GLdouble zNear = 0.1;
//    GLdouble zFar = 100.0;

//    //(45.0, aspectRatio ,zNear,zFar);
//    GLdouble rFov = 45.0 * 3.14159265 / 180.0;
//    glFrustum( -zNear * tan( rFov / 2.0 ) * aspectRatio,
//               zNear * tan( rFov / 2.0 ) * aspectRatio,
//               -zNear * tan( rFov / 2.0 ),
//               zNear * tan( rFov / 2.0 ),
//               zNear, zFar );

//    glMatrixMode(GL_MODELVIEW);

    //glLoadIdentity();

    control.setWindowSize(width,height);
}


void OpenglWidget::mouseMoveEvent(QMouseEvent *event){
    if(onDrag){
        dragX +=(event->x()-lastX);
        dragY +=(event->y()-lastY);
        updateCamera();
    }
    lastX = event->x();
    lastY = event->y();

}

void OpenglWidget::mousePressEvent(QMouseEvent *event){

    onDrag = true;

}

void OpenglWidget::mouseReleaseEvent(QMouseEvent *event){
    onDrag = false;
}

void OpenglWidget::wheelEvent(QWheelEvent *event){
    int val= event->delta();
    cameraPosY += val;
    updateCamera();
}

void OpenglWidget::keyPressEvent(QKeyEvent *event){
    int key = event->key();
    if(key==Qt::Key_W||key==Qt::Key_Up){
        cameraPosZ -=stepLength;
    }else if(key==Qt::Key_S||key==Qt::Key_Down){
        cameraPosZ +=stepLength;
    }else if(key==Qt::Key_A||key==Qt::Key_Left){
        cameraPosX -=stepLength;
    }else if(key==Qt::Key_D||key==Qt::Key_Right){
        cameraPosX += stepLength;
    }

    updateCamera();
}
