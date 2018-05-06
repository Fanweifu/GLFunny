#include<cstring>
#include "image3d.h"

#define drawOne(ix,iy,fvl,rfvl,bfvl,color)   glColor4fv(color);\
glBegin(GL_QUADS);\
glVertex3f(ix,iy,fvl);\
glVertex3f(ix+1,iy,fvl);\
glVertex3f(ix+1,iy+1,fvl);\
glVertex3f(ix,iy+1,fvl);\
if(ix<cols-1){\
    glVertex3f(ix+1,iy,fvl);\
    glVertex3f(ix+1,iy,rfvl);\
    glVertex3f(ix+1,iy+1,rfvl);\
    glVertex3f(ix+1,iy+1,fvl);\
}\
if(iy<rows-1){\
    glVertex3f(ix,iy+1,fvl);\
    glVertex3f(ix,iy+1,bfvl);\
    glVertex3f(ix+1,iy+1,bfvl);\
    glVertex3f(ix+1,iy+1,fvl);\
}\
glEnd();


Image3D::Image3D()
{

}

Image3D::~Image3D(){
    if(heights) delete heights;
    if(colors) delete colors;
    if(vertexs) delete vertexs;
    isempty = true;
}

void Image3D::setSrcData(cv::Mat img){

    assert(img.depth()==CV_8U&&img.channels()==3);

    initSize(img.rows,img.cols);

    data = img.data;
    chns = img.channels();
    step = img.step;

    calcGray(data,step,rows,cols,chns);


}

void Image3D::generateData(){
    if(vao==NULL) vao = VAOArrayMan::createVAO();

    for(uint y =0;y<rows;y++){
        for(uint x=0;x<cols;x++){
            int stp = y*cols+x;

            getColor(x,y,data+stp,chns,color4f);
            memcpy(colors+stp,color4f,sizeof(color4f));

            float height = heights[stp];

            vertexs[12*stp+0] = x;
            vertexs[12*stp+1] = y;
            vertexs[12*stp+2] = height;

            vertexs[12*stp+3] = x+1;
            vertexs[12*stp+4] = y;
            vertexs[12*stp+5] = height;

            vertexs[12*stp+6] = x+1;
            vertexs[12*stp+7] = y+1;
            vertexs[12*stp+8] = height;

            vertexs[12*stp+9] = x;
            vertexs[12*stp+10] = y+1;
            vertexs[12*stp+11] = height;

        }
    }

    vao->setVertex(vertexs);
    //vao->setColor(colors,4);
    havedata = true;
}

bool Image3D::initSize(int rowc, int colc){
    if(rowc*colc==0) return false;

    if(rows!=rowc||cols!=colc){
        if(vertexs) delete vertexs;
        vertexs = new float[rowc*colc*4*3];

        if(colors) delete colors;
        colors = new float[rowc*colc*4];

        rows = rowc;cols = colc;
    }
}


void Image3D::ondraw(){

    if(!havedata) generateData();

    glEnable(GL_COLOR_MATERIAL);

    glTranslatef(-(float)cols/2,-(float)rows/2,0);
    glScalef(pixelSzie,pixelSzie,1);

    vao->renderData(GL_QUADS);
    //drawpiexls(0,0,cols,rows);

    glDisable(GL_COLOR_MATERIAL);

}

void Image3D::drawpiexls(uint x, uint y, uint w, uint h){

    uint xcols = min(cols,x+w);
    uint yrows = min(rows,y+h);

        for(uint ix=x;ix<xcols;ix++){
            for(uint iy =y;iy<yrows;iy++){
                    float fval = getPiexlVal(ix,iy);
                    float rval = getPiexlVal(ix+1,iy);
                    float bval = getPiexlVal(ix,iy+1);

                //}

            }

    }

}

float Image3D::getPiexlVal(uint x,uint y){
    if(isempty) return 0;
    return heights[y*rows+x];
}


void Image3D::getColor(uint x, uint y, uchar*pixelPtr, uint chns, float*color4){
    if(isempty) return;

    if(chns>=3){
        for(uint i=0;i<3;i++) color4[i] = (float)pixelPtr[2-i]/255;
        if(chns==4) color4[3] = (float)pixelPtr[3]/255;
        else color4[3] = 1;
    }else if(chns==1){
        color4[1] = color4[2] = color4[3] = (float)pixelPtr[0]/255;
        color4[3] = 1;
    }
}





float* Image3D::calcGray(uchar* data,uint step,uint rows,uint cols,uint chns){

    if(heights==NULL) heights = new float[cols*rows];
    else if(sizeof(heights)/sizeof(float)!= cols*rows){
        delete [] heights;
        heights = new float[cols*rows];
    }

    for(uint x=0;x<rows;x++){
        for(uint y=0;y<cols;y++){
            uint sum =0;
            for(uint c=0;c<chns;c++){
                sum+= data[x*step+y*chns+c];
            }
            heights[x*cols+y] = (float)sum/chns;
        }
    }

    return heights;

}
