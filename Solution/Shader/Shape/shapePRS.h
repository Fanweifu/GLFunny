#ifndef SHAPE_H
#define SHAPE_H

#include"shader.h"
#include"texture.h"
#include"shape.h"

class ShapePRS : public ShapeBase
{
public:
    ShapePRS();

    bool visible = true;

    GLuint texture0 = 0;
	GLuint texture1 = 0;
	GLuint texture2 = 0;

    float posX() { return pvec.x; }
    float posY() { return pvec.y; }
    float posZ() { return pvec.z; }

	void setPosX(float x) { setPosition(x, pvec.y, pvec.z); }
	void setPosY(float y) { setPosition(pvec.x, y, pvec.z); }
	void setPosZ(float z) { setPosition(pvec.x, pvec.y, z); }

    void setPosition(float x, float y, float z)
    {
        pvec = glm::vec3(x, y, z);
        updateModel();
    }
    const float* getPositionf3() { return glm::value_ptr(pvec); }
   

    float rotX() { return rvec.x; }
    float rotY() { return rvec.y; }
    float rotZ() { return rvec.z; }
    void setRotation(float x, float y, float z) {
         rvec = glm::vec3(x, y, z);
         updateModel();
    }

    const float* getRotationf3() { return glm::value_ptr(rvec); }
   

    float sclX() { return svec.x; }
    float sclY() { return svec.y; }
    float sclZ() { return svec.z; }
    void setScale(float x, float y, float z) {
        svec = glm::vec3(x, y, z);
        updateModel();
    }
    void setScale(float s) {
        svec = glm::vec3(s);
        updateModel();
    }


    const float* getScalef3() { return glm::value_ptr(svec); }

    void toLocalPos(float&x, float &y, float &z);
    void toWorldPos(float&x, float &y, float &z);
    void toLocalDir(float&x, float &y, float &z);
    void toWorldDir(float&x, float &y, float &z);
    void draw();
    const float* getModelMatPtr() { return glm::value_ptr(modelmat); }
    const float* getModelMatInvPtr() { return glm::value_ptr(modelmatInv); }

protected:

    bool useShader = true;

    glm::vec3 pvec = glm::vec3(0);
    glm::vec3 rvec = glm::vec3(0);
    glm::vec3 svec = glm::vec3(1);

    glm::mat4 modelmat;
    glm::mat4 modelmatInv;

    virtual void updateModel();
    virtual void ondraw() { };
};

#endif // SHAPE_H
