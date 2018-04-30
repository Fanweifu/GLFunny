#include "openglwidget.h"
using namespace cv;
using namespace std;



void OpenglWidget::initTimer(){
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(20);
}


OpenglWidget::OpenglWidget(QWidget* parent,bool fs )
    :QOpenGLWidget(parent)
{
    initWidget();

    fullScreen = fs;
    setGeometry( 0, 0, 640, 480 );

    if(fs){
        showFullScreen();
    }
}

bool OpenglWidget::loadImg(string path){
    try{
        iImg = imread(path,IMREAD_COLOR);

        if(iImg.empty()){
            qDebug()<<"input is empty!";
            return false;
        }

        cv::resize(iImg,iImg,Size(500,500));

        Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                               Size( 2*4 + 1, 2*4+1 ),
                                               Point( -1, -1 ) );

        cv::erode(iImg,rImg,element,Point(-1,-1),1);

        inputImg.setSrcData(iImg);
        inputImg.setDstData(rImg);
        inputImg.active(true);
        inputImg.enableReplay(true);

        return true;
    }catch(exception e){
        qDebug()<<e.what();
        return false;
    }

}



void OpenglWidget::initializeGL()
{
     camera.init();
     updateCamera();
     initTimer();
}



void OpenglWidget::initWidget()
{
    this->setMouseTracking(true);
    setWindowTitle(tr("opengl demo"));

    camera.scene = &inputImg;

}

void OpenglWidget::updateCamera() {

    camera.setPosition(cameraPosX,cameraPosY,cameraPosZ);
    camera.setRotation(-dragY*0.1,0,-dragX*0.1);

}

void OpenglWidget::paintGL()
{
    camera.drawView();
}






void OpenglWidget::resizeGL(int width, int height)
{
    camera.setViewPort(0,0,width,height);
}

void OpenglWidget::mouseMoveEvent(QMouseEvent *event){
    int x = event->x(),y = event->y();

    if(onDrag){
        dragX +=(x-lastX);
        dragY +=(y-lastY);
        updateCamera();        
    }

    lastX = x;
    lastY = y;


}

void OpenglWidget::mousePressEvent(QMouseEvent *event){

    onDrag = true;

}

void OpenglWidget::mouseReleaseEvent(QMouseEvent *event){
    onDrag = false;
}

void OpenglWidget::wheelEvent(QWheelEvent *event){
    int val= event->delta();
    cameraPosZ += (float)val/100;
    updateCamera();

}

void OpenglWidget::keyPressEvent(QKeyEvent *event){
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

    }

    updateCamera();
}
