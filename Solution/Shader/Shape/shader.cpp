#include "shader.h"

#include<sstream>
#include<iostream>
#include<fstream>
#include<stdio.h>

Shader::Shader()
{
}

Shader::~Shader() {
    unInit();
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
		
        GLint infoLen = 0; 
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        char* buf = (char*)malloc(infoLen+1);
        glGetShaderInfoLog(shader, infoLen, NULL, buf);
        printf("compile failed!\n");
        printf(buf);
        free(buf);
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
        if (!isInited) init();
        glAttachShader(m_handle, shader);
        glDeleteShader(shader);
        return true;
    }
}

void Shader::init()
{
    if (!isInited&&m_handle==0) {
        m_handle = glCreateProgram();
        isInited = true;
    }
}

void Shader::unInit()
{
    if (isInited) {
        glDeleteProgram(m_handle);
        m_handle = 0;
        isInited = isBinded = isVaild = false;
    }
}

GLint Shader::getUniformLoc(const char * name)
{
	GLint idx = glGetUniformLocation(m_handle, name);
	return idx;
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


bool Shader::link() {
    glLinkProgram(m_handle);

    GLint success;
    
    glGetProgramiv(m_handle, GL_LINK_STATUS, &success);

    if (!success) {
        GLint infoLen = 0;
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &infoLen);

        char* buf = (char*)malloc(infoLen);
        glGetProgramInfoLog(m_handle, infoLen, NULL, buf);
        printf("link failed\n%s",buf);
        free(buf);

        return (isVaild = false);
    }
    else
    {
        return (isVaild = true);
    }
}

void Shader::clear()
{
    unInit();
}

void Shader::bind() {
    if (isVaild) glUseProgram(m_handle);
}

void Shader::unBind()
{
    if (isVaild) glUseProgram(0);
}

void Shader::setUniform1f(const char * pNm, float val) {
	GLint idx = glGetUniformLocation(m_handle, pNm);
    if (idx >= 0) glUniform1f(idx, val);
}

void Shader::setUniform1i(const char * pNm, int val)
{
	GLint idx = glGetUniformLocation(m_handle, pNm);
    if (idx >= 0)glUniform1i(idx, val);
}


void Shader::setUniform1fv(const char * pNm, int size, const float * ptr)
{
	GLint idx = glGetUniformLocation(m_handle, pNm);
    if (idx >= 0)glUniform1fv(idx, size, ptr);
}

void Shader::setUniform2f(const char * pNm, float val0, float val1)
{
	GLint idx = glGetUniformLocation(m_handle, pNm);
    if (idx >= 0)glUniform2f(idx, val0, val1);
}

void Shader::setUniform3f(const char * pNm, float val0, float val1, float val2)
{
	GLint idx = glGetUniformLocation(m_handle, pNm);
    if (idx >= 0)glUniform3f(idx, val0, val1, val2);
}

void Shader::setUniform4f(const char * pNm, float val0, float val1, float val2, float val3)
{
    GLint idx = glGetUniformLocation(m_handle, pNm);
    if (idx >= 0)glUniform4f(idx, val0, val1, val2, val3);
}

void Shader::setUniformMat4(const char * pNm, const float*matPtr, bool transpose)
{
	GLint idx = glGetUniformLocation(m_handle, pNm);
    if (idx >= 0)glUniformMatrix4fv(idx, 1, transpose, matPtr);
}
