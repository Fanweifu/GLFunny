#include "ComplexShape.h"

ComplexShape::ComplexShape()
{
}

ComplexShape::~ComplexShape()
{
}

void ComplexShape::addPoint(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty, float r, float g, float b, float a)
{
    position.push_back(px);
    position.push_back(py);
    position.push_back(pz);

    normal.push_back(nx);
    normal.push_back(ny);
    normal.push_back(nz);

    texcoord.push_back(tx);
    texcoord.push_back(ty);

    color.push_back(r);
    color.push_back(g);
    color.push_back(b);
    color.push_back(a);

    if (inited) init();
}

void ComplexShape::clear()
{
    position.clear();
    texcoord.clear();
    color.clear();
    normal.clear();
}

void ComplexShape::init()
{
    buffdata.vertexNums = position.size()/3;

    buffdata.setVertex(&position[0]);
    buffdata.setColor(&color[0]);
    buffdata.setNormal(&normal[0]);
    buffdata.setTexCoord(&texcoord[0]);

    inited = true;
}

void ComplexShape::ondraw()
{
    if (!inited) init();
    buffdata.renderData(GL_TRIANGLES);
}
