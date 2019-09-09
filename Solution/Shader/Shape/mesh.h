#pragma once
#include "shapePRS.h"
#include "vaoarray.h"
#include<vector>


enum class DrawType :GLenum
{
	Triangles = GL_TRIANGLES,
	TrianglesFan = GL_TRIANGLE_FAN,
    Quads = GL_QUADS,
    Points = GL_POINTS,
    Lines = GL_LINES,
	LineStrip = GL_LINE_STRIP
};

struct meshVertex
{
	glm::vec3 vertex;
	glm::vec2 coord;
	glm::vec3 normal;
	glm::vec4 color;

	meshVertex():vertex(0),coord(0),normal(0),color(0) {}
};


class Mesh :
    public ShapePRS
{
public:
    static bool activeVAO;
    DrawType drawStyle = DrawType::Triangles;

    Mesh();
    ~Mesh();

    void addPoint(float px, float py, float pz, float u = 0, float v = 0, float nx = 0, float ny = 0, float nz = 1, float r = 1, float g = 1, float b = 1, float a = 1);
	void addPoint(const meshVertex& vertex);
	void updatePoint(int idx, const meshVertex& vertex);
    void clear();
	bool count() const { return mBuffer.size(); }
    
    static void buildCube(Mesh& shp, float x1 = -1, float x2 = 1, float y1 = -1, float y2 = 1, float z1 = -1, float z2 = 1);
    static void buildQuad(Mesh& shp, float x1 = -1, float x2 = 1, float y1 = -1, float y2 = 1);

protected:
    bool inited = false;
	std::vector<meshVertex> mBuffer;
	std::vector<dataLayout> mlayouts;
    AttribVAO vData;

    virtual void init();
	void initlayout();
    void buildVAO();
    void ondraw() override;
    void renderClient();
    void renderVAO();
};

