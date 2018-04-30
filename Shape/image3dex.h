#ifndef IMAGE3DEX_H
#define IMAGE3DEX_H

#include"image3d.h"


class Image3DEx: public Image3D
{
public:
    Image3DEx();

    void setDstData(cv::Mat img){
        aimImg.setSrcData(img);
    }


    void active(bool change){ actived = change; }
    void recover(){ t =0; }
    void enableReplay(bool enable){ needreplay = enable; }
    void setChangeSpeed(float sp){ if(sp<=0)return;  step = sp ;}
    bool isplay();

protected:
    bool actived = false;
    bool needreplay = false;


    float step = 0.05;
    float curentspeed =0;
    float t = 0;

    Image3D aimImg;
    float* aimColor4f = new float[4];

    float getPiexlVal(uint x, uint y);

    void ondraw();
    void calT();
};

#endif // IMAG3DEX_H
