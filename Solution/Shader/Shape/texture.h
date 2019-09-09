#ifndef TEXTRUE_H
#define TEXTRUE_H

#include"stb\stb_image.h"
#include"stb\stb_image_write.h"
#include"stb\stb_image_resize.h"
#include"object.h"
#include"shader.h"
#include"vaoarray.h"
#include"shape.h"

class FBObject : public glObject {
public:
	FBObject();
	~FBObject();

	void init() override;
	void unInit() override;
	void bind() override;
	void unBind() override;
	bool vaild() override;
	void clearBuffers();
	int Width() { return width; }
	int Height() { return height; }
	void attchStencilDepth(GLuint tex);
	void attchColor(GLuint tex);
	void attchFloatColor(GLuint tex);
	void resize(int w, int h);

private:
	int width = 500, height = 500;
	std::map<GLenum, GLint> texbindinfo;
};

enum class FilterType:GLint
{
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
};

class  Texture : public glObject {
public:

	Texture() {};
	~Texture() {};

	unsigned int Level() { return m_activeLevel; }
	
	virtual void bind(int level) = 0;
	void setFilter(FilterType t);
	void init() override;
	void unInit() override;
	void bind() override;

protected:
	unsigned int m_activeLevel = 0;
};

class Texture2D : public Texture {
public:
	bool empty() { return Width() == 0 || Height() == 0; }
	int Width() { return m_width; }
	int Height() { return m_height; }

	void bind(int level = 0) override;
	void unBind() override;

	bool loadImg(const char* path, int desChns = 4);
	bool loadBin(const char* path, int w, int h, int chn);
	bool setTexImg(const void *data, int cols, int rows, int interformat = GL_RGBA, int format = GL_RGBA, int type = GL_UNSIGNED_BYTE);
	bool updateImg(const void *newData, int left, int top, int width, int height);
	void buildByColor(float r, float g, float b, float a = 1);
	void attchDepthStencilFBO(FBObject &fbo);
	void attchColorFBO(FBObject &fbo);

protected:
	int m_width = 0;
	int m_height = 0;
	int m_innerformat = 0;
	int m_format = 0;
	int m_dataType = 0;
};

class DepthTexture : public Texture2D {
public:
	int width = 4096, height = 4096;
	float n = 0.0f, f = 1000.0f, distance = 100.0f;
	float range = 50;

	DepthTexture();
	~DepthTexture();

	//camera position:vec3(cx,cy,cz)  vec3(lx,ly,lz) is lw==0? light dircation £º spot light position
	void begin(float cx, float cy, float cz, float lx, float ly, float lz, float lw = 0);
	void end();
	float shadowAcneBias();
	const float* getLightPrjViewMat() { return glm::value_ptr(lightPrjViewMat); }
protected:
	glm::mat4 lightPrjMat, lightViewMat, lightPrjViewMat;
};

#endif