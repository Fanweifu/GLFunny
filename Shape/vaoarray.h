#ifndef VAOARRAY_H
#define VAOARRAY_H


#include<gl/glew.h>
#include<assert.h>
#define VAONUM 10000
#define VBONUM 4

class VAOArray
{
public :
    friend class VAOArrayMan;
    unsigned int getVAOID() { return vaoID ;}

    void renderData(GLenum mode = GL_TRIANGLES);

    void setVertex(GLfloat* arr);
    void setColor(GLfloat* arr,int vecNum = 3);
    void setNormal(GLfloat* arr);
    void setTexCoord(GLfloat* arr);

protected:
    VAOArray(){}
    ~VAOArray(){}
    GLuint vaoID = 0;


private:

    bool hasVertex = false;
    bool hasColor = false;
    bool hasNormal = false;
    bool hasTextrue = false;




};

class VAOArrayMan
{
public:
    friend class VAOArray;
    static void init();
    static VAOArray* createVAO();
    static GLuint capacity(){ return capNum;}
    static GLuint vaoCount(){ return curNum;}

protected:

    static void  setVertex(GLuint vaoId,GLfloat* arr);
    static void  setColor(GLuint vaoId,GLfloat* arr,int vecNum = 3);
    static void  setNormal(GLuint vaoId,GLfloat* arr);
    static void  setTexCoord(GLuint vaoId,GLfloat* arr);


private:
    static bool inited;
    static GLuint capNum;
    static GLuint curNum;
    static GLuint VAOS[];
    static GLuint VBOS[];

};



#endif // VAOARRAY_H
