#pragma once

#include<stdio.h>
#include<stdint.h>
#include<vector>
#include<Bitmap.h>
#include<Shape\mesh.h>
#include<Shape\layer.h>
#include<Shape\texture.h>
using namespace bmp; 

enum class Button :int {
	LEFT,
	MIDDLE,
	RIGHT,
};

enum class Action :int {
	DOWN,
	UP,
};

class pixaloop {
public:
	bool loadSourceImg(const char* path);
	bool prepare();
	void procKeyEvent(unsigned char key, Action act);
	void procMouseClickEvent(Button mouse, Action act,int x,int y);
	void procMouseDragEvent(int x, int y);
	void render();
	
	void getWindowSize(int& x, int& y);

protected:

	bool triangulation();
	void buildflowMesh();
	void clearVertex();
	void addInitAnchor();
	void beginNewCurve();
	void endCurve();
	void addAnchor(float x, float y);
	void addCurvePoint(float x, float y);
	void simlifyCurve(float tdist);
	void buildAnimator();
	void updateMVP();
	void testFunc();
	float getsplitDist();
	glm::vec2 vertex2Coord(glm::vec2 vertex);
	glm::vec2 windows2Vertex(int x, int y);

	bmp::Bitmap<RgbaC> originMap;
	bmp::Bitmap<uchar> lockMask;
	bmp::Bitmap<RgbaC> excludeMaskMap;
	bmp::Bitmap<RgbaF> flowMap;

private:

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 prj;
	glm::mat4 mvp;

	std::vector<std::vector<glm::vec2>> curves;
	std::vector<glm::vec4> simplifyRays;
	std::vector<glm::vec2> anchors;

	std::vector<size_t> idxs;


	Shader meshShd;
	Shader texShd;
	Shader flowShd;

	Texture2D originTex;
	Mesh meshAnchors;
	Mesh meshcurve;
	Mesh meshRayPoints;
	Mesh meshRayLine;
	Mesh imageQuad;
	Mesh flowMesh;

	bool showPoints = true;
	bool inEditLine = false;
	bool shdDev = false;
	float time = 0;
	float ratio = 0;
	float smooth = 1.0f;
	float scale = 1.0f;
	int dx = -100, dy = -100;
};