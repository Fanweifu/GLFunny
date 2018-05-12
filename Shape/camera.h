#ifndef CAMERA_H
#define CAMERA_H

#include"..\Math\matrices.h"
#include"vaoarray.h"
#include"shape.h"
#include"light.h"

#define DEG2RAD 3.141593f / 180
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

class Camera : public Shape
{
public:
    Camera();
    bool isOrtho = false;
    bool isMultiScreen = true;
    Shape* Scene = NULL;

    float getNear() { return Near; }
    float getFar() { return Far; }
    float getFov() { return Fov; }
    void setNear(float _near) {
        if (_near<0 || _near>Far) return;
        Near = _near;
        updateProjection();
    }
    void setFar(float _far) {
        if (_far < Near) return;
        Far = _far;
        updateProjection();
    }
    void setFov(float _fov) {
        if (!_fov > 0 || _fov > 360) return;
        Fov = _fov;
        updateProjection();
    }

    int getViewX() { return left; }
    int getViewY() { return buttom; }
    int getViewWidth() { return width; }
    int getViewHeight() { return height; }

    void setViewPort(int x, int y, int width, int height);
    void setWindowSize(int width, int height);

    void getBackColor(float &r, float &g, float &b, float &a) {
        r = backgroud[0];
        g = backgroud[1];
        b = backgroud[2];
        a = backgroud[3];
    }
    void setBackColor(float r, float g, float b, float a) {
        backgroud[0] = r;
        backgroud[1] = g;
        backgroud[2] = b;
        backgroud[3] = a;
    }
    Light& getLight() { return mainlight; }

    void init();
    void lookAt(float tx, float ty, float tz, bool isup = true);
    void drawView();
    void dragMouse(int x, int y, float speed = 0.1f);
    void moveMouse(int x, int y);
    void keyMove(char keyCmd, float step=0.1f);
protected:
    bool inited = false;

    bool windowsChanged = true;

    float Near = 0.1;
    float Far = 1000;
    float Fov = 60;
    float Ratio = 1;

    int left = 0;
    int buttom = 0;
    int width = 300;
    int height = 300;

    int mouseX = 0;
    int mouseY = 0;

    float backgroud[4] = { 0.5,0.5,1,1 };

    Matrix4 matrixProjection;
    Light mainlight;

    Matrix4 setFrustum(float left, float right, float buttom, float top, float n, float f);
    Matrix4 setFrustum(float fovy, float ratio, float n, float f);
    Matrix4 setOrthoFrustum(float l, float r, float b, float t, float n, float f);

    void initGl();
    void ondraw();

    void updateProjection() { matrixProjection = setFrustum(Fov, Ratio, Near, Far); }
    void updateViewPort();
};

#endif // CAMERA_H
