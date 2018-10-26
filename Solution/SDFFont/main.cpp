#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<Shape\layer.h>
#include<Shape\texture.h>
#include<GL\freeglut.h>
#include"Ext\import-font.h"
#include"freetype\sdf.h"
//#include"sdfExample.h"
#define TESTNUM 20
#define TESTSIZE 32
#define TESTWIDTH (TESTSIZE*20)
#define TESTHEIGHT TESTSIZE
#define TESTPIEXLS (TESTHEIGHT*TESTWIDTH)
unsigned char sdfAllData[TESTPIEXLS] = { 0.0f };
float sdfcurleft = 0, sdfcurtop = 0;
unsigned char texAllData[TESTPIEXLS] = { 0.0f };
float fontcurleft = 0, fontcurtop = 0;

float markDist = 0;
float rotateZ = 0;
float scale = 1;
Camera camera;
Texture2D sdfTex;
Texture2D fontTex;
Mesh sdfQuad;
Mesh textQuad;
Shader shd;


void copyRect(unsigned char *src,int sleft,int stop, int sw, int sh, int sstride,unsigned char *dst,int dleft,int dtop, int dw,int dh, int dstride) {
   
    for (size_t i = 0; i < sh ; i++)
    {
        memcpy(dst + (i + dtop)*dstride + dleft, src + (i+stop)*sstride + sleft, sw);
    }
    
}

void copyImg(unsigned char *src, int sw, int sh, unsigned char *dst, int dleft, int dtop, int dw, int dh) {

    for (size_t i = 0; i < sh; i++)
    {
        memcpy(dst + (i + dtop)*dw + dleft, src + i *sw , sw);
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
        markDist += 0.01;
        break;
    case GLUT_KEY_RIGHT:
        markDist -= 0.01;
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



    glScaled(scale, scale, scale);

    glRotatef(rotateZ, 0, 0, 1);

    shd.bind();
    shd.setUniform1i("sdfTex", 0);
    shd.setUniform1f("markDist", markDist);
    shd.setUniform2f("sdfSize",TESTWIDTH , TESTHEIGHT);
 

    
    shd.setUniform4f("foreColor", 0, 1, 1, 1);
    shd.setUniform4f("backColor", 1, 1, 1, 0);
    shd.setUniform1i("stroke", 1);
    shd.setUniform1i("shadow", 1);
  

    glTranslatef(0, TESTHEIGHT/2, 0);
    sdfQuad.texture0 = sdfTex;
    sdfQuad.draw();
    shd.unBind();


    glTranslatef(0, -TESTHEIGHT, 0);
    sdfQuad.texture0 = fontTex;
    sdfQuad.draw();
  /*  glTranslatef(-width*(TESTNUM - 1) / 2, 0, 0);
    for (size_t i = 0; i < TESTNUM; i++)
    {
        charQuad.texture0 = sdfTex[i];
        charQuad.draw();
        glTranslatef(width, 0, 0);
    }*/
    
    

   


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



void initTexture() {
    //int cols, rows, chns;
    //unsigned char* data = loadImg("font.png",cols,rows,chns,1);
    //
    //fontTex.setTexImg(data, cols, rows, GL_R8, GL_RED);
    //unsigned char* outdata = (unsigned char*)malloc(cols*rows);
    // 

    ////stbi_write_png("fontsdf.png", cols, rows, 1, outdata, cols);
    
    const wchar_t* test = L"VideoÐ¡Ó°ÊÓÆµ¤·¤ç¤¦¤¨¤¤";
    int length = wcslen(test);
    for (size_t i = 0; i < length; i++)
    {
        char sdfFileName[30];
        sprintf_s(sdfFileName, "fontsdf%d.png", i);
        char fontFileName[30];
        sprintf_s(fontFileName, "font%d.bmp", i);
        float * data = NULL;

        msdfgen::GlyphInfo info;
        msdfgen::Vector2 basePt;
        msdfgen::Vector2 lefttop;
        int width = 0;
        msdfgen::fontSDF("C:\\Windows\\Fonts\\msyh.ttc", test[i] , sdfFileName , fontFileName, TESTSIZE , width , info ,basePt , lefttop);

        int row, col, chns;
        auto sdfdata = stbi_load(sdfFileName, &col, &row, &chns, 1);

        int sleft = roundf(basePt.x + info.offset_x), stop = roundf(basePt.y - info.offset_y);
        int dleft = roundf(sdfcurleft - basePt.x), dtop = roundf(sdfcurtop - basePt.y);
        copyImg(sdfdata, col, row, sdfAllData, sdfcurleft, 0, TESTWIDTH, TESTHEIGHT);
        sdfcurleft += col;

        int fontwidth, fontheight, fontchns;
        auto fontfile = stbi_load(fontFileName, &fontwidth, &fontheight, &fontchns, 1);

        copyImg(fontfile, fontwidth, fontheight, texAllData, fontcurleft + (int)roundf(lefttop.x), fontcurtop + (int)roundf(lefttop.y), TESTWIDTH, TESTHEIGHT);
        fontcurleft += col;
        //copyRect(filedata ,sleft,stop, (int)roundf(info.width) , (int)roundf(info.height) , col , texData , dleft,dtop, TESTWIDTH, TESTHEIGHT, TESTWIDTH);
        //curx += info.advance_x;
        
        delete[] sdfdata;
        delete[] fontfile;
    }
    //sdfBuild(sdfAllData, TESTWIDTH, TESTSIZE, texAllData, TESTWIDTH, TESTHEIGHT, TESTWIDTH);
    fontTex.setTexImg(texAllData, TESTWIDTH, TESTHEIGHT, GL_RED, GL_RED, GL_UNSIGNED_BYTE);
    sdfTex.setTexImg(sdfAllData, TESTWIDTH, TESTHEIGHT, GL_RED, GL_RED, GL_UNSIGNED_BYTE);
    stbi_write_bmp("GenSDF.bmp", TESTWIDTH, TESTHEIGHT, 1, sdfAllData);
}

void sdfFontTest() {

    initTexture();

    //cube build
    Mesh::activeVAO = false;
    
    float halfw = TESTWIDTH / 2, halfh = TESTHEIGHT / 2;
    sdfQuad.drawStyle = Quads;
    sdfQuad.addPoint(-halfw, halfh, 0, 0, 0);
    sdfQuad.addPoint(halfw, halfh, 0, 1, 0);
    sdfQuad.addPoint(halfw, -halfh, 0, 1, 1);
    sdfQuad.addPoint(-halfw, -halfh, 0, 0, 1);
    //sdfQuad.texture0 = sdfTex;

    /*textQuad.drawStyle = Quads;
    textQuad.addPoint(-halfw, halfh, 0, 0, 0);
    textQuad.addPoint(halfw, halfh, 0, 1, 0);
    textQuad.addPoint(halfw, -halfh, 0, 1, 1);
    textQuad.addPoint(-halfw, -halfh, 0, 0, 1);*/
    //textQuad.texture0 = fontTex;


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