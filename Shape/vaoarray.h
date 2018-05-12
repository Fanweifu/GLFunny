#ifndef VAOARRAY_H
#define VAOARRAY_H

#include<gl/glew.h>
#include<assert.h>
#define VAONUM 10000
#define VBONUM 5

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
    virtual void renderData(GLenum mode){}

    BindBufferClass();
    ~BindBufferClass();

protected:
    
    GLuint vao = 0;
};


class BufferData:public BindBufferClass
{
public:
    

    GLuint vertexNums = 0;
    void setVertex(GLfloat* arr);
    void setColor(GLfloat* arr, int vecNum = 3);
    void setNormal(GLfloat* arr);
    void setTexCoord(GLfloat* arr);
    void renderData(GLenum mode);

    BufferData():BindBufferClass() {}
    ~BufferData() { BindBufferClass::~BindBufferClass(); }
protected:
    
    

private:

};

class ElementData :public BindBufferClass
{
public:
    
    ElementData():BindBufferClass() {}
    ~ElementData() { BindBufferClass::~BindBufferClass(); }

    void setVertex(GLfloat* arr,int ptNum,GLuint*indeice,int idxCnt);
    void setColor(GLfloat* arr,int vecSize, int ptNum);
    void setNormal(GLfloat* arr,int ptNum);
    void setTexCoord(GLfloat* arr);
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
    
    
    static GLuint vaoCount() { return curNum; }

protected:
    static void  init();
    static void  setVertex(GLuint vaoIdx, GLfloat* arr, int vecNum);
    static void  setColor(GLuint vaoIdx, GLfloat* arr, int vecNum, int vecSize = 3);
    static void  setNormal(GLuint vaoIdx, GLfloat* arr, int vecNum);
    static void  setTexCoord(GLuint vaoIdx, GLfloat* arr, int vecNum);
    static void  setVertexArrayIdx(GLuint vaoIdx, GLfloat*valarr, int vecNum , GLuint*idxarr,int idxNum);
    static void  setColorArray(GLuint vaoIdx, GLfloat*valarr, int vecNum, int vecSize);
    static void  setNormalArray(GLuint vaoIdx, GLfloat*valarr, int vecNum);
    static void  setTexCoordDataIdx(GLuint vaoIdx, GLfloat*valarr, int vecNum, GLuint*idxarr, int idxNum);
private:
    static bool inited;
    static GLuint curNum;
    static GLuint VBOS[];
};

#endif // VAOARRAY_H
