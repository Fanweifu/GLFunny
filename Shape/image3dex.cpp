#include "image3dex.h"

Image3DEx::Image3DEx()
{
}

void Image3DEx::setDstImage(cv::Mat & dst)
{
    dstData = dst.data;
    dstchns = dst.channels();
    dststep = dst.step;
}

void Image3DEx::calT() {
    if (!actived) return;

    t += speed;

    float val = (sin(t) + 1) / 2;

    pshader->setUniform1f(tstr, val);
}

void Image3DEx::calcHeight()
{
    delete heights;
    heights = new float[rows*cols];

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            float sum = 0;
            for (int c = 0; c < chns; c++) {
                sum += data[y*step + x*chns + c];
            }
            float dstsum = 0;
            for (int c = 0; c < chns; c++) {
                dstsum += dstData[y *dststep + x*dstchns + c];
            }
            heights[y*cols + x] = floor(sum / chns) + floor(dstsum / dstchns) / 1000;
        }
    }
}

void Image3DEx::getColor(uint x, uint y, float * outputVec)
{
    float color[4];
    Image3D::getColor(data, x, y, outputVec);
    Image3D::getColor(dstData, x, y, color);
    for (int i = 0; i < 4; i++) outputVec[i] += color[i] / 1000;
}

void Image3DEx::initShader()
{
    if (shaderinited) return;
    Shader* ins = new Shader();
    bool res = ins->loadVertexCode("#version 330 compatibility\n"
        "layout(location = 0) in vec3 pos;\n"
        "layout(location = 1) in vec4 clr;\n"
        "layout(location = 2) in vec3 nor;\n"
        "uniform float t;\n"
        "void main() {\n"
        "   float h  = mix(floor(pos.z),fract(pos.z)*1000,t);\n"
        "   gl_Position= gl_ModelViewProjectionMatrix*vec4(pos.x, pos.y , h ,1.);\n"
        "   vec4 rclr = mix(floor(clr*255),fract(clr*255)*1000,t)/255;\n"
        "   vec3 normal= normalize(gl_NormalMatrix * nor);\n"
        "   vec3 lightDir = normalize(vec3(gl_LightSource[0].position));\n"
        "   float NdotL = max(dot(normal,lightDir), 0.0);\n"
        "   vec4 diffuse = NdotL*rclr * gl_LightSource[0].diffuse;\n"
        "   vec4 ambient = rclr*gl_LightSource[0].ambient;\n"

        "   float NdotHV = max(dot(normal, gl_LightSource[0].halfVector.xyz),0.0);\n"
        "   vec4 specular = vec4(0,0,0.3,1) * gl_LightSource[0].specular *pow(NdotHV, 10);\n"
        "   vec4 result = diffuse+ambient+specular;\n"
        "   gl_FrontColor = result;\n"
        "   gl_BackColor = result;\n"
        "}\n"
    );

    if (res) {
        if (ins->link()) {
            setShader(*ins);
            shaderinited = true;
        }
    }
}

void Image3DEx::ondraw() {
    calT();
    Image3D::ondraw();
}