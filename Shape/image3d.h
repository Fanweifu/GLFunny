#ifndef IMAGE3D_H
#define IMAGE3D_H

#define IMAGE_RENDER_QUAD 1
#define IMAGE_RENDER_CUBE 2
#define IMAGE_RENDER_TRI 3

#include<assert.h>
#include<opencv2/opencv.hpp>

#include"vaoarray.h"
#include"shader.h"
#include"shape.h"
using namespace std;

class Image3D : public Shape
{
public:

    float pixelSzie = 1;
    float heightRate = 0.5f;

    Image3D();
    ~Image3D();

    bool isEmpty() { return isempty; }
    void setSrcData(cv::Mat& img);
    void generateData();
protected:
   
    virtual void initShader();
    virtual void ondraw();
    virtual void getColor(uint x, uint y, float* outputVec);
    void getColor(uchar* pdata, uint x, uint y, float*outColor);
    virtual void calcHeight();

    bool isempty = true;

    uint cols = 0;
    uint rows = 0;
    uint step = 0;
    uint chns = 0;
    uchar* data = NULL;

    float color4f[4];

    float* heights = NULL;
    float* vertexs = NULL;
    float* colors = NULL;
    float* normal = NULL;
    uint* vertexIdx = NULL;

    ElementData vao;

    bool reShape(int rows, int cols);
    
};

#endif // IMAGE3D_H
