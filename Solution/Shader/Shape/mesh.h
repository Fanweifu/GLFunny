#pragma once
#include "shapePRS.h"
#include "vaoarray.h"
#include<vector>

enum DrawType
{
    Triangles = GL_TRIANGLES,
    Quads = GL_QUADS,
    Points = GL_POINTS,
    Lines = GL_LINES
};

class Mesh :
    public ShapePRS
{
public:
    static bool activeVAO;
    DrawType drawStyle = Triangles;

    Mesh();
    ~Mesh();

    void addPoint(float px, float py, float pz, float u = 0, float v = 0, float nx = 0, float ny = 0, float nz = 1, float r = 1, float g = 1, float b = 1, float a = 1);
    void clear();
    bool empty() { return position.size() == 0; }
    
    static void buildCube(Mesh& shp, float x1 = -1, float x2 = 1, float y1 = -1, float y2 = 1, float z1 = -1, float z2 = 1);
    static void buildQuad(Mesh& shp, float x1 = -1, float x2 = 1, float y1 = -1, float y2 = 1);

protected:
    bool inited = false;
    std::vector<float> position;
    std::vector<float> texcoord;
    std::vector<float> color;
    std::vector<float> normal;
    AttribArray vData;

    virtual void init();
    void buildVAO();
    void ondraw() override;
    void renderClient();
    void renderVAO();
};

