#include"texture.h"

void Texture::bind(uint level)
{
    if (!isValid) return;
    glActiveTexture(GL_TEXTURE0 + level);
    glBindTexture(GL_TEXTURE_2D, textImgID);
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
        glGenTextures(1, &textImgID);
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


    glBindTexture(GL_TEXTURE_2D, textImgID);
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

    glBindTexture(GL_TEXTURE_2D, textImgID);
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