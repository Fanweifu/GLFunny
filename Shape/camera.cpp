#include "camera.h"

Camera::Camera()
{
}

void Camera::drawView() {
    if (windowsChanged) {
        updateViewPort();
        windowsChanged = false;
    }

    if (isMultiScreen) {
        glViewport(left, buttom, width, height);
        glScissor(left, buttom, width, height);
    }

    if (projectionChanged||isMultiScreen) {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&matrixProjection[0][0]);
        projectionChanged = false;
        glMatrixMode(GL_MODELVIEW);
    }

    glLoadMatrixf(&modelmatInv[0][0]);

    glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    mainlight.updatePostion();
    if (Scene) Scene->draw();
}

void Camera::dragMouse(int x, int y, float speed)
{
    float Rz = rvec.z + (mouseX - x)*speed;
    float Rx = rvec.x + (mouseY - y)*speed;
    setRotation(Rx, rvec.y, Rz);

    moveMouse(x, y);
}

void Camera::moveMouse(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

void Camera::localMove( float right, float forward, float up)
{
    auto npvec = pvec;
    npvec += (right*rightV + forward*forwardV + up*upV);
    setPosition(npvec.x, npvec.y, npvec.z);
}

//void Camera::Move(int keyCmd, float step)
//{
//    auto nposv = pvec;
//
//    switch (keyCmd)
//    {
//    case CAM_FORWARD:
//        nposv += step*forward; break;
//    case CAM_BACK:
//        nposv -= step*forward; break;
//    case CAM_LEFT:
//        nposv -= step*right; break;
//    case CAM_RIGHT:
//        nposv += step*right; break;
//    case CAM_UP:
//        nposv += step*up; break;
//    case CAM_DOWN:
//        nposv -= step*up; break;
//    default:
//        break;
//    }
//
//    setPosition(nposv.x, nposv.y, nposv.z);
//}

void drawFrustum(float fovY, float aspectRatio, float nearPlane, float farPlane)
{
    float tangent = tanf(fovY / 2 * DEG2RAD);
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

    float colorLine1[4] = { 0.7f, 0.2f, 0.7f, 1 };
    float colorLine2[4] = { 0.2f, 0.7f, 0.2f, 1 };

    glDisable(GL_LIGHTING);

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

    glEnable(GL_LIGHTING);
}

void Camera::ondraw() {
    drawFrustum(Fov, Ratio, Near, Far);
}

void Camera::lookAt(float ex, float ey, float ez, float tx, float ty, float tz) {
    setPosition(ex, ey, ez);
    auto dir = glm::vec3(tx - ex, ty - ey, tz - ez);
    setDirectionVec3(dir);
}

glm::vec3 getDIR(float rx, float ry, float rz) {
    return glm::vec3(cos(rz)*cos(rx), sin(rz)*cos(rx), sin(rx));
}

void Camera::updateModel()
{
    forwardV = getDIR(rvec.x*DEG2RAD, rvec.y*DEG2RAD, rvec.z*DEG2RAD);
    rightV = glm::vec3(sin(rvec.z*DEG2RAD), -cos(rvec.z*DEG2RAD), 0);
    upV = glm::cross(rightV, forwardV);

    auto centre = pvec + forwardV;
    
    modelmatInv = glm::lookAt(pvec, centre, upV);
    modelmat = glm::inverse(modelmatInv);
}

void Camera::setViewPort(int x, int y, int w, int h) {
    if (w == 0 || h == 0) return;

    owidth *= (float(w) / width);
    oheight *= (float(h) / height);

    left = x;
    buttom = y;
    width = w;
    height = h;
    Ratio = float(w) / h;

    windowsChanged = true;
}

void Camera::setWindowSize(int width, int height) {
    Camera::setViewPort(0, 0, width, height);
}

void Camera::updateProjection()
{
    if (isOrtho) {
        matrixProjection = glm::ortho(-owidth / 2, owidth / 2, -oheight / 2, oheight / 2, Near, Far);
    }
    else {
        matrixProjection = glm::perspective(Fov*DEG2RAD, Ratio, Near, Far);
    }

    matrixProjectionInv = glm::inverse(matrixProjection);

    projectionChanged = true;
}

void Camera::updateViewPort() {
    glViewport((GLsizei)left, (GLsizei)buttom, (GLsizei)width, (GLsizei)height);
    glScissor(left, buttom, width, height);
    updateProjection();
}

void Camera::setDirectionVec3(glm::vec3 dir)
{
    rvec.x = asinf(dir.z / dir.length()) / DEG2RAD;
    rvec.z = atan2f(dir.y, dir.x) / DEG2RAD;
    updateModel();
}

void Camera::setDirection(float vx, float vy, float vz)
{
    setDirectionVec3(glm::vec3(vx, vy, vz));
}

void Camera::init() {
    initGl();
    mainlight.init();

    inited = true;
}

void Camera::initGl() {
    glewInit();
    glShadeModel(GL_SMOOTH);                        // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);          // 4-byte pixel alignment

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_POLYGON_SMOOTH);
    //glEnable(GL_LINE_SMOOTH);

    glEnable(GL_MULTISAMPLE);

    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_SCISSOR_TEST);

    //// track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0, 0, 1);
    glClearStencil(0);
    glClearDepth(1.0f);

    glMatrixMode(GL_MODELVIEW);
}