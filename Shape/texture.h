#ifndef TEXTRUE_H
#define TEXTRUE_H

#include"stb_image.h"
#include"shader.h"
#include"shape.h"
class Texture {
public:

    Texture() {};
    ~Texture() {
        glDeleteTextures(1, &texID);
    };

    unsigned int TexID() { return texID; }

    virtual void bind(int level = 0);
    virtual void unbind();

protected:
    bool inited = false;
    bool isValid = false;
    int level = 0;
    unsigned int texID = 0;

    virtual void init();
};

class ImgTexture :public Texture {
public:
    bool empty() { return cols == 0 || rows == 0 || chns == 0; }
    int width() { return cols; }
    int height() { return rows; }
    int chanels() { return chns; }

    bool loadRgbImg(char* path);
    void makeColor(float r, float g, float b, float a = 1);
protected:
    int cols, rows, chns;
};

class DepthTexture : public Texture {
public:
    DepthTexture();
    ~DepthTexture();
    int shadowSmooth = 2;
    bool enablePbr = false;
    bool loadDepthMap(float camposx, float camposy, float camposz, float lightx, float lighty, float lightz, float lightw, Shape& scene);
    void setViewMatInv(const float* matptr);
    void bindShadow();
    void unbindShadow();
protected:
    GLuint depthMapFBO = 0;
    int width = 2048, height = 2048;
    float n = 0.0f, f = 1000.0f, distance = 100.0f;
    float range = 100;
    glm::mat4 lightPrjMat, lightViewMat, lightPrjViewMat;
    Shader shadowPro;
    void init();
    float calcBias();
};

#endif