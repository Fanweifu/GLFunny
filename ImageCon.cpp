#include<gl/glew.h>
#include<gl/GL.h>

#include<gl/glut.h>
#include<string>
#include<time.h>
#include"Shape\camera.h"
#include"Shape\shader.h"
#include"Shape\image3dex.h"
#include"Shape\layer.h"
using namespace std;

string iResolution = "iResolution";
string iTime = "iTime";
string iMouse = "iMouse";
string cameraRotationMat = "cameraRotationMat";
string cameraPosition = "cameraPosition";

Camera cam;
Camera cam2;

Layer ly;

Image3DEx img3d;
ElementData edata;
float timeVal = 0;

void reshape(int width, int height) {
    cam.setViewPort(0, 0, width/2, height);
    cam2.setViewPort(width / 2, 0, width / 2, height);
}
void moveMouse(int x, int y) {
    if(x<cam2.getViewX())
        cam.moveMouse(x, y);
    else {
        cam2.moveMouse(x, y);
    }
}

void dragMouse(int x, int y) {
    if (x<cam2.getViewX())
        cam.dragMouse(x, y);
    else {
        cam2.dragMouse(x, y);
    }
}

void keyFunc(uchar key, int x, int y) {
    switch (key)
    {
    case 'w':
        cam.Move(CAM_FORWARD, 3);
        break;
    case 's':
        cam.Move(CAM_BACK, 3);
        break;
    case 'a':
        cam.Move(CAM_LEFT, 3);
        break;
    case 'd':
        cam.Move(CAM_RIGHT, 3);
        break;
    }
}
void spkeyFunc(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_UP:
        cam.Move(CAM_UP, 3);
        break;
    case GLUT_KEY_DOWN:
        cam.Move(CAM_DOWN, 3);
    }
}

void render() {
    cam.drawView();
    cam2.drawView();
    glutSwapBuffers();
}

void initCamera() {
    cam.init();
    cam.setFar(1000);
    cam.isdrawAxis = true;
    cam.setPosition(0, -10, 300);
    cam.setViewPort(0, 0, 250, 500);

    cam2.init();
    cam2.setFar(1000);
    cam2.isdrawAxis = true;
    cam2.setPosition(0, 10, 300);
    cam2.setViewPort(250, 0, 250, 500);

}

void ImageTest(string imgpath, int size) {
    double t1 = GetTickCount();

    cv::Mat img = cv::imread(imgpath);

    if (img.empty()) {
        cout << "load failed! press any key to exit" << endl;
        getchar();
        exit(0);
    }

    cv::Mat aim;
    cv::medianBlur(img, aim, size * 2 + 1);

    double t2 = GetTickCount();

    cout << "image load and process cost " << t2 - t1 << " ms" << endl;

    img3d.initShader();
    img3d.reShape(img.rows, img.cols);

    double t3 = GetTickCount();
    img3d.setSrcData(img);
    img3d.setAnimation(aim);
    img3d.generateData();

    double t4 = GetTickCount();
    cout << "generate 3d data cost " << t4 - t3 << " ms" << endl;

    img3d.active(true);
    
    
    ly.add(&img3d);
    ly.add(&cam2);
    ly.add(&cam);
    

    cam.Scene = &ly;
    cam2.Scene = &ly;
}

void initWindows() {
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);

    glutCreateWindow("Func:blur;height=floor(r+g+b/3)");

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

    initWindows();

    initCamera();

    cout << "input a image path and pleasure it!(control key: wsadqe):";
    char path[100];
    cin.getline(path, 100);

    cout << "input k -->blur(src, dst, Size(k*2+1, k*2+1))):";
    uint k = 0;
    cin >> k;

    ImageTest(path, k);
    glutMainLoop();
    return 0;
}