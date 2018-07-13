#include "shape.h"
#include<cstring>

Shape::Shape()
{
}

void Shape::draw() {

    if (!visible) return;

    glPushMatrix();

    glMultMatrixf(glm::value_ptr(modelmat));

    pshader.use();
    ondraw();
    pshader.unuse();

    if (drawAxis) Shape::drawAsix(axisLength);

    glPopMatrix();
}


void Shape::updateModel() {
    modelmat = glm::translate(glm::mat4(1.0), pvec);
    modelmat = glm::scale(modelmat, svec);
    modelmat = glm::rotate(modelmat, rvec.z*DEG2RAD, glm::vec3(0, 0, 1));
    modelmat = glm::rotate(modelmat, rvec.y*DEG2RAD, glm::vec3(0, 1, 0));
    modelmat = glm::rotate(modelmat, rvec.x*DEG2RAD, glm::vec3(1, 0, 0));

    modelmatInv = glm::inverse(modelmat);
}

void Shape::toLocalPos(float &x, float &y, float &z) {
    glm::vec4 vl(x, y, z, 1);
    glm::vec4 wv = vl*modelmatInv;

    x = wv[0];
    y = wv[1];
    z = wv[2];
}

void Shape::toWorldPos(float &x, float &y, float &z) {
    glm::vec4 vl(x, y, z, 1);
    glm::vec4 wv = vl*modelmat;

    x = wv[0];
    y = wv[1];
    z = wv[2];
}

void Shape::toLocalDir(float & x, float & y, float & z)
{
    toLocalPos(x, y, z);
    float ox, oy, oz;
    toLocalPos(ox, oy, oz);

    x -= ox; y -= oy; z -= oz;
}

void Shape::toWorldDir(float & x, float & y, float & z)
{
    toWorldPos(x, y, z);
    float ox, oy, oz;
    toWorldPos(ox, oy, oz);

    x -= ox; y -= oy; z -= oz;
}

void Shape::drawAsix(float size) {
    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
    glDisable(GL_LIGHTING);
    glPushMatrix();             //NOTE: There is a bug on Mac misbehaviours of
    //      the light position when you draw GL_LINES
    //      and GL_POINTS. remember the matrix.

    // draw axis
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);
    glEnd();
    glLineWidth(1);

    // draw arrows(actually big square dots)
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    glVertex3f(size, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, size, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, size);
    glEnd();
    glPointSize(1);

    // restore default settings
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
}

void Shape::ondraw() {
    if (drawFunc) (*drawFunc)();
}