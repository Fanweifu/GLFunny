#pragma once
#include "shapePRS.h"
#include "vaoarray.h"
#include<stdio.h>
#include<vector>
class TrianglesShape :
    public ShapePRS
{
public:
    static bool activeVAO;
    TrianglesShape();
    virtual ~TrianglesShape();

    void addPoint(float px, float py, float pz,float nx=0,float ny=0,float nz=1,float tx=0,float ty=0,float r=1,float g=1,float b=1,float a =1);
    void addIndex(unsigned int idx0, unsigned int idx1, unsigned int idx2);
    void clear();
    bool empty() { return position.size()==0; }
protected:
    bool inited = false;
    std::vector<float> position;
    std::vector<float> texcoord;
    std::vector<float> color;
    std::vector<float> normal;
    std::vector<unsigned int> indexes;
    AttribArray vData;

    virtual void init();
    void buildVAO();
    void ondraw() override;
    void renderClient();
    void renderVAO();
};

class Cube: public TrianglesShape {

protected:
    void init() override;
};

