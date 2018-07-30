#include "camera.h"

using namespace glm;

Camera::Camera()
{
}

void Camera::beginRender()
{
    if (windowsChanged) {
        updateViewPort();
        windowsChanged = false;
    }

    if (isMultiScreen) {
        glViewport(left, buttom, width, height);
        glScissor(left, buttom, width, height);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(value_ptr(matProjection));

    glMatrixMode(GL_MODELVIEW);

    glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glLoadIdentity();
    drawBack();

    glLoadMatrixf(value_ptr(modelmatInv));

    mainLight.draw();
}

void Camera::endRender()
{
    renderTime += 1;
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

void Camera::mouseCoordToUV(int mx, int my, float & u, float & v)
{
    glm::vec2 uv = glm::vec2(mx, height - my) / glm::vec2(width, height);

    uv = 2.0f*uv - 1.0f;

    u = uv.x;
    v = uv.y;
}

void Camera::mouseRay(int mx, int my, float & x, float & y, float & z)
{
    float u, v;
    mouseCoordToUV(mx, my, u, v);

    updateProjection();
    updateModel();

    glm::vec4 dir = matProjectionInv*glm::vec4(u, v, 1, 1);
    dir = dir / dir.w;
    auto zero = glm::vec4(0, 0, 0, 1);
    auto pos = modelmat*zero;
    auto dirw = modelmat*dir;

    glm::vec4 rdir = dirw - pos;

    glm::vec3 rdir3 = glm::normalize(glm::vec3(rdir.x, rdir.y, rdir.z));

    mouseTarget = rdir3 + pvec;

    x = rdir3.x; y = rdir3.y; z = rdir3.z;
}

void Camera::localMove(float right, float forward, float up)
{
    auto npvec = pvec;
    npvec += (right*rightV + forward*forwardV + up*upV);
    setPosition(npvec.x, npvec.y, npvec.z);
}

void Camera::setCamUniform(Shader & shd)
{
    shd.bind();
    shd.setUniform2f(UNIFORM_VIEWPORT_VEC2, getViewWidth(), getViewHeight());
    shd.setUniform1f(UNIFORM_TIME_FLOAT, getRenderTimes(0.01f));
    shd.setUniformMat4(UNIFORM_CAMERAVIEWINV_MAT4, getViewMatInvPtr());
    shd.setUniformMat4(UNIFORM_PROJECTIONINV_MAT4, getProjectionMatInvPtr());

    float x, y, z, w;
    mainLight.getPositon(x, y, z, w);
    shd.setUniform4f(UNIFORM_WORLDLIGHT_VEC4, x, y, z, w);
    shd.setUniform3f(UNIFORM_CAMERAPOS_VEC3, posX(), posY(), posZ());
}

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

    target = pvec + forwardV;

    modelmatInv = glm::lookAt(pvec, target, upV);
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
        matProjection = glm::ortho(-owidth / 2, owidth / 2, -oheight / 2, oheight / 2, Near, Far);
    }
    else {
        matProjection = glm::perspective(Fov*DEG2RAD, Ratio, Near, Far);
    }

    matProjectionInv = glm::inverse(matProjection);

    projectionChanged = true;
}

void Camera::updateViewPort() {
    glViewport((GLsizei)left, (GLsizei)buttom, (GLsizei)width, (GLsizei)height);
    glScissor(left, buttom, width, height);
    updateProjection();
}

void Camera::setDirectionVec3(glm::vec3 dir)
{
    rvec.x = asinf(dir.z / glm::length(dir)) / DEG2RAD;
    if (dir.y != 0 || dir.x != 0) rvec.z = atan2f(dir.y, dir.x) / DEG2RAD;
    updateModel();
}

void Camera::setDirection(float vx, float vy, float vz)
{
    setDirectionVec3(glm::vec3(vx, vy, vz));
}

void Camera::init() {
    if (inited) return;

    initGl();
    initBack();
    mainLight.init();

    updateProjection();
    updateModel();

    inited = true;
}

void Camera::initGl() {
    glewInit();
    glShadeModel(GL_SMOOTH);                        // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);          // 4-byte pixel alignment

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_SCISSOR_TEST);

    glEnable(GL_FRAMEBUFFER_SRGB);

    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
}

void Camera::drawBack() {
    setCamUniform(backshd);
    backshd.bind();
    glDepthRange(0.99999, 1);

    backBlock.draw();

    glDepthRange(0, 1);
    backshd.unBind();
}

void Camera::initBack()
{
    backBlock.drawStyle = QUADS;
    backBlock.addPoint(-1, -1, -0.1, 0, 0, 1, 0, 0);
    backBlock.addPoint(1, -1, -0.1, 0, 0, 1, 1, 0);
    backBlock.addPoint(1, 1, -0.1, 0, 0, 1, 1, 1);
    backBlock.addPoint(-1, 1, -0.1, 0, 0, 1, 0, 1);
    /*backBlock.addIndex(0, 1, 2);
    backBlock.addIndex(0, 2, 3);*/

    backshd.loadFragFile("GLSL/sky.glsl");
    backshd.link();
}