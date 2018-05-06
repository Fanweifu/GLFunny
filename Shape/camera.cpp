#include "camera.h"

Camera::Camera()
{

}

Matrix4 Camera::setFrustum(float fovy, float ratio, float n, float f){
    float tangent = tanf(fovy/2 * DEG2RAD);   // tangent of half fovY
    float height = n * tangent;           // half height of near plane
    float width = height * ratio;             // half width of near plane

    // params: left, right, bottom, top, near, far
    return setFrustum(-width, width, -height, height, n, f);
}

Matrix4 Camera::setFrustum(float left, float right, float buttom, float top, float n, float f){
    Matrix4 matrix;
    matrix[0]  =  2 * n / (right - left);
    matrix[5]  =  2 * n / (top - buttom);
    matrix[8]  =  (right + left) / (right - left);
    matrix[9]  =  (top + buttom) / (top - buttom);
    matrix[10] = -(f + n) / (f - n);
    matrix[11] = -1;
    matrix[14] = -(2 * f * n) / (f - n);
    matrix[15] =  0;
    return matrix;
}

Matrix4 Camera::setOrthoFrustum(float l, float r, float b, float t, float n, float f)
{
    Matrix4 matrix;
    matrix[0]  =  2 / (r - l);
    matrix[5]  =  2 / (t - b);
    matrix[10] = -2 / (f - n);
    matrix[12] = -(r + l) / (r - l);
    matrix[13] = -(t + b) / (t - b);
    matrix[14] = -(f + n) / (f - n);
    return matrix;
}

void Camera::updateViewMatrix(){
    viewMat.identity();
    viewMatInv.identity();

    viewMat.translate(-px, -py, -pz);
    viewMatInv.translate(px, py, pz);


    viewMat.rotateZ(-rz);    // roll
    viewMatInv.rotateZ(rz);

    viewMat.rotateY(-ry);    // heading
    viewMatInv.rotateY(ry);

    viewMat.rotateX(-rx);    // pitch
    viewMatInv.rotateX(rx);




}


//glMatrixMode(GL_PROJECTION);
//glLoadMatrixf(matrixProjection.get());
//glMatrixMode(GL_MODELVIEW);
//glLoadIdentity();
void Camera::drawView(){

    if(!inited) init();

    if(windowsChanged){
        updateViewPort();
        windowsChanged = false;
    }

    if(isMultiScreen){
        glViewport(left, buttom, width, height);
        glScissor(left, buttom , width, height);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matrixProjection.get());

    glMatrixMode(GL_MODELVIEW);

    glLoadMatrixf(viewMat.get());


    glClearColor(backgroud[0],backgroud[1],backgroud[2],backgroud[3]);   // background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    mainlight.setPostion();
    if(scene!=NULL) scene->draw();
}

void drawFrustum(float fovY, float aspectRatio, float nearPlane, float farPlane)
{
    float tangent = tanf(fovY/2 * DEG2RAD);
    float nearHeight = nearPlane * tangent;
    float nearWidth = nearHeight * aspectRatio;
    float farHeight = farPlane * tangent;
    float farWidth = farHeight * aspectRatio;

    // compute 8 vertices of the frustum
    float vertices[8][3];
    // near top right
    vertices[0][0] = nearWidth;     vertices[0][1] = nearHeight;    vertices[0][2] = -nearPlane;
    // near top left
    vertices[1][0] = -nearWidth;    vertices[1][1] = nearHeight;    vertices[1][2] = -nearPlane;
    // near bottom left
    vertices[2][0] = -nearWidth;    vertices[2][1] = -nearHeight;   vertices[2][2] = -nearPlane;
    // near bottom right
    vertices[3][0] = nearWidth;     vertices[3][1] = -nearHeight;   vertices[3][2] = -nearPlane;
    // far top right
    vertices[4][0] = farWidth;      vertices[4][1] = farHeight;     vertices[4][2] = -farPlane;
    // far top left
    vertices[5][0] = -farWidth;     vertices[5][1] = farHeight;     vertices[5][2] = -farPlane;
    // far bottom left
    vertices[6][0] = -farWidth;     vertices[6][1] = -farHeight;    vertices[6][2] = -farPlane;
    // far bottom right
    vertices[7][0] = farWidth;      vertices[7][1] = -farHeight;    vertices[7][2] = -farPlane;

    float colorLine1[4] = { 0.7f, 0.7f, 0.7f, 0.7f };
    float colorLine2[4] = { 0.2f, 0.2f, 0.2f, 0.7f };
    float colorPlane[4] = { 0.5f, 0.5f, 0.5f, 0.5f };

    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw the edges around frustum
    glBegin(GL_LINES);
    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[4]);

    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[5]);

    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[6]);

    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[7]);
    glEnd();

    glColor4fv(colorLine1);
    glBegin(GL_LINE_LOOP);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[7]);
    glEnd();

    glColor4fv(colorLine1);
    glBegin(GL_LINE_LOOP);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[3]);
    glEnd();

    // draw near and far plane
    glColor4fv(colorPlane);
    glBegin(GL_QUADS);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[7]);
    glEnd();

    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
}

void Camera::ondraw(){
    drawFrustum(Fov,Ratio,Near,Far);
}

void Camera::lookAt(float tx, float ty, float tz,bool isup){

    double vx = tx - px,vy = ty -py, vz = tz -pz;

    float heading = atan2f(vz,vx);
    float pitch = asinf(vy/sqrt(vx*vx+vz*vz));

    ry = heading/DEG2RAD;
    rx = pitch/DEG2RAD;
    if(isup) rz = 0;

    updateViewMatrix();
}

void Camera::setViewPort(int x, int y, int w, int h){

    left = x;
    buttom = y;
    width = w;
    height = h;
    Ratio = (float)(w)/h;


    windowsChanged = true;
}


void Camera::setWindowSize(int width, int height) {
    Camera::setViewPort(0, 0, width, height);
}

void Camera::updateViewPort(){
    glViewport((GLsizei)left, (GLsizei)buttom, (GLsizei)width, (GLsizei)height);
    updateProjection();
}

void Camera::init(){

    initGl();
    mainlight.init();

    inited = true;
}


void Camera::initGl() {

    glShadeModel(GL_SMOOTH);                        // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);          // 4-byte pixel alignment


    // enable/disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH,GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH,GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_SCISSOR_TEST);

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0 , 0, 1);   // background color
    glClearStencil(0);                              // clear stencil buffer
    glClearDepth(1.0f);                             // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    VAOArrayMan::init();

}

