#include "shader.h"

#include<sstream>
#include<iostream>
#include<fstream>
#include<stdio.h>

Shader::Shader()
{
}

Shader::~Shader() {
    uninitProgram();
}

string Shader::readFile(const char *filename) {
    ifstream in(filename, ios::in);
    std::stringstream  sb;
    sb << in.rdbuf();
    return string(sb.str());
}

GLuint createShader(GLenum type, const char*source, GLint& success) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char info[1000];
        glGetShaderInfoLog(shader, 512, NULL, info);
        sprintf(info, "Compile Failed!\n%s", info);
        printf(info);
    }
    else
    {
        printf("compile success!\n");
    }

    return shader;
}

bool Shader::complie_attch(GLenum type, const char *source) {
    GLint succsee = 0;
    GLuint shader = createShader(type, source, succsee);
    if (succsee == 0) {
        glDeleteShader(shader);
        return false;
    }
    else {
        if (!inited) initProgram();
        glAttachShader(program, shader);
        //shaders.insert(pair<int, GLuint>(type, shader));
        glDeleteShader(shader);
        return true;
    }
}

void Shader::initProgram()
{
    if (!inited) {
        program = glCreateProgram();
        inited = true;
    }
}

void Shader::uninitProgram()
{
    if (inited) {
        glDeleteProgram(program);

        for (auto item : shaders) {
            glDeleteShader(item.second);
        }

        shaders.clear();

        inited = false;
    }
}

bool Shader::loadFragCode(const char *source) {
    return complie_attch(GL_FRAGMENT_SHADER, source);
}

bool Shader::loadFragFile(const char *filename) {
    string code = readFile(filename);
    return loadFragCode(code.c_str());
}

bool Shader::loadVertexCode(const char *source) {
    return complie_attch(GL_VERTEX_SHADER, source);
}

bool Shader::loadVertexFile(const char *filename) {
    string code = readFile(filename);
    return loadVertexCode(code.c_str());
}

GLint Shader::getParamID(const string &pNm) {
    map<string, GLint>::iterator it = paramsMap.find(pNm);
    if (it != paramsMap.end()) {
        return (*it).second;
    }
    else {
        GLint idx = glGetUniformLocation(program, pNm.c_str());
        if (idx == -1) {
            cout << pNm << " can not find params!\n";
        }
        paramsMap.insert(pair<string, GLint>(pNm, idx));
        return idx;
    }
}

bool Shader::link() {
    glLinkProgram(program);

    GLint success;
    char info[200];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, info);
        cout << "Link Failed\n" << info;
        return (isVaild = false);
    }
    else
    {
        cout << "Link Success\n";
        return (isVaild = true);
    }
}

void Shader::bind() {
    if(isVaild) glUseProgram(program);
}

void Shader::unBind()
{
    if(isVaild) glUseProgram(0);
}

void Shader::setUniform1f(const string & pNm, float val) {
    GLint idx = getParamID(pNm);
    if (idx >= 0) glUniform1f(idx, val);
}

void Shader::setUniform1i(const string & pNm, int val)
{
    GLint idx = getParamID(pNm);
    if (idx >= 0)glUniform1i(idx, val);
}


void Shader::setUniform1fv(const string & pNm, int size, const float * ptr)
{
    GLint idx = getParamID(pNm);
    if (idx >= 0)glUniform1fv(idx, size, ptr);
}

void Shader::setUniform2f(const string & pNm, float val0, float val1)
{
    GLint idx = getParamID(pNm);
    if (idx >= 0)glUniform2f(idx, val0, val1);
}

void Shader::setUniform3f(const string & pNm, float val0, float val1, float val2)
{
    GLint idx = getParamID(pNm);
    if (idx >= 0)glUniform3f(idx, val0, val1, val2);
}

void Shader::setUniform4f(const string & pNm, float val0, float val1, float val2, float val3)
{
    GLint idx = getParamID(pNm);
    if (idx >= 0)glUniform4f(idx, val0, val1, val2, val3);
}

void Shader::setUniformMat4(const string & pNm, const float*matPtr ,bool transpose)
{
    GLint idx = getParamID(pNm);
    if (idx >= 0)glUniformMatrix4fv(idx, 1, transpose , matPtr);
}