#ifndef SeaShader_H
#define SeaShader_H

#include<glm.hpp>
#include<Shape\shader.h>
#include<Shape\camera.h>
//const int ITER_GEOMETRY = 5;
//const int ITER_FRAGMENT = 3;
//const float SEA_HEIGHT = 0.6;
//const float SEA_CHOPPY = 6.0;
//const float SEA_SPEED = 0.8;
//const float SEA_FREQ = 0.16;
//const vec3 SEA_BASE = vec3(0.1, 0.19, 0.22);
//const vec3 SEA_WATER_COLOR = vec3(0.5, 1, 0.6);

class SeaShader:public Shader
{
public:
    SeaShader();
    ~SeaShader();

    int iter_geometry = 5;
    int iter_fragment = 3;
    float height = 0.6f;
    float choppy = 6.0f;
    float speed = 0.8f;
    float frequency = 0.16f;

    glm::vec3 baseColor = glm::vec3(0.1, 0.19, 0.22);
    glm::vec3 waterColor = glm::vec3(0.5, 1, 0.6);

    void bindCamera(Camera& cam);
    void unbindCamera() { camobj = NULL; };
    bool load();
    void use();

    const static string pIter_Geometry;
    const static string pIter_Fragment;
    const static string pHeight;
    const static string pChoppy;
    const static string pSpeed;
    const static string pFrequency;
    const static string pBaseColor;
    const static string pWaterColor;

private:

    Camera* camobj = NULL;

};



#endif // SeaShader_H


