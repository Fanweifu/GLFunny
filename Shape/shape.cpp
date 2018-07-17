#include "shape.h"
#include<cstring>

Shape::Shape()
{
}

void Shape::draw() {

    if (!visible) return;

    ShapeBase::draw();

    glPushMatrix();

    glMultMatrixf(glm::value_ptr(modelmat));

    pTexture.bind();
    pshader.use();

    ondraw();

    pshader.unuse();
    pTexture.unbind();


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


void Shape::ondraw() {
    if (drawFunc) (*drawFunc)();
}