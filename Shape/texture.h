#ifndef TEXTRUE_H
#define TEXTRUE_H

#include<opencv2/opencv.hpp>
#include"shader.h"
#include"shapebase.h"
class Texture {
public:

    Texture() {};
    ~Texture() {
        glDeleteTextures(1, &texID);
    };

    unsigned int TexID() { return texID; }
    
    virtual void bind(int level =0);
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
    bool loadRgbImg(char* path);
    uint cols, rows, chns;
    
    void makeSingleColor(float r, float g, float b);
protected:
    uchar * readImgData(cv::Mat &img);
};

class DepthTexture : public Texture {

public:
    DepthTexture();
    
    bool enablePbr = false;
    bool loadDepthMap(float camposx, float camposy, float camposz, float lightx, float lighty, float lightz, float lightw ,ShapeBase& scene);
    void updateViewInv(glm::mat4& cameraViewInv);
    void bindShadow();
    void unbindShadow();
protected:
    GLuint depthMapFBO = 0;
    int width =1024 , height = 1024;
    float n = 0.0f, f = 1000.0f, distance = 10.0f;
    float range = 200.0f;
    glm::mat4 lightPrjMat, lightViewMat, lightPrjViewMat;
    Shader shadowPro;
    void init();
    float calcBias();
};


#endif