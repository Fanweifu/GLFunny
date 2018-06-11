#include "openglwidget.h"
using namespace cv;
using namespace std;




   



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
        srcimg = imread(path,IMREAD_COLOR);

        if(srcimg.empty()){
            qDebug()<<"input is empty!";
            return false;
        }

        Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                               Size( 2*4 + 1, 2*4+1 ),
                                               Point( -1, -1 ) );

        

        cv::erode(srcimg,dstimg,element,Point(-1,-1),1);

        inputImg.setSrcImage(srcimg);
        inputImg.setDstImage(dstimg);
       
        inputImg.active(true);

        return true;
    }catch(exception e){
        qDebug()<<e.what();
        return false;
    }

}



void OpenglWidget::initializeGL()
{
     camera.init();
     inputImg.generateData();
     camera.Scene = &inputImg;
}



void OpenglWidget::initWidget()
{
    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(20);
}

void OpenglWidget::uninitWidget()
{
    timer.stop();
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
    if(!onDrag) camera.moveMouse(event->x(), event->y());
    else camera.dragMouse(event->x(), event->y());

}

void OpenglWidget::mousePressEvent(QMouseEvent *event){

    onDrag = true;

}

void OpenglWidget::mouseReleaseEvent(QMouseEvent *event){
    onDrag = false;
}

void OpenglWidget::wheelEvent(QWheelEvent *event){
    int val= event->delta();
   
   

}

void OpenglWidget::keyPressEvent(QKeyEvent *event){
    int key = event->key();
    if(key==Qt::Key_W||key==Qt::Key_Up){
        camera.localMove(0, 1, 0);
    }else if(key==Qt::Key_S||key==Qt::Key_Down){
        camera.localMove(0, -1, 0);
    }else if(key==Qt::Key_A||key==Qt::Key_Left){
        camera.localMove(-1, 0, 0);
    }else if(key==Qt::Key_D||key==Qt::Key_Right){
        camera.localMove(1, 0, 0);
    }else if(key==Qt::Key_Space){

    }

   
}
