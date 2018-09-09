#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<Shape\layer.h>
#include<Shape\texture.h>
#include<GL\freeglut.h>

float step = 0.1;
int smooth = 2;
Camera camera;
Layer lyr;
Mesh testshp;
DepthTexture depthMap;
Texture2D texture;
Shader shadowShd;

void reshape(int width, int height) {
    camera.setWindowSize(width, height);
}

void moveMouse(int x, int y) {
    camera.moveMouse(x, y);

    float dx, dy, dz;
    camera.mouseRay(x, y, dx, dy, dz);
    camera.setLightPos(dx, dy, -dz, 0);
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
    float x, y, z, w;
    camera.getLightPos(x, y, z, w);

    depthMap.beginLoad(camera.posX(), camera.posY(), camera.posZ(), x, y, z, w);
    lyr.draw();
    depthMap.endLoad();

    camera.beginRender();

    depthMap.bind(3);

    shadowShd.bind();
    shadowShd.setUniform1i("depthTex", 3);
    shadowShd.setUniform1i("baseTex", 0);
    shadowShd.setUniformMat4("lightSpace", depthMap.getLightPrjViewMat());
    shadowShd.setUniformMat4("cameraViewInv", camera.getViewMatInvPtr());
    shadowShd.setUniform1i("smoothLevel", smooth);
    shadowShd.setUniform1f("biasFactor", depthMap.shadowAcneBias());

    lyr.draw();

    shadowShd.unBind();
    depthMap.unBind();

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
    Mesh::activeVAO = true;
    Mesh::buildCube(testshp);

    //shader init
    if (Mesh::activeVAO) shadowShd.loadVertexFile("shadowv_attr_vert.glsl");
    else shadowShd.loadVertexFile("shadowv_vert.glsl");
    shadowShd.loadFragFile("shadow_frag.glsl");
    shadowShd.link();

    //texture
    texture.loadFileImg("..\\Image\\wood.jpg");
    testshp.texture0 = texture;



    Layer * comp1 = new Layer();
    comp1->addUnique(&testshp);
    Layer * comp2 = new Layer();
    comp2->setPosition(0, 0, -5);
    comp2->setScale(10, 10, 1);
    comp2->addUnique(&testshp);

    lyr.addUnique(comp1);
    lyr.addUnique(comp2);

    camera.setPosition(0, 0.5, 1);
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