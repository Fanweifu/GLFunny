#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<GL\freeglut.h>
#include<math.h>
#define TESTFRAGFILE1 "water_frag.glsl"
#define TESTFRAGFILE2 "eve_frag.glsl"
//#define USEFBO

float step = 0.1f;
float offsetX = 0;
float offsetY = 0;
float offsetZ = 0;
float zScale = 1;
float zta = 0;
Texture2D tex;
Texture2D depthMap;
Texture2D colorMap;
FBObject fbo;
Camera camera;
Mesh mesh;
Mesh cube;
Mesh quad;
Shader procShd;
Shader rayShd1;
Shader rayShd2;

void reshape(int width, int height) {
    camera.setWindowSize(width, height);
    fbo.resize(width, height);
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
        rayShd1.clear();
        rayShd1.loadFragFile(TESTFRAGFILE1);
        rayShd1.link();
    }
}

void spkeyFunc(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_UP:
        offsetZ += 0.1;
        break;
    case GLUT_KEY_DOWN:
        offsetZ -= 0.1;
        break;
    case GLUT_KEY_LEFT:
        offsetX -= 0.1;
        break;
    case GLUT_KEY_RIGHT:
        offsetZ += 0.1;
        break;
    }
}

void mouseWheel(int b, int d, int x,int y) {
    zScale += d*0.1;
}

void initCamera() {
    camera.init();
    camera.lookAt(5, 5, 5, 0, 0, 0);
    camera.setViewPort(0, 0, 500, 500);
}


void render() {

    zta += 0.01;
    camera.setLightPos(cos(zta)*cos(1), sin(zta)*cos(1), sin(1),0);

    camera.beginRender();
#ifdef USEFBO
    fbo.bind();
    fbo.clearBuffers();
#endif 
   

    

    rayShd2.bind();
    rayShd2.setUniform2f("viewport", camera.ViewWidth(), camera.ViewHeight());
    rayShd2.setUniform1f("time", camera.getRenderTimes(0.01));
    glScalef(1, 1, zScale);
    cube.draw();
    glScalef(1, 1, 1/ zScale);
    rayShd2.unBind();

    glTranslatef(offsetX, offsetY, offsetZ);
    cube.draw();
    glTranslatef(-offsetX, -offsetY, -offsetZ);
    
    rayShd1.bind();
    rayShd1.setUniform2f("viewport", camera.ViewWidth(), camera.ViewHeight());
    rayShd1.setUniform1f("time", camera.getRenderTimes(0.01));

    mesh.draw();
    rayShd1.unBind();

#ifdef USEFBO
    fbo.unBind();
#endif

    camera.endRender();

#ifdef USEFBO


    colorMap.bind(0);
    depthMap.bind(1);
    procShd.bind();
    procShd.setUniform2f("viewport", camera.ViewWidth(), camera.ViewHeight());
    procShd.setUniform1i("colorMap", 0);
    procShd.setUniform1i("depthMap", 1);
    procShd.setUniform1i("smoothl", 6);

    quad.draw();

    procShd.unBind();
    depthMap.unBind();
    colorMap.unBind();
    
#endif

    glutSwapBuffers();
}

void initGlut() {
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);

    glutCreateWindow("raymarch");

    glutPassiveMotionFunc(moveMouse);
    glutMotionFunc(dragMouse);
    glutMouseWheelFunc(mouseWheel);
    glutKeyboardFunc(keyFunc);
    glutSpecialFunc(spkeyFunc);
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
}

void rayMarch() {
    Mesh::activeVAO = false;
    Mesh::buildCube(mesh,-100,100,-100,100,-6,6);
    Mesh::buildCube(cube);
    Mesh::buildQuad(quad);

    tex.loadFileImg("..\\Image\\wood.jpg");
    cube.texture0 = tex;

    rayShd1.loadFragFile(TESTFRAGFILE1);
    rayShd1.link();
    rayShd2.loadFragFile(TESTFRAGFILE2);
    rayShd2.link();

    //colorMap.attchColorFBO(fbo);
    //depthMap.attchDepthStencilFBO(fbo);

    procShd.loadVertexFile("proc_vert.glsl");
    procShd.loadFragFile("proc_frag.glsl");
    procShd.link();
}

int main(int arg, char**argv) {
    glutInit(&arg, argv);
    glewInit();
    initGlut();
    initCamera();
    rayMarch();
    glutMainLoop();
    return 0;
}