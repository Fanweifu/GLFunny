#ifndef VAOARRAY_H
#define VAOARRAY_H


#include<assert.h>
#include<map>
#include<vector>
#include"gl_def.h"
#include"object.h"

#define VERTEX_VBO_LAYOUT 0
#define COLOR_VBO_LAYOUT 1
#define NORMAL_VBO_LAYOUT 2
#define TEXCOORD_VBO_LAYOUT 3
#define TAGENT_VBO_LAYOUT 4
#define BITTAGENT_VBO_LAYOUT 5

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
    
    void init(int cnt);

    virtual void renderData(GLenum mode) = 0;

protected:
    int vbocnt = 0;
    GLuint * m_vbosID;
    void initBuffs(int cnt);

};

class AttribArray :public BindData
{
public:

    AttribArray() : BindData() {}
    void init(int ptsNum,int buffs = 4 );
    void setVertex3f(GLfloat* arr);
    void setColor4f(GLfloat* arr);
    void setNormal3f(GLfloat* arr);
    void setTexCoord2f(GLfloat* arr);
    void setOtherData3f(GLfloat* arr, int layout);
    void renderData(GLenum mode) override;
protected:
    GLuint vertexNum = 0;
private:
};





#endif // VAOARRAY_H
