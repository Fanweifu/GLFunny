#pragma once
#include<gldef.h>

class ShapeBase
{
public:

    bool isDrawAxis = false;
    float axisLength = 1;
    bool visible = true;


    ShapeBase();
    ~ShapeBase();

    virtual void draw();
protected:
    static void drawAxis(float size=1);
};

