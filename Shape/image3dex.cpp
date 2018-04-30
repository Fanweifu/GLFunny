#include "image3dex.h"



Image3DEx::Image3DEx()
{

}


float Image3DEx::getPiexlVal(uint x, uint y){
    float vl = Image3D::getPiexlVal(x,y);
    float v2 = aimImg.getPiexlVal(x,y);

    return aimImg.isEmpty()?vl:vl*(1-t)+v2*t;
}


void Image3DEx::calT(){
    if(!actived) return;
    if(t<=0) curentspeed = step;
    t+=curentspeed;
    if(t>=1&&needreplay) t = 0;
}

void Image3DEx::ondraw(){
    calT();
    Image3D::ondraw();
}
