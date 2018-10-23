#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<Shape\layer.h>
#include<Shape\texture.h>
#include<sdf.h>
#include<GL\freeglut.h>

float step = 0.1;
int smooth = 2;
float smoothDelta = 0.002;
float fillDist = 0.005;
Camera camera;
Texture2D sdfTex;
Texture2D fontTex;
Mesh sdfshp;
Mesh texshp;
Shader shd;

unsigned char* imgdata = NULL;
int fontRows, fontCols, fontChns;
int imgWidth = 512;
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
    case ' ':
        shd.clear();
        shd.loadVertexFile("sdfFont_vert.glsl");
        shd.loadFragFile("sdfFont_frag.glsl");
        shd.link();
        break;
    }
}

void swiftTexture(bool up) {
    /*   imgWidth = imgWidth*(up ? 2 : 0.5);
       if (imgWidth > 512) imgWidth = 32;
       else if (imgWidth < 32) imgWidth = 512;

       char imgPath[30];
       sprintf_s(imgPath, "bigcharSDF%dX%d.png", imgWidth, imgWidth);

       if (sdfTex.loadFileImg(imgPath)) {
           printf("%s loaded!\n",imgPath);
       }*/
   
}

void spkeyFunc(int key, int x, int y) {
    switch (key)
    {
    case GLUT_KEY_UP:
        smoothDelta += 0.001;
        break;
    case GLUT_KEY_DOWN:
        smoothDelta -= 0.001;
        if (smoothDelta < 0) smoothDelta = 0;
        break;
    case GLUT_KEY_LEFT:
        fillDist += 0.001;
        break;
    case GLUT_KEY_RIGHT:
        fillDist -= 0.001;
        if (fillDist < 0) fillDist = 0;
        break;
    }
}

void mouseWheel(int b, int d, int x, int y) {
    bool up = d > 0;
    swiftTexture(up);
}

void initCamera() {
    camera.init();
    camera.setPosition(0, 0, 5);
    camera.setViewPort(0, 0, 500, 500);
}

void render() {
    camera.beginRender();

    shd.bind();
    shd.setUniform1i("sdfTex", 0);
    float tsx = sdfTex.Width() / (float)camera.ViewWidth(), tsy = sdfTex.Height() / (float)camera.ViewHeight();
    shd.setUniform2f("tscl", tsx, tsy);
    shd.setUniform1f("smoothDelta", smoothDelta);
    shd.setUniform1f("markDist", fillDist);
    shd.setUniform2f("sdfSize", sdfTex.Width(), sdfTex.Height());
   

    sdfshp.draw();


    shd.unBind();

    texshp.draw();


    camera.endRender();


    //glDrawPixels(fontCols, fontRows, GL_RGBA, GL_UNSIGNED_BYTE, imgdata);

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
    int cols, rows, chns;
    unsigned char* data = stbi_load("b.png",&cols,&rows,&chns,1);
    
    int fcols = ((cols - 1) / 4 + 1) * 4, frows = ((rows - 1) / 4 + 1) * 4;
    if (fcols != cols || frows != rows) {
        unsigned char* fdata = (unsigned char*)malloc(fcols*frows);
        memset(fdata, 0, fcols*frows);
        int orow = (frows - rows) / 2, ocol = (fcols - cols) / 2;
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols; j++)
            {
                fdata[(i + orow)*fcols + (j + ocol)] = data[i*cols + j];
            }
        }

        delete[] data;
        data = fdata;
        rows = frows, cols = fcols;
    }


    fontTex.setTexImg(data, cols, rows, GL_R8, GL_RED);




    unsigned char* outdata = (unsigned char*)malloc(cols*rows);
    sdfBuild(outdata, cols, 8 , data, cols, rows, cols);
    //int minrows = rows / 4, mincols = cols / 4, stride = ((mincols - 1) / 4 + 1) * 4;
    //unsigned char* resizedata = (unsigned char*)malloc(stride* minrows);
    //stbir_resize_uint8(outdata, cols, rows, cols, resizedata, mincols, minrows, stride,1);


    //stbi_write_png("fontsdf.png", mincols, minrows, 1, resizedata, stride);
    stbi_write_png("fontsdf.png", cols, rows, 1, outdata, cols);
    sdfTex.setTexImg(outdata, cols, rows, GL_R8 , GL_RED);

    delete[] outdata;
    delete[] data;
    //sdfTex.loadFileImg("fontsdf.png");

    
}

void sdfFontTest() {
    //cube build
    Mesh::activeVAO = false;
   
    initTexture();
    camera.setOrthoH()
    //imgdata = stbi_load("linesFont.png",&fontCols,&fontRows,&fontChns,4);

    //sdfTex.loadFileImg("bigcharSDF32X32.png");
    sdfshp.texture0 = sdfTex;
    texshp.texture0 = fontTex;
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