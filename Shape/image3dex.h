#ifndef IMAGE3DEX_H
#define IMAGE3DEX_H

#include"image3d.h"

class Image3DEx : public Image3D
{
public:

    float speed = 0.008f;

    Image3DEx();

   

    void setDstImage(cv::Mat& dst);

    void active(bool change) { actived = change; }
    void recover() { t = 0; }
    void initShader();

protected:
    bool actived = false;
    bool shaderinited = false;
    
    float t = 0;
    string tstr = "t";

    uchar* dstData = NULL;
    uint dstchns = 0;
    uint dststep = 0;

    Shader innerShd;
    void calcHeight();
    void getColor(uint x, uint y, float* outputVec);
    void ondraw();
    void calT();
};

#endif // IMAG3DEX_H
