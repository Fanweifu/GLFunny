#include "vaoarray.h"
#include "stdio.h"

GLenum getEnum(bool _static)
{
    return _static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
}

void AttribArray::init(int ptsNum)
{
    vertexNum = ptsNum;
    BindData::init();
}

void AttribArray::setVertex3f(GLfloat * arr)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbosID[VERTEX_VBO_LAYOUT]);
    glBufferData(GL_ARRAY_BUFFER, vertexNum* VERTEX_SIZE * sizeof(float), arr, getEnum(true));
    glVertexAttribPointer(VERTEX_VBO_LAYOUT, VERTEX_SIZE, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(VERTEX_VBO_LAYOUT);
}

void AttribArray::setColor4f(GLfloat * arr)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbosID[COLOR_VBO_LAYOUT]);
    glBufferData(GL_ARRAY_BUFFER, vertexNum * COLOR_SIZE * sizeof(float), arr, getEnum(true));
    glVertexAttribPointer(COLOR_VBO_LAYOUT, COLOR_SIZE, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(COLOR_VBO_LAYOUT);
}

void AttribArray::setNormal3f(GLfloat * arr)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbosID[NORMAL_VBO_LAYOUT]);
    glBufferData(GL_ARRAY_BUFFER, vertexNum * NORMAL_SIZE * sizeof(float), arr, getEnum(true));
    glVertexAttribPointer(NORMAL_VBO_LAYOUT, NORMAL_SIZE, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(NORMAL_VBO_LAYOUT);
}

void AttribArray::setTexCoord2f(GLfloat * arr)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbosID[TEXCOORD_VBO_LAYOUT]);
    glBufferData(GL_ARRAY_BUFFER, vertexNum * TEXCOORD_SIZE * sizeof(float), arr, getEnum(true));
    glVertexAttribPointer(TEXCOORD_VBO_LAYOUT, TEXCOORD_SIZE, GL_FLOAT, GL_TRUE, 0, (void*)0);
    glEnableVertexAttribArray(TEXCOORD_VBO_LAYOUT);
}

void AttribArray::renderData(GLenum mode)
{
    glBindVertexArray(m_vaoID);
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

void BindData::init()
{
    if (isInited) return;
    glGenVertexArrays(1, &m_vaoID);
    glGenBuffers(VBOCNT, m_vbosID);
    isInited = true;
}

void BindData::unInit() {
    if (!isInited) return;
    glDeleteVertexArrays(1, &m_vaoID);
    glDeleteBuffers(VBOCNT, m_vbosID);
    isInited = isVaild = isBinded = false;
}

void BindData::bind()
{
    glBindVertexArray(m_vaoID);
    isBinded = true;
}

void BindData::unBind()
{
    glBindVertexArray(0);
    isBinded = false;
}

