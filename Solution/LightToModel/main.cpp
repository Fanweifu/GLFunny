#include<gl_def.h>
#include<Shape\camera.h>
#include<Shape\texture.h>
#include<Shape\mesh.h>
#include<GL\freeglut.h>


#define TESTFRAGFILE "lighttomodel_frag.glsl"
float step = 0.1f;
float offsetX = 0;
float offsetY = 0;
float offsetZ = 0;
float zta = 0;
Texture2D tex;
Camera camera;
Mesh mesh;
Shader procShd;

void reshape(int width, int height) {
    camera.setWindowSize(width, height);
   
}

void moveMouse(int x, int y) {
    camera.moveMouse(x, y);
    float dx, dy, dz;
    camera.mouseRay(x, y, dx, dy, dz);
    //camera.setLightPos(dx , dy, 1, 0);
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
        procShd.clear();
        procShd.loadFragFile(TESTFRAGFILE);
        procShd.link();
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

void initCamera() {
    camera.init();
    camera.lookAt(5, 5, 5, 0, 0, 0);
    camera.setViewPort(0, 0, 500, 500);
}


void render() {

    zta += 0.01;
    camera.setLightPos(cos(zta), sin(zta), 1, 0);

    camera.beginRender();

    procShd.bind();
    procShd.setUniform2f("viewport", camera.ViewWidth(), camera.ViewHeight());
    procShd.setUniform1f("time", camera.getRenderTimes(0.01));
    mesh.draw();
    procShd.unBind();

    glTranslatef(offsetX, offsetY, offsetZ);
    mesh.draw();
    
    camera.endRender();

    glutSwapBuffers();
}

void initGlut() {
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);

    glutCreateWindow("raymarch");

    glutPassiveMotionFunc(moveMouse);
    glutMotionFunc(dragMouse);
    glutKeyboardFunc(keyFunc);
    glutSpecialFunc(spkeyFunc);
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
}

void rayMarch() {
    Mesh::activeVAO = true;
    Mesh::buildCube(mesh);
 

    tex.loadFileImg("..\\Image\\wood.jpg");
    mesh.texture0 = tex;

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