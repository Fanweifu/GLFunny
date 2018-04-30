#ifndef IMAGE3D_H
#define IMAGE3D_H

#define IMAGE_RENDER_QUAD 1
#define IMAGE_RENDER_CUBE 2
#define IMAGE_RENDER_TRI 3

#include<assert.h>
#include<opencv2/opencv.hpp>

#include"vaoarray.h"
#include"shape.h"


using namespace std;


class Image3D: public Shape
{
public:

    float pixelSzie = 1;


    Image3D();
    ~Image3D();

    bool isEmpty(){ return isempty; }
    void setSrcData(cv::Mat img);

    virtual float getPiexlVal(uint x,uint y);


protected :
    bool havedata = false;
    void generateData();


    virtual void ondraw();
    void drawpiexls(uint x,uint y,uint cols, uint rows);
    void getColor(uint x,uint y,uchar*pixptr,uint chns, float* outputVec );
private :

    bool isempty = true;


    uint cols = 0;
    uint rows = 0;
    uchar* data = NULL;
    uint step = 0;
    uint chns = 0;

    float color4f[4];

    float* heights = NULL;
    float * vertexs = NULL;
    float * colors = NULL;
    VAOArray* vao = NULL;

    bool initSize(int rows,int cols);
    float* calcGray(uchar* data,uint step,uint rows,uint cols,uint chns);

};

#endif // IMAGE3D_H
