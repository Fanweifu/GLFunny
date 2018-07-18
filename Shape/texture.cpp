#include"texture.h"

void Texture::bind(int level)
{
    if (!isValid) return;
    glActiveTexture(GL_TEXTURE0 + level);
    glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::unbind()
{
    if (!isValid) return;
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

    cv::Mat img = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (img.empty() || img.depth() != CV_8U || img.channels() == 1) {
        printf("loaded failed!\n");
        return false;
    }

    uchar* data = readImgData(img);

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cols, rows, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    delete[] data;

    return (isValid = true);
}

void ImgTexture::makeSingleColor(float r, float g, float b)
{
    cv::Mat img = cv::Mat(cv::Size(1, 1), CV_8UC3, cv::Scalar(b * 255, g * 255, r * 255));

    uchar* data = readImgData(img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cols, rows, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    delete[] data;

    isValid = true;
}

uchar * ImgTexture::readImgData(cv::Mat & img)
{
    cols = img.cols, rows = img.rows, chns = img.channels();
    uchar* imgdata = new uchar[cols*rows*chns];

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int k = 0; k < chns; k++)
            {
                imgdata[i*cols*chns + j*chns + (chns - k - 1)] = img.data[i*img.step + j*chns + k];
            }
        }
    }
    return imgdata;
}

DepthTexture::DepthTexture()
{

}

bool DepthTexture::loadDepthMap(float camposx, float camposy, float camposz, float lightx, float lighty, float lightz, float lightw , float width, float height,ShapeBase& scene)
{
    widthf = width;
    heightf = height;
    if (!inited) init();

    glPushMatrix();

    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::vec3 camPos(camposx,camposy,camposz);
    glm::vec4 lightPos(lightx, lighty, lightz, lightw);



    lightPrjMat = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, n, f);
    lightViewMat = glm::lookAt((glm::vec3(lightPos)), glm::vec3(0), glm::vec3(0, 0, 1));
    lightPrjViewMat = lightPrjMat*lightViewMat;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(value_ptr(lightPrjMat));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(value_ptr(lightViewMat));

    scene.draw();

    glBindTexture(GL_TEXTURE_2D, texID);

    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, width, height, 0);
    /*glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);*/

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();

    return(isValid = true);
}


void DepthTexture::bind(int levle)
{
    Texture::bind(1);

    shadowPro.use();
    shadowPro.setUniform1i("baseTex", 0);
    shadowPro.setUniform1i("depthTex", 1);
    shadowPro.setUniformMat4("lightspace", value_ptr(lightPrjViewMat));
}

void DepthTexture::unbind()
{
    Texture::unbind();
    shadowPro.unuse();
}


void DepthTexture::init()
{
    if (inited) return;
    glGenFramebuffers(1, &fboID);
    glGenTextures(1, &texID);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, widthf, heightf, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    shadowPro.loadVertexFile("GLSL/shadowv.glsl");
    shadowPro.loadFragFile("GLSL/shadowf.glsl");
    shadowPro.link();

    inited = true;
}