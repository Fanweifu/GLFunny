#ifndef SHADER_H
#define SHADER_H

#include<sstream>
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<map>
#include<GL/glew.h>

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
    void use();
    void unuse();

    void setUniform1f(string & pNm, float val);

    void setUniform1f(GLuint layout, float val);

    void setUniform1fv(string & pNm, int size, float * ptr);

    void setUniform1fv(GLuint layout, int size, float * ptr);

    void setUniform2f(string & pNm, float val0, float val1);

    void setUniform2f(GLuint layout, int size, float * ptr);

    void setUniform3f(string & pNm, float val0, float val1, float val2);

    void setUniform3f(GLuint layout, int size, float * ptr);

    void setUniform4f(string & pNm, float val0, float val1, float val2, float val3);

    void setUniform4f(GLuint layout, int size, float * ptr);

    void setUniformMat4(string & pNm, const float* matPtr);

    void setUniformMat4(GLuint layout, int size, const float * ptr);

    static Shader* createDefaultShader();

protected:
    bool isVaild = false;
    unsigned int program;
    map<string, int> paramsMap;

    int getParamID(string& pNm);
    string readFile(const char*filename);
    bool complie_attch(GLenum type, const char * source);
    void initProgram();
    void uninitProgram();

private:
    bool inited = false;
};

#endif // SHADER_H
