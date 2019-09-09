#include "mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool Mesh::activeVAO = false;

void Mesh::addPoint(float px, float py, float pz, float u, float v, float nx, float ny, float nz, float r, float g, float b, float a)
{
	meshVertex point;
	point.vertex = glm::vec3(px, py, pz);
	point.coord = glm::vec2(u, v);
	point.normal = glm::vec3(nx, ny, nz);
	point.color = glm::vec4(r, g, b, a);
	addPoint(point);
}

void Mesh::addPoint(const meshVertex& vertex)
{
	mBuffer.push_back(vertex);
	if (Mesh::activeVAO) inited = false;
}

void Mesh::updatePoint(int idx, const meshVertex& vertex){
	if(idx < 0 || idx >= count()) return;
	mBuffer[idx] = vertex;
}





void Mesh::clear()
{
	mBuffer.clear();
    inited = false;
}

void Mesh::buildCube(Mesh & shp, float x1, float x2, float y1, float y2, float z1, float z2)
{
    shp.drawStyle = DrawType::Quads;

    shp.clear();

    shp.addPoint(x2, y1, z1, 0, 0, 0, 0, -1);
    shp.addPoint(x1, y1, z1, 1, 0, 0, 0, -1);
    shp.addPoint(x1, y2, z1, 1, 1, 0, 0, -1);
    shp.addPoint(x2, y2, z1, 0, 1, 0, 0, -1);

    //top:4,5,6,7
    shp.addPoint(x1, y1, z2, 0, 0, 0, 0, 1);
    shp.addPoint(x2, y1, z2, 1, 0, 0, 0, 1);
    shp.addPoint(x2, y2, z2, 1, 1, 0, 0, 1);
    shp.addPoint(x1, y2, z2, 0, 1, 0, 0, 1);
    //left:8,9,10,11
    shp.addPoint(x1, y2, z1, 0, 0, 0, -1, 0);
    shp.addPoint(x1, y1, z1, 1, 0, 0, -1, 0);
    shp.addPoint(x1, y1, z2, 1, 1, 0, -1, 0);
    shp.addPoint(x1, y2, z2, 0, 1, 0, -1, 0);
    //right:12,13,14,15
    shp.addPoint(x2, y1, z1, 0, 0, 1, 0, 0);
    shp.addPoint(x2, y2, z1, 1, 0, 1, 0, 0);
    shp.addPoint(x2, y2, z2, 1, 1, 1, 0, 0);
    shp.addPoint(x2, y1, z2, 0, 1, 1, 0, 0);
    //front:16,17,18,19;
    shp.addPoint(x2, y2, z1, 0, 0, 0, 1, 0);
    shp.addPoint(x1, y2, z1, 1, 0, 0, 1, 0);
    shp.addPoint(x1, y2, z2, 1, 1, 0, 1, 0);
    shp.addPoint(x2, y2, z2, 0, 1, 0, 1, 0);
    //back:20,21,22,23;
    shp.addPoint(x1, y1, z1, 0, 0, 0, -1, 0);
    shp.addPoint(x2, y1, z1, 1, 0, 0, -1, 0);
    shp.addPoint(x2, y1, z2, 1, 1, 0, -1, 0);
    shp.addPoint(x1, y1, z2, 0, 1, 0, -1, 0);
}

void Mesh::buildQuad(Mesh & shp, float x1, float x2, float y1, float y2)
{
    shp.drawStyle = DrawType::Quads;

    shp.clear();

    shp.addPoint(x2, y1, 0, 0, 0, 0, 0, 1);
    shp.addPoint(x1, y1, 0, 1, 0, 0, 0, 1);
    shp.addPoint(x1, y2, 0, 1, 1, 0, 0, 1);
    shp.addPoint(x2, y2, 0, 0, 1, 0, 0, 1);
}

void Mesh::init()
{
    if (activeVAO) {
        buildVAO();
        inited = true;
    }
}

#define OFFSET(s, m) (&((s*)0)->m)

void Mesh::initlayout()
{
	mlayouts.clear();
	int stride = sizeof(meshVertex);
	dataLayout vertex = {nullptr,0,stride,3,OFFSET(meshVertex,vertex)};
	mlayouts.push_back(vertex);
	dataLayout coord = { nullptr,1,stride,2,OFFSET(meshVertex,coord) };
	mlayouts.push_back(coord);
	dataLayout normal = { nullptr,2,stride,3,OFFSET(meshVertex,normal) };
	mlayouts.push_back(normal);
	dataLayout color = { nullptr,3,stride,4,OFFSET(meshVertex,color) };
	mlayouts.push_back(color);

}

void Mesh::buildVAO()
{
    if (count()==0) return;

    vData.init();
    vData.bind();

	vData.updateBuffer(&mBuffer[0], mBuffer.size()*sizeof(meshVertex));

	if (mlayouts.size() == 0) initlayout();

	vData.setLayouts(mlayouts);
    vData.unBind();
}


void Mesh::ondraw()
{
    if (count()==0) return;
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

    glVertexPointer(3, GL_FLOAT, sizeof(meshVertex), &mBuffer[0].vertex);
    glColorPointer(4, GL_FLOAT, sizeof(meshVertex), &mBuffer[0].color);
    glNormalPointer(GL_FLOAT, sizeof(meshVertex), &mBuffer[0].normal);
    glTexCoordPointer(3, GL_FLOAT, sizeof(meshVertex), &mBuffer[0].coord);

    glDrawArrays((GLenum)drawStyle, 0, mBuffer.size());

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Mesh::renderVAO()
{
    vData.renderData((GLenum)drawStyle,mBuffer.size());
}