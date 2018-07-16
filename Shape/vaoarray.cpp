#include "vaoarray.h"
#include "stdio.h"

bool VAOFunctions::inited = false;
GLuint VAOFunctions::VBOS[VBOCNT];

void VAOFunctions::init() {
    if (!inited) {
        glewInit();
        glGenBuffers(VBOCNT, VBOS);
        inited = true;
    }
}

void VAOFunctions::setVertex(GLuint vaObj, GLfloat *arr, int num, bool _static) {
    if (!inited) init();

    glBindVertexArray(vaObj);

    glBindBuffer(GL_ARRAY_BUFFER, VAOFunctions::VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(float), arr, getEnum(_static));

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    
}

void VAOFunctions::setVertexShd(GLuint vaObj, GLfloat * valarr, int cnt, bool _static)
{
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, cnt * 3 * sizeof(float), valarr, getEnum(_static));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void VAOFunctions::setIndiceShd(GLuint vaObj, GLuint * idxarr, int idxNum,bool _static)
{
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOS[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxNum * sizeof(GLuint), idxarr , getEnum(_static));
}

void  VAOFunctions::setColor(GLuint vaObj, GLfloat *arr, int cnt, int chns,bool _static) {
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);

    glBufferData(GL_ARRAY_BUFFER, cnt * chns * sizeof(float), arr, getEnum(_static));

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(chns, GL_FLOAT, 0, NULL);
}

void VAOFunctions::setColorShd(GLuint vaObj, GLfloat * valarr, int vecNum, int chns, bool _static)
{
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
    glBufferData(GL_ARRAY_BUFFER, vecNum * chns * sizeof(float), valarr, getEnum(_static));

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
}

void VAOFunctions::setNormalShd(GLuint vaObj, GLfloat * valarr, int vecNum, bool _static)
{
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[2]);
    glBufferData(GL_ARRAY_BUFFER, vecNum * 3 * sizeof(float), valarr, getEnum(_static));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(2);
}

void VAOFunctions::setTexCoord2DShd(GLuint vaObj, GLfloat * valarr, int vecNum, bool _static)
{
    if (!inited) init();
    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[3]);
    glBufferData(GL_ARRAY_BUFFER, vecNum * 3 * sizeof(float), valarr, getEnum(_static));

    glVertexAttribPointer(3, 2 , GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(3);
}




void  VAOFunctions::setNormal(GLuint vaObj, GLfloat *arr, int num, bool _static) {
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[2]);

    glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(float), arr, getEnum(_static));
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, NULL);
}

void  VAOFunctions::setTexCoord(GLuint vaObj, GLfloat *arr, int num, bool _static) {
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[3]);

    glBufferData(GL_ARRAY_BUFFER, num * 2 * sizeof(float), arr, getEnum(_static));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, NULL);
}

void VAOFunctions::setIndice(GLuint vaObj, GLuint * idxarr, int idxNum, bool _static)
{
    if (!inited) init();

    glBindVertexArray(vaObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOS[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxNum * sizeof(GLuint), idxarr, getEnum(_static));

}

GLenum VAOFunctions::getEnum(bool _static)
{
    return _static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
}


void  ArrayData::setVertex(GLfloat *arr) {
    VAOFunctions::setVertex(getVAOID(), arr, vertexNums);
}
void  ArrayData::setColor(GLfloat *arr, int vecNum) {
    VAOFunctions::setColor(getVAOID(), arr, vertexNums, vecNum);
}
void  ArrayData::setNormal(GLfloat *arr) {
    VAOFunctions::setNormal(getVAOID(), arr, vertexNums);
}
void  ArrayData::setTexCoord(GLfloat *arr) {
    VAOFunctions::setTexCoord(getVAOID(), arr, vertexNums);
}

void ArrayData::setIndex(GLuint * arr, int idxnum)
{
    indexNums = idxnum;
    VAOFunctions::setIndice(getVAOID() , arr, idxnum);
}

void ArrayData::renderData(GLenum mode)
{
    glBindVertexArray(vao);
    glDrawArrays(mode, 0, vertexNums);
}

void ArrayData::renderIndex(GLenum mode)
{
    glBindVertexArray(vao);
    glDrawElements(mode, indexNums, GL_UNSIGNED_INT, 0);
}

void ElementData::setIndex(GLuint * indeice, int idxNum)
{
    indexNum = idxNum;
    VAOFunctions::setIndice(getVAOID(), indeice, idxNum);
}

void ElementData::setVertex(GLfloat * arr, int ptNum)
{
    VAOFunctions::setVertexShd(getVAOID(), arr, ptNum);
}

void ElementData::setColor(GLfloat * arr, int chns, int ptNum)
{
    VAOFunctions::setColorShd(getVAOID(), arr, ptNum, chns);
}

void ElementData::setNormal(GLfloat * arr, int ptNum)
{
    VAOFunctions::setNormalShd(getVAOID(), arr, ptNum);
}

void ElementData::setTexCoord(GLfloat * arr,int ptNum)
{
    VAOFunctions::setTexCoord2DShd(getVAOID(), arr, ptNum);
}

void ElementData::renderData(GLenum mode)
{
    glBindVertexArray(vao);

    glDrawElements(mode, indexNum, GL_UNSIGNED_INT, 0);
}

BindData::BindData()
{
}

BindData::~BindData()
{
    glDeleteVertexArrays(1, &vao);
}