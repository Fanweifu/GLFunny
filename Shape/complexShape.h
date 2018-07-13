#pragma once
#include "shape.h"
#include "vaoarray.h"
#include<stdio.h>
#include<vector>
class ComplexShape :
    public Shape
{
public:
    ComplexShape();
    virtual ~ComplexShape();

    void addPoint(float px, float py, float pz,float nx=0,float ny=0,float nz=1,float tx=0,float ty=0,float r=1,float g=1,float b=1,float a =1);
    void clear();

protected:
    bool inited = false;
    std::vector<float> position;
    std::vector<float> texcoord;
    std::vector<float> color;
    std::vector<float> normal;
    ArrayData buffdata;

    void init();
    void ondraw();
};

