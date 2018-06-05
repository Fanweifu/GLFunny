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

        if (imgdata) delete[]  imgdata;
    };

    bool loadImg(char* path);
    uint texID() { return textImgID; }
    void createImg(float r, float g, float b);
    
    void bind();
    void unbind();

protected:
    bool inited = false;
    uint textImgID = 0;
    uchar* imgdata = NULL;
    uint cols, rows, chns;

    void init();
    void readImgData(cv::Mat &img);
};

#endif