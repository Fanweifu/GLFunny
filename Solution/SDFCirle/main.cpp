#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<Shape\layer.h>
#include<Shape\texture.h>
#include<GL\freeglut.h>

float step = 0.1;
int smooth = 2;
Camera camera;
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

void initCamera() {
    camera.init();
    camera.setPosition(0, 0, 5);
    camera.setViewPort(0, 0, 500, 500);
}

void render() {
   
    camera.beginRender();
    
    shd.bind();
    shd.setUniform2f("viewport", camera.ViewWidth(), camera.ViewHeight());

    testshp.draw();

    camera.endRender();

    glutSwapBuffers();
}

void initGlut() {
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);

    glutCreateWindow("Shadow");

    glutPassiveMotionFunc(moveMouse);
    glutMotionFunc(dragMouse);
    glutKeyboardFunc(keyFunc);
    glutSpecialFunc(spkeyFunc);
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
}

void shadowTest() {
    //cube build
    Mesh::activeVAO = false;
    Mesh::buildCube(testshp);
    shd.loadVertexFile("sdf_vert.glsl");
    shd.loadFragFile("sdf_frag.glsl");
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