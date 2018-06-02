#ifndef LIGHT_H
#define LIGHT_H

#include<GL/gl.h>
#include<assert.h>

//#define GL_LIGHT0 0x4000
class Light
{
public:
    Light() {  }
    Light(int lightid) {
        assert(lightid >= 0 && lightid < 8);
        id = lightid;
        enumval = id + 0x4000;
    }

    int getLightID() { return id; }

    void setEnable(bool val) {
        enable = val;
        if (val)
            glEnable(enumval);
        else
            glDisable(enumval);
    }

    void setPostion(float px, float py, float pz, float w) {
        lightPos[0] = px;
        lightPos[1] = py;
        lightPos[2] = pz;
        lightPos[3] = w;
        glLightfv(enumval, GL_POSITION, lightPos);
    }
    void updatePostion() { glLightfv(enumval, GL_POSITION, lightPos); }
    void getPositon(float &px, float &py, float &pz, float &w) {
        px = lightPos[0];
        py = lightPos[1];
        pz = lightPos[2];
        w = lightPos[3];
    }

    void setAmbient(float r, float g, float b, float a) {
        lightKa[0] = r;
        lightKa[1] = g;
        lightKa[2] = b;
        lightKa[3] = a;
        glLightfv(enumval, GL_AMBIENT, lightKa);
    }
    void getAmbient(float& r, float& g, float& b, float& a) {
        r = lightKa[0];
        g = lightKa[1];
        b = lightKa[2];
        a = lightKa[3];
    }

    void setDiffuse(float r, float g, float b, float a) {
        lightKd[0] = r;
        lightKd[1] = g;
        lightKd[2] = b;
        lightKd[3] = a;
        glLightfv(enumval, GL_DIFFUSE, lightKd);
    }
    void getDiffuse(float& r, float& g, float& b, float& a) {
        r = lightKd[0];
        g = lightKd[1];
        b = lightKd[2];
        a = lightKd[3];
    }

    void setSpecular(float r, float g, float b, float a) {
        lightKs[0] = r;
        lightKs[1] = g;
        lightKs[2] = b;
        lightKs[3] = a;
        glLightfv(enumval, GL_SPECULAR, lightKs);
    }
    void getSpecular(float& r, float& g, float& b, float& a) {
        r = lightKs[0];
        g = lightKs[1];
        b = lightKs[2];
        a = lightKs[3];
    }

    void init() {
        setEnable(true);
        glLightfv(enumval, GL_POSITION, lightPos);
        glLightfv(enumval, GL_AMBIENT, lightKa);
        glLightfv(enumval, GL_DIFFUSE, lightKd);
        glLightfv(enumval, GL_SPECULAR, lightKs);
    }
protected:

    int id = 0;
    GLenum enumval = GL_LIGHT0;

    bool enable = false;
    float lightPos[4] = { 0,0,1,0 };
    float lightKa[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float lightKd[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float lightKs[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
};

#endif // LIGHT_H
