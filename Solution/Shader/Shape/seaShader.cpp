#include "SeaShader.h"

const string SeaShader::pIter_Geometry = "iter_geometry";
const string SeaShader::pIter_Fragment = "iter_fragment";

const string SeaShader::pHeight = "height";
const string SeaShader::pChoppy = "choppy";
const string SeaShader::pSpeed = "speed";
const string SeaShader::pFrequency = "frequency";

const string SeaShader::pBaseColor = "baseColor";
const string SeaShader::pWaterColor = "waterColor";

SeaShader::SeaShader()
{
}


SeaShader::~SeaShader()
{
}

void SeaShader::bindCamera(Camera & cam)
{
    camobj = &cam;
    
}

bool SeaShader::load()
{
    if (isVaild) return true;
    if (!loadFragFile("res/sea.txt")) return false;
    return link();
}

void SeaShader::use()
{
    Shader::bind();

    setUniform1i(SeaShader::pIter_Geometry,iter_geometry);
    setUniform1i(SeaShader::pIter_Fragment, iter_fragment);

    setUniform1f(SeaShader::pHeight, height);
    setUniform1f(SeaShader::pChoppy, choppy);
    setUniform1f(SeaShader::pSpeed, speed);
    setUniform1f(SeaShader::pFrequency, frequency);

    setUniform1fv(SeaShader::pBaseColor, 3 ,glm::value_ptr(baseColor));
    setUniform1fv(SeaShader::pWaterColor,3, glm::value_ptr(waterColor));

    if (camobj) {

        setUniform1f(Shader::pTime, (float)camobj->getRenderTimes() / 100);
        setUniform2f(Shader::pView, camobj->getViewWidth(), camobj->getViewHeight());
        setUniformMat4(Shader::pCameraViewInv, camobj->getModelMatPtr());
        setUniformMat4(Shader::pProjectionInv, camobj->getProjectionMatInvPtr());
    }

}
