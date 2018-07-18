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
       
    virtual void bind(int level=0);
    virtual void unbind();

protected:
    bool inited = false;
    bool isValid = false;
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
    float n = 0.1, f = 10000, distance = 10;
    bool loadDepthMap(float camposx, float camposy, float camposz, float lightx, float lighty, float lightz, float lightw, float width, float height,ShapeBase& scene);
    void bind(int levle = 0);
    void unbind();
protected:
    GLuint fboID = 0 ;
    float widthf, heightf;
    glm::mat4 lightPrjMat, lightViewMat, lightPrjViewMat;
    Shader shadowPro;
    void init();
};


#endif