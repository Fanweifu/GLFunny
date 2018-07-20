#include<cstring>
#include "image3d.h"

Image3D::Image3D()
{
}

Image3D::~Image3D() {
    if (heights) delete heights;
    if (colors) delete colors;
    if (vertexs) delete vertexs;
    if (normals) delete normals;
    if (vertexIdxs) delete vertexIdxs;
}

void Image3D::setSrcImage(cv::Mat& img) {
    assert(img.depth() == CV_8U&&img.channels() == 3);

    reShape(img.rows, img.cols);

    data = img.data;
    chns = img.channels();
    step = img.step;
}

void Image3D::generateData() {
    initShader();
    calcHeight();

    int pointNum = rows*cols * 4 * 3;
    uint idci = 0, idx = 0;
    for (uint y = 0; y < rows; y++) {
        for (uint x = 0; x < cols; x++) {
            int curpxl = y*cols + x;
            int rpxl = y*cols + (x + 1);
            int bpxl = (y + 1)*cols + x;

            getColor(x, y, color4f);

            for (int i = 0; i < 12; i++)
            {
                memcpy(colors + (48 * curpxl) + i * 4, color4f, 4 * sizeof(float));
            }

            float h = heights[curpxl];
            float r = x < cols - 1 ? heights[rpxl] : 0;
            float b = y < rows - 1 ? heights[bpxl] : 0;

            float ver[12][3] = {
                {x,y,h},{x + 1,y,h},{x + 1,y + 1,h},{x,y + 1,h},
                { x + 1,y + 1,h },{ x + 1,y,h },{x + 1,y,r},{x + 1,y + 1,r},
                {x,y + 1,h},{x + 1,y + 1,h},{x + 1,y + 1,b},{x,y + 1,b}
            };

            memcpy(vertexs + 36 * curpxl, ver, 36 * sizeof(float));

            float nor[12][3] = {
                {0,0,1},{ 0,0,1 },{ 0,0,1 },{ 0,0,1 },
                { 1,0,0 },{ 1,0,0 },{ 1,0,0 },{ 1,0,0 },
                { 0,1,0 },{ 0,1,0 },{ 0,1,0 },{ 0,1,0 },
            };

            memcpy(normals + 36 * curpxl, ver, 36 * sizeof(float));

            for (int i = 0; i < 4; i++) {
                vertexIdxs[idci++] = idx++;
            }

            if (x < cols - 1) {
                for (int i = 0; i < 4; i++) {
                    vertexIdxs[idci++] = idx++;
                }
            }
            else {
                idx += 4;
            }
            if (y < rows - 1) {
                for (int i = 0; i < 4; i++) {
                    vertexIdxs[idci++] = idx++;
                }
            }
            else {
                idx += 4;
            }
        }
    }

    assert(idci == pointNum - (cols + rows) * 4);

    vao.setIndex(vertexIdxs,idci);
    vao.setVertex(vertexs, pointNum);
    vao.setNormal(normals, pointNum);
    vao.setColor(colors, 4, pointNum);
}

void Image3D::initShader()
{
    shader.loadVertexCode("#version 330 compatibility\n"
        "layout(location = 0) in vec3 pos;\n"
        "layout(location = 1) in vec4 clr;\n"
        "layout(location = 2) in vec3 nor;\n"
        "void main() {\n"
        "   gl_Position= gl_ModelViewProjectionMatrix*vec4(pos,1.);\n"
        "   vec3 normal= normalize(gl_NormalMatrix * nor);\n"
        "   vec3 lightDir = normalize(vec3(gl_LightSource[0].position));\n"
        "   float NdotL = max(dot(normal,lightDir), 0.0);\n"
        "   vec4 diffuse = NdotL*clr * gl_LightSource[0].diffuse;\n"
        "   vec4 ambient = clr*gl_LightSource[0].ambient;\n"

        "   float NdotHV = max(dot(nor, gl_LightSource[0].halfVector.xyz),0.0);\n"
        "   vec4 specular = clr * gl_LightSource[0].specular *pow(NdotHV, 20);\n"
        "   vec4 result = diffuse+ambient +specular;\n"
        "   gl_FrontColor = result*0.8;\n"
        "   gl_BackColor = result*0.8;\n"
        "}\n"
    );

    shader.link();
    
}

void Image3D::uninitShader()
{
}

bool Image3D::reShape(int rowc, int colc) {
    if (rowc*colc == 0) return false;

    int pointsNum = rowc*colc * 4 * 3;

    if (rows != rowc || cols != colc) {
        if (vertexs) delete vertexs;
        vertexs = new float[pointsNum * 3];
        if (colors) delete colors;
        colors = new float[pointsNum * 4];
        if (normals) delete colors;
        normals = new float[pointsNum * 3];

        if (vertexIdxs) delete vertexIdxs;
        vertexIdxs = new uint[pointsNum - rowc * 4 - colc * 4];

        rows = rowc; cols = colc;
    }
}

void Image3D::ondraw() {
    glTranslatef(-(float)cols / 2, -(float)rows / 2, 0);
    glScalef(pixelSzie, pixelSzie, heightRate);

    vao.renderData(GL_QUADS);
}

void Image3D::getColor(uint x, uint y, float*color4) {
    getColor(data, x, y, color4);
}

void Image3D::getColor(uchar * pdata, uint x, uint y, float * outColor)
{
    uchar* pixelPtr = pdata + y*step + x*chns;
    if (chns >= 3) {
        for (uint i = 0; i < 3; i++) outColor[i] = (float)pixelPtr[2 - i] / 255;
        if (chns == 4) outColor[3] = (float)pixelPtr[3] / 255;
        else outColor[3] = 1;
    }
    else if (chns == 1) {
        outColor[1] = outColor[2] = outColor[3] = (float)pixelPtr[0] / 255;
        outColor[3] = 1;
    }
}

void Image3D::calcHeight() {
    if (heights == NULL) heights = new float[cols*rows];
    else if (sizeof(heights) / sizeof(float) != cols*rows) {
        delete[] heights;
        heights = new float[cols*rows];
    }

    for (uint y = 0; y < rows; y++) {
        for (uint x = 0; x < cols; x++) {
            uint sum = 0;
            for (uint c = 0; c < chns; c++) {
                sum += data[y*step + x*chns + c];
            }
            heights[y*cols + x] = (float)sum / chns;
        }
    }
}