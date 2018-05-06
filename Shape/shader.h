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

    bool loadFragCode(const char*source);
    bool loadFragFile(const char*filename);
    bool loadVertexCode(const char*source);
    bool loadVertexFile(const char*filename);

    bool link();
    void active();

    void setUniform1f(string & pNm, float val);

    void setUniform1fv(string & pNm, int size, float * ptr);

    void setUniform2f(string & pNm, float val0, float val1);

    void setUniform3f(string & pNm, float val0, float val1, float val2);

    void setUniform4f(string & pNm, float val0, float val1, float val2, float val3);

    void setUniformMat4(string & pNm,float* matPtr);

    unsigned int getProgramID();

protected:

    string readFile(const char*filename);
    int getParamID(string& pNm);

    bool createProgram = false;
    unsigned int program;

    map<string,int> paramsMap;

private:

};

#endif // SHADER_H
