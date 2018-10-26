#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<Shape\layer.h>
#include<Shape\texture.h>
#include<GL\freeglut.h>
#include"Ext\import-font.h"
//#include"sdfExample.h"
#define TESTNUM 10
#define TESTSIZE 32
#define TESTWIDTH 512
#define TESTHEIGHT 64
unsigned char texData[TESTWIDTH*TESTHEIGHT] = { 0.0f };
float curx = 8, cury = 32;

float markDist = 0;
float rotateZ = 0;
float scale = 1;
Camera camera;
Texture2D sdfTex[TESTNUM];
Texture2D fontTex;
Mesh charQuad;
Mesh textQuad;
Shader shd;


void copyRect(unsigned char *src,int sleft,int stop, int sw, int sh, int sstride,unsigned char *dst,int dleft,int dtop, int dw,int dh, int dstride) {
   
    for (size_t i = 0; i < sh ; i++)
    {
        memcpy(dst + (i + dtop)*dstride + dleft, src + (i+stop)*sstride + sleft, sw);
    }
    
}


void reshape(int width, int height) {
    camera.setWindowSize(width, height);
}

void moveMouse(int x, int y) {
    camera.moveMouse(x, y);
}

void dragMouse(int x, int y) {
   //camera.dragMouse(x, y);
}

void keyFunc(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'w':
        camera.localMove(0, 0 , 1);
        break;
    case 's':
        camera.localMove(0, 0 , -1);
        break;
    case 'a':
        camera.localMove(-1, 0, 0);
        break;
    case 'd':
        camera.localMove(1, 0, 0);
        break;
    case ' ':
        shd.clear();
        shd.loadVertexFile("sdfFont_vert.glsl");
        shd.loadFragFile("sdfFont_frag.glsl");
        shd.link();
        break;

        
    }
}

void spkeyFunc(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_UP:
        rotateZ += 1;
        break;
    case GLUT_KEY_DOWN:
        rotateZ -= 1;
        break;
    case GLUT_KEY_LEFT:
        markDist += 0.001;
        break;
    case GLUT_KEY_RIGHT:
        markDist -= 0.001;
        break;
    }
}

void mouseWheel(int b, int d, int x, int y) {
    scale *= (1 + d*0.1f);
}

void initCamera() {
    
    glEnable(GL_DEPTH_CLAMP);

    camera.EnableOrtho = true;
    camera.ortho_autoRect = true;
    camera.init();
    camera.setLightPos(0, 0, 1, 0);
    camera.setPosition(0, 0, 1000);
    camera.setViewPort(0, 0, 500, 500);
}

void render() {
    
   

    camera.setViewPort();
    camera.clearBuffer();

    camera.loadProjection();
    camera.loadModelView();

    camera.drawBack();


    int width = TESTSIZE;
    shd.bind();
    shd.setUniform1i("sdfTex", 0);
    shd.setUniform1f("markDist", markDist);
    shd.setUniform2f("sdfSize", width, width);
   
    glScaled(scale, scale, scale);


    shd.setUniform4f("foreColor", 0, 1, 1, 1);
    shd.setUniform4f("backColor", 0, 0, 0, 1);
    shd.setUniform1i("stroke", 1);
    shd.setUniform1i("shadow", 1);
   /* shd.setUniform4f("foreColor", 1, 0, 0, 1);
    shd.setUniform4f("backColor", 1, 1, 1, 0);
    shd.setUniform1i("stroke", 0);
    shd.setUniform1i("shadow", 0);*/
    charQuad.setRotation(0, 0, rotateZ);
    glTranslatef(0, TESTHEIGHT, 0);
    textQuad.draw();

    glTranslatef(0, -TESTHEIGHT, 0);

    glTranslatef(-width*(TESTNUM - 1) / 2, 0, 0);
    for (size_t i = 0; i < TESTNUM; i++)
    {
        charQuad.texture0 = sdfTex[i];
        charQuad.draw();
        glTranslatef(width, 0, 0);
    }
    
    

   


    camera.countTimes();
    
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

unsigned char* loadImg(const char* file, int &cols, int &rows, int &chns, int deschns) {

    unsigned char* data = stbi_load(file, &cols, &rows, &chns, 1);

    int fcols = ((cols - 1) / 4 + 1) * 4, frows = ((rows - 1) / 4 + 1) * 4;
    if (fcols != cols || frows != rows) {
        unsigned char* fdata = (unsigned char*)malloc(fcols*frows);
        memset(fdata, 0, fcols*frows);
        int orow = (frows - rows) / 2, ocol = (fcols - cols) / 2;
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols; j++)
            {
                fdata[(i + orow)*fcols + (j + ocol)] = data[i*cols + j]+0.5;
            }
        }

        delete[] data;
        data = fdata;
        rows = frows, cols = fcols;
    }

    return data;
}

void initTexture() {
    //int cols, rows, chns;
    //unsigned char* data = loadImg("font.png",cols,rows,chns,1);
    //
    //fontTex.setTexImg(data, cols, rows, GL_R8, GL_RED);
    //unsigned char* outdata = (unsigned char*)malloc(cols*rows);
    // 

    ////stbi_write_png("fontsdf.png", cols, rows, 1, outdata, cols);
    

    for (size_t i = 0; i < TESTNUM; i++)
    {
        char fileName[20];
        sprintf_s(fileName, "fontsdf%d.png", i);
        float * data = NULL;

        msdfgen::GlyphInfo info;
        msdfgen::Vector2 basePt;

        msdfgen::fontSDF("C:\\Windows\\Fonts\\STHUPO.TTF", 0x0041+(i%2)*0x0020+i, fileName , TESTSIZE, TESTSIZE, info ,basePt , data);

        int row, col, chns;
        auto filedata = stbi_load(fileName, &col, &row, &chns, 1);

        sdfTex[i].setTexImg(filedata, TESTSIZE, TESTSIZE, GL_RED, GL_RED, GL_UNSIGNED_BYTE);

        int sleft = roundf(basePt.x + info.offset_x), stop = roundf(basePt.y - info.offset_y);
        int dleft = roundf(curx - basePt.x), dtop = roundf(cury - basePt.y);

        copyRect(filedata,sleft,stop, (int)roundf(info.width) , (int)roundf(info.height) , TESTSIZE , texData , dleft,dtop, TESTWIDTH, TESTHEIGHT, TESTWIDTH);
        curx += info.advance_x;
    }

    fontTex.setTexImg(texData, TESTWIDTH, TESTHEIGHT, GL_RED, GL_RED, GL_UNSIGNED_BYTE);

    //delete[] outdata;
    //delete[] data;
}

void sdfFontTest() {

    initTexture();

    //cube build
    Mesh::activeVAO = false;
    
    float halfw = TESTSIZE / 2, halfh = TESTSIZE / 2;
    charQuad.drawStyle = Quads;
    charQuad.addPoint(-halfw, halfh, 0, 0, 0);
    charQuad.addPoint(halfw, halfh, 0, 1, 0);
    charQuad.addPoint(halfw, -halfh, 0, 1, 1);
    charQuad.addPoint(-halfw, -halfh, 0, 0, 1);

    halfw = TESTWIDTH / 2, halfh = TESTHEIGHT / 2;
    textQuad.drawStyle = Quads;
    textQuad.addPoint(-halfw, halfh, 0, 0, 0);
    textQuad.addPoint(halfw, halfh, 0, 1, 0);
    textQuad.addPoint(halfw, -halfh, 0, 1, 1);
    textQuad.addPoint(-halfw, -halfh, 0, 0, 1);
    textQuad.texture0 = fontTex;


    shd.loadVertexFile("sdfFont_vert.glsl");
    shd.loadFragFile("sdfFont_frag.glsl");
    shd.link();
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