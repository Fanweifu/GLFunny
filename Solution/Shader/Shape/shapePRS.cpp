#include "shapePRS.h"
#include<cstring>

ShapePRS::ShapePRS()
{
}

void ShapePRS::draw() {

    if (!visible) return;

    

    glPushMatrix();

    glMultMatrixf(glm::value_ptr(modelmat));

    texture0.bind(0);
    texture1.bind(1);
    texture2.bind(2);

    ondraw();

    texture2.unBind();
    texture1.unBind();
    texture0.unBind();

    ShapeBase::draw();

    glPopMatrix();
}


void ShapePRS::updateModel() {
    modelmat = glm::translate(glm::mat4(1.0), pvec);
    modelmat = glm::scale(modelmat, svec);
    modelmat = glm::rotate(modelmat, rvec.z*DEG2RAD, glm::vec3(0, 0, 1));
    modelmat = glm::rotate(modelmat, rvec.y*DEG2RAD, glm::vec3(0, 1, 0));
    modelmat = glm::rotate(modelmat, rvec.x*DEG2RAD, glm::vec3(1, 0, 0));

    modelmatInv = glm::inverse(modelmat);
}

void ShapePRS::toLocalPos(float &x, float &y, float &z) {
    glm::vec4 vl(x, y, z, 1);
    glm::vec4 wv = vl*modelmatInv;

    x = wv[0];
    y = wv[1];
    z = wv[2];
}

void ShapePRS::toWorldPos(float &x, float &y, float &z) {
    glm::vec4 vl(x, y, z, 1);
    glm::vec4 wv = vl*modelmat;

    x = wv[0];
    y = wv[1];
    z = wv[2];
}

void ShapePRS::toLocalDir(float & x, float & y, float & z)
{
    toLocalPos(x, y, z);
    float ox, oy, oz;
    toLocalPos(ox, oy, oz);

    x -= ox; y -= oy; z -= oz;
}

void ShapePRS::toWorldDir(float & x, float & y, float & z)
{
    toWorldPos(x, y, z);
    float ox, oy, oz;
    toWorldPos(ox, oy, oz);

    x -= ox; y -= oy; z -= oz;
}
