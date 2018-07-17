#ifndef CAMERA_H
#define CAMERA_H

#include"vaoarray.h"
#include"shape.h"
#include"light.h"
#include"camera.h"



class Camera : public Shape
{
public:
    Camera();
    bool isOrtho = false;
    bool isMultiScreen = true;
    bool isShaderBack = true;
    bool isRenderShadow = true;
    Shape* Scene = NULL;

    void setNear(float _near) {
        if (_near<0 || _near>Far) return;
        Near = _near;
        updateProjection();
    }
    float getNear() { return Near; }

    void setFar(float _far) {
        if (_far < Near) return;
        Far = _far;
        updateProjection();
    }
    float getFar() { return Far; }

    void setFov(float _fov) {
        if (!_fov > 0 || _fov > 360) return;
        Fov = _fov;
        updateProjection();
    }
    float getFov() { return Fov; }

    int getViewLeft() { return left; }
    int getViewButtom() { return buttom; }
    int getViewWidth() { return width; }
    int getViewHeight() { return height; }

    double getRenderTimes(float k = 0.0001) {
        return (double)renderTime*k;
    }
    void setViewPort(int x, int y, int width, int height);
    void setWindowSize(int width, int height);

    void setBackColor(float r, float g, float b, float a) {
        backColor.a = r;
        backColor.g = g;
        backColor.b = b;
        backColor.a = a;
    }
    void getBackColor(float &r, float &g, float &b, float &a) {
        r = backColor.r;
        g = backColor.g;
        b = backColor.b;
        a = backColor.a;
    }
    const float* getBackColor4f() { return glm::value_ptr(backColor); }

    void setDirection(float vx, float vy, float vz);
    void getDirection(float& vx, float& vy, float &vz) {
        vx = forwardV.x;
        vy = forwardV.y;
        vz = forwardV.z;
    }
    const float* getDirectionf3() { return glm::value_ptr(forwardV); }

    Light& getLight() { return mainlight; }

    void init();

    void lookAt(float ex, float ey, float ez, float tx, float ty, float tz);

    void setOrthoH(int h) { owidth = Ratio*h; oheight = h; updateProjection(); }
    void drawView();
    void dragMouse(int x, int y, float speed = 0.1f);
    void moveMouse(int x, int y);
    void mouseCoordToUV(int mx, int my, float &u, float &v);
    void mouseCoordToDir(int mx, int my, float& x, float &y, float &z);
    void localMove(float right, float forward, float up);
    void setCameraInShader(Shader& shd);

    const float* getProjectionMatPtr() { return  glm::value_ptr(matrixProjection); }
    const float* getProjectionMatInvPtr() { return glm::value_ptr(matrixProjectionInv); }
    const float* getLightPosf4() { return mainlight.getPositonf4(); }
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

    long renderTime = 0;

    glm::vec4 backColor = glm::vec4(0, 1, 1, 1);
    glm::vec3 uplook = glm::vec3(0, 0, 1);
    glm::vec3 forwardV = glm::vec3(0, 1, 0);
    glm::vec3 rightV = glm::vec3(1, 0, 0);
    glm::vec3 upV = glm::vec3(0, 0, 1);
    glm::vec3 target = glm::vec3(0, 1, 0);
    glm::vec3 mouseTarget = glm::vec3(0, 1, 0);

    glm::mat4 matrixProjection;
    glm::mat4 matrixProjectionInv;
    Light mainlight;
    Shader backshd;
    DepthTexture depthMap;
    void initGl();
    void initBack();
    void ondraw();
    void drawBack();
    void updateModel();
    void updateProjection();
    void updateViewPort();
    void setDirectionVec3(glm::vec3 v);
};




#endif // CAMERA_H
