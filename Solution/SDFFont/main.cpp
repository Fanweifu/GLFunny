#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<Shape\layer.h>
#include<Shape\texture.h>
#include<GL\freeglut.h>
#include"Ext\import-font.h"
#include"freetype\sdf.h"
//#include"sdfExample.h"
#define TESTNUM 20
#define TESTSIZE 40
#define TESTWIDTH (TESTSIZE*20)
#define TESTHEIGHT TESTSIZE
#define TESTPIEXLS (TESTHEIGHT*TESTWIDTH)
unsigned char sdfAllData[TESTPIEXLS] = { 0 };
float sdfcurleft = 0, sdfcurtop = 0;
unsigned char texAllData[TESTPIEXLS] = { 0 };
float fontcurleft = 0, fontcurtop = 0;

bool sdfvisible = true;
float rotateZ = 0;
float scale = 1;
float antiFactor = 0.6;

Camera camera;
Texture2D fontTex;
Texture2D sdfTex;
Texture2D backTex;
Mesh sdfQuad;
Mesh textQuad;
Shader sdfshd;
Shader baseshd;

//���� �� ������

void copyRect(unsigned char *src, int sleft, int stop, int recW, int recH, int sstride, unsigned char *dst, int dleft, int dtop, int dstride, int dh) {
    for (size_t i = 0; i < recH; i++)
    {
        memcpy(dst + (i + dtop)*dstride + dleft, src + (i + stop)*sstride + sleft, recW);
    }
}

void copyImg(unsigned char *src, int sw, int sh, unsigned char *dst, int dleft, int dtop, int dw, int dh) {
    for (size_t i = 0; i < sh; i++)
    {
        memcpy(dst + (i + dtop)*dw + dleft, src + i *sw, sw);
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
		scale *= 1.1f;
		printf("scale=%f\n\n", scale);
        break;
    case 's':
		scale *= 0.9f;
		printf("scale=%f\n", scale);
        break;
    case 'a':
		rotateZ += 1;
		printf("rotateZ=%f\n", rotateZ);
        break;
    case 'd':
		rotateZ -= 1;
		printf("rotateZ=%f\n", rotateZ);
        break;
    case ' ':
        /*  shd.clear();
          shd.loadVertexFile("sdfFont_vert.glsl");
          shd.loadFragFile("sdfFont_frag.glsl");
          shd.link();*/
        sdfvisible = !sdfvisible;
		printf("sdfvisible=%d\n", sdfvisible);

		break;
	case '\r':
		scale = 1;
        break;
	case '1':
		sdfshd.clear();
		sdfshd.loadVertexFile("sdfFont_vert.glsl");
		sdfshd.loadFragFile("sdfFont_frag.glsl");
		sdfshd.link();
		break;
	case '2':
		sdfshd.clear();
		sdfshd.loadVertexFile("sdfFont_vert.glsl");
		sdfshd.loadFragFile("puresdfFont_frag.glsl");
		sdfshd.link();
		break;
	
    }
}

void spkeyFunc(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_UP:
		antiFactor += 0.01;
		printf("antiFactor=%f\n", antiFactor);
        break;
    case GLUT_KEY_DOWN:
		antiFactor -= 0.01;
		printf("antiFactor=%f\n", antiFactor);

        break;
    case GLUT_KEY_LEFT:
       
        break;
    case GLUT_KEY_RIGHT:
      
        break;
    }
}

void mouseEvent(int b, int d, int x, int y) {
    scale *= (1 + d*0.1f);
}

void initCamera() {
    glEnable(GL_DEPTH_CLAMP);
	camera.setBackColor(0, 0, 0, 1);
    camera.enableOrtho = true;
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

    //camera.drawBack();

    glScaled(scale, scale, scale);

    glRotatef(rotateZ, 0, 0, 1);

    if (sdfvisible) {
        sdfshd.bind();
        sdfshd.setUniform1i("sdfTex", 0);  
		sdfshd.setUniform1f("antiFactor", antiFactor);
      
        /*sdfshd.setUniform4f("foreColor", 1, 1, 1, 1);
        sdfshd.setUniform4f("backColor", 0, 0, 0, 1);
        sdfshd.setUniform1i("stroke", 0);
        sdfshd.setUniform1i("shadow", 0);*/

        //glTranslatef(0, TESTHEIGHT/2, 0);
        sdfQuad.texture0 = sdfTex.ObjectID();
        sdfQuad.draw();
        sdfshd.unBind();
    }

    //glTranslatef(0, -TESTHEIGHT, 0);

   
	
	baseshd.bind();
	baseshd.setUniform1i("baseTex", 0);
	sdfQuad.texture0 = fontTex.ObjectID();
	sdfQuad.draw();
	baseshd.unBind();
    /*  glTranslatef(-width*(TESTNUM - 1) / 2, 0, 0);
      for (size_t i = 0; i < TESTNUM; i++)
      {
          charQuad.texture0 = sdfTex[i];
          charQuad.draw();
          glTranslatef(width, 0, 0);
      }*/

	camera.doCount();

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
    glutMouseWheelFunc(mouseEvent);
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
}

void initTextureGenSDF() {
    const wchar_t* test = L"VideoСӰ��Ƶ���礦����";
    int length = wcslen(test);
    for (size_t i = 0; i < length; i++)
    {
        char sdfFileName[30];
        sprintf_s(sdfFileName, "fontsdf%d\n.png", i);
        char fontFileName[30];
        sprintf_s(fontFileName, "font%d\n.bmp", i);
        float * data = NULL;

        msdfgen::GlyphInfo info;
        msdfgen::Vector2 basePt;
        msdfgen::Vector2 lefttop;
        int width = 0;
        //msdfgen::fontSDF("C:\\Windows\\Fonts\\msyh.ttc", test[i], sdfFileName, fontFileName, TESTSIZE, width, info, basePt, lefttop);

        int row, col, chns;
        //auto sdfdata = stbi_load(sdfFileName, &col, &row, &chns, 1);

        //int sleft = roundf(basePt.x + info.offset_x), stop = roundf(basePt.y - info.offset_y);
        //int dleft = roundf(sdfcurleft - basePt.x), dtop = roundf(sdfcurtop - basePt.y);
        //copyImg(sdfdata, col, row, sdfAllData, sdfcurleft, 0, TESTWIDTH, TESTHEIGHT);
        //sdfcurleft += col;

        int fontwidth, fontheight, fontchns;
        auto fontfile = stbi_load(fontFileName, &fontwidth, &fontheight, &fontchns, 1);
        int padding = 4, exwidth = fontwidth + 2 * padding, exheight = fontheight + 2 * padding;
        exwidth = (((exwidth - 1) >> 2) + 1) << 2;
        int exsize = exwidth*exheight;
        unsigned char* fontImgctx = (unsigned char*)malloc(exsize);
        memset(fontImgctx, 0, exsize);
        copyRect(fontfile, 0, 0, fontwidth, fontheight, fontwidth, fontImgctx, padding, padding, exwidth, exheight);

        copyImg(fontfile, fontwidth, fontheight, texAllData, fontcurleft, 0, TESTWIDTH, TESTHEIGHT);
        fontcurleft += fontwidth;

        unsigned char* sdftex = (unsigned char*)malloc(exsize);
        memset(sdftex, 0, exsize);
        sdfBuild(sdftex, exwidth, 8, fontImgctx, exwidth, exheight, exwidth);
        //sdfCoverageToDistance(sdftex, exwidth, fontImgctx, exwidth, exheight, exwidth);
        copyRect(sdftex, padding, padding, fontwidth, fontheight, exwidth, sdfAllData, sdfcurleft, 0, TESTWIDTH, TESTHEIGHT);
        sdfcurleft += fontwidth;
        //copyRect(filedata ,sleft,stop, (int)roundf(info.width) , (int)roundf(info.height) , col , texData , dleft,dtop, TESTWIDTH, TESTHEIGHT, TESTWIDTH);
        //curx += info.advance_x;

        //delete[] sdftex;
        delete[] fontfile;
    }
    //sdfBuild(sdfAllData, TESTWIDTH, TESTSIZE, texAllData, TESTWIDTH, TESTHEIGHT, TESTWIDTH);
    fontTex.setTexImg(texAllData, TESTWIDTH, TESTHEIGHT, GL_RED, GL_RED, GL_UNSIGNED_BYTE);
    sdfTex.setTexImg(sdfAllData, TESTWIDTH, TESTHEIGHT, GL_RED, GL_RED, GL_UNSIGNED_BYTE);
    //stbi_write_bmp("GenSDF.bmp", TESTWIDTH, TESTHEIGHT, 1, sdfAllData);
    //fontTex.loadFileImg("GenSDF.bmp");
}

void initTextFileTest() {
	backTex.buildByColor(0, 0, 0, 1);
	fontTex.loadBin("C:/Users/482/Desktop/SDFVIdeoTest/Font0.015/fonttexture_78x12.rgb32", 78, 12, 4);
	sdfTex.loadImg("C:/Users/482/Desktop/SDFVIdeoTest/Font0.03/sdfrender_154x24.bmp");
}

void sdfFontTest() {
	initTextFileTest();

    //cube build
    Mesh::activeVAO = false;
	
    float halfw = fontTex.Width() / 2, halfh = fontTex.Height() / 2;
    sdfQuad.drawStyle = DrawType::Quads;
    sdfQuad.addPoint(-halfw, halfh, 0, 0, 0);
    sdfQuad.addPoint(halfw, halfh, 0, 1, 0);
    sdfQuad.addPoint(halfw, -halfh, 0, 1, 1);
    sdfQuad.addPoint(-halfw, -halfh, 0, 0, 1);

    sdfshd.loadVertexFile("sdfFont_vert.glsl");
    sdfshd.loadFragFile("sdfFont_frag.glsl");
    sdfshd.link();

	baseshd.loadFragFile("fonttext_frag.glsl");
	baseshd.link();
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