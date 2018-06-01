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

Camera cam1;
Camera cam2;

Layer ly1;
Layer ly2;
int w, h;
Image3DEx img3d;
ElementData edata;
float timeVal = 0;

void reshape(int width, int height) {
    cam1.setViewPort(0, 0, width/2, height);
    cam2.setViewPort(width / 2, 0, width / 2, height);
}
void moveMouse(int x, int y) {
    if(x<cam2.getViewX())
        cam1.moveMouse(x, y);
    else {
        cam2.moveMouse(x, y);
    }
}

void dragMouse(int x, int y) {
    if (x<cam2.getViewX())
        cam1.dragMouse(x, y);
    else {
        cam2.dragMouse(x, y);
    }
}

void keyFunc(uchar key, int x, int y) {
    switch (key)
    {
    case 'w':
        cam1.localMove(0, 1, 0);
        break;
    case 's':
        cam1.localMove(0, -1, 0);
        break;
    case 'a':
        cam1.localMove(-1, 0, 0);
        break;
    case 'd':
        cam1.localMove(1, 0, 0);
        break;
    case ' ':
        cam1.setOrthoH(h);
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
    cam1.drawView();
    cam2.drawView();
    glutSwapBuffers();
}

void initCamera() {
    cam1.init();
    cam1.setFar(1000);
    cam1.isdrawAxis = true;
    cam1.setPosition(0, -10, 300);
    cam1.setViewPort(0, 0, 250, 500);

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
    cv::medianBlur(img, aim, 2 * size + 1);
    
    cv::Mat sp[3];
    cv::split(img, sp);
    for(int i=0;i<3;i++)
    {
        cv::equalizeHist(sp[i], sp[i]);
    }
    cv::merge(sp,3,aim);


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
    w = img.cols;
    h = img.rows;
    ly1.add(&img3d);
    ly1.add(&cam2);
    ly2.add(&img3d);
    ly2.add(&cam1);
    

    cam1.Scene = &ly1;
    cam2.Scene = &ly2;
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