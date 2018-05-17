#ifndef SHAPE_H
#define SHAPE_H

#ifdef _WIN32
#include <windows.h>
#endif

#include"shader.h"
#include<GL/gl.h>
#include"..\math\matrices.h"
typedef unsigned int uint;
typedef unsigned char uchar;

typedef void(*xyzChangedHdl)(float x, float y, float z);
typedef void(*voidHdl)();

class Shape
{
public:
    Shape();

    bool isdrawAxis = false;
    float axisLength = 1;

    float positionX() { return px; }
    float positionY() { return py; }
    float positionZ() { return pz; }
    void setPosition(float x, float y, float z)
    {
        bool diff = px != x || py != y || pz != z;
        px = x; py = y; pz = z;
        if (diff) {
            updateModel();
            onPositionChanged();
        }
    }
    virtual void onPositionChanged() {}

    float rotationX() { return rx; }
    float rotationY() { return ry; }
    float rotationZ() { return rz; }
    void setRotation(float x, float y, float z) {
        bool diff = rx != x || ry != y || rz != z;
        rx = x; ry = y; rz = z;
        if (diff) {
            updateModel();
            onRotationChanged();
        }
    }
    virtual void onRotationChanged() {}

    float scaleX() { return sx; }
    float scaleY() { return sy; }
    float scaleZ() { return sz; }
    void setScale(float x, float y, float z) {
        bool diff = sx != x || sy != y || sz != z;
        sx = x; sy = y; sz = z;
        if (diff) {
            updateModel();
            onScaleChanged();
        }
    }
    virtual void onScaleChanged() {}

    float *getModelMat();
    void toLocalPos(float&x, float &y, float &z);
    void toWorldPos(float&x, float &y, float &z);
    void draw();
    virtual void initShader() {}
    voidHdl testDrawFunc = NULL;

protected:

    float px = 0;
    float py = 0;
    float pz = 0;
    float rx = 0;
    float ry = 0;
    float rz = 0;
    float sx = 1;
    float sy = 1;
    float sz = 1;

    bool useShader = true;

    xyzChangedHdl posEvent = NULL;
    xyzChangedHdl rotEvent = NULL;
    xyzChangedHdl sclEvent = NULL;

    Matrix4 modelmat;
    Matrix4 modelmatInv;
    Shader pshader;
   
    virtual void updateModel();
    virtual void ondraw();

    static void drawAsix(float size);
    
};

#endif // SHAPE_H
