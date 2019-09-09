#include<Shape\camera.h>
#include<Shape\mesh.h>
#include<Shape\layer.h>
#include<Shape\texture.h>
#include<GL\freeglut.h>

#include<Bitmap.h>
#include"pixaloop.h"


pixaloop app;

float step = 0.5;

void reshape(int width, int height) {
	//todo:

}

void moveMouse(int x, int y) {
}

void dragMouse(int x, int y) {
	app.procMouseDragEvent(x, y);
}

void keyFunc(unsigned char key, int x, int y) {
	app.procKeyEvent(key, Action::DOWN);
}

void spkeyFunc(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	}
}

void mouseEvent(int b, int d, int x, int y) {
	Button bt = static_cast<Button>(b);
	Action at = static_cast<Action>(d);
	app.procMouseClickEvent(bt, at, x, y);
}

void render() {
	glEnable(GL_BLEND);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(1, 0, 0, 1);
	int x, y;
	app.getWindowSize(x, y);
	glViewport(0, 0, x, y);
	glutReshapeWindow(x, y);

	app.render();
	

	glutSwapBuffers();
}

void initGlut() {
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("sdfFont");
	glutPassiveMotionFunc(moveMouse);
	glutMotionFunc(dragMouse);
	glutMouseFunc(mouseEvent);
	glutKeyboardFunc(keyFunc);
	glutSpecialFunc(spkeyFunc);
	glutDisplayFunc(render);
	glutIdleFunc(render);
	glutReshapeFunc(reshape);
}


void initGL() {

	glShadeModel(GL_SMOOTH);                        // shading method: GL_SMOOTH or GL_FLAT
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);          // 4-byte pixel alignment
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);

	glPointSize(5.0);
	glLineWidth(3.0);
}



int main(int arg, char**argv) {

	if (!app.loadSourceImg("nazha.jpg")) return 0;

	glutInit(&arg, argv);
	initGlut();
	glewInit();
	initGL();

	app.prepare();
	int x, y;
	app.getWindowSize(x, y);
	glutReshapeWindow(x, y);


	glutMainLoop();
	return 0;
}