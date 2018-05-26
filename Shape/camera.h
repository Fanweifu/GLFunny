#ifndef CAMERA_H
#define CAMERA_H

#include"glm.hpp"
#include"gtc/matrix_transform.hpp"

#include"vaoarray.h"
#include"shape.h"
#include"light.h"



#define CAM_FORWARD 0
#define CAM_BACK 1
#define CAM_LEFT 2
#define CAM_RIGHT 3
#define CAM_UP 4
#define CAM_DOWN 5

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
        r = backColor.r;
        g = backColor.g;
        b = backColor.b;
        a = backColor.a;
    }
    void setBackColor(float r, float g, float b, float a) {
        backColor.a = r;
        backColor.g = g;
        backColor.b = b;
        backColor.a = a;
    }
    Light& getLight() { return mainlight; }

    void init();
    void lookAt(float tx, float ty, float tz, bool isup = true);
    void setOrtho(int w, int h) { owidth = w; oheight = h; updateProjection(); }
    void drawView();
    void dragMouse(int x, int y, float speed = 0.1f);
    void moveMouse(int x, int y);
    void Move(int cmd, float step = 0.1f);
protected:
    bool inited = false;
    bool windowsChanged = true;
    bool projectionChanged = true;
    float Near = 0.01;
    float Far = 10000;
    float Fov = 60;
    float Ratio = 1;

    int left = 0;
    int buttom = 0;
    int width = 300;
    int height = 300;
    float owidth = 300;
    float oheight = 300;

    int mouseX = 0;
    int mouseY = 0;

    glm::vec4 backColor = glm::vec4(0.5, 0.5, 1, 1);

    glm::mat4 matrixProjection;
    Light mainlight;

    void initGl();
    void ondraw();

    void updateProjection();
    void updateViewPort();
};

#endif // CAMERA_H
