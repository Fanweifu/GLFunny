#include<glHead.h>


#include<string>
#include<time.h>
#include<windows.h>

#include<gl\glut.h>
#include"Shape\camera.h"
#include"Shape\shader.h"
#include"Shape\texture.h"
#include"Shape\image3dex.h"
#include"Shape\complexShape.h"
#include"Shape\layer.h"

using namespace std;

int w, h;

Camera cam1;
Camera cam2;

Layer ly1;
Layer ly2;
Image3DEx img3d;
ComplexShape testshp;
Texture texture;
Shader water;
ElementData edata;
float timeVal = 0;
bool mutiScreen = true;
float step = 0.1;
void reshape(int width, int height) {
    if (mutiScreen) {
        cam1.setViewPort(0, 0, width / 2, height);
        cam2.setViewPort(width / 2, 0, width / 2, height);
        w = width / 2, h = height;
        
    }
    else {
        cam1.setViewPort(0, 0, width , height);
        w = width, h = height;
    }
}

void moveMouse(int x, int y) {
    if (mutiScreen) {

        if (x < cam2.getViewX())
            cam1.moveMouse(x, y);
        else {
            cam2.moveMouse(x, y);
        }
    }
    else {
        cam1.moveMouse(x, y);
    }

    Light& l = cam1.getLight();
    float dx, dy, dz;
    cam1.mouseCoordToDir(x, y, dx, dy, dz);
    printf("mouse:(%d,%d) dir:(%f,%f,%f)\n", x, y, dx, dy, dz);
    //l.setPostion(dx, dy, dz, 0);
}

void dragMouse(int x, int y) {
    if (mutiScreen) {
        if (x < cam2.getViewX())
            cam1.dragMouse(x, y);
        else {
            cam2.dragMouse(x, y);
        }

        
    }
    else {
        cam1.dragMouse(x, y);
    }


    
}

void keyFunc(uchar key, int x, int y) {
    switch (key)
    {
    case 'w':
        cam1.localMove(0, 1 * step, 0);
        break;
    case 's':
        cam1.localMove(0, -1* step, 0);
        break;
    case 'a':
        cam1.localMove(-1 * step, 0, 0);
        break;
    case 'd':
        cam1.localMove(1 * step, 0, 0);
        break;
    case ' ':
        cam1.setOrthoH(500);
        cam1.isOrtho = !cam1.isOrtho;
        break;
    }
}

void spkeyFunc(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_UP:
        cam1.localMove(0, 0, 1);
        break;
    case GLUT_KEY_DOWN:
        cam1.localMove(0, 0, -1);
    }
}

void render() {

    static float time = 0;
    water.use();
    water.setUniform2f(Shader::pView, w, h);
    water.setUniform1f(Shader::pTime, time);
    water.setUniformMat4(Shader::pMdlInvMat, cam1.getModelViewPtr());
    water.setUniformMat4(Shader::pPrjInvMat, cam1.getProjectionMatInvPtr());
    time += 0.01;

    cam1.drawView();
    if(mutiScreen) cam2.drawView();
    glutSwapBuffers();
}

void initCamera() {
    cam1.init();
    cam1.setFar(1000);
    cam1.drawAxis = true;
    cam1.setPosition(0, -10, 300);
    cam1.setViewPort(0, 0, 250, 500);

    cam2.init();
    cam2.setFar(1000);
    cam2.drawAxis = true;
    cam2.setPosition(0, 10, 300);
    cam2.setViewPort(250, 0, 250, 500);

    ly1.add(&cam2);
    ly2.add(&cam1);
    cam1.Scene = &ly1;
    cam2.Scene = &ly2;

}

void imgShapeTest() {
    cout << "input a image path and pleasure it!(control key: wsadqe):";
    char path[100];
    cin.getline(path, 100);

    cout << "input k -->blur(src, dst, Size(k*2+1, k*2+1))):";
    uint k = 0;
    cin >> k;

    double t1 = GetTickCount();

    cv::Mat img = cv::imread(path);

    if (img.empty()) {
        cout << "load failed! press any key to exit" << endl;
        getchar();
        exit(0);
    }

    cv::Mat aim;
    cv::medianBlur(img, aim, 2 * k + 1);

    cv::Mat sp[3];
    cv::split(img, sp);
    for (int i = 0; i < 3; i++)
    {
        cv::equalizeHist(sp[i], sp[i]);
    }
    cv::merge(sp, 3, aim);

    double t2 = GetTickCount();

    cout << "image load and process cost " << t2 - t1 << " ms" << endl;

    img3d.initShader();
    img3d.reShape(img.rows, img.cols);

    double t3 = GetTickCount();
    img3d.setSrcImage(img);
    img3d.setDstImage(aim);
    img3d.generateData();

    double t4 = GetTickCount();
    cout << "generate 3d data cost " << t4 - t3 << " ms" << endl;

    img3d.active(true);
    img3d.speed = 0.05;
    ly1.add(&img3d);
   
    ly2.add(&img3d);

}

void waterTest() {

    testshp.addPoint(-100, -100, 0);
    testshp.addPoint(-100, 100, 0);
    testshp.addPoint(100, 100, 0);
    testshp.drawAxis = true;
    //testshp.addPoint(100, -100, 0);
    water.loadFragFile("res/water.glsl");
    water.link();
    
    testshp.pshader = water;
   


    ly1.add(&testshp);
    ly2.add(&testshp);

    cam1.setPosition(0, 0.5, 1);
    cam2.setPosition(0, -0.5, 1);
    cam2.visible = false;

    mutiScreen = false;
}

void printVersion() {
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    printf("GL Vendor : %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version(string) : %s\n", version);
    printf("GL Version(integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);

}

void initWindows() {
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    
    glutCreateWindow("GLSL");
    printVersion();
    glutPassiveMotionFunc(moveMouse);
    glutMotionFunc(dragMouse);
    glutKeyboardFunc(keyFunc);
    glutSpecialFunc(spkeyFunc);
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
}

int main(int arg, char**argv) {
    glutInit(&arg, argv);
    glewInit();
    initWindows();

    initCamera();

    waterTest();
    //imgShapeTest();
    glutMainLoop();
    return 0;
}
