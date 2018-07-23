#pragma once
#include<glHead.h>

class Shape
{
public:

    bool drawAxis = false;
    float axisLength = 1;
    bool visible = true;


    Shape();
    ~Shape();

    virtual void draw();
protected:
    static void drawAsix(float size=1);
};

