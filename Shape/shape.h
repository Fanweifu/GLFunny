#ifndef SHAPE_H
#define SHAPE_H

#include"shader.h"
#include"texture.h"
#include"ShapeBase.h"

typedef void(*xyzChangedHdl)(float x, float y, float z);
typedef void(*voidHdl)();

 class Shape: public ShapeBase
{
public:
    Shape();

    bool visible = true;

    Shader pshader;
    ImgTexture pTexture;

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
    const float* getPositionf3() { return glm::value_ptr(pvec); }
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
    const float* getRotationf3() { return glm::value_ptr(rvec); }
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
    const float* getScalef3() { return glm::value_ptr(svec); }
    virtual void onScaleChanged() {}

    void toLocalPos(float&x, float &y, float &z);
    void toWorldPos(float&x, float &y, float &z);
    void toLocalDir(float&x, float &y, float &z);
    void toWorldDir(float&x, float &y, float &z);
    void draw();
    const float* getModelMatPtr() { return glm::value_ptr(modelmat); }
    const float* getModelMatInvPtr() { return glm::value_ptr(modelmatInv); }

    voidHdl drawFunc = NULL;

protected:

    bool useShader = true;

    xyzChangedHdl posEvent = NULL;
    xyzChangedHdl rotEvent = NULL;
    xyzChangedHdl sclEvent = NULL;

    glm::vec3 pvec = glm::vec3(0);
    glm::vec3 rvec = glm::vec3(0);
    glm::vec3 svec = glm::vec3(1);

    glm::mat4 modelmat;
    glm::mat4 modelmatInv;

    virtual void updateModel();
    virtual void ondraw();

    
};

#endif // SHAPE_H
