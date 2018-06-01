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

void VertexArray::setVertex(GLuint vaoId, GLfloat *arr, int num, bool isStatic) {
    if (!inited) init();

    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, VertexArray::VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(float), arr, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
}

void VertexArray::setVertexElem(GLuint vaoId, GLfloat * valarr, int cnt, bool isStatic)
{
    if (!inited) init();

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, cnt * 3 * sizeof(float), valarr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void VertexArray::setIndice(GLuint vaoIdx, GLuint * idxarr, int idxNum,bool isStatic)
{
    if (!inited) init();

    glBindVertexArray(vaoIdx);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOS[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxNum * sizeof(GLuint), idxarr , GL_STATIC_DRAW);
}

void  VertexArray::setColor(GLuint vaoId, GLfloat *arr, int cnt, int vecSize,bool isStatic) {
    if (!inited) init();

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);

    glBufferData(GL_ARRAY_BUFFER, cnt * vecSize * sizeof(float), arr, GL_STATIC_DRAW);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(vecSize, GL_FLOAT, 0, NULL);
}

void VertexArray::setColorElem(GLuint vaoId, GLfloat * valarr, int vecNum, int vecSize, bool isStatic)
{
    if (!inited) init();

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
    glBufferData(GL_ARRAY_BUFFER, vecNum * vecSize * sizeof(float), valarr, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
}

void VertexArray::setNormalElem(GLuint vaoId, GLfloat * valarr, int vecNum, bool isStatic)
{
    if (!inited) init();

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[2]);
    glBufferData(GL_ARRAY_BUFFER, vecNum * 3 * sizeof(float), valarr, GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(2);
}

void VertexArray::setTexCoordElem(GLuint vaoIdx, GLfloat * valarr, int vecNum, bool isStatic)
{

}

void  VertexArray::setNormal(GLuint vaoId, GLfloat *arr, int num, bool isStatic) {
    if (!inited) init();

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[2]);

    glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(float), arr, GL_STATIC_DRAW);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, NULL);
}

void  VertexArray::setTexCoord(GLuint vaoId, GLfloat *arr, int num, bool isStatic) {
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

void ElementData::setIndex(GLuint * indeice, int idxNum)
{
    vertexNum = idxNum;
    VertexArray::setIndice(getVAOID(), indeice, idxNum);
}

void ElementData::setVertex(GLfloat * arr, int ptNum)
{
    VertexArray::setVertexElem(getVAOID(), arr, ptNum);
}

void ElementData::setColor(GLfloat * arr, int vecSize, int ptNum)
{
    VertexArray::setColorElem(getVAOID(), arr, ptNum, vecSize);
}

void ElementData::setNormal(GLfloat * arr, int ptNum)
{
    VertexArray::setNormalElem(vao, arr, ptNum);
}

void ElementData::setTexCoord(GLfloat * arr)
{
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