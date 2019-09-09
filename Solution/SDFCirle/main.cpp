#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<Shape\layer.h>
#include<Shape\texture.h>
#include<Shape\perlin.h>
#include<GL\freeglut.h>
#include<stb\stb_image.h>
#include<stb\stb_image_write.h>
float step = 0.1;
int smooth = 2;
float alpha = 0;
Camera camera;
Mesh sdfshp;
Mesh board;
Shader sdfshd;
Shader norshd;
Texture2D tex;
Perlin randX(2,0);
Perlin randY(2,1);
Perlin randZ(2,200);
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
        sdfshd.clear();
        sdfshd.loadVertexFile("sdf_vert.glsl");
        sdfshd.loadFragFile("sdf_frag.glsl");
        sdfshd.link();
        break;
	case 'f':
		alpha += 0.01;
		camera.setLightPos(cos(alpha), sin(alpha), 0.7);
		break;
	case '\r':
		int right = camera.getViewLeft() + camera.ViewWidth();
		int bottom = camera.getViewButtom() + camera.ViewHeight();
		GLubyte* data = new GLubyte[right*bottom * 4];
		glReadPixels(0, 0, right, bottom, GL_RGBA, GL_UNSIGNED_BYTE,data);
		stbi_write_bmp("frame buffer.bmp", right, bottom, 4, data);
		break;

    }
}

void mouseEvent(int b, int d, int x, int y) {
    camera.localMove(0, 0, d);
}


void initCamera() {
    camera.init();
    camera.setPosition(0, 0, 2);
    camera.setViewPort(0, 0, 400, 400);
}

void render() {
 
    camera.beginFrame();
	/*board.setScale(1.0);
	board.draw();
	board.setScale(2.0);
	board.draw();*/
	if (0) {
		sdfshd.bind();
		sdfshd.setUniform2f("viewport", camera.ViewWidth(), camera.ViewHeight());
		sdfshd.setUniform1f("time", sin(10 * camera.getRenderTimes()));
		//float w = 16;
		//float v = camera.getRenderTimes() * w;
		//float y = randX.fBm(v)*2;
		//
		//float x = randY.fBm(v)*2;
		////float z = randZ.fBm(v);




		sdfshp.setRotation(0, 0, 45);

		sdfshp.draw();
	}

	if (1) {
		tex.bind();
		norshd.bind();
		norshd.setUniform1i("tex", tex.ObjectID());
		board.draw();
		norshd.unBind();
		tex.unBind();
	}
	


	camera.doCount();

    glutSwapBuffers();
}

void initGlut() {
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);

    glutCreateWindow("sdfCirle");

    glutPassiveMotionFunc(moveMouse);
    glutMotionFunc(dragMouse);
    glutKeyboardFunc(keyFunc);
    glutMouseWheelFunc(mouseEvent);
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
}

void buildImageMesh(const char* file) {
	int w, h, ch = 0;
	auto data = stbi_load(file, &w, &h, &ch, 4);
	
	for (int i = 0; i < h-1; i++)
	{
		for (int j = 0; j < w-1; j++)
		{
#define addPixel(x,y)  board.addPoint(x - w*0.5, y - h*0.5, (*(data+(w*4*(y))+(x)*4)-127)/10,float(x)/w,float(y)/h);
			addPixel(j, i);
			addPixel(j+1, i);
			addPixel(j+1, i+1);
			addPixel(j, i+1);
		}
	}
	board.drawStyle = DrawType::Quads;
	tex.setTexImg(data, w, h);
}

void sdfFontTest() {
    //cube build
    Mesh::activeVAO = false;
    Mesh::buildQuad(sdfshp);
	//Mesh::buildQuad(board);
	
	//board.drawStyle = LineStrip;
    //sdfshd.loadVertexFile("sdf_vert.glsl");
    //sdfshd.loadFragFile("sdf_frag.glsl");

	

    //sdfshd.link();
	norshd.loadVertexFile("sdf_vert.glsl");
	norshd.loadFragFile("map_frag.glsl");
	norshd.link();

	buildImageMesh("C:\\Users\\1\\Downloads\\msdfgen-master\\sdf.png");
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