#include"texture.h"

FBObject::FBObject()
{

}

FBObject::~FBObject()
{
    unInit();
}

void FBObject::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    isBinded = true;
}

void FBObject::unBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    isBinded = false;
}

bool FBObject::vaild()
{
    if (isBinded) return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    else {
        bind();
        auto res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        unBind();
        return res == GL_FRAMEBUFFER_COMPLETE;
    }
}

void FBObject::resize(int w, int h)
{
    if ((w != width || h != height) && (w > 0 && h > 0)) {
        width = w; height = h;
        for each(auto kv in texbindinfo) {
            switch (kv.first)
            {
            case GL_DEPTH_ATTACHMENT:
                attchDepth(kv.second);
                break;
            default:
                attchColor(kv.second, kv.first - GL_COLOR_ATTACHMENT0);
                break;
            }
        }
    }
}

void FBObject::init()
{
    if (isInited) return;

    glGenFramebuffers(1, &fbo);

    isInited = true;
}

void FBObject::unInit()
{
    if (!isInited) return;

    glDeleteFramebuffers(1, &fbo);
    isInited = isVaild = isBinded = false;
}

void FBObject::attchDepth(GLuint tex)
{
    init();
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    bind();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    unBind();

    glBindTexture(GL_TEXTURE_2D, 0);

    texbindinfo[GL_DEPTH_ATTACHMENT] = tex;
}

void FBObject::attchColor(GLuint tex, int clrlv)
{
    init();
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    bind();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + clrlv, GL_TEXTURE_2D, tex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    unBind();

    glBindTexture(GL_TEXTURE_2D, 0);

    texbindinfo[GL_COLOR_ATTACHMENT0 + clrlv] = tex;
}

int Texture2D::Width()
{
    int w = 0;

    if (!isVaild) return w;

    if (isBinded) {
        glGetTexGeniv(GL_TEXTURE_2D, GL_TEXTURE_WIDTH, &w);
        glGetTextureParameterIiv(m_texID, GL_TEXTURE_WIDTH, &w);
        glGetTextureParameterIiv(m_texID, GL_TEXTURE_WIDTH, &w);
        return w;
    }
    else {
        glBindTexture(GL_TEXTURE_2D, m_texID);
        glGetTexGeniv(GL_TEXTURE_2D, GL_TEXTURE_WIDTH, &w);
        glGetTextureParameterIiv(m_texID, GL_TEXTURE_WIDTH, &w);
        glBindTexture(GL_TEXTURE_2D, 0);
        return w;
    }
}

int Texture2D::Height()
{
    return 0;
}

void Texture2D::bind(int lev)
{
    if (!isVaild) return;
    m_activeLevel = lev;
    glActiveTexture(GL_TEXTURE0 + m_activeLevel);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    isBinded = true;
}

void Texture2D::unBind()
{
    if (!isVaild) return;
    glActiveTexture(GL_TEXTURE0 + m_activeLevel);
    glBindTexture(GL_TEXTURE_2D, 0);
    isBinded = false;
}

void Texture::init()
{
    if (!isInited) {
        glGenTextures(1, &m_texID);
        isInited = true;
    }
}

void Texture::unInit()
{
    if (!isInited) return;
    glDeleteTextures(1, &m_texID);
    isInited = isVaild = isBinded = false;
}

void Texture::bind()
{
    bind(0);
}

bool Texture2D::loadFileImg(char * path)
{
    init();

    int cols, rows, chns;
    unsigned char* data = stbi_load(path, &cols, &rows, &chns, 4);

    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cols, rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;

    return (isVaild = true);
}

void Texture2D::buildByColor(float r, float g, float b, float a)
{
    init();

    unsigned char *data = new unsigned char[4]{ unsigned char(r * 255), unsigned char(g * 255) ,unsigned char(b * 255) ,unsigned char(a * 255) };
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    delete[] data;

    isVaild = true;
}

void Texture2D::attchDepthFBO(FBObject &fbo)
{
    init();
    fbo.attchDepth(m_texID);
    isVaild = true;
}

void Texture2D::attchColorFBO(FBObject &fbo)
{
    init();
    fbo.attchColor(m_texID);
    isVaild = true;
}


DepthTexture::DepthTexture()
{
}

DepthTexture::~DepthTexture()
{
    Texture::~Texture();
    glDeleteFramebuffers(1, &depthMapFBO);
}

void DepthTexture::load(float camposx, float camposy, float camposz, float lightx, float lighty, float lightz, float lightw, Shape& scene)
{
    beginLoad(camposx, camposy, camposz, lightx, lighty, lightz, lightw);

    scene.draw();

    endLoad();

}

void DepthTexture::beginLoad(float camposx, float camposy, float camposz, float lightx, float lighty, float lightz, float lightw)
{
    if (!isInited) init();

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
}

void DepthTexture::endLoad()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glPopMatrix();
}

void DepthTexture::init()
{
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &m_texID);

    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    isVaild = isInited = true;
    
}


float DepthTexture::shadowAcneBias()
{
    return range / width / (f - n) / 2;
}
