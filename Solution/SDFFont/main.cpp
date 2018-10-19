#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<Shape\layer.h>
#include<Shape\texture.h>
#include<GL\freeglut.h>

float step = 0.1;
int smooth = 2;
int imgWidth = 32;
float smoothDelta = 0.002;
float fillDist = 0.005;
Camera camera;
Texture2D sdfTex;
Mesh testshp;
Shader shd;
void reshape(int width, int height) {
    camera.setWindowSize(width, height);
}

void moveMouse(int x, int y) {
    camera.moveMouse(x, y);
}

void dragMouse(int x, int y) {
    camera.dragMouse(x, y);
}

void keyFunc(unsigned char key, int x, int y) {
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
        shd.clear();
        shd.loadVertexFile("sdfFont_vert.glsl");
        shd.loadFragFile("sdfFont_frag.glsl");
        shd.link();
        break;
    }
}

void swiftTexture(bool up) {
    
    imgWidth = imgWidth*(up ? 2 : 0.5);
    if (imgWidth > 512) imgWidth = 32;
    else if (imgWidth < 32) imgWidth = 512;

    char imgPath[30];
    sprintf_s(imgPath, "charSDF%dX%d.png", imgWidth, imgWidth);

    if (sdfTex.loadFileImg(imgPath)) {
        printf("%s loaded!\n",imgPath);
    }

}

void spkeyFunc(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_UP:  
        smoothDelta += 0.001;
        break;
    case GLUT_KEY_DOWN:
        smoothDelta -= 0.001;
        if (smoothDelta < 0) smoothDelta = 0;
        break;
    case GLUT_KEY_LEFT:
        fillDist += 0.001;
        break;
    case GLUT_KEY_RIGHT:
        fillDist -= 0.001;
        if (fillDist < 0) fillDist = 0;
        break;
    }
}

void mouseWheel(int b, int d, int x, int y) {
    bool up = d > 0;
    swiftTexture(up);
}


void initCamera() {
    camera.init();
    camera.setPosition(0, 0, 5);
    camera.setViewPort(0, 0, 500, 500);
}

void render() {

    camera.beginRender();

    shd.bind();
    shd.setUniform1i("sdfTex",0);
    shd.setUniform1f("smoothDelta", smoothDelta);
    shd.setUniform1f("fillDist", fillDist);
    shd.setUniform2f("texSize", imgWidth, imgWidth);
    float time = camera.getRenderTimes(0.0005);
    float scl = (sin(time) + 2);
    testshp.setScale(scl);
    testshp.setRotation(0, 0, time*50);

    testshp.draw();

    camera.endRender();

    glutSwapBuffers();
}

void initGlut() {
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);

    glutCreateWindow("sdfFont");
    glutPassiveMotionFunc(moveMouse);
    glutMotionFunc(dragMouse);
    glutKeyboardFunc(keyFunc);
    glutSpecialFunc(spkeyFunc);
    glutMouseWheelFunc(mouseWheel);
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
}

void shadowTest() {
    //cube build
    Mesh::activeVAO = false;
    Mesh::buildCube(testshp);


    sdfTex.loadFileImg("charSDF32X32.png");
    testshp.texture0 = sdfTex;

    shd.loadVertexFile("sdfFont_vert.glsl");
    shd.loadFragFile("sdfFont_frag.glsl");
    shd.link();
}

int main(int arg, char**argv) {
    glutInit(&arg, argv);
    glewInit();
    initGlut();
    initCamera();

    shadowTest();
    glutMainLoop();
    return 0;
}