#ifndef VAOARRAY_H
#define VAOARRAY_H

#include<gl/glew.h>
#include<assert.h>
#define VBOCNT 5

class BindBufferClass {
public:
    friend class VertexArray;
    unsigned int getVAOID() {
        if (vao == 0) {
            glewInit();
            glGenVertexArrays(1, &vao);
        }
        return vao;
    }
    virtual void renderData(GLenum mode) {}

    BindBufferClass();
    ~BindBufferClass();

protected:

    GLuint vao = 0;
};

class BufferData :public BindBufferClass
{
public:

    GLuint vertexNums = 0;
    void setVertex(GLfloat* arr);
    void setColor(GLfloat* arr, int vecNum = 3);
    void setNormal(GLfloat* arr);
    void setTexCoord(GLfloat* arr);
    void renderData(GLenum mode);

    BufferData() :BindBufferClass() {}
   
protected:

private:
};

class ElementData :public BindBufferClass
{
public:

    ElementData() :BindBufferClass() {}
    //~ElementData() { BindBufferClass::~BindBufferClass(); }
    void setIndex(GLuint *indeice, int idxNum);
    void setVertex(GLfloat* arr, int ptNum);
    void setColor(GLfloat* arr, int chns, int ptNum);
    void setNormal(GLfloat* arr, int ptNum);
    void setTexCoord(GLfloat* arr,int ptNum);
    void renderData(GLenum mode);
protected:
    GLuint vertexNum = 0;
private:
};

class VertexArray
{
public:
    friend class BufferData;
    friend class ElementData;

protected:
    static void  init();
    static void  setVertex(GLuint vaObj, GLfloat* arr, int vecNum,bool _static = true);
    static void  setColor(GLuint vaObj, GLfloat* arr, int vecNum, int chns = 3, bool _static = true);
    static void  setNormal(GLuint vaObj, GLfloat* arr, int vecNum, bool _static = true);
    static void  setTexCoord(GLuint vaObj, GLfloat* arr, int vecNum, bool _static = true);

    static void  setIndice(GLuint vaObj, GLuint *idxarr, int idxNum, bool _static = true);
    static void  setVertexElem(GLuint vaObj, GLfloat*valarr, int vecNum, bool _static = true);
    static void  setColorElem(GLuint vaObj, GLfloat*valarr, int vecNum, int chns, bool _static = true);
    static void  setNormalElem(GLuint vaObj, GLfloat*valarr, int vecNum, bool _static = true);
    static void  setTexCoord2DElem(GLuint vaObj, GLfloat*valarr, int vecNum, bool _static = true);

    

private:
    static GLenum getEnum(bool _static);
    static bool inited;
    static GLuint VBOS[];
};

#endif // VAOARRAY_H
