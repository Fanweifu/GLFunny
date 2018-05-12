#ifndef IMAGE3DEX_H
#define IMAGE3DEX_H

#include"image3d.h"

class Image3DEx : public Image3D
{
public:
    Image3DEx();

    void setAnimation(cv::Mat& dst);

    void active(bool change) { actived = change; }
    void recover() { t = 0; }
    void enableReplay(bool enable) { needreplay = enable; }
    void setChangeSpeed(float sp) { if (sp <= 0) return;  step = sp; }

protected:
    bool actived = false;
    bool needreplay = false;
    string tstr = "t";
    float curentspeed = 0.003f;
    float t = 0;

    uchar* dstData = NULL;

    void calcHeight();
    void getColor(uint x, uint y, float* outputVec);
    void initShader();

    void ondraw();
    void calT();
};

#endif // IMAG3DEX_H
