#include "vaoarray.h"
#include "stdio.h"



bool VAOArrayMan::inited = false;
GLuint VAOArrayMan::capNum = 0;
GLuint VAOArrayMan::curNum = 0;
GLuint VAOArrayMan::VAOS[VAONUM];
GLuint VAOArrayMan::VBOS[VBONUM];

void VAOArrayMan::init(){
    VAOArrayMan::capNum = VAONUM;

    GLenum err = glewInit();
    const unsigned char*str = glewGetString(err);

    glGenVertexArrays(VAONUM,VAOS);
    glGenBuffers(VBONUM, VBOS);

    curNum = 0;
}

VAOArray* VAOArrayMan::createVAO(){
    if(!VAOArrayMan::inited) VAOArrayMan::init();

    VAOArray* arr = new VAOArray();
    arr->vaoID = curNum++;
    return arr;
}

void VAOArrayMan::setVertex(GLuint vaoId, GLfloat *arr){
    assert(vaoId<curNum);

    glBindVertexArray(VAOArrayMan::VAOS[vaoId]);
    glBindBuffer(GL_ARRAY_BUFFER, VAOArrayMan::VBOS[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr, GL_DYNAMIC_DRAW);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
}

void  VAOArrayMan::setColor(GLuint vaoId, GLfloat *arr,int vecNum){
    assert(vaoId<curNum);

    glBindVertexArray(VAOS[vaoId]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr, GL_DYNAMIC_DRAW);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(vecNum, GL_FLOAT, 0, NULL);
}

void  VAOArrayMan::setNormal(GLuint vaoId, GLfloat *arr){
    assert(vaoId<curNum);

    glBindVertexArray(VAOS[vaoId]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[2]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr, GL_DYNAMIC_DRAW);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, NULL);
}

void  VAOArrayMan::setTexCoord(GLuint vaoId, GLfloat *arr){
    assert(vaoId<curNum);

    glBindVertexArray(VAOS[vaoId]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[3]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr, GL_DYNAMIC_DRAW);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, NULL);

}

void  VAOArray::setVertex(GLfloat *arr){
    VAOArrayMan::setVertex(vaoID,arr);
}
void  VAOArray::setColor(GLfloat *arr,int vecNum){
    VAOArrayMan::setColor(vaoID,arr,vecNum);

}
void  VAOArray::setNormal(GLfloat *arr){
    VAOArrayMan::setNormal(vaoID,arr);

}
void  VAOArray::setTexCoord(GLfloat *arr){
    VAOArrayMan::setTexCoord(vaoID,arr);
}

void VAOArray::renderData(){
    glBindVertexArray(vaoID);
    glDrawArrays(GL_QUADS, 0, 4);

}
