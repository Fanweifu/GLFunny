#include "shape.h"
#include<cstring>
Shape::Shape()
{

}



void Shape::draw(){

    glPushMatrix();

    glMultMatrixf(modelmat.get());

    ondraw();

    if(isdrawAxis) Shape::drawAsix(axisLength);

    glPopMatrix();
}

void Shape::updateModel(){
    modelmat.identity();
    modelmat.translate(px,py,pz);
    modelmat.scale(sx,sy,sz);
    modelmat.rotateZ(rz);
    modelmat.rotateY(ry);
    modelmat.rotateX(rx);
    modelmatInv = modelmat.invert();
}

float* Shape::getModelMat(){
    const float* ar = modelmat.get();
    float dst[16];
    memcpy(dst,ar,sizeof(ar));
    return dst;
}

void Shape::toLocalPos(float &x, float &y, float &z){
    Vector3 vec(x,y,z);
    vec =vec*modelmatInv;

    x = vec[0];
    y = vec[1];
    z = vec[2];
}

void Shape::toWorldPos(float &x, float &y, float &z){
    Vector3 vec(x,y,z);
    vec =vec*modelmat;
    x = vec[0];
    y = vec[1];
    z = vec[2];
}

void Shape::drawAsix(float size){

    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
    glDisable(GL_LIGHTING);
    glPushMatrix();             //NOTE: There is a bug on Mac misbehaviours of
    //      the light position when you draw GL_LINES
    //      and GL_POINTS. remember the matrix.

    // draw axis
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);
    glEnd();
    glLineWidth(1);

    // draw arrows(actually big square dots)
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    glVertex3f(size, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, size, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, size);
    glEnd();
    glPointSize(1);

    // restore default settings
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
}

void Shape::ondraw(){
    if(testDrawFunc) (*testDrawFunc)();
}
