#ifndef VAOARRAY_H
#define VAOARRAY_H

#include<glHead.h>
#include<assert.h>
#include<map>
#include"object.h"
#define VBOCNT 4

#define VERTEX_VBO_LAYOUT 0
#define COLOR_VBO_LAYOUT 1
#define NORMAL_VBO_LAYOUT 2
#define TEXCOORD_VBO_LAYOUT 3

#define VERTEX_SIZE 3
#define COLOR_SIZE 4
#define NORMAL_SIZE 3
#define TEXCOORD_SIZE 2

class BindData : public glObject {
public:
    BindData();
    ~BindData();

    void init() override;
    void unInit() override;
    void bind() override;
    void unBind() override;
    virtual void renderData(GLenum mode) = 0;

protected:

    GLuint m_vaoID = 0;
    GLuint m_vbosID[VBOCNT];
    
};

class AttribArray :public BindData
{
public:

    AttribArray() : BindData() {}
    void init(int ptsNum);
    void setVertex3f(GLfloat* arr);
    void setColor4f(GLfloat* arr);
    void setNormal3f(GLfloat* arr);
    void setTexCoord2f(GLfloat* arr);
    void renderData(GLenum mode) override;
protected:
    GLuint vertexNum = 0;
private:
};



#endif // VAOARRAY_H
