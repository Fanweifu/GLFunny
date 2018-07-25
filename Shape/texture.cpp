#include"texture.h"

void Texture::bind(int lev)
{
    if (!isValid) return;
    level = lev;
    glActiveTexture(GL_TEXTURE0 + level);
    glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::unbind()
{
    if (!isValid) return;
    glActiveTexture(GL_TEXTURE0 + level);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::init()
{
    if (!inited) {
        glewInit();
        glGenTextures(1, &texID);
        inited = true;
    }
}

bool ImgTexture::loadRgbImg(char * path)
{
    init();

   

    unsigned char* data = stbi_load(path, &cols, &rows, &chns, 4);

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cols, rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;

    return (isValid = true);
}

void ImgTexture::makeColor(float r, float g, float b, float a)
{
    init();

    unsigned char *data = new unsigned char[4]{ unsigned char(r * 255), unsigned char(g * 255) ,unsigned char(b * 255) ,unsigned char(a * 255) };
    cols = 1; rows = 1; chns = 4;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cols, rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    delete[] data;

    isValid = true;
}

DepthTexture::DepthTexture()
{
}

DepthTexture::~DepthTexture()
{
    Texture::~Texture();
    glDeleteFramebuffers(1, &depthMapFBO);
}

bool DepthTexture::loadDepthMap(float camposx, float camposy, float camposz, float lightx, float lighty, float lightz, float lightw, Shape& scene)
{
    if (!inited) init();

    glPushMatrix();

    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::vec3 camPos(camposx, camposy, camposz);
    glm::vec4 lightPos(lightx, lighty, lightz, lightw);

    lightPrjMat = glm::ortho(-range / 2, range / 2, -range / 2, range / 2, n, f);
    lightViewMat = glm::lookAt((glm::vec3(lightPos))*distance + camPos, glm::vec3(0) + camPos, glm::vec3(0, 0, 1));
    lightPrjViewMat = lightPrjMat*lightViewMat;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(value_ptr(lightPrjMat));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(value_ptr(lightViewMat));

    scene.draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glPopMatrix();

    return(isValid = true);
}

void DepthTexture::setViewMatInv(const float* matptr)
{
    shadowPro.setUniformMat4(Shader::pCameraViewInv, matptr);
}

void DepthTexture::bindShadow()
{
    Texture::bind(3);

    shadowPro.use();
    shadowPro.setUniform1i("baseTex", 0);
    shadowPro.setUniform1i("depthTex", 3);
    shadowPro.setUniform1f("biasFactor", calcBias());
    shadowPro.setUniformMat4("lightSpace", value_ptr(lightPrjViewMat));
    shadowPro.setUniform1i("smoothLevel", shadowSmooth);

    if (enablePbr) {
        shadowPro.setUniform1i("enablePbr", true);
        
        shadowPro.setUniform1i("normalTex", 1);
        shadowPro.setUniform1i("specularTex", 2);
    }
    else {
        shadowPro.setUniform1i("enablePbr", false);
    }
}

void DepthTexture::unbindShadow()
{
    Texture::unbind();

    shadowPro.unuse();
}

void DepthTexture::init()
{
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &texID);

    shadowPro.loadVertexFile("GLSL/shadowv.glsl");
    shadowPro.loadFragFile("GLSL/shadowf.glsl");
    shadowPro.link();

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    inited = true;
}

float DepthTexture::calcBias()
{
    return range / width / (f - n) / 2;
}