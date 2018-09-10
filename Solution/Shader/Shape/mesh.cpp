#include "mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool Mesh::activeVAO = false;

void Mesh::addPoint(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty, float r, float g, float b, float a)
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

    //if (inited) init();
}


void Mesh::clear()
{
    position.clear();
    texcoord.clear();
    color.clear();
    normal.clear();

    inited = false;
}

void Mesh::buildCube(Mesh & shp, float x1, float x2, float y1, float y2, float z1, float z2)
{
    shp.drawStyle = Quads;

    shp.clear();

    shp.addPoint(x2, y1, z1, 0, 0, -1, 0, 0);
    shp.addPoint(x1, y1, z1, 0, 0, -1, 1, 0);
    shp.addPoint(x1, y2, z1, 0, 0, -1, 1, 1);
    shp.addPoint(x2, y2, z1, 0, 0, -1, 0, 1);

    //top:4,5,6,7
    shp.addPoint(x1, y1, z2, 0, 0, 1, 0, 0);
    shp.addPoint(x2, y1, z2, 0, 0, 1, 1, 0);
    shp.addPoint(x2, y2, z2, 0, 0, 1, 1, 1);
    shp.addPoint(x1, y2, z2, 0, 0, 1, 0, 1);
    //left:8,9,10,11
    shp.addPoint(x1, y2, z1, 0, -1, 0, 0, 0);
    shp.addPoint(x1, y1, z1, 0, -1, 0, 1, 0);
    shp.addPoint(x1, y1, z2, 0, -1, 0, 1, 1);
    shp.addPoint(x1, y2, z2, 0, -1, 0, 0, 1);
    //right:12,13,14,15
    shp.addPoint(x2, y1, z1, 1, 0, 0, 0, 0);
    shp.addPoint(x2, y2, z1, 1, 0, 0, 1, 0);
    shp.addPoint(x2, y2, z2, 1, 0, 0, 1, 1);
    shp.addPoint(x2, y1, z2, 1, 0, 0, 0, 1);
    //front:16,17,18,19;
    shp.addPoint(x2, y2, z1, 0, 1, 0, 0, 0);
    shp.addPoint(x1, y2, z1, 0, 1, 0, 1, 0);
    shp.addPoint(x1, y2, z2, 0, 1, 0, 1, 1);
    shp.addPoint(x2, y2, z2, 0, 1, 0, 0, 1);
    //back:20,21,22,23;
    shp.addPoint(x1, y1, z1, 0, -1, 0, 0, 0);
    shp.addPoint(x2, y1, z1, 0, -1, 0, 1, 0);
    shp.addPoint(x2, y1, z2, 0, -1, 0, 1, 1);
    shp.addPoint(x1, y1, z2, 0, -1, 0, 0, 1);
}

void Mesh::buildQuad(Mesh & shp, float x1, float x2, float y1, float y2)
{
    shp.drawStyle = Quads;

    shp.clear();

    shp.addPoint(x2, y1, 0, 0, 0, 1, 0, 0);
    shp.addPoint(x1, y1, 0, 0, 0, 1, 1, 0);
    shp.addPoint(x1, y2, 0, 0, 0, 1, 1, 1);
    shp.addPoint(x2, y2, 0, 0, 0, 1, 0, 1);
}

void Mesh::init()
{
    if (activeVAO) {
        buildVAO();
        inited = true;
    }
}

void Mesh::buildVAO()
{
    if (position.empty()) return;

    vData.init(position.size()/3);
    vData.bind();
    vData.setVertex3f(&position[0]);
    vData.setColor4f(&color[0]);
    vData.setNormal3f(&normal[0]);
    vData.setTexCoord2f(&texcoord[0]);
    vData.unBind();

}

void Mesh::ondraw()
{
    if (empty()) return;
    if (!inited) init();
    
    if (!activeVAO) renderClient();
    else renderVAO();
}

void Mesh::renderClient()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(VERTEX_SIZE , GL_FLOAT, 0, &position[0]);
    glColorPointer(COLOR_SIZE, GL_FLOAT, 0, &color[0]);
    glNormalPointer(GL_FLOAT, 0, &normal[0]);
    glTexCoordPointer(TEXCOORD_SIZE , GL_FLOAT, 0, &texcoord[0]);

    glDrawArrays(drawStyle, 0, position.size() / 3);
   

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Mesh::renderVAO()
{
    vData.renderData(drawStyle);
}