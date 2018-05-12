#include<gl/glew.h>
#include<gl/GL.h>

#include<gl/glut.h>
#include<string>
#include<functional>
#include"Shape/camera.h"
#include"Shape\shader.h"
#include"Shape\image3dex.h"
using namespace std;

string iResolution = "iResolution";
string iTime = "iTime";
string iMouse = "iMouse";
string cameraRotationMat = "cameraRotationMat";
string cameraPosition = "cameraPosition";

GLuint VBO[2];
GLuint VAO[2];

//三角形顶点数据
GLfloat triangleVertexs[][3] = { { -0.8,-0.8,0 },{ -0.2,-0.2,0 },{ 0.5,-0.8,0 } };
GLfloat triangleColors[][3] = { { 1.0,0,0 },{ 0,1.0,0 },{ 0,0,1.0 } };
GLuint idx[] = { 0,1,2,3 };
//四角形顶点数据
GLfloat rectVertexs[] = { -1,-1,0 , 1,-1,0 ,1,1,0 , -1,1,0 };

GLfloat rectColors[] = { 1,0,0,1 , 0,1,0,1 , 0,0,1,1, 1,1,0,1 };

Camera cam;
Shape obj;
Shader shader;
Shader* dshader = NULL;
Image3DEx img3d;
BufferData bdata;
ElementData edata;
float timeVal = 0;

void setShapes() {
    edata.setVertex(rectVertexs, 4, idx , 4);
    edata.setColor(rectColors, 4 , 4);
    dshader = Shader::createDefaultShader();
    dshader->use();
}

void setShaders(char * path)
{
    shader.loadFragFile(path);
    shader.link();
    shader.use();
}

void reshape(int width, int height) {
    cam.setViewPort(0, 0, width, height);

    //shader.setUniform3f(iResolution, width, height, 0);
}
void moveMouse(int x, int y) {
    cam.moveMouse(x, y);
}
void dragMouse(int x, int y) {
    cam.dragMouse(x, y);
}

void keyFunc(uchar key, int x, int y) {
    cam.keyMove(key,1);
}

void renderQuads() {

    edata.renderData(GL_QUADS);
}

void renderScene(void)
{
    timeVal += 0.01;

    shader.setUniform1f(iTime, timeVal);

    renderQuads();
}

void render() {
    cam.drawView();
    glutSwapBuffers();
}

void initCamera() {
    cam.init();
    cam.setPosition(0, 0.001, 100);
    cam.setViewPort(0, 0, 500, 500);
}

void shaderTest() {
    obj.testDrawFunc = renderScene;
    cam.Scene = &obj;
    setShapes();
    setShaders("test1.txt");
}

void ImageTest() {
    cv::Mat img = cv::imread("C:\\Users\\fwf\\Desktop\\7bd0d5d3ffe0edd98fa1e5af665d2bb5334af342.7.jpg");
    cv::Mat aim;
    cv::bilateralFilter(img, aim, 25,25*2,25/2);

    img3d.setSrcData(img);
    img3d.setAnimation(aim);
    img3d.generateData();
    img3d.active(true);
    img3d.enableReplay(true);
    cam.Scene = &img3d;
}

void quadTest() {
    obj.testDrawFunc = renderQuads;
    cam.Scene = &obj;
    setShapes();
}

int main(int arg, char**argv) {
    glutInit(&arg, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);

    glutCreateWindow("GLSL的第一步");

    glutPassiveMotionFunc(moveMouse);
    glutMotionFunc(dragMouse);
    glutKeyboardFunc(keyFunc);
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);

    initCamera();
    ImageTest();
    glutMainLoop();
    return 0;
}