
#include<gl/glew.h>
#include<gl/GL.h>

#include"TextReader.h"

#include<gl/glut.h>
#include<string>
#include<functional>
#include"../Shape/camera.h"

using namespace std;

GLuint shaderProgram;
GLuint iResolutionIdx;
GLuint iTimeIdx;
GLuint iMouseIdx;

GLuint VBO[2];
GLuint VAO[2];

//三角形顶点数据
GLfloat triangleVertexs[][3] = {{ -0.8,-0.8,0 },{ -0.2,-0.2,0 },{ 0.5,-0.8,0 }};
GLfloat triangleColors[][3] = {{ 1.0,0,0 },{ 0,1.0,0 },{ 0,0,1.0 }};

//四角形顶点数据
GLfloat rectVertexs[][3] = {{ -1,-1,0 },{ 1,-1,0 },{ 1,1,0 },{ -1,1,0 }};

GLfloat rectColors[][3] = {{ 1,0,0 },{ 0,1,0 },{ 0,0,1 },{ 1,1,0 }};

Camera cam;
Shape obj;
float timeVal = 0;
void setShapes() {
    glewInit();

    glGenVertexArrays(2, VAO);

    glGenBuffers(2, VBO);
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertexs), triangleVertexs, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColors), triangleColors, GL_STATIC_DRAW);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, NULL);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertexs), rectVertexs, GL_STATIC_DRAW);
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectColors), rectColors, GL_STATIC_DRAW);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, NULL);
}


void setShaders(char * path)
{
    string souce = reader::readFile(path);
    const char* fragSouce = souce.data();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSouce , NULL);
    glCompileShader(fragmentShader);

    GLint success;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    char *info = NULL;

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, info);
        cout << "ERROR::SHADER::FRAGMENT 2::COMPILATION_FAILED\n" << info;
    }
    else
    {
        cout << "Fragment Shader 2 compile success!";
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, info);
        cout << "ERROR::SHADER::PROGRAM 2::LINKING_FAILED\n" << info;
    }
    else
    {
        cout << "Link Program2 success!";
    }
    glUseProgram(shaderProgram);

    iResolutionIdx = glGetUniformLocation(shaderProgram, "iResolution");
    iTimeIdx = glGetUniformLocation(shaderProgram, "iTime");
    iMouseIdx = glGetUniformLocation(shaderProgram, "iMouse");

}


void reshape(int width, int height) {
    cam.setViewPort(0, 0, width , height);
    glUniform3f(iResolutionIdx, width, height, 0);
}

void renderScene(void)
{
    
    timeVal += 0.01;

    glUniform1f(iTimeIdx, timeVal);

    //glBindVertexArray(VAO[0]);//重新激活顶点数组  
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_QUADS, 0, 4);
    glutSwapBuffers();
}

void render() {
    cam.drawView();
}


void initCamera() {
    cam.init();
    cam.setPosition(0, 0.001, 1);
    cam.setViewPort(0, 0, 500, 500);
    cam.scene = &obj;
    obj.testDrawFunc = renderScene;
}


int main(int arg,char**argv) {
    glutInit(&arg, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);

    glutCreateWindow("GLSL的第一步");

   

    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);

    initCamera();
    setShapes();
    setShaders("test1.txt");
    glutMainLoop();
    return 0;
}

