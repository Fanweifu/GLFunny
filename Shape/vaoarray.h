#ifndef VAOARRAY_H
#define VAOARRAY_H

#include<glHead.h>
#include<assert.h>


#define VBOCNT 4

#define VERTEX_VBO_INDEX 0
#define COLOR_VBO_INDEX 1
#define NORMAL_VBO_INDEX 2
#define TEXCOORD_VBO_INDEX 3

#define VERTEX_SIZE 3
#define COLOR_SIZE 4
#define NORMAL_SIZE 3
#define TEXCOORD_SIZE 2

class BindData {
public:
   
    virtual void renderData(GLenum mode) {}

    BindData();
    ~BindData();

protected:

    GLuint vao = 0;
    GLuint vbo[VBOCNT];
    GLuint ebo = 0;
    void init() {
        if (vao == 0) {
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &ebo);
            glGenBuffers(VBOCNT, vbo);
            glBindVertexArray(vao);
        }
    }
    void unInit() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(VBOCNT, &ebo);
        glDeleteBuffers(VBOCNT, vbo);
    }
};


class AttribArray :public BindData
{
public:

    AttribArray() :BindData() {}
    void init(int ptsNum);
    void setIndex1ui(GLuint *indeice, int idxNum);
    void setVertex3f(GLfloat* arr);
    void setColor4f(GLfloat* arr);
    void setNormal3f(GLfloat* arr);
    void setTexCoord2f(GLfloat* arr);
    void drawArray(GLenum mode);
protected:
    GLuint vertexNum = 0;
    GLuint indexNum = 0;
private:
};



#endif // VAOARRAY_H
