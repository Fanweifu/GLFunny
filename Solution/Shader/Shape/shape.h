#pragma once
#include<gl_def.h>

class Shape
{
public:

    bool isDrawAxis = false;
    float axisLength = 1;
    bool visible = true;


    Shape();
    ~Shape();

    virtual void draw();
protected:
    static void drawAxis(float size=1);
};

