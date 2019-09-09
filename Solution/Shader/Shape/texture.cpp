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
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
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

	glGenFramebuffers(1, &m_handle);

	isInited = true;
}

void FBObject::unInit()
{
	if (!isInited) return;

	glDeleteFramebuffers(1, &m_handle);
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

	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
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

	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	texbindinfo[GL_COLOR_ATTACHMENT0] = tex;
}

void FBObject::attchFloatColor(GLuint tex)
{
	init();
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	texbindinfo[GL_COLOR_ATTACHMENT0] = tex;
}



//int Texture2D::Width()
//{
//    int w = 0;
//
//    if (!isVaild) return w;
//
//    CHECKBIND(glGetTexGeniv(GL_TEXTURE_2D, GL_TEXTURE_WIDTH, &w);)
//        return w;
//}
//
//int Texture2D::Height()
//{
//    int h = 0;
//
//    if (!isVaild) return h;
//    CHECKBIND(glGetTexGeniv(GL_TEXTURE_2D, GL_TEXTURE_HEIGHT, &h);)
//        return h;
//}

void Texture2D::bind(int lev)
{
	m_activeLevel = lev;
	glActiveTexture(GL_TEXTURE0 + m_activeLevel);
	glBindTexture(GL_TEXTURE_2D, m_handle);
	
	isBinded = true;
}

void Texture2D::unBind()
{
	glActiveTexture(GL_TEXTURE0 + m_activeLevel);
	glBindTexture(GL_TEXTURE_2D, 0);
	isBinded = false;
}

void Texture::setFilter(FilterType t)
{
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)t);
	unBind();
	
}

void Texture::init()
{
	if (!isInited) {
		glGenTextures(1, &m_handle);
		isInited = true;
	}
}

void Texture::unInit()
{
	if (!isInited) return;
	glDeleteTextures(1, &m_handle);
	isInited = isVaild = isBinded = false;
}

void Texture::bind()
{
	bind(0);
}

bool Texture2D::loadImg(const char * path, int deschns)
{
	int cols, rows, chns;
	unsigned char* data = stbi_load(path, &cols, &rows, &chns, deschns);

	int format = deschns == 1 ? GL_RED : (deschns == 3 ? GL_RGB : GL_RGBA);
	bool res = setTexImg(data, cols, rows, format, format);

	free(data);

	return res;
}

unsigned char * loadBinData(const char*path, int &size) {
	FILE*reader = fopen(path, "rb");
	if (reader == NULL) return NULL;

	fseek(reader, 0, SEEK_END);
	size = ftell(reader);
	rewind(reader);
	unsigned char*data = (unsigned char*)malloc(size);
	memset(data, 255, size);
	fread(data, 1, size, reader);
	fclose(reader);
	return data;
}

bool Texture2D::loadBin(const char * path, int w, int h, int chns)
{
	int desireSize = w*h*chns;
	int factSize = 0;

	auto data = loadBinData(path, factSize);
	auto data1 = loadBinData("C:/Users/1/Desktop/linuxrgba/fontTex358x184.rgb32", factSize);

	if (data == NULL) return false;

	if (desireSize != factSize) return false;

	int format = chns == 1 ? GL_RED : (chns == 3 ? GL_RGB : GL_RGBA);
	setTexImg(data, w, h, format, format);

	free(data);
}

bool Texture2D::setTexImg(const void * data, int cols, int rows, int interformat, int format, int type)
{
	init();

	m_width = cols, m_height = rows;
	m_innerformat = interformat;
	m_format = format;
	m_dataType = type;


	glBindTexture(GL_TEXTURE_2D, m_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, interformat, cols, rows, 0, format, type, data);
	return (isVaild = true);
}

bool Texture2D::updateImg(const void * newData, int left, int top, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, m_handle);
	glTexSubImage2D(GL_TEXTURE_2D, 0, left, top, width, height, m_format, m_dataType, newData);
	return true;
}

void Texture2D::buildByColor(float r, float g, float b, float a)
{
	init();

	unsigned char data[4]{ r * 255,g * 255,b * 255,a * 255 };

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, m_handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	isVaild = true;
}

void Texture2D::attchDepthStencilFBO(FBObject &fbo)
{
	init();
	fbo.attchStencilDepth(m_handle);
	isVaild = true;
}

void Texture2D::attchColorFBO(FBObject &fbo)
{
	init();
	fbo.attchColor(m_handle);
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

void DepthTexture::begin(float cx, float cy, float cz, float lx, float ly, float lz, float lw)
{
	glPushMatrix();

	glm::vec3 camPos(cx, cy, cz);
	glm::vec4 lightPos(lx, ly, lz, lw);

	float ratio = (float)width / height;
	lightPrjMat = glm::ortho(-range / 2, range / 2, -range / 2 / ratio, range / 2 / ratio, n, f);
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