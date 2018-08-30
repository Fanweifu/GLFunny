#ifndef SHADER_H
#define SHADER_H

#include"glHead.h"
#include<map>

#define UNIFORM_VIEWPORT_VEC2 "viewport"
#define UNIFORM_TIME_FLOAT "time"
#define UNIFORM_PROJECTION_MAT4 "projection"
#define UNIFORM_PROJECTIONINV_MAT4 "projectionInv"
#define UNIFORM_CAMERAVIEW_MAT4 "cameraView"
#define UNIFORM_CAMERAVIEWINV_MAT4 "cameraViewInv"
#define UNIFORM_WORLDLIGHT_VEC4 "worldLight"
#define UNIFORM_CAMERAPOS_VEC3 "cameraPos"

using namespace std;
class Shader
{
public:
    Shader();
    ~Shader();

    bool available() { return isVaild; }

    bool loadFragCode(const char*source);
    bool loadFragFile(const char*filename);
    bool loadVertexCode(const char*source);
    bool loadVertexFile(const char*filename);

    bool link();
    void clear();
    virtual void bind();
    virtual void unBind();

    void setUniform1f(const string & pNm, float val);

    void setUniform1i(const string &pNm, int val);

    void setUniform1fv(const string & pNm, int size, const float * ptr);

    void setUniform2f(const string & pNm, float val0, float val1);

    void setUniform3f(const string & pNm, float val0, float val1, float val2);

    void setUniform4f(const string & pNm, float val0, float val1, float val2, float val3);

    void setUniformMat4(const string & pNm, const float* matPtr, bool transpose = false);

protected:
    bool isVaild = false;
    unsigned int program;
    map<const string, int> paramsMap;
    map<int, GLuint> shaders;

    int getParamID(const string& pNm);
    string readFile(const char*filename);
    bool complie_attch(GLenum type, const char * source);
    void initProgram();
    void uninitProgram();

private:
    bool inited = false;
};

#endif // SHADER_H
