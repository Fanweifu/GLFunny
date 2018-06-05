#include "shader.h"
#include<GL/glew.h>

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
        char info[500];
        glGetShaderInfoLog(shader, 512, NULL, info);
        cout << "compile failed\n" << info;
    }
    else
    {
        cout << "compile success!\n";
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

GLint Shader::getParamID(string &pNm) {
    map<string, GLint>::iterator it = paramsMap.find(pNm);
    if (it != paramsMap.end()) {
        return (*it).second;
    }
    else {
        GLint idx = glGetUniformLocation(program, pNm.data());
        if (idx == -1) {
            cout << pNm << " can not find params!\n";
            return -1;
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
        return (isVaild=true);
    }
}

void Shader::use() {
    glUseProgram(program);
}

void Shader::unuse()
{
    glUseProgram(0);

}

void Shader::setUniform1f(string&pNm, float val) {
    GLint idx = getParamID(pNm);
    glUniform1f(idx, val);
}

void Shader::setUniform1i(string & pNm, int val)
{
    GLint idx = getParamID(pNm);
    glUniform1i(idx, val);
}




void Shader::setUniform1fv(string & pNm, int size, float * ptr)
{
    GLint idx = getParamID(pNm);
    glUniform1fv(idx, size, ptr);
}



void Shader::setUniform2f(string & pNm, float val0, float val1)
{
    GLint idx = getParamID(pNm);
    glUniform2f(idx, val0, val1);
}



void Shader::setUniform3f(string & pNm, float val0, float val1, float val2)
{
    GLint idx = getParamID(pNm);
    glUniform3f(idx, val0, val1, val2);
}



void Shader::setUniform4f(string & pNm, float val0, float val1, float val2, float val3)
{
    GLint idx = getParamID(pNm);
    glUniform4f(idx, val0, val1, val2, val3);
}



void Shader::setUniformMat4(string & pNm, const float*matPtr)
{
    GLint idx = getParamID(pNm);
    glUniformMatrix4fv(idx, 1, true , matPtr);
}



Shader * Shader::createDefaultShader()
{
    Shader* shd = new Shader();
    shd->loadVertexCode("#version 330 compatibility\n"
        "layout(location = 0) in vec3 pos;\n"
        "layout(location = 1) in vec4 clr;\n"
        "void main() {\n"
        "gl_Position= gl_ModelViewProjectionMatrix*vec4(pos,1.);\n"
        "gl_FrontColor = vec4(clr);\n"
        "}\n"
        );

    return shd;
}
