#pragma once
#include<glHead.h>

class ShapeBase
{
public:

    bool drawAxis = false;
    float axisLength = 1;
    bool visible = true;


    ShapeBase();
    ~ShapeBase();

    virtual void draw();
protected:
    static void drawAsix(float size=1);
};

