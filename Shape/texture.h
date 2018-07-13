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

    bool loadImg(char* path);
    uint texID() { return textImgID; }
    void makeColor(float r, float g, float b);
    
    void bind(uint level=0);
    void unbind();

protected:
    bool inited = false;
    uint textImgID = 0;
    uint cols, rows, chns;

    void init();
    uchar * readImgData(cv::Mat &img);
};

#endif