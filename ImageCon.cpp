#include<gl/glew.h>
#include<gl/GL.h>

#include<gl/glut.h>
#include<string>
#include<time.h>
#include"Shape\camera.h"
#include"Shape\shader.h"
#include"Shape\image3dex.h"
using namespace std;

string iResolution = "iResolution";
string iTime = "iTime";
string iMouse = "iMouse";
string cameraRotationMat = "cameraRotationMat";
string cameraPosition = "cameraPosition";

Camera cam;
Image3DEx img3d;
ElementData edata;
float timeVal = 0;



void reshape(int width, int height) {
    cam.setViewPort(0, 0, width, height);
}
void moveMouse(int x, int y) {
    cam.moveMouse(x, y);
}
void dragMouse(int x, int y) {
    cam.dragMouse(x, y);
}

void keyFunc(uchar key, int x, int y) {
    cam.keyMove(key, 1);
}

void render() {
    cam.drawView();
    glutSwapBuffers();
}

void initCamera() {
    cam.init();
    cam.setFar(10000);
    cam.setPosition(0, 0.001, 300);
    cam.setViewPort(0, 0, 500, 500);
}


void ImageTest(string imgpath,int size) {


    double t1 = GetTickCount();

    cv::Mat img = cv::imread(imgpath);

    if (img.empty()) {
        cout << "load failed! press any key to exit"<<endl;
        getchar();
        exit(0);
    }


    cv::Mat aim;
    cv::blur(img, aim, cv::Size(size*2+1, size*2+1));

    double t2 = GetTickCount();

    cout << "image load and process cost" << t2 - t1 << "ms"<< endl;

    img3d.initShader();
    img3d.reShape(img.rows, img.cols);

    double t3 = GetTickCount();
    img3d.setSrcData(img);
    img3d.setAnimation(aim);
    img3d.generateData();
   

    double t4 = GetTickCount();
    cout << "generate 3d data cost" << t4 - t3 << "ms" << endl;

    img3d.active(true);
    cam.Scene = &img3d;
}



void initWindows() {
    
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);

    glutCreateWindow("Func:blur;height=floor(r+g+b/3)");

    glutPassiveMotionFunc(moveMouse);
    glutMotionFunc(dragMouse);
    glutKeyboardFunc(keyFunc);
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
}

int main(int arg, char**argv) {
    glutInit(&arg, argv);

    initWindows();

    initCamera();

    cout << "input a image path and pleasure it!(control key: wsadqe):";
    char path[100];
    cin.getline(path, 100);

    cout << "input k -->blur(src, dst, Size(k*2+1, k*2+1))):";
    uint k = 0;
    cin >> k;

    ImageTest(path,k);
    glutMainLoop();
    return 0;
}