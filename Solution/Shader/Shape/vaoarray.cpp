#include "vaoarray.h"
#include "stdio.h"



//void VAOFunctions::setVertex(GLuint vaObj, GLfloat * valarr, int cnt, bool _static)
//{
//    if (!inited) init();
//
//    glBindVertexArray(vaObj);
//    glBindBuffer(GL_ARRAY_BUFFER, VBOS[VERTEX_VBO_INDEX]);
//    glBufferData(GL_ARRAY_BUFFER, cnt * 3 * sizeof(float), valarr, getEnum(_static));
//
//    glVertexAttribPointer(VERTEX_VBO_INDEX, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//    glEnableVertexAttribArray(VERTEX_VBO_INDEX);
//    glBindVertexArray(0);
//}
//
//void VAOFunctions::setIndice(GLuint vaObj,  GLuint * idxarr, int idxNum, bool _static)
//{
//   
//    glBindVertexArray(vaObj);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOS[INDEX_VBO_INDEX]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxNum * sizeof(GLuint), idxarr, getEnum(_static));
//    glBindVertexArray(0);
//}
//
//void VAOFunctions::setColor(GLuint vaObj, GLfloat * valarr, int vecNum , bool _static)
//{
//    if (!inited) init();
//
//    glBindVertexArray(vaObj);
//    glBindBuffer(GL_ARRAY_BUFFER, VBOS[COLOR_VBO_INDEX]);
//    glBufferData(GL_ARRAY_BUFFER, vecNum * 4 * sizeof(float), valarr, getEnum(_static));
//
//    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
//    glEnableVertexAttribArray(1);
//    glBindVertexArray(0);
//}
//
//void VAOFunctions::setNormal(GLuint vaObj, GLfloat * valarr, int vecNum, bool _static)
//{
//    if (!inited) init();
//
//    glBindVertexArray(vaObj);
//    glBindBuffer(GL_ARRAY_BUFFER, VBOS[NORMAL_VBO_INDEX]);
//    glBufferData(GL_ARRAY_BUFFER, vecNum * 3 * sizeof(float), valarr, getEnum(_static));
//
//    glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
//    glEnableVertexAttribArray(2);
//}
//
//void VAOFunctions::setTexCoord2D(GLuint vaObj, GLfloat * valarr, int vecNum, bool _static)
//{
//    if (!inited) init();
//    glBindVertexArray(vaObj);
//    glBindBuffer(GL_ARRAY_BUFFER, VBOS[TEXCOORD_VBO_INDEX]);
//    glBufferData(GL_ARRAY_BUFFER, vecNum * 3 * sizeof(float), valarr, getEnum(_static));
//
//    glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, 0, (void*)0);
//    glEnableVertexAttribArray(3);
//    glBindVertexArray(0);
//}

GLenum getEnum(bool _static)
{
    return _static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
}

void AttribArray::init(int ptsNum)
{
    vertexNum = ptsNum;
    BindData::init();
}

void AttribArray::setIndex1ui(GLuint * indeice, int idxNum)
{
    indexNum = idxNum;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNum *sizeof(GLuint), indeice, getEnum(true));
}

void AttribArray::setVertex3f(GLfloat * arr)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_VBO_INDEX]);
    glBufferData(GL_ARRAY_BUFFER, vertexNum* VERTEX_SIZE * sizeof(float), arr , getEnum(true));
    glVertexAttribPointer(VERTEX_VBO_INDEX, VERTEX_SIZE , GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(VERTEX_VBO_INDEX);
}

void AttribArray::setColor4f(GLfloat * arr)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR_VBO_INDEX]);
    glBufferData(GL_ARRAY_BUFFER, vertexNum * COLOR_SIZE * sizeof(float), arr, getEnum(true));
    glVertexAttribPointer(COLOR_VBO_INDEX, COLOR_SIZE, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(COLOR_VBO_INDEX);
}

void AttribArray::setNormal3f(GLfloat * arr)
{
   
    glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_VBO_INDEX]);
    glBufferData(GL_ARRAY_BUFFER, vertexNum * NORMAL_SIZE * sizeof(float), arr, getEnum(true));
    glVertexAttribPointer(NORMAL_VBO_INDEX, NORMAL_SIZE, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(NORMAL_VBO_INDEX);
    
   
}

void AttribArray::setTexCoord2f(GLfloat * arr)
{
   
    glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_VBO_INDEX]);
    glBufferData(GL_ARRAY_BUFFER, vertexNum * TEXCOORD_SIZE * sizeof(float), arr, getEnum(true));
    glVertexAttribPointer(TEXCOORD_VBO_INDEX, TEXCOORD_SIZE, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(TEXCOORD_VBO_INDEX);
    
    
}

void AttribArray::drawArray(GLenum mode)
{
    glBindVertexArray(vao);
    glDrawArrays(mode, 0, vertexNum);
    glBindVertexArray(0);
}


BindData::BindData()
{

}

BindData::~BindData()
{
    unInit();
}