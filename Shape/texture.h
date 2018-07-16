#ifndef TEXTRUE_H
#define TEXTRUE_H

#include<gl/glew.h>
#include<gl/GL.h>
#include<opencv2/opencv.hpp>
class Texture {
public:

    Texture() {};
    ~Texture() {
        glDeleteTextures(1, &textImgID);
    };

    uint texID() { return textImgID; }
       
    void bind(uint level=0);
    void unbind();

protected:
    bool inited = false;
    bool isValid = false;
    uint textImgID = 0;

    void init();
    
};

class ImgTexture :public Texture {

public:
    bool loadRgbImg(char* path);
    uint cols, rows, chns;
    
    void makeSingleColor(float r, float g, float b);
protected:
    uchar * readImgData(cv::Mat &img);
};


#endif