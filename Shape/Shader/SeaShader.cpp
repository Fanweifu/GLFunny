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
    Shader::use();

    setUniform1i(SeaShader::pIter_Geometry,iter_geometry);
    setUniform1i(SeaShader::pIter_Fragment, iter_fragment);

    setUniform1f(SeaShader::pHeight, height);
    setUniform1f(SeaShader::pChoppy, choppy);
    setUniform1f(SeaShader::pSpeed, speed);
    setUniform1f(SeaShader::pFrequency, frequency);

    setUniform3f(SeaShader::pBaseColor, baseColor.r, baseColor.g, baseColor.b);
    setUniform3f(SeaShader::pWaterColor,waterColor.r,waterColor.g,waterColor.b);

    if (camobj) {

        setUniform1f(Shader::pTime, (float)camobj->getRenderTimes() / 100);
        setUniform2f(Shader::pView, camobj->getViewWidth(), camobj->getViewHeight());
        setUniformMat4(Shader::pMdlInvMat, camobj->getModelViewPtr());
        setUniformMat4(Shader::pPrjInvMat, camobj->getProjectionMatInvPtr());
    }

}
