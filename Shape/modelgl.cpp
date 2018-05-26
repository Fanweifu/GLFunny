///////////////////////////////////////////////////////////////////////////////
// ModelGL.cpp
// ===========
// Model component of OpenGL
// All OpenGL calls should be here.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2008-09-15
// UPDATED: 2017-10-24
///////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>
#include "modelgl.h"

// constants
const float DEG2RAD = 3.141593f / 180;
const float FOV_Y = 60.0f;              // vertical FOV in degree
const float NEAR_PLANE = 1.0f;
const float FAR_PLANE = 100.0f;
const float CAMERA_ANGLE_X = 45.0f;     // pitch in degree
const float CAMERA_ANGLE_Y = -45.0f;    // heading in degree
const float CAMERA_DISTANCE = 25.0f;    // camera distance

///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ModelGL::ModelGL() : windowWidth(0), windowHeight(0),
drawModeChanged(false), drawMode(0),
cameraAngleX(CAMERA_ANGLE_X), cameraAngleY(CAMERA_ANGLE_Y),
cameraDistance(CAMERA_DISTANCE), windowSizeChanged(false)
{
    cameraPosition[0] = cameraPosition[1] = cameraPosition[2] = 0;
    cameraAngle[0] = cameraAngle[1] = cameraAngle[2] = 0;
    modelPosition[0] = modelPosition[1] = modelPosition[2] = 0;
    modelAngle[0] = modelAngle[1] = modelAngle[2] = 0;
    bgColor[0] = bgColor[1] = bgColor[2] = bgColor[3] = 0;

    matrixView = glm::mat4();
    matrixModel = glm::mat4();
    matrixModelView = glm::mat4();
    matrixProjection = glm::mat4();
}

///////////////////////////////////////////////////////////////////////////////
// destructor
///////////////////////////////////////////////////////////////////////////////
ModelGL::~ModelGL()
{
}

///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL states and scene
///////////////////////////////////////////////////////////////////////////////
void ModelGL::init()
{
    glShadeModel(GL_SMOOTH);                        // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);          // 4-byte pixel alignment

    // enable/disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_SCISSOR_TEST);

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);   // background color
    glClearStencil(0);                              // clear stencil buffer
    glClearDepth(1.0f);                             // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();
}

///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void ModelGL::initLights()
{
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = { 1.0f, 1.0f, 1.0f, 1.0f };      // ambient light
    GLfloat lightKd[] = { .9f, .9f, .9f, 1.0f };      // diffuse light
    GLfloat lightKs[] = { 1, 1, 1, 1 };               // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light in eye space
    float lightPos[4] = { 0, -1, -1, 0 };               // directional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                            // MUST enable each light source after configuration
}

///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
    //float forward[4];
    //float up[4];
    //float left[4];
    //float position[4];
    //float invLength;

    //// determine forward vector (direction reversed because it is camera)
    //forward[0] = posX - targetX;    // x
    //forward[1] = posY - targetY;    // y
    //forward[2] = posZ - targetZ;    // z
    //forward[3] = 0.0f;              // w
    //// normalize it without w-component
    //invLength = 1.0f / sqrtf(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);
    //forward[0] *= invLength;
    //forward[1] *= invLength;
    //forward[2] *= invLength;

    //// assume up direction is straight up
    //up[0] = 0.0f;   // x
    //up[1] = 1.0f;   // y
    //up[2] = 0.0f;   // z
    //up[3] = 0.0f;   // w

    //// compute left vector with cross product
    //left[0] = up[1] * forward[2] - up[2] * forward[1];  // x
    //left[1] = up[2] * forward[0] - up[0] * forward[2];  // y
    //left[2] = up[0] * forward[1] - up[1] * forward[0];  // z
    //left[3] = 1.0f;                                 // w

    //// re-compute orthogonal up vector
    //up[0] = forward[1] * left[2] - forward[2] * left[1];    // x
    //up[1] = forward[2] * left[0] - forward[0] * left[2];    // y
    //up[2] = forward[0] * left[1] - forward[1] * left[0];    // z
    //up[3] = 0.0f;                                       // w

    //// camera position
    //position[0] = -posX;
    //position[1] = -posY;
    //position[2] = -posZ;
    //position[3] = 1.0f;

    //// copy axis vectors to matrix
    //matrixView.identity();


    //matrixView.setColumn(0, left);
    //matrixView.setColumn(1, up);
    //matrixView.setColumn(2, forward);
    //matrixView.setColumn(3, position);

    matrixView = glm::lookAt(glm::vec3(posX, posY, posZ), glm::vec3(targetX, targetY, targetZ), glm::vec3(0, 0, 1));
}

///////////////////////////////////////////////////////////////////////////////
// set rendering window size
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setWindowSize(int width, int height)
{
    // assign the width/height of viewport
    windowWidth = width;
    windowHeight = height;
    windowSizeChanged = true;
}

///////////////////////////////////////////////////////////////////////////////
// configure projection and viewport
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setViewport(int x, int y, int w, int h)
{
    // set viewport to be the entire window
    glViewport((GLsizei)x, (GLsizei)y, (GLsizei)w, (GLsizei)h);

    // set perspective viewing frustum
    matrixProjection = setFrustum(FOV_Y, (float)(w) / h, NEAR_PLANE, FAR_PLANE); // FOV, AspectRatio, NearClip, FarClip

    // copy projection matrix to OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&matrixProjection[0][0]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
// configure projection and viewport of sub window
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setViewportSub(int x, int y, int width, int height, float nearPlane, float farPlane)
{
    // set viewport
    glViewport(x, y, width, height);
    glScissor(x, y, width, height);

    // set perspective viewing frustum
    glm::mat4 matrix = setFrustum(FOV_Y, (float)(width) / height, nearPlane, farPlane); // FOV, AspectRatio, NearClip, FarClip

    // copy projection matrix to OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&matrix[0][0]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
// draw 2D/3D scene
///////////////////////////////////////////////////////////////////////////////
void ModelGL::draw()
{
    drawSub1();
    drawSub2();

    // post frame
    if (windowSizeChanged)
    {
        setViewport(0, 0, windowWidth, windowHeight);
        windowSizeChanged = false;
    }

    if (drawModeChanged)
    {
        if (drawMode == 0)           // fill mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }
        else if (drawMode == 1)      // wireframe mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        else if (drawMode == 2)      // point mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        drawModeChanged = false;
    }
}

///////////////////////////////////////////////////////////////////////////////
// draw upper window (view from the camera)
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawSub1()
{
    int halfHeight = windowHeight / 2;

    setViewportSub(0, halfHeight, windowWidth, halfHeight, 1, 10);
    glClearColor(0.0f, 0.0f, 0.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // set upper viewport, make it square
    if (windowWidth > halfHeight)
        setViewportSub((windowWidth - halfHeight) / 2, halfHeight, halfHeight, halfHeight, 1, 10);
    else
        setViewportSub(0, halfHeight + (halfHeight - windowWidth) / 2, windowWidth, windowWidth, 1, 10);

    // clear buffer (square area)
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPushMatrix();

    // set view matrix ========================================================
    // copy the matrix to OpenGL GL_MODELVIEW matrix
    // Note that OpenGL uses column-major matrix, so transpose the matrix first
    // See updateViewMatrix() how matrixView is constructed. The equivalent
    // OpenGL calls are;
    //    glLoadIdentity();
    //    glRotatef(-cameraAngle[2], 0, 0, 1); // roll
    //    glRotatef(-cameraAngle[1], 0, 1, 0); // heading
    //    glRotatef(-cameraAngle[0], 1, 0, 0); // pitch
    //    glTranslatef(-cameraPosition[0], -cameraPosition[1], -cameraPosition[2]);
    glLoadMatrixf(&matrixView[0][0]);

    // always draw the grid at the origin (before any modeling transform)
    drawGrid(10, 1);

    // transform objects ======================================================
    // From now, all transform will be for modeling matrix only.
    // (from object space to world space)
    // See updateModelMatrix() how matrixModel is constructed. The equivalent
    // OpenGL calls are;
    //    glLoadIdentity();
    //    glTranslatef(modelPosition[0], modelPosition[1], modelPosition[2]);
    //    glRotatef(modelAngle[0], 1, 0, 0);
    //    glRotatef(modelAngle[1], 0, 1, 0);
    //    glRotatef(modelAngle[2], 0, 0, 1);

    // compute GL_MODELVIEW matrix by multiplying matrixView and matrixModel
    // before drawing the object:
    // ModelView_M = View_M * Model_M
    // This modelview matrix transforms the objects from object space to eye space.
    // copy modelview matrix to OpenGL after transpose
    glLoadMatrixf(&matrixModelView[0][0]);

    // draw a teapot after ModelView transform
    // v' = Mmv * v
    drawAxis(4);
    //drawTeapot();

    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// draw bottom window (3rd person view)
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawSub2()
{
    glViewport(0, 0, windowWidth, windowHeight / 2);
    glScissor(0, 0, windowWidth, windowHeight / 2);

    // set perspective viewing frustum
    glm::mat4 matProj = setFrustum(FOV_Y, (float)(windowWidth) / (windowHeight / 2), 1, 1000); // FOV, AspectRatio, NearClip, FarClip

    // copy projection matrix to OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&matProj[0][0]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);   // background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPushMatrix();

    glm::mat4 matView, matModel, matModelView;
    glm::rotate(matView, cameraAngleY, glm::vec3(0, 1, 0));
    glm::rotate(matView, cameraAngleX, glm::vec3(1, 0, 0));
    glm::translate(matView, glm::vec3(0, 0, -cameraDistance));
   
    glLoadMatrixf(&matView[0][0]);

    drawGrid(10, 1);

    glm::rotate(matModel, modelAngle[2], glm::vec3(0, 0, 1));
    glm::rotate(matModel, modelAngle[1], glm::vec3(0, 1, 0));
    glm::rotate(matModel, modelAngle[0], glm::vec3(1, 0, 0));
    glm::translate(matModel, glm::vec3(modelPosition[0], modelPosition[1], modelPosition[2]));
    matModelView = matView * matModel;
    glLoadMatrixf(&matModelView[0][0]);

    drawAxis(4);

    matModel = glm::mat4();

    glm::rotate(matModel, cameraAngle[2], glm::vec3(0, 0, 1));
    glm::rotate(matModel, cameraAngle[1], glm::vec3(0, 1, 0));
    glm::rotate(matModel, cameraAngle[0], glm::vec3(1, 0, 0));

    
    glm::translate(matModel, glm::vec3(cameraPosition[0], cameraPosition[1], cameraPosition[2]));
    matModelView = matView * matModel;
    glLoadMatrixf(&matModelView[0][0]);

    drawFrustum(FOV_Y, 1, 1, 10);

    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// draw a grid on the xz plane
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawGrid(float size, float step)
{
    // disable lighting
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);

    glColor3f(1.0f, 1.0f, 0.0f);
    for (float i = step; i <= size; i += step)
    {
        glVertex3f(-size, 0, i);   // lines parallel to X-axis
        glVertex3f(size, 0, i);
        glVertex3f(-size, 0, -i);   // lines parallel to X-axis
        glVertex3f(size, 0, -i);

        glVertex3f(i, 0, -size);   // lines parallel to Z-axis
        glVertex3f(i, 0, size);
        glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
        glVertex3f(-i, 0, size);
    }

    // x-axis
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-size, 0, 0);
    glVertex3f(size, 0, 0);

    // z-axis
    glColor3f(1.0f, 0, 1.0f);
    glVertex3f(0, 0, -size);
    glVertex3f(0, 0, size);

    glEnd();

    // enable lighting back
    glEnable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////////////////
// draw the local axis of an object
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawAxis(float size)
{
    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
    glDisable(GL_LIGHTING);
    glPushMatrix();             //NOTE: There is a bug on Mac misbehaviours of
                                //      the light position when you draw GL_LINES
                                //      and GL_POINTS. remember the matrix.

    // draw axis
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);
    glEnd();
    glLineWidth(1);

    // draw arrows(actually big square dots)
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    glVertex3f(size, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, size, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, size);
    glEnd();
    glPointSize(1);

    // restore default settings
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
}

///////////////////////////////////////////////////////////////////////////////
// draw frustum
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawFrustum(float fovY, float aspectRatio, float nearPlane, float farPlane)
{
    float tangent = tanf(fovY / 2 * DEG2RAD);
    float nearHeight = nearPlane * tangent;
    float nearWidth = nearHeight * aspectRatio;
    float farHeight = farPlane * tangent;
    float farWidth = farHeight * aspectRatio;

    // compute 8 vertices of the frustum
    float vertices[8][3];
    // near top right
    vertices[0][0] = nearWidth;     vertices[0][1] = nearHeight;    vertices[0][2] = -nearPlane;
    // near top left
    vertices[1][0] = -nearWidth;    vertices[1][1] = nearHeight;    vertices[1][2] = -nearPlane;
    // near bottom left
    vertices[2][0] = -nearWidth;    vertices[2][1] = -nearHeight;   vertices[2][2] = -nearPlane;
    // near bottom right
    vertices[3][0] = nearWidth;     vertices[3][1] = -nearHeight;   vertices[3][2] = -nearPlane;
    // far top right
    vertices[4][0] = farWidth;      vertices[4][1] = farHeight;     vertices[4][2] = -farPlane;
    // far top left
    vertices[5][0] = -farWidth;     vertices[5][1] = farHeight;     vertices[5][2] = -farPlane;
    // far bottom left
    vertices[6][0] = -farWidth;     vertices[6][1] = -farHeight;    vertices[6][2] = -farPlane;
    // far bottom right
    vertices[7][0] = farWidth;      vertices[7][1] = -farHeight;    vertices[7][2] = -farPlane;

    float colorLine1[4] = { 0.7f, 0.7f, 0.7f, 0.7f };
    float colorLine2[4] = { 0.2f, 0.2f, 0.2f, 0.7f };
    float colorPlane[4] = { 0.5f, 0.5f, 0.5f, 0.5f };

    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw the edges around frustum
    glBegin(GL_LINES);
    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[4]);

    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[5]);

    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[6]);

    glColor4fv(colorLine2);
    glVertex3f(0, 0, 0);
    glColor4fv(colorLine1);
    glVertex3fv(vertices[7]);
    glEnd();

    glColor4fv(colorLine1);
    glBegin(GL_LINE_LOOP);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[7]);
    glEnd();

    glColor4fv(colorLine1);
    glBegin(GL_LINE_LOOP);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[3]);
    glEnd();

    // draw near and far plane
    glColor4fv(colorPlane);
    glBegin(GL_QUADS);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[7]);
    glEnd();

    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////////////////
// set the camera position and rotation
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setViewMatrix(float x, float y, float z, float pitch, float heading, float roll)
{
    cameraPosition[0] = x;
    cameraPosition[1] = y;
    cameraPosition[2] = z;
    cameraAngle[0] = pitch;
    cameraAngle[1] = heading;
    cameraAngle[2] = roll;

    updateViewMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// set the object position and rotation
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setModelMatrix(float x, float y, float z, float rx, float ry, float rz)
{
    modelPosition[0] = x;
    modelPosition[1] = y;
    modelPosition[2] = z;
    modelAngle[0] = rx;
    modelAngle[1] = ry;
    modelAngle[2] = rz;

    updateModelMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// update matrix
///////////////////////////////////////////////////////////////////////////////
void ModelGL::updateViewMatrix()
{
    // transform the camera (viewing matrix) from world space to eye space
    // Notice all values are negated, because we move the whole scene with the
    // inverse of camera transform    matrixView.identity();
    matrixView = glm::mat4();
    glm::translate(matrixView,glm::vec3(-cameraPosition[0], -cameraPosition[1], -cameraPosition[2]));
    glm::rotate(matrixView, -cameraAngle[2], glm::vec3(0, 0, 1));
    glm::rotate(matrixView, -cameraAngle[1], glm::vec3(0, 1, 0));
    glm::rotate(matrixView, -cameraAngle[0], glm::vec3(1, 0, 0));

    matrixModelView = matrixView * matrixModel;
}

void ModelGL::updateModelMatrix()
{
    // transform objects from object space to world space
    matrixModel = glm::mat4();
    glm::rotate(matrixModel, -modelAngle[2], glm::vec3(0, 0, 1));
    glm::rotate(matrixModel, -modelAngle[1], glm::vec3(0, 1, 0));
    glm::rotate(matrixModel, -modelAngle[0], glm::vec3(1, 0, 0));
    glm::translate(matrixModel, glm::vec3(modelPosition[0], modelPosition[1], modelPosition[2]));

    matrixModelView = matrixView * matrixModel;
}

///////////////////////////////////////////////////////////////////////////////
// rotate the camera for subWin2 (3rd person view)
///////////////////////////////////////////////////////////////////////////////
void ModelGL::rotateCamera(int x, int y)
{
    cameraAngleY += (x - mouseX);
    cameraAngleX += (y - mouseY);
    mouseX = x;
    mouseY = y;
}

///////////////////////////////////////////////////////////////////////////////
// zoom the camera for subWin2 (3rd person view)
///////////////////////////////////////////////////////////////////////////////
void ModelGL::zoomCamera(int y)
{
    cameraDistance -= (y - mouseY) * 0.1f;
    mouseY = y;
}
void ModelGL::zoomCameraDelta(float delta)
{
    cameraDistance -= delta;
}

///////////////////////////////////////////////////////////////////////////////
// change drawing mode
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setDrawMode(int mode)
{
    if (drawMode != mode)
    {
        drawModeChanged = true;
        drawMode = mode;
    }
}

///////////////////////////////////////////////////////////////////////////////
// set a perspective frustum with 6 params similar to glFrustum()
// (left, right, bottom, top, near, far)
// Note: this is for row-major notation. OpenGL needs transpose it
///////////////////////////////////////////////////////////////////////////////
glm::mat4 ModelGL::setFrustum(float l, float r, float b, float t, float n, float f)
{ 
    return glm::frustum(l, r, b, t, n, f);
}

///////////////////////////////////////////////////////////////////////////////
// set a symmetric perspective frustum with 4 params similar to gluPerspective
// (vertical field of view, aspect ratio, near, far)
///////////////////////////////////////////////////////////////////////////////
glm::mat4 ModelGL::setFrustum(float fovY, float aspectRatio, float front, float back)
{
    float tangent = tanf(fovY / 2 * DEG2RAD);   // tangent of half fovY
    float height = front * tangent;           // half height of near plane
    float width = height * aspectRatio;       // half width of near plane

    // params: left, right, bottom, top, near, far
    return  glm::frustum(-width, width, -height, height, front, back);
}

///////////////////////////////////////////////////////////////////////////////
// set a orthographic frustum with 6 params similar to glOrtho()
// (left, right, bottom, top, near, far)
// Note: this is for row-major notation. OpenGL needs transpose it
///////////////////////////////////////////////////////////////////////////////
glm::mat4 ModelGL::setOrthoFrustum(float l, float r, float b, float t, float n, float f)
{
    //Matrix4 matrix;
    //matrix[0] = 2 / (r - l);
    //matrix[5] = 2 / (t - b);
    //matrix[10] = -2 / (f - n);
    //matrix[12] = -(r + l) / (r - l);
    //matrix[13] = -(t + b) / (t - b);
    //matrix[14] = -(f + n) / (f - n);

    return glm::ortho(l, r, b, t, n, f);
}