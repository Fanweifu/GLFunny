#include "vaoarray.h"
#include "stdio.h"

bool VertexArray::inited = false;
GLuint VertexArray::VBOS[VBONUM];

void VertexArray::init() {
    if (!inited) {
        glewInit();
        glGenBuffers(VBONUM, VBOS);
        inited = true;
    }
}

void VertexArray::setVertex(GLuint vaoId, GLfloat *arr, int num) {
    if (!inited) init();

    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, VertexArray::VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(float), arr, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
}

void VertexArray::setVertexArrayIdx(GLuint vaoId, GLfloat * valarr, int cnt, GLuint * indices, int idxNum)
{
    if (!inited) init();

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, cnt * 3 * sizeof(float), valarr, GL_STATIC_DRAW);
    /*glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(3 , GL_FLOAT, 0, NULL);*/

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOS[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxNum * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void  VertexArray::setColor(GLuint vaoId, GLfloat *arr, int cnt, int vecSize) {
    if (!inited) init();

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);

    glBufferData(GL_ARRAY_BUFFER, cnt * vecSize * sizeof(float), arr, GL_STATIC_DRAW);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(vecSize, GL_FLOAT, 0, NULL);
}

void VertexArray::setColorArray(GLuint vaoId, GLfloat * valarr, int vecNum, int vecSize)
{
    if (!inited) init();

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
    glBufferData(GL_ARRAY_BUFFER, vecNum * vecSize * sizeof(float), valarr, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
}

void VertexArray::setNormalArray(GLuint vaoId, GLfloat * valarr, int vecNum)
{
    if (!inited) init();

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[2]);
    glBufferData(GL_ARRAY_BUFFER, vecNum * 3 * sizeof(float), valarr, GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(2);
}

void  VertexArray::setNormal(GLuint vaoId, GLfloat *arr, int num) {
    if (!inited) init();

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[2]);

    glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(float), arr, GL_STATIC_DRAW);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, NULL);
}

void  VertexArray::setTexCoord(GLuint vaoId, GLfloat *arr, int num) {
    if (!inited) init();

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[3]);

    glBufferData(GL_ARRAY_BUFFER, num * 2 * sizeof(float), arr, GL_DYNAMIC_DRAW);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, NULL);
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

void ElementData::setVertex(GLfloat * arr, int ptNum, GLuint * indeice, int idxCnt)
{
    vertexNum = idxCnt;
    VertexArray::setVertexArrayIdx(getVAOID(), arr, ptNum, indeice, idxCnt);
}

void ElementData::setColor(GLfloat * arr, int vecSize, int ptNum)
{
    VertexArray::setColorArray(getVAOID(), arr, ptNum, vecSize);
}

void ElementData::setNormal(GLfloat * arr, int ptNum)
{
    VertexArray::setNormalArray(vao, arr, ptNum);
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