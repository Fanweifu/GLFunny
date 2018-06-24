#ifndef SHAPE_H
#define SHAPE_H

#ifdef _WIN32
#include <windows.h>
#endif

#include"shader.h"
#include"glm.hpp"
#include"gtc/matrix_transform.hpp"

#define DEG2RAD 3.141593f / 180


typedef void(*xyzChangedHdl)(float x, float y, float z);
typedef void(*voidHdl)();

class Shape
{
public:
    Shape();

    bool isdrawAxis = false;
    float axisLength = 1;

    float posX() { return pvec.x; }
    float posY() { return pvec.y; }
    float posZ() { return pvec.z; }
    void setPosition(float x, float y, float z)
    {
        auto newv = glm::vec3(x, y, z);
        bool diff = pvec != newv;
        pvec = newv;
        if (diff) {
            updateModel();
            onPositionChanged();
        }
    }
    virtual void onPositionChanged() {}

    float rotX() { return rvec.x; }
    float rotY() { return rvec.y; }
    float rotZ() { return rvec.z; }
    void setRotation(float x, float y, float z) {
        auto newv = glm::vec3(x, y, z);
        bool diff = rvec != newv;
        rvec = newv;
        if (diff) {
            updateModel();
            onRotationChanged();
        }
    }
    virtual void onRotationChanged() {}

    float sclX() { return svec.x; }
    float sclY() { return svec.y; }
    float sclZ() { return svec.z; }
    void setScale(float x, float y, float z) {
        auto newv = glm::vec3(x, y, z);
        bool diff = svec != newv;
        svec = newv;
        if (diff) {
            updateModel();
            onScaleChanged();
        }
    }
    virtual void onScaleChanged() {}

    void toLocalPos(float&x, float &y, float &z);
    void toWorldPos(float&x, float &y, float &z);
    void toLocalDir(float&x, float &y, float &z);
    void toWorldDir(float&x, float &y, float &z);
    void draw();
    void setShader(Shader &shd);
    const float* getModelViewPtr() { return &modelmat[0][0]; }
    const float* getModelViewInvPtr() { return &modelmatInv[0][0]; }

    voidHdl drawFunc = NULL;

protected:

    glm::vec3 pvec = glm::vec3(0);
    glm::vec3 rvec = glm::vec3(0);
    glm::vec3 svec = glm::vec3(1);

    bool useShader = true;
    xyzChangedHdl posEvent = NULL;
    xyzChangedHdl rotEvent = NULL;
    xyzChangedHdl sclEvent = NULL;

    glm::mat4 modelmat;
    glm::mat4 modelmatInv;
    Shader* pshader = NULL;
   
    virtual void updateModel();
    virtual void ondraw();

    static void drawAsix(float size);
    
};

#endif // SHAPE_H
