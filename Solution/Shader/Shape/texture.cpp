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
    glBindFramebuffer(GL_FRAMEBUFFER, m_objID);
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
    isBinded = true;
}

void FBObject::unBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    isBinded = false;
}

bool FBObject::vaild()
{
    GLenum sta;

    CHECKBIND(sta = glCheckFramebufferStatus(GL_FRAMEBUFFER);)
        return sta == GL_FRAMEBUFFER_COMPLETE;
}

void FBObject::clearBuffers()
{
    CHECKBIND(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);)
}

void FBObject::resize(int w, int h)
{
    if ((w != width || h != height) && (w > 0 && h > 0)) {
        width = w; height = h;
        for each(auto kv in texbindinfo) {
            switch (kv.first)
            {
            case GL_DEPTH_STENCIL_ATTACHMENT:
                attchStencilDepth(kv.second);
                break;
            
            case GL_COLOR_ATTACHMENT0:
                attchColor(kv.second);
                break;
           /* case GL_STENCIL_ATTACHMENT:
                attchStencil(kv.second);
                break;*/
            
            }
        }
    }
}

void FBObject::init()
{
    if (isInited) return;

    glGenFramebuffers(1, &m_objID);

    isInited = true;
}

void FBObject::unInit()
{
    if (!isInited) return;

    glDeleteFramebuffers(1, &m_objID);
    isInited = isVaild = isBinded = false;
}

void FBObject::attchStencilDepth(GLuint tex)
{
    init();

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);

    float borderColor[] = { 1.0, 1.0, 1.0, 0.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, m_objID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex, 0);
   
    //glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D,tex,0);
    //glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_STENCIL_ATTACHMENT_EXT,GL_TEXTURE_2D, tex, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    texbindinfo[GL_DEPTH_STENCIL_ATTACHMENT] = tex;
}

void FBObject::attchColor(GLuint tex)
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

    glBindFramebuffer(GL_FRAMEBUFFER, m_objID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    texbindinfo[GL_COLOR_ATTACHMENT0] = tex;
}

int Texture2D::Width()
{
    int w = 0;

    if (!isVaild) return w;

    CHECKBIND(glGetTexGeniv(GL_TEXTURE_2D, GL_TEXTURE_WIDTH, &w);)
        return w;
}

int Texture2D::Height()
{
    int h = 0;

    if (!isVaild) return h;
    CHECKBIND(glGetTexGeniv(GL_TEXTURE_2D, GL_TEXTURE_HEIGHT, &h);)
        return h;
}

void Texture2D::bind(int lev)
{
    if (!isVaild) return;
    m_activeLevel = lev;
    glActiveTexture(GL_TEXTURE0 + m_activeLevel);
    glBindTexture(GL_TEXTURE_2D, m_objID);
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
        glGenTextures(1, &m_objID);
        isInited = true;
    }
}

void Texture::unInit()
{
    if (!isInited) return;
    glDeleteTextures(1, &m_objID);
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

    glBindTexture(GL_TEXTURE_2D, m_objID);
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
    glBindTexture(GL_TEXTURE_2D, m_objID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    delete[] data;

    isVaild = true;
}

void Texture2D::attchDepthStencilFBO(FBObject &fbo)
{
    init();
    fbo.attchStencilDepth(m_objID);
    isVaild = true;
}

void Texture2D::attchColorFBO(FBObject &fbo)
{
    init();
    fbo.attchColor(m_objID);
    isVaild = true;
}


DepthTexture::DepthTexture()
{
}

DepthTexture::~DepthTexture()
{
    Texture::~Texture();
    /*glDeleteFramebuffers(1, &depthMapFBO);*/
}

//void DepthTexture::beginLoad(float camposx, float camposy, float camposz, float lightx, float lighty, float lightz, float lightw)
//{
//    initFBO();
//
//    glPushMatrix();
//    glViewport(0, 0, width, height);
//    glScissor(0, 0, width, height);
//
//    glm::vec3 camPos(camposx, camposy, camposz);
//    glm::vec4 lightPos(lightx, lighty, lightz, lightw);
//
//    lightPrjMat = glm::ortho(-range / 2, range / 2, -range / 2, range / 2, n, f);
//    lightViewMat = glm::lookAt((glm::vec3(lightPos))*distance + camPos, glm::vec3(0) + camPos, glm::vec3(0, 0, 1));
//    lightPrjViewMat = lightPrjMat*lightViewMat;
//
//    glMatrixMode(GL_PROJECTION);
//    glLoadMatrixf(value_ptr(lightPrjMat));
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadMatrixf(value_ptr(lightViewMat));
//
//    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//    glClear(GL_DEPTH_BUFFER_BIT);
//}
//
//void DepthTexture::endLoad()
//{
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    glPopMatrix();
//}

void DepthTexture::begin(float cx, float cy, float cz, float lx, float ly, float lz,float lw)
{
    
    glPushMatrix();
    
    glm::vec3 camPos(cx, cy, cz);
    glm::vec4 lightPos(lx, ly, lz, lw);

    float ratio = (float)width / height;
    lightPrjMat = glm::ortho(-range / 2, range / 2, -range / 2/ ratio, range / 2/ ratio, n, f);
    lightViewMat = glm::lookAt((glm::vec3(lightPos))*distance + camPos, camPos, glm::vec3(0, 0, 1));
    lightPrjViewMat = lightPrjMat*lightViewMat;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(value_ptr(lightPrjMat));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(value_ptr(lightViewMat));
}



void DepthTexture::end()
{
    glPopMatrix();
}

//void DepthTexture::initFBO()
//{
//    if (isInited) return;
//
//    glGenFramebuffers(1, &depthMapFBO);
//    glGenTextures(1, &m_objID);
//
//    glBindTexture(GL_TEXTURE_2D, m_objID);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//
//    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
//    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//
//    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_objID, 0);
//    glDrawBuffer(GL_NONE);
//    glReadBuffer(GL_NONE);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    glBindTexture(GL_TEXTURE_2D, 0);
//
//    isVaild = isInited = true;
//}

float DepthTexture::shadowAcneBias()
{
    return range / width / (f - n) / 2;
}