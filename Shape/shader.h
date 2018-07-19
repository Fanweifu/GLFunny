#ifndef SHADER_H
#define SHADER_H

#include"glHead.h"
#include<sstream>
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<map>

using namespace std;
class Shader
{
public:
    Shader();
    ~Shader();

    const static string pView;
    const static string pTime;
    const static string pProjectionInv;
    const static string pCameraViewInv;
    const static string pWorldLight;
    const static string pCameraPos;

    bool available() { return isVaild; }

    bool loadFragCode(const char*source);
    bool loadFragFile(const char*filename);
    bool loadVertexCode(const char*source);
    bool loadVertexFile(const char*filename);

    bool link();
    virtual void use();
    virtual void unuse();



    void setUniform1f(const string & pNm, float val);

    void setUniform1i(const string &pNm, int val);


    void setUniform1fv(const string & pNm, int size, const float * ptr);

    void setUniform2f(const string & pNm, float val0, float val1);

    void setUniform3f(const string & pNm, float val0, float val1, float val2);

    void setUniform4f(const string & pNm, float val0, float val1, float val2, float val3);

    void setUniformMat4(const string & pNm, const float* matPtr, bool transpose = false);


    static Shader* createDefaultShader();

protected:
    bool isVaild = false;
    unsigned int program;
    map<const string, int> paramsMap;

    int getParamID(const string& pNm);
    string readFile(const char*filename);
    bool complie_attch(GLenum type, const char * source);
    void initProgram();
    void uninitProgram();

private:
    bool inited = false;
};

   

#endif // SHADER_H
