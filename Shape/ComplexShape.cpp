#include "ComplexShape.h"

TrianglesShape::TrianglesShape()
{
}

TrianglesShape::~TrianglesShape()
{
}

bool TrianglesShape::activeVAO = false;

void TrianglesShape::addPoint(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty, float r, float g, float b, float a)
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

void TrianglesShape::addIndex(unsigned int idx0, unsigned int idx1, unsigned int idx2)
{
    indexes.push_back(idx0);
    indexes.push_back(idx1);
    indexes.push_back(idx2);

    if (inited) init();
}

void TrianglesShape::clear()
{
    position.clear();
    texcoord.clear();
    color.clear();
    normal.clear();
}

void TrianglesShape::init()
{
    if (activeVAO) {
        buildVAO();
        inited = true;
    }
}

void TrianglesShape::buildVAO()
{
    vData.init(position.size()/3);
    vData.setVertex3f(&position[0]);
    vData.setColor4f(&color[0]);
    vData.setNormal3f(&normal[0]);
    vData.setTexCoord2f(&texcoord[0]);

    if (!indexes.empty()) vData.setIndex1ui(&indexes[0], indexes.size());
}

void TrianglesShape::ondraw()
{
    if (!inited) init();
    if (empty()) return;

    if (!activeVAO) renderClient();
    else renderVAO();
}

void TrianglesShape::renderClient()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &position[0]);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, &color[0]);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, &normal[0]);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, &texcoord[0]);

    if (indexes.empty()) glDrawArrays(GL_TRIANGLES, 0, position.size() / 3);
    glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, &indexes[0]);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void TrianglesShape::renderVAO()
{
    if (!indexes.empty()) vData.drawElements(GL_TRIANGLES);
    vData.drawArray(GL_TRIANGLES);
}

void Cube::init()
{
    //buttom£º0,1,2,3
    addPoint(1, -1, -1, 0, 0, -1, 0, 0);
    addPoint(-1, -1, -1, 0, 0, -1, 1, 0);
    addPoint(-1, 1, -1, 0, 0, -1, 1, 1);
    addPoint(1, 1, -1, 0, 0, -1, 0, 1);

    //top:4,5,6,7
    addPoint(-1, -1, 1, 0, 0, 1, 0, 0);
    addPoint(1, -1, 1, 0, 0, 1, 1, 0);
    addPoint(1, 1, 1, 0, 0, 1, 1, 1);
    addPoint(-1, 1, 1, 0, 0, 1, 0, 1);
    //left:8,9,10,11
    addPoint(-1, 1, -1, 0, -1, 0, 0, 0);
    addPoint(-1, -1, -1, 0, -1, 0, 1, 0);
    addPoint(-1, -1, 1, 0, -1, 0, 1, 1);
    addPoint(-1, 1, 1, 0, -1, 0, 0, 1);
    //right:12,13,14,15
    addPoint(1, -1, -1, 1, 0, 0, 0, 0);
    addPoint(1, 1, -1, 1, 0, 0, 1, 0);
    addPoint(1, 1, 1, 1, 0, 0, 1, 1);
    addPoint(1, -1, 1, 1, 0, 0, 0, 1);
    //front:16,17,18,19;
    addPoint(1, 1, -1, 0, 1, 0, 0, 0);
    addPoint(-1, 1, -1, 0, 1, 0, 1, 0);
    addPoint(-1, 1, 1, 0, 1, 0, 1, 1);
    addPoint(1, 1, 1, 0, 1, 0, 0, 1);
    //back:20,21,22,23;
    addPoint(-1, -1, -1, 0, -1, 0, 0, 0);
    addPoint(1, -1, -1, 0, -1, 0, 1, 0);
    addPoint(1, -1, 1, 0, -1, 0, 1, 1);
    addPoint(-1, -1, 1, 0, -1, 0, 0, 1);

    addIndex(0, 1, 2);
    addIndex(0, 2, 3);

    addIndex(4, 5, 6);
    addIndex(4, 6, 7);

    addIndex(8, 9, 10);
    addIndex(8, 10, 11);

    addIndex(12, 13, 14);
    addIndex(12, 14, 15);

    addIndex(16, 17, 18);
    addIndex(16, 18, 19);

    addIndex(20, 21, 22);
    addIndex(20, 22, 23);

    TrianglesShape::init();
}