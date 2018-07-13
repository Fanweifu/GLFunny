#ifndef VAOARRAY_H
#define VAOARRAY_H

#include<glHead.h>
#include<assert.h>


#define VBOCNT 5
#define POSVBOI 0
#define COLVBOI 1
#define NRMVBOI 2
#define TXCVBOI 3
#define IDXVBOI 4

class BindData {
public:
    friend class VAOFunctions;
    unsigned int getVAOID() {
        if (vao == 0) {
            glewInit();
            glGenVertexArrays(1, &vao);
        }
        return vao;
    }
    virtual void renderData(GLenum mode) {}

    BindData();
    ~BindData();

protected:

    GLuint vao = 0;
};

class ArrayData :public BindData
{
public:

    GLuint vertexNums = 0;
    void setVertex(GLfloat* arr);
    void setColor(GLfloat* arr, int cnt = 3);
    void setNormal(GLfloat* arr);
    void setTexCoord(GLfloat* arr);
    void renderData(GLenum mode);

    ArrayData() :BindData() {}
   
protected:

private:
};

class ElementData :public BindData
{
public:

    ElementData() :BindData() {}
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

class VAOFunctions
{
public:
    friend class ArrayData;
    friend class ElementData;

protected:
    static void  init();
    static void  setVertex(GLuint vaObj, GLfloat* arr, int cnt,bool _static = true);
    static void  setColor(GLuint vaObj, GLfloat* arr, int cnt, int chns = 3, bool _static = true);
    static void  setNormal(GLuint vaObj, GLfloat* arr, int cnt, bool _static = true);
    static void  setTexCoord(GLuint vaObj, GLfloat* arr, int cnt, bool _static = true);

    static void  setIndice(GLuint vaObj, GLuint *idxarr, int idxNum, bool _static = true);
    static void  setVertexElem(GLuint vaObj, GLfloat*valarr, int cnt, bool _static = true);
    static void  setColorElem(GLuint vaObj, GLfloat*valarr, int cnt, int chns, bool _static = true);
    static void  setNormalElem(GLuint vaObj, GLfloat*valarr, int cnt, bool _static = true);
    static void  setTexCoord2DElem(GLuint vaObj, GLfloat*valarr, int cnt, bool _static = true);

    

private:
    static GLenum getEnum(bool _static);
    static bool inited;
    static GLuint VBOS[];
};

#endif // VAOARRAY_H
