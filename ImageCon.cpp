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

Camera camera;

Layer lyr;
Image3DEx img3d;
Cube testshp;
ImgTexture texture;
ImgTexture textureNor;
ImgTexture textureSpe;
DepthTexture depthMap;
Shader water;
AttribArray edata;
float timeVal = 0;
float step = 0.1;

void reshape(int width, int height) {
   
    camera.setViewPort(0, 0, width, height);
    w = width, h = height;
   
}

void moveMouse(int x, int y) {
    
    camera.moveMouse(x, y);
   

    Light& l = camera.getLight();
    float dx, dy, dz;
    camera.mouseRay(x, y, dx, dy, dz); 
    //l.setPostion(dx, dy, -dz,0);
    l.setPostion(1, 1, 1,0);
}

void dragMouse(int x, int y) {
  
    camera.dragMouse(x, y);
    
}

void keyFunc(uchar key, int x, int y) {
    switch (key)
    {
    case 'w':
        camera.localMove(0, 1 * step, 0);
        break;
    case 's':
        camera.localMove(0, -1 * step, 0);
        break;
    case 'a':
        camera.localMove(-1 * step, 0, 0);
        break;
    case 'd':
        camera.localMove(1 * step, 0, 0);
        break;
    case ' ':
        depthMap.enablePbr = !depthMap.enablePbr;
        break;
    }
}

void spkeyFunc(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_UP:
        camera.localMove(0, 0, 1);
        break;
    case GLUT_KEY_DOWN:
        camera.localMove(0, 0, -1);
    }
}

void renderShadow() {
    float x, y, z, w;
    camera.getLight().getPositon(x, y, z, w);
    depthMap.loadDepthMap(camera.posX(), camera.posY(), camera.posZ(), x, y, z, w, lyr);

    camera.beginRender();

    depthMap.bindShadow();
    depthMap.setViewMatInv(camera.getViewMatInvPtr());

    testshp.draw();

    depthMap.unbindShadow();
    camera.endRender();

    

    glutSwapBuffers();
}

void renderWater() {
    camera.beginRender();

    camera.setCamUniform(water);

    lyr.draw();

    camera.endRender();

    glutSwapBuffers();
}

void initCamera() {
    camera.init();
    camera.setFar(1000);
    camera.isDrawAxis = true;
    camera.setPosition(0, 0, 0);
    camera.setViewPort(0, 0, 500, 500);
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
    lyr.add(&img3d);

   
}



void waterTest() {


    testshp.isDrawAxis = true;


    water.loadFragFile("GLSL/water.glsl");
    water.link();

    camera.setPosition(0, 0, 10);
   
}

void shadowTest() {

    texture.loadRgbImg("Img/wood.jpg");
    textureNor.loadRgbImg("Img/woodNormal.png");
    textureSpe.loadRgbImg("Img/woodSpec.png");
 
    testshp.texture0 = texture;
    testshp.texture1 = textureNor;
    testshp.texture2 = textureSpe;

    camera.isShaderBack = false;
    camera.setPosition(0, 0.5, 1);
    
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
    glutDisplayFunc(renderShadow);
    glutIdleFunc(renderShadow);
    glutReshapeFunc(reshape);
}

int main(int arg, char**argv) {
    glutInit(&arg, argv);
    glewInit();
    initWindows();
    initCamera();

    shadowTest();
    glutMainLoop();
    return 0;
}