#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<GL\freeglut.h>
float step = 0.1f;

Texture2D tex;
Texture2D depthMap;
Texture2D colorMap;
Texture2D stencilMap;
FBObject fbo;
Camera camera;
Mesh mesh;
Mesh cube;
Mesh quad;
Shader procShd;
Shader rayShd;

void reshape(int width, int height) {
    camera.setWindowSize(width, height);
    fbo.resize(width, height);
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
    camera.setFar(20);
    camera.lookAt(5, 5, 5, 0, 0, 0);
    camera.setViewPort(0, 0, 500, 500);
}

void render() {


    bool valid = fbo.vaild();

    camera.beginRender();

    fbo.bind();
    fbo.clearBuffers();

    rayShd.bind();
    rayShd.setUniform2f("viewport", camera.ViewWidth(), camera.ViewHeight());

     for (int i = 0; i <= 0; i++) {
         glPushMatrix();
         glTranslatef(0, i * 5, 0);
         glScaled(i + 1, i + 1, i + 1);

         mesh.draw();
         glPushMatrix();
    }
    rayShd.unBind();

    glTranslatef(-0.5, -0.8, -1);

    cube.draw();
    fbo.unBind();
    camera.endRender();

    camera.beginRender();

    colorMap.bind(0);
    depthMap.bind(1);
    stencilMap.bind(2);

    procShd.bind();
    procShd.setUniform2f("viewport", camera.ViewWidth(), camera.ViewHeight());
    procShd.setUniform1i("colorMap", 0);
    procShd.setUniform1i("depthMap", 1);
    procShd.setUniform1i("stencilMap", 2);
    procShd.setUniformMat4("prjMatInv", camera.getProjectionMatInvPtr());

    quad.draw();

    procShd.unBind();

    stencilMap.unBind();
    depthMap.unBind();
    colorMap.unBind();


   

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
    Mesh::activeVAO = false;
    Mesh::buildCube(mesh);
    Mesh::buildCube(cube);
    Mesh::buildQuad(quad);
    quad.setPosition(0, 0, 1.1);
    

    tex.loadFileImg("..\\Image\\wood.jpg");
    cube.texture0 = tex;

    rayShd.loadFragFile("sphere_frag.glsl");
    rayShd.link();

    colorMap.attchColorFBO(fbo);
    depthMap.attchDepthStencilFBO(fbo);

    procShd.loadVertexFile("proc_vert.glsl");
    procShd.loadFragFile("proc_frag.glsl");
    procShd.link();
}

void initStencil() {
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0xFF, 0xFF);
    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
}

int main(int arg, char**argv) {
    glutInit(&arg, argv);
    glewInit();
    initGlut();
    initCamera();
    initStencil();
    raymarchTest();
    glutMainLoop();
    return 0;
}