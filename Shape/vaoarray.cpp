#include "vaoarray.h"
#include "stdio.h"

bool VertexArray::inited = false;
GLuint VertexArray::VBOS[VBOCNT];

void VertexArray::init() {
    if (!inited) {
        glewInit();
        glGenBuffers(VBOCNT, VBOS);
        inited = true;
    }
}

void VertexArray::setVertex(GLuint vaObj, GLfloat *arr, int num, bool _static) {
    if (!inited) init();

    glBindVertexArray(vaObj);

    glBindBuffer(GL_ARRAY_BUFFER, VertexArray::VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(float), arr, getEnum(_static));

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
}

void VertexArray::setVertexElem(GLuint vaObj, GLfloat * valarr, int cnt, bool _static)
{
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, cnt * 3 * sizeof(float), valarr, getEnum(_static));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void VertexArray::setIndice(GLuint vaObj, GLuint * idxarr, int idxNum,bool _static)
{
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOS[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxNum * sizeof(GLuint), idxarr , getEnum(_static));
}

void  VertexArray::setColor(GLuint vaObj, GLfloat *arr, int cnt, int chns,bool _static) {
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);

    glBufferData(GL_ARRAY_BUFFER, cnt * chns * sizeof(float), arr, getEnum(_static));

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(chns, GL_FLOAT, 0, NULL);
}

void VertexArray::setColorElem(GLuint vaObj, GLfloat * valarr, int vecNum, int chns, bool _static)
{
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
    glBufferData(GL_ARRAY_BUFFER, vecNum * chns * sizeof(float), valarr, getEnum(_static));

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
}

void VertexArray::setNormalElem(GLuint vaObj, GLfloat * valarr, int vecNum, bool _static)
{
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[2]);
    glBufferData(GL_ARRAY_BUFFER, vecNum * 3 * sizeof(float), valarr, getEnum(_static));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(2);
}

void VertexArray::setTexCoord2DElem(GLuint vaObj, GLfloat * valarr, int vecNum, bool _static)
{
    if (!inited) init();
    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[2]);
    glBufferData(GL_ARRAY_BUFFER, vecNum * 3 * sizeof(float), valarr, getEnum(_static));

    glVertexAttribPointer(3, 2 , GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(3);
}




void  VertexArray::setNormal(GLuint vaObj, GLfloat *arr, int num, bool _static) {
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[2]);

    glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(float), arr, getEnum(_static));
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, NULL);
}

void  VertexArray::setTexCoord(GLuint vaObj, GLfloat *arr, int num, bool _static) {
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[3]);

    glBufferData(GL_ARRAY_BUFFER, num * 2 * sizeof(float), arr, getEnum(_static));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, NULL);
}

GLenum VertexArray::getEnum(bool _static)
{
    return _static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
}


void  BufferData::setVertex(GLfloat *arr) {
    VertexArray::setVertex(getVAOID(), arr, vertexNums);
}
void  BufferData::setColor(GLfloat *arr, int vecNum) {
    VertexArray::setColor(getVAOID(), arr, vertexNums, vecNum);
}
void  BufferData::setNormal(GLfloat *arr) {
    VertexArray::setNormal(getVAOID(), arr, vertexNums);
}
void  BufferData::setTexCoord(GLfloat *arr) {
    VertexArray::setTexCoord(getVAOID(), arr, vertexNums);
}

void BufferData::renderData(GLenum mode)
{
    glBindVertexArray(vao);
    glDrawArrays(mode, 0, vertexNums);

    //glDisableVertexAttribArray(0);

    //glDrawArrays(mode, 0, vertexNums);
}

void ElementData::setIndex(GLuint * indeice, int idxNum)
{
    vertexNum = idxNum;
    VertexArray::setIndice(getVAOID(), indeice, idxNum);
}

void ElementData::setVertex(GLfloat * arr, int ptNum)
{
    VertexArray::setVertexElem(getVAOID(), arr, ptNum);
}

void ElementData::setColor(GLfloat * arr, int chns, int ptNum)
{
    VertexArray::setColorElem(getVAOID(), arr, ptNum, chns);
}

void ElementData::setNormal(GLfloat * arr, int ptNum)
{
    VertexArray::setNormalElem(getVAOID(), arr, ptNum);
}

void ElementData::setTexCoord(GLfloat * arr,int ptNum)
{
    VertexArray::setTexCoord2DElem(getVAOID(), arr, ptNum);
}

void ElementData::renderData(GLenum mode)
{
    glBindVertexArray(vao);

    glDrawElements(mode, vertexNum, GL_UNSIGNED_INT, 0);
}

BindBufferClass::BindBufferClass()
{
}

BindBufferClass::~BindBufferClass()
{
    glDeleteVertexArrays(1, &vao);
}