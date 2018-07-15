#include"ShaderPlay.h"

ShaderPlay::ShaderPlay(QWidget* parent, bool fs)
    :QOpenGLWidget(parent)
{
    setGeometry(0, 0, 640, 480);

    initWidget();

    if (fs) {
        showFullScreen();
    }
}

void ShaderPlay::BindShader(SeaShader & shd)
{
    shd.bindCamera(camera);
    testShape.pshader =shd ;
}

void renderQuads() {
    glBegin(GL_QUADS);
    glVertex3f(-100, -100, 0);
    glVertex3f(100, -100, 0);
    glVertex3f(100, 100, 0);
    glVertex3f(-100, 100, 0);
    glEnd();
}

void ShaderPlay::initializeGL()
{
    camera.init();
    camera.setPosition(0, 0, 1);
    testShape.drawFunc = renderQuads;
    camera.Scene = &testShape;
}

void ShaderPlay::initWidget()
{
    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);

    QSurfaceFormat format = this->format();
    format.setSamples(2);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    this->setFormat(format);

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(20);
}

void ShaderPlay::uninitWidget()
{
    timer.stop();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

void ShaderPlay::paintGL()
{
    camera.drawView();
}
void ShaderPlay::resizeGL(int width, int height)
{
    camera.setViewPort(0, 0, width, height);
}

void ShaderPlay::mouseMoveEvent(QMouseEvent *event) {
    if (!onDrag) camera.moveMouse(event->x(), event->y());
    else camera.dragMouse(event->x(), event->y());
}

void ShaderPlay::mousePressEvent(QMouseEvent *event) {
    onDrag = true;
}

void ShaderPlay::mouseReleaseEvent(QMouseEvent *event) {
    onDrag = false;
}

void ShaderPlay::wheelEvent(QWheelEvent *event) {
}

void ShaderPlay::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (key == Qt::Key_W || key == Qt::Key_Up) {
        camera.localMove(0, 1, 0);
    }
    else if (key == Qt::Key_S || key == Qt::Key_Down) {
        camera.localMove(0, -1, 0);
    }
    else if (key == Qt::Key_A || key == Qt::Key_Left) {
        camera.localMove(-1, 0, 0);
    }
    else if (key == Qt::Key_D || key == Qt::Key_Right) {
        camera.localMove(1, 0, 0);
    }
    else if (key == Qt::Key_Space) {
    }
}