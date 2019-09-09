#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<Shape\layer.h>
#include<Shape\texture.h>
#include<GL\freeglut.h>
#include<glm.hpp>
float step = 0.1f;
int smooth = 2;
Camera camera;
Layer lyr;
Mesh sdfshp;
FBObject fbo;
DepthTexture depthMap;
Texture2D texture;
Shader shadowShd;
glm::vec3 angle;
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

	case 'x':
		angle.x += 0.1f;
		lyr.setRotation(angle.x,angle.y,angle.z);
		break;
	case 'y':
		angle.y += 0.1f;
		lyr.setRotation(angle.x, angle.y, angle.z);
		break;
	case 'z':
		angle.z += 0.1f;
		lyr.setRotation(angle.x, angle.y, angle.z);
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

    depthMap.begin(camera.posX(), camera.posY(), camera.posZ(), x, y, z, w);
    
    fbo.bind();
    fbo.clearBuffers();
	lyr.isDrawAxis = true;
    lyr.draw();
    fbo.unBind();

    depthMap.end();
    
    
    camera.beginFrame();

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

void sdfFontTest() {
    //cube build
    Mesh::activeVAO = false;
    Mesh::buildCube(sdfshp);

    //shader init
    if (Mesh::activeVAO) shadowShd.loadVertexFile("shadowv_attr_vert.glsl");
    else shadowShd.loadVertexFile("shadowv_vert.glsl");
    shadowShd.loadFragFile("shadow_frag.glsl");
    shadowShd.link();

    //texture
    texture.loadImg("..\\Image\\wood.jpg");
    sdfshp.texture0 = texture.ObjectID();
    
    fbo.resize(depthMap.width, depthMap.height);
    depthMap.attchDepthStencilFBO(fbo);

    Layer * comp1 = new Layer();
    comp1->addUnique(&sdfshp);
    Layer * comp2 = new Layer();
    comp2->setPosition(0, 0, -5);
    comp2->setScale(10, 10, 1);
    comp2->addUnique(&sdfshp);

    lyr.addUnique(comp1);
    lyr.addUnique(comp2);

    camera.setPosition(0, 0.5, 1);
}

int main(int arg, char**argv) {
    glutInit(&arg, argv);
    glewInit();
    initGlut();
    initCamera();

    sdfFontTest();
    glutMainLoop();
    return 0;
}