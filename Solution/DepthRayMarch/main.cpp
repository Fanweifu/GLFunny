#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<GL\freeglut.h>
float step = 0.1f;
ImgTexture tex;
Camera camera;
Mesh mesh;
Mesh cube;
Shader rayShd;

void reshape(int width, int height) {
    camera.setWindowSize(width, height);
}

void moveMouse(int x, int y) {
    camera.moveMouse(x, y);
    float dx, dy, dz;
    camera.mouseRay(x, y, dx, dy, dz);
    camera.setLightPos(1, 1, 1, 0);
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
        rayShd.clear();
        rayShd.loadFragFile("sphere_frag.glsl");
        rayShd.link();
    }
}

void spkeyFunc(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_UP:
        cube.setPosition(cube.posX(), cube.posY() + 0.1, cube.posZ());
        break;
    case GLUT_KEY_DOWN:
        cube.setPosition(cube.posX(), cube.posY() - 0.1, cube.posZ());
        break;
    case GLUT_KEY_LEFT:
        cube.setPosition(cube.posX() - 0.1, cube.posY(), cube.posZ());
        break;
    case GLUT_KEY_RIGHT:
        cube.setPosition(cube.posX() + 0.1, cube.posY(), cube.posZ());
        break;
    }
}

void initCamera() {

    camera.init();
    camera.lookAt(5, 5, 5, 0, 0, 0);
    camera.setViewPort(0, 0, 500, 500);
}

void render() {
    camera.beginRender();

    rayShd.bind();
    camera.setCamUniform(rayShd);

    for (int i = 0; i <= 5; i++) {
        glPushMatrix();
        glTranslatef(0, i * 5, 0);
        glScaled(i + 1, i + 1, i + 1);

        mesh.draw();
        glPushMatrix();
    }


    rayShd.unBind();

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

void raymarchTest() {

    Mesh::buildCube(mesh);
    Mesh::buildCube(cube);
    
    //MeshShape::activeVAO = false;
    tex.loadRgbImg("..\\wood.jpg");
    cube.texture0 = tex;
    //rayShd.loadVertexFile("sphere_vert.glsl");
    rayShd.loadFragFile("sphere_fragdepth.glsl");
    rayShd.link();
}

int main(int arg, char**argv) {
    glutInit(&arg, argv);
    glewInit();
    initGlut();
    initCamera();

    raymarchTest();
    glutMainLoop();
    return 0;
}