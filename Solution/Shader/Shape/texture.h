#ifndef TEXTRUE_H
#define TEXTRUE_H

#include"stb_image.h"
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

    int Width() { return width; }
    int Height() { return height; }
    void attchDepth(GLuint tex);
    void attchColor(GLuint tex, int clrlv = 0);
    void resize(int w, int h);

private:
    unsigned int m_fbo;
    int width = 500, height = 500;
    std::map<GLenum, GLint> texbindinfo;
};

class Texture : public glObject {
public:

    Texture() {};
    ~Texture() {};

    unsigned int TexID() { return m_texID; }
    unsigned int Level() { return m_activeLevel; }

    virtual void bind(int level)=0;
    void init() override;
    void unInit() override;
    void bind() override;
    
protected: 
    unsigned int m_activeLevel = 0;
    unsigned int m_texID = 0;

    
};

class Texture2D : public  Texture {
public:
    bool empty() { return Width() == 0 || Height() == 0; }
    int Width();
    int Height();

    void bind(int level) override;
    void unBind() override;

    bool loadFileImg(char* path);
    void buildByColor(float r, float g, float b, float a = 1);
    void attchDepthFBO(FBObject &fbo);
    void attchColorFBO(FBObject &fbo);

protected:
    

};

class ImgTexture :public Texture {

};

class DepthTexture : public Texture2D{
public:
    int width = 4096, height = 4096;
    float n = 0.0f, f = 1000.0f, distance = 100.0f;
    float range = 50;

    DepthTexture();
    ~DepthTexture();

    void beginLoad(float camposx, float camposy, float camposz, float lightx, float lighty, float lightz, float lightw);
    void endLoad();

    float shadowAcneBias();
    const float* getLightPrjViewMat() { return glm::value_ptr(lightPrjViewMat); }

protected:
    GLuint depthMapFBO = 0;
    glm::mat4 lightPrjMat, lightViewMat, lightPrjViewMat;
    void init() override;
};

#endif